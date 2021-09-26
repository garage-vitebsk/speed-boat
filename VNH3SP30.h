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
    MotorDriver(Motor*, int, int);

    void setSpeed(int);
    int getSpeed();
    void enable(boolean);
    float getCurrent();
  private:
    Motor* motor;
    int speed = 0;
    int driverEnalePin;
    int currentMeasurePin;
    float prevCurrent;
};
