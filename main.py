import cv2
from PIL import Image
import numpy as np
import serial

# ser = serial.Serial("COM5", 9600)

# ser.write(b'run')


yellow = [0, 255, 255]
red = [0, 0, 255]
blue = [255, 0, 0]

cap = cv2.VideoCapture(1)




def get_limits(color):
    c = np.uint8([[color]])  # BGR values
    hsvC = cv2.cvtColor(c, cv2.COLOR_BGR2HSV)

    hue = hsvC[0][0][0]  # Get the hue value

    # Handle red hue wrap-around
    if hue >= 165:  # Upper limit for divided red hue
        lowerLimit = np.array([hue - 5, 100, 100], dtype=np.uint8)
        upperLimit = np.array([180, 255, 255], dtype=np.uint8)
    elif hue <= 15:  # Lower limit for divided red hue
        lowerLimit = np.array([0, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 5, 255, 255], dtype=np.uint8)
    else:
        lowerLimit = np.array([hue - 5, 100, 100], dtype=np.uint8)
        upperLimit = np.array([hue + 5, 255, 255], dtype=np.uint8)

    return lowerLimit, upperLimit


while True:
    ret, frame = cap.read()
    hsvImage = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lowerLimit, upperLimit = get_limits(color=yellow)
    lowerred, upperred = get_limits(color=red)
    lowerblue, upperblue = get_limits(color=blue)

    mask_yellow = cv2.inRange(hsvImage, lowerLimit, upperLimit)
    mask_red = cv2.inRange(hsvImage, lowerred, upperred)
    mask_blue = cv2.inRange(hsvImage, lowerblue, upperblue)

    combined_mask = cv2.bitwise_or(mask_yellow, cv2.bitwise_or(mask_red, mask_blue))
    mask_ = Image.fromarray(combined_mask)

    bbox = mask_.getbbox()
    if bbox is not None:
        x1, y1, x2, y2 = bbox
        # Lấy màu tương ứng với bbox đầu tiên
        if np.any(mask_yellow[y1:y2, x1:x2]):
            text = 'Yellow'
            color = (0, 255, 255)  # Màu văn bản màu vàng
        elif np.any(mask_red[y1:y2, x1:x2]):
            text = 'Red'
            color = (0, 0, 255)  # Màu văn bản màu đỏ
        elif np.any(mask_blue[y1:y2, x1:x2]):
            text = 'Blue'
            color = (255, 0, 0)  # Màu văn bản màu xanh dương

        font = cv2.FONT_HERSHEY_PLAIN
        frame = cv2.rectangle(frame, (x1, y1), (x2, y2), color, 5)
        frame = cv2.putText(frame, text, (x1, y1 - 4), font, 1, color, 1, cv2.LINE_AA)

    cv2.imshow('frame', frame)
    # cv2.imshow('Mask', combined_mask)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break




cap.release()
cv2.destroyAllWindows()

