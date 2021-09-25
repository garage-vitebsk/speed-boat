#include <Arduino.h>

class Motor {
  public:
    Motor(int, int, int);
    void setForward();
    void setReverse();
    void stop();
    void setSpeed(int);
    byte getDirection();
  private:
    byte direction = 0;
    int pwmPin;
    int pin1, pin2;
};

class MotorDriver {
  public:
    MotorDriver(Motor*);

    void setSpeed(int);
    int getSpeed();
  private:
    Motor* motor;
    int speed = 0;
};

class DriveSystem {
  public:
    DriveSystem(Motor*, Motor*);

    int getLeftSpeed();
    int getRightSpeed();

    void setSpeed(int, int);
  private:
    MotorDriver *leftDriver;
    MotorDriver *rightDriver;

    int leftSpeed = 0;
    int rightSpeed = 0;
};
