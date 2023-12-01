#include <Arduino.h>

// Pin setup
#define PWM_A 9
#define DIR_A1 4
#define DIR_A2 5

// #define PWM_B 3
// #define DIR_B1 4

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

// Wheel characteristics
#define countsPerRev  240
#define distPerRev  2394 //mm

void run_motors(int pwm, int dir);
void motor_setup();

void parseData();
void showParsedData();
void recvWithStartEndMarkers();

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; // temporary array for use when parsing

// variables to hold the parsed data
char messageFromPC[numChars] = {0};
int theta = 0.0;

boolean newData = false;

// PWM initialization values
int pwm = 95;
int pwmA = pwm;
int pwmB = pwm;

// PD gains
int Kp = 1.2;
int Kd = 1;

//============

void setup()
{

  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(dirA1, OUTPUT);
  pinMode(dirA2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(dirB1, OUTPUT);
  pinMode(dirB2, OUTPUT);
  pinMode(voltage, INPUT);
  pinMode(encoderA1, INPUT);
  // pinMode(encoderA2,INPUT);
  pinMode(encoderB1, INPUT);
  // pinMode(encoderB2,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderA1), indexLeftEncoderCount, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB1), indexRightEncoderCount, CHANGE);
  digitalWrite(dirA1, HIGH);
  digitalWrite(dirA2, LOW);
  digitalWrite(dirB1, HIGH);
  digitalWrite(dirB2, LOW);
  // motor_setup();
}

//============

long previousMillis = 0;
int prevDir = 1;

void loop()
{
  previousMillis = millis();
  if (prevDir == 1)
  {
    prevDir = 0;
  }
  else
  {
    prevDir = 1;
  }

  recvWithStartEndMarkers();
  if (newData == true)
  {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
  }


  if (theta < 0){
    // left motor speed up
    pwmA = pwm + Kp*(abs(theta)); //Kd*(abs(deltError));
    pwmB = pwm;
    // pwmB = pwm - Kp*(abs(theta)); //Kd*(abs(deltError)); 
  }
  else if (theta > 0){
    pwmA = pwm;
    // pwmA = pwm - Kp*(abs(theta)); //Kd*(abs(deltError));
    pwmB = pwm + Kp*(abs(theta)); //Kd*(abs(deltError));
  }
  else {
    pwmA = pwm;
    pwmB = pwm;
  }

  analogWrite(ENA, pwmA);
  analogWrite(ENB, pwmB);

  // Serial.print("<Enc_R:");
  // Serial.print(rightEncoderCount);
  // Serial.print(",");
  // Serial.print("Enc_L:");
  // Serial.print(leftEncoderCount);
  // Serial.println(">");

  showParsedData();

}

//============

void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false)
  {
    rc = Serial.read();

    if (recvInProgress == true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker)
    {
      recvInProgress = true;
    }
  }
}

//============

void parseData()
{ // split the data into its parts

  char *strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ","); // get the first part - the string
  strcpy(messageFromPC, strtokIndx);   // copy it to messageFromPC


  strtokIndx = strtok(NULL, ",");
  theta = atof(strtokIndx); // convert this part to an float

}

//============

void showParsedData()
{
  Serial.println("Recvd:<" + String(messageFromPC) + "," + String(theta) + ">");
}

void motor_setup()
{
  // Define motor driver pins as outputs
  pinMode(PWM_A, OUTPUT);
  pinMode(DIR_A1, OUTPUT);
  // pinMode(PWM_B, OUTPUT);
  // pinMode(DIR_B1, OUTPUT);
  // Initialize all motor pins to zero
  digitalWrite(PWM_A, 0);
  digitalWrite(DIR_A1, 0);
  // digitalWrite(PWM_B, 0);
  // digitalWrite(DIR_B1, 0);
  return;
}

void run_motors(int pwm, int dir)
{
  // Motor A
  if (dir == 1)
  {
    digitalWrite(DIR_A1, 1);
    digitalWrite(DIR_A2, 0);
  }
  else
  {
    digitalWrite(DIR_A1, 0);
    digitalWrite(DIR_A2, 1);
  }
  analogWrite(PWM_A, pwm);
  // Motor B
  // analogWrite(PWM_B, pwm);
  // digitalWrite(DIR_B1, 1);
  return;
}

void indexLeftEncoderCount()
{
  leftEncoderCount++;
}
void indexRightEncoderCount()
{
  rightEncoderCount++;
}
