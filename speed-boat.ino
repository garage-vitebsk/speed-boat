#include <PinChangeInterrupt.h>
#include "L298N.h"

#define LED_PIN 13
#define LIGHTS_PIN 11
#define MOTOR_SPEED_PIN 5
#define DIRECTION_PIN_1 6
#define DIRECTION_PIN_2 7
#define ACCELERATOR_INPUT_PIN 10
#define DIRECTION_INPUT_PIN 9
#define LIGHTS_INPUT_PIN 8

#define ACCELERATOR_INPUT_INDEX 0
#define DIRECTION_INPUT_INDEX 1
#define LIGHTS_INPUT_INDEX 2

#define DEBUG true

#define INPUT_THRESHOLD 900
#define INPUT_MIN 1000
#define INPUT_MAX 2000
#define INPUT_MID 1500
#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define ZERO_AREA 15

MotorDriver *motor;

const byte channelPin[] = {ACCELERATOR_INPUT_PIN, DIRECTION_INPUT_PIN, LIGHTS_INPUT_PIN};
volatile unsigned long signalStart[] = {0, 0, 0};
volatile long signalLength[] = {0, 0, 0};

int speed;
int direction;
bool lightsState;

void setup() {
  Serial.begin(57600);
  pinMode(channelPin[0], INPUT);
  pinMode(channelPin[1], INPUT);
  pinMode(channelPin[2], INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LIGHTS_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(LIGHTS_PIN, LOW);

  motor = new MotorDriver(new Motor(MOTOR_SPEED_PIN, DIRECTION_PIN_1, DIRECTION_PIN_2));
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channelPin[ACCELERATOR_INPUT_INDEX]), onRisingAccelerator, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channelPin[DIRECTION_INPUT_INDEX]), onRisingDirection, CHANGE);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(channelPin[LIGHTS_INPUT_INDEX]), onRisingLights, CHANGE);

  digitalWrite(LED_PIN, LOW);
}


void onRisingAccelerator(void) {
  processPin(ACCELERATOR_INPUT_INDEX);
}

void onRisingDirection(void) {
  processPin(DIRECTION_INPUT_INDEX);
}

void onRisingLights(void) {
  processPin(LIGHTS_INPUT_INDEX);
}

void processPin(byte pinIndex) {
  uint8_t trigger = getPinChangeInterruptTrigger(digitalPinToPCINT(channelPin[pinIndex]));
  if (trigger == RISING) {
    signalStart[pinIndex] = micros();
  } else if (trigger == FALLING) {
    signalLength[pinIndex] = micros() - signalStart[pinIndex];
  }
}

void loop() {
  speed = calculateSpeed(signalLength[ACCELERATOR_INPUT_INDEX]);
  direction = calculateDirection(signalLength[DIRECTION_INPUT_INDEX]);
  lightsState = calculateLightsState(signalLength[LIGHTS_INPUT_INDEX]);
  if (DEBUG) {
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.print("\tDirection: ");
    Serial.print(direction > 0 ? "Forward" : "Reverse");
    Serial.print("\tLights: ");
    Serial.print(lightsState ? "On" : "Off");
    Serial.print("\tRaw: ");
    Serial.print(signalLength[0]);
    Serial.print(", ");
    Serial.print(signalLength[1]);
    Serial.print(", ");
    Serial.println(signalLength[2]);
  }
  motor->setSpeed(speed * direction);
  digitalWrite(LIGHTS_PIN, lightsState);
}

int calculateSpeed(long pwmInput) {
  if (pwmInput > INPUT_THRESHOLD) {
    if (pwmInput > INPUT_MAX - ZERO_AREA) {
      return OUTPUT_MAX;
    } else if (pwmInput < INPUT_MIN + ZERO_AREA) {
      return OUTPUT_MIN;
    } else {
      return map(pwmInput, INPUT_MIN, INPUT_MAX, OUTPUT_MIN, OUTPUT_MAX);
    }
  }
  return 0;
}

int calculateDirection(long pwmInput) {
  return pwmInput > INPUT_MID ? -1 : 1;
}

bool calculateLightsState(long pwmInput) {
  return pwmInput > INPUT_MID ? false : true;
}
