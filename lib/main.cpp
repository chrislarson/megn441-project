#include <Arduino.h>

#define LEFT_ENCODER_PIN 2
#define RIGHT_ENCODER_PIN 3

#define LEFT_MOTOR_PIN 4
#define RIGHT_MOTOR_PIN 5

int incrementLeftEncoder();
int incrementRightEncoder();
int resetLeftEncoder();
int resetRightEncoder();

void setup()
{
  Serial.begin(115200);
}

int leftEncoderCount = 0;
int rightEncoderCount = 0;

void loop()
{
  if (Serial.available() > 0)
  {
    // read instruction
    String instruction = Serial.readStringUntil('\n');

    instruction.split(',');
    // do instruction
    Serial.println("R:" + instruction);

    // send back new encoder state
    // String encoderState = "STATE," + String(leftEncoderCount) + "," + String(rightEncoderCount);
    // Serial.println(encoderState);
  }
}

int incrementLeftEncoder()
{
  leftEncoderCount++;
  return leftEncoderCount;
}

int incrementRightEncoder()
{
  rightEncoderCount++;
  return rightEncoderCount;
}

int resetLeftEncoder()
{
  leftEncoderCount = 0;
  return leftEncoderCount;
}

int resetRightEncoder()
{
  rightEncoderCount = 0;
  return rightEncoderCount;
}