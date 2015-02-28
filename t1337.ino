#include <Makeblock.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

MeDCMotor MotorL(M1);  
MeDCMotor MotorR(M2);
MeInfraredReceiver infraredReceiverDecode(PORT_6);
MeUltrasonicSensor UltrasonicSensor(PORT_3);
int moveSpeed = 190;
boolean leftflag,rightflag;
int minSpeed = 55;
int factor = 23;
int distance = 0;

int waitForInput = 1;

// up up down down left right left right b a
// UP 1
// DOWN 2
// LEFT 3
// RIGHT 4
// A 5
// B 6
// 1 1 2 2 3 4 3 4 6 5
const int LEN = 10;
int KONAMI[LEN] = { 1, 1, 2, 2, 3, 4, 3, 4, 6, 5 };
int current[LEN] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int idx = 0, res = 0;

int checkKonami(int start) {
  start = (start + 1) % LEN;
  for (int i = 0; i < LEN; i++) {
    if (KONAMI[i] != current[start]) {
      return 0;
    }
    start = (start + 1) % LEN;
  }
  return 1;
}

void setup() {
  infraredReceiverDecode.begin();
  beep();
}

void destroy() {
//  beep();
  Forward();
}

void search() {
//  boop();
  TurnLeft();
}

void handleInput(int in) {
  if (waitForInput) {
//    beep();
    current[idx] = in;
    idx = (idx + 1) % LEN;
    res = checkKonami(idx);
    if (res) {
      beepbeep();
    }
    waitForInput = 0;
  }
}

int snd = 0;

void loop() {
  if (snd) {
    distance = UltrasonicSensor.distanceCm();
    
    if (distance < 40) {
      destroy();
    } else {
      search();
    }
  }
  
  if (infraredReceiverDecode.available() || infraredReceiverDecode.buttonState()) {
    switch (infraredReceiverDecode.read()) {
      case IR_BUTTON_PLUS:
        handleInput(1);
        Forward();
        break;
      case IR_BUTTON_MINUS:
        handleInput(2);
        Backward();
        break;
      case IR_BUTTON_NEXT:
        handleInput(3);
        TurnRight();
        break;
      case IR_BUTTON_PREVIOUS:
        handleInput(4);
        TurnLeft();
        break;
      case IR_BUTTON_9:
        ChangeSpeed(factor * 9 + minSpeed);
        break;
      case IR_BUTTON_8:
        ChangeSpeed(factor * 8 + minSpeed);
        break;
      case IR_BUTTON_7:
        ChangeSpeed(factor * 7 + minSpeed);
        break;
      case IR_BUTTON_6:
        ChangeSpeed(factor * 6 + minSpeed);
        break;
      case IR_BUTTON_5:
        ChangeSpeed(factor * 5 + minSpeed);
        break;
      case IR_BUTTON_4:
        ChangeSpeed(factor * 4 + minSpeed);
        break;
      case IR_BUTTON_3:
        ChangeSpeed(factor * 3 + minSpeed);
        break;
      case IR_BUTTON_2:
        ChangeSpeed(factor * 2 + minSpeed);
        break;
      case IR_BUTTON_1:
        ChangeSpeed(factor * 1 + minSpeed);
        break;
      case IR_BUTTON_A:
        handleInput(5);
        break;
      case IR_BUTTON_B:
        handleInput(6);
        break;
      case IR_BUTTON_C:
        snd = 1;
        break;
      case IR_BUTTON_E:
        snd = 0;
        break;
    }
    waitForInput = 0;
  } else {
    waitForInput = 1;
    Stop();
  }
}

void beep() {
  buzzerOn();
  delay(50);
  buzzerOff();
  delay(50);
}

void boop() {
  buzzerOn();
  delay(500);
  buzzerOff();
  delay(500);
}

void beepbeep() {
  buzzerOn();
  delay(100);
  buzzerOff();
  delay(100);
  buzzerOn();
  delay(100);
  buzzerOff();
  delay(100);
  buzzerOn();
  delay(1000);
  buzzerOff();
}

void Forward() {
  MotorL.run(moveSpeed);
  MotorR.run(moveSpeed);
}
void Backward() {
  MotorL.run(-moveSpeed);
  MotorR.run(-moveSpeed);
}
void TurnLeft() {
  MotorL.run(-moveSpeed);
  MotorR.run(moveSpeed);
}
void TurnRight() {
  MotorL.run(moveSpeed);
  MotorR.run(-moveSpeed);
}
void Stop() {
  MotorL.run(0);
  MotorR.run(0);
}
void ChangeSpeed(int spd) {
  moveSpeed = spd;
}
