import cv2
import numpy as np
import serial

yellow = [0, 255, 255]
red = [0, 0, 255]

cap = cv2.VideoCapture(1)
# ser = serial.Serial("COM5", 9600)

# ser.write(b'run')

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

    # Get color limits for red and yellow
    lower_red, upper_red = get_limits(red)
    lower_yellow, upper_yellow = get_limits(yellow)

    # Thresholding
    mask_red = cv2.inRange(hsvImage, lower_red, upper_red)
    mask_yellow = cv2.inRange(hsvImage, lower_yellow, upper_yellow)

    # Combine masks for red and yellow
    combined_mask = cv2.bitwise_or(mask_red, mask_yellow)

    # Morphological opening to remove noise
    kernel = np.ones((7, 7), np.uint8)  # Increase kernel size for morphological operation
    combined_mask = cv2.morphologyEx(combined_mask, cv2.MORPH_OPEN, kernel)

    # Find contours
    contours, _ = cv2.findContours(combined_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    for contour in contours:
        area = cv2.contourArea(contour)
        if area > 2500:
            x, y, w, h = cv2.boundingRect(contour)
            centroid_x = int(x + w / 2)
            centroid_y = int(y + h / 2)
            if np.any(mask_yellow[y:y + h, x:x + w]):
                color_text = 'Yellow'
                color = (0, 255, 255)  # Yellow color
                # print('Yellow')
                # ser.write(b'yellow')
            elif np.any(mask_red[y:y + h, x:x + w]):
                color_text = 'Red'
                color = (0, 0, 255)  # Red color
                print('Red')
                # ser.write(b'red')
            cv2.rectangle(frame, (x, y), (x + w, y + h), color, 2)
            cv2.putText(frame, color_text, (x, y - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color, 2)

    cv2.imshow('frame', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
