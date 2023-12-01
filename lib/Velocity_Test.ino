#include <Arduino.h>
#include <movingAvg.h>

// Function initialization
void indexLeftEncoderCount();
void indexRightEncoderCount();

// Pin definition
#define ENA 6 
#define dirA1 7
#define dirA2 8
#define ENB 9
#define dirB1 10
#define dirB2 11
#define voltage A7
#define encoderA1 4
#define encoderA2 5
#define encoderB1 2
#define encoderB2 3

// Wheel characteristics
#define countsPerRev  240
#define distPerRev  2394 //mm

// Variables for velocity calculations
#define sampleTime 100
#define Kv 30 // velcity gain for increase resolution
volatile unsigned int currentTime = 0;
volatile unsigned int prevTime = 0;
volatile unsigned int deltTime;
volatile unsigned int velRight = 0;
volatile unsigned int velLeft = 0;
float currDistRight = 0.0;
float prevDistRight = 0.0;
float currDistLeft = 0.0;
float prevDistLeft = 0.0;
float deltDistRight = 0.0;
float deltDistLeft = 0.0;

// Velocity Averaging
movingAvg moveAvgRight(100);
volatile unsigned int velRightAvg;
movingAvg moveAvgLeft(100);
volatile unsigned int velLeftAvg;

// Encoder count variables
volatile unsigned int leftEncoderCount = 0; 
volatile unsigned int rightEncoderCount = 0;

// PWM initialization values
int pwm = 135;
int pwmA = pwm;
int pwmB = pwm;

// PD gains
int Kp = 10;
int Kd = 1;

// Velocity control
int velError = 0;
int currError = 0;
int prevError = 0;
int deltError = 0;

void setup() {
  pinMode(ENA,OUTPUT);
  pinMode(dirA1,OUTPUT);
  pinMode(dirA2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(dirB1,OUTPUT);
  pinMode(dirB2,OUTPUT);
  pinMode(voltage,INPUT);
  pinMode(encoderA1,INPUT);
  pinMode(encoderB1,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderA1), indexRightEncoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1), indexLeftEncoderCount, CHANGE);
  moveAvgRight.begin();
  moveAvgLeft.begin();
  digitalWrite(dirA1,HIGH);
  digitalWrite(dirA2,LOW);
  digitalWrite(dirB1,HIGH);
  digitalWrite(dirB2,LOW);
  delay(100);
  Serial.begin(9600);
}

void loop() {
  currentTime = millis();
  currDistRight = (distPerRev/countsPerRev)*rightEncoderCount;
  currDistLeft = (distPerRev/countsPerRev)*leftEncoderCount;
  deltTime = currentTime - prevTime;
  deltDistRight = currDistRight - prevDistRight;
  deltDistLeft = currDistLeft - prevDistLeft;
  deltError = currError/deltTime; // dE/dt

  if (deltTime > sampleTime){
    prevTime = currentTime;
    prevDistRight = currDistRight;
    prevDistLeft = currDistLeft;
    prevError = currError;
  }
  
  // Velocity calculation and averaging
  velRight = (Kv*deltDistRight/deltTime);
  velRightAvg = moveAvgRight.reading(velRight);
  velLeft = (Kv*deltDistLeft/deltTime);
  velLeftAvg = moveAvgLeft.reading(velLeft);

  // Error update
  velError = velRightAvg - velLeftAvg;
  currError = velError - prevError;
  
  // PD control based on velocity error
  if (velRightAvg > velLeftAvg + 2){
    pwmA = pwm - Kp*(abs(velError)) - Kd*(abs(deltError));
    pwmB = pwm + Kp*(abs(velError)) + Kd*(abs(deltError));
  }
  else if (velLeftAvg > velRightAvg + 2){
    pwmB = pwm - Kp*(abs(velError)) - Kd*(abs(deltError));
    pwmA = pwm + Kp*(abs(velError)) + Kd*(abs(deltError));
  }
  else{
    pwmA = pwm;
    pwmB = pwm;
  }

  analogWrite(ENA,pwmA);
  analogWrite(ENB,pwmB);
  // Serial.print("Velocity Right: ");
  // Serial.print(velRightAvg);
  // Serial.print("  Velocity Left: ");
  // Serial.print(velLeftAvg);
  // Serial.println(" ");

}

void indexLeftEncoderCount() {
  leftEncoderCount++;
}
void indexRightEncoderCount() {
  rightEncoderCount++;
}
