#include "VNH3SP30.h"
#define CURRENT_COEF 50

//------Motor------
Motor::Motor(int newPwmPin, int newPin1, int newPin2) {
  pwmPin = newPwmPin;
  pin1 = newPin1;
  pin2 = newPin2;
  pinMode(pwmPin, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void Motor::setForward() {
  direction = 1;
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void Motor::setReverse() {
  direction = -1;
  digitalWrite(pin2, LOW);
  digitalWrite(pin1, HIGH);
}

void Motor::stop() {
  direction = 0;
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  setSpeed(0);
}

void Motor::setSpeed(int speed) {
  analogWrite(pwmPin, speed);
}

//-----MotorDriver------
MotorDriver::MotorDriver(Motor* motor, int driverEnalePin, int currentMeasurePin) {
  this->motor = motor;
  this->driverEnalePin = driverEnalePin;
  this->currentMeasurePin = currentMeasurePin;
  pinMode(driverEnalePin, OUTPUT);
}

void MotorDriver::setSpeed(int speed) {
  this->speed = speed;
  if (speed == 0) {
    motor->stop();
  } else if (speed < 0) {
    motor->setReverse();
    motor->setSpeed(-speed);
  } else {
    motor->setForward();
    motor->setSpeed(speed);
  }
}

MotorDriver::getSpeed() {
  return speed;
}

void MotorDriver::enable(boolean enableFlag) {
  digitalWrite(driverEnalePin, enableFlag ? HIGH : LOW);
}

float MotorDriver::getCurrent() {
  float currentNow = analogRead(currentMeasurePin);
  float averageCurrent = (currentNow + prevCurrent) / 2;
  prevCurrent = averageCurrent;
  return averageCurrent / CURRENT_COEF;
}
