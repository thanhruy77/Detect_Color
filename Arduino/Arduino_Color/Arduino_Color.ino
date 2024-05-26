#include <Servo.h>

Servo servoYellow;
Servo servoRed;

#define relay 2

#define sensorInfrared1 9
#define sensorInfrared2 11


int angleYellow = 90;
int angleRed = 90;
int state = 0;
int red_color = 0;
int yellow_color = 0;
boolean isBangtai = false;

int countYellow = 0;
int countRed = 0;

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(sensorInfrared1, INPUT);
  pinMode(sensorInfrared2, INPUT);
  servoYellow.attach(12);
  servoRed.attach(13);
  Serial.begin(9600);
  isBangtai = true;
  controlBangtai();
}

void controlBangtai() {
  digitalWrite(relay, isBangtai ? HIGH : LOW);
}

void updateCounts() {
  Serial.print("countYellow=");
  Serial.print(countYellow);
  Serial.print(",countRed=");
  Serial.println(countRed);
}


void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    if (command == "yellow") {
      yellow_color = 1;
    }
    if (command == "red") {
      red_color = 1;
    }
    if (command == "save") {
      updateCounts();
    }
  }
  if (digitalRead(sensorInfrared1) == 0 && yellow_color == 1) {
    isBangtai = false;
    controlBangtai();
    delay(200);
    if (state == 0) {
      servoYellow.write(0);
      delay(1000);
      state = 1;
    }
    if (state == 1) {
      servoYellow.write(angleYellow);
      delay(1000);
      state = 0;
    }
    isBangtai = true;
    controlBangtai();
    delay(100);
    yellow_color = 0;
    countYellow += 1;
    isBangtai = true;
    controlBangtai();
  }
  if (digitalRead(sensorInfrared2) == 0 && red_color == 1) {
    isBangtai = false;
    controlBangtai();
    delay(200);
    if (state == 0) {
      servoRed.write(0);
      delay(1000);
      state = 1;
    }
    if (state == 1) {
      servoRed.write(angleRed);
      delay(1000);
      state = 0;
    }
    isBangtai = true;
    controlBangtai();
    delay(100);
    red_color = 0;
    isBangtai = true;
    countRed += 1;
    controlBangtai();
  }
}