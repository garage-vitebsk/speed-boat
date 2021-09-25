#include "L298N.h"

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
MotorDriver::MotorDriver(Motor* motor) {
  this->motor = motor;
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

//-----DriveSystem------
DriveSystem::DriveSystem(Motor *leftMotor, Motor *rightMotor) {
  leftDriver = new MotorDriver(leftMotor);
  rightDriver = new MotorDriver(rightMotor);
}

int DriveSystem::getLeftSpeed() {
  return leftDriver->getSpeed();
}

int DriveSystem::getRightSpeed() {
  return rightDriver->getSpeed();
}

void DriveSystem::setSpeed(int leftSpeed, int rightSpeed) {
  leftDriver->setSpeed(leftSpeed);
  rightDriver->setSpeed(rightSpeed);
}
