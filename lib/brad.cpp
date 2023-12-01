#include <Arduino.h>

// // Pin setup
// #define PWM_A 9
// #define DIR_A1 4
// #define DIR_A2 5

// #define BUTTON_A 2

// void run_motor(int pwm, int dir);
// void motor_setup();

// boolean buttonPushed = false;
// int buttonState = 0;
// long starttime, stoptime, deltatime;

// void setup()
// {
//     pinMode(BUTTON_A, INPUT_PULLUP);
//     motor_setup();
//     Serial.begin(9600);
// }

// void loop()
// {
//     buttonState = digitalRead(BUTTON_A);
//     if (buttonState == 0)
//     {
//         run_motor(255 * 1.0, 1);
//         if (buttonPushed == false)
//         {
//             buttonPushed = true;
//             starttime = millis();
//             Serial.print(starttime);
//         }
//     }
//     else
//     {
//         run_motor(0, 1);
//         if (buttonPushed == true)
//         {
//             stoptime = millis();
//             Serial.print(" ");
//             Serial.print(stoptime);
//             buttonPushed = false;
//             deltatime = stoptime - starttime;
//             // print results
//             Serial.print("\t");
//             Serial.println(deltatime);
//         }
//     }
// }

// void motor_setup()
// {
//     // Define motor driver pins as outputs
//     pinMode(PWM_A, OUTPUT);
//     pinMode(DIR_A1, OUTPUT);
//     pinMode(DIR_A2, OUTPUT);
//     // Initialize all motor pins to zero
//     digitalWrite(PWM_A, 0);
//     digitalWrite(DIR_A1, 0);
//     digitalWrite(DIR_A2, 0);
//     return;
// }

// void run_motor(int pwm, int dir)
// {
//     if (dir == 1)
//     {
//         digitalWrite(DIR_A1, 1);
//         digitalWrite(DIR_A2, 0);
//     }
//     else
//     {
//         digitalWrite(DIR_A1, 0);
//         digitalWrite(DIR_A2, 1);
//     }
//     analogWrite(PWM_A, pwm);
//     return;
// }

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
void indexLeftEncoderCount();
void indexRightEncoderCount();

volatile unsigned int leftEncoderCount = 0;
volatile unsigned int rightEncoderCount = 0;

void setup() {
  pinMode(ENA,OUTPUT);
  pinMode(dirA1,OUTPUT);
  pinMode(dirA2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(dirB1,OUTPUT);
  pinMode(dirB2,OUTPUT);
  pinMode(voltage,INPUT);
  pinMode(encoderA1,INPUT);
  // pinMode(encoderA2,INPUT);
  pinMode(encoderB1,INPUT);
  // pinMode(encoderB2,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderA1), indexLeftEncoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1), indexRightEncoderCount, CHANGE);
  digitalWrite(dirA1,HIGH);
  digitalWrite(dirA2,LOW);
  digitalWrite(dirB1,HIGH);
  digitalWrite(dirB2,LOW);
  Serial.begin(9600);

}

void loop() {
  // int pwm = 200;
  // analogWrite(ENA,pwm);
  // analogWrite(ENB,pwm);
  // delay(2000);
  // analogWrite(ENA,0);
  // analogWrite(ENB,0);
  // delay(100000);
  // Serial.println(analogRead(voltage)*0.01588);
  
  Serial.print("Forward Right:");
  Serial.print(rightEncoderCount);
  Serial.print("  ");
  Serial.print("Forward Left:");
  Serial.print(leftEncoderCount);
  Serial.println(" ");
  delay(1000);
}

void indexLeftEncoderCount() {
    leftEncoderCount++;
}
void indexRightEncoderCount() {
    rightEncoderCount++;
}
