#include <Servo.h>

Servo servoWhite;
Servo servoBrown;

#define relay 2

#define sensorInfrared1 8
#define sensorInfrared2 9
#define sensorInfrared3 11

int angleWhite = 90;
int angleBrown = 90;
int state = 0;
int color = 0;
boolean isBangtai = false;

int whiteColor = 0;
int brownColor = 0;

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(sensorInfrared1, INPUT);
  pinMode(sensorInfrared2, INPUT);
  pinMode(sensorInfrared3, INPUT);
  servoWhite.attach(12);
  servoBrown.attach(13);
  Serial.begin(9600);
}

void controlBangtai() {
  digitalWrite(relay, isBangtai ? HIGH : LOW);
}


void loop() {
  String receivedString = "";  // Chuỗi để lưu trữ dữ liệu từ cổng serial


  while (Serial.available() > 0) {
    char c = Serial.read();
    receivedString += c;
    delay(2);
  }
  if (receivedString.length() > 0) {
    if (receivedString == "yellow") {
      if (digitalRead(sensorInfrared2) == 0) {
        isBangtai = false;
        controlBangtai();
        delay(200);
        if (state == 0) {
          servoWhite.write(0);
          delay(1000);
          state = 1;
        }
        if (state == 1) {
          servoWhite.write(angleWhite);
          delay(1000);
          state = 0;
        }
        color = 0;
        isBangtai = true;
        controlBangtai();
        color = 0;
        delay(100);
      }
    }

    if (receivedString == "red") {
      if (digitalRead(sensorInfrared3) == 0) {
        Serial.println("brown");
        isBangtai = false;
        controlBangtai();
        delay(200);
        if (state == 0) {
          servoBrown.write(0);
          delay(1000);
          state = 1;
        }
        if (state == 1) {
          servoBrown.write(angleBrown);
          delay(1000);
          state = 0;
        }
        color = 0;
        isBangtai = true;
        controlBangtai();
        color = 0;
        delay(100);
      }
    }
  }
}