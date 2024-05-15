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
  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n');
    if (receivedString == "run") {
      isBangtai = true;
      controlBangtai();
    } else if (receivedString == "stop") {
      isBangtai = false;
      controlBangtai();
    }
  }
  if (digitalRead(sensorInfrared1) == 0) {
    if (color == 0) {
      isBangtai = false;
      controlBangtai();
      for (int i = 0; i <= 2; i++) {
        surveyColor();
      }
      isBangtai = true;
      controlBangtai();
    }
  }

  if (color == 1 && digitalRead(sensorInfrared2) == 0) {
    Serial.println("white");
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

  if (color == 2 && digitalRead(sensorInfrared3) == 0) {
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