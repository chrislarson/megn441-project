#include <Arduino.h>

// Pin setup
#define PWM_A 9
#define DIR_A1 4
#define DIR_A2 5

// #define PWM_B 3
// #define DIR_B1 4

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
int integerFromPC = 0;
float floatFromPC = 0.0;

boolean newData = false;

//============

void setup()
{
    Serial.begin(9600);
    motor_setup();
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
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

    long currentMillis = millis();

    while (currentMillis - previousMillis < 1000)
    {
        run_motors(0, prevDir);
        currentMillis = millis();
    }

    run_motors(0, prevDir);

    delay(2000);
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

    strtokIndx = strtok(NULL, ",");   // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx); // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    floatFromPC = atof(strtokIndx); // convert this part to a float
}

//============

void showParsedData()
{
    Serial.println("This is what I received:" + String(messageFromPC) + " " + String(integerFromPC) + " " + String(floatFromPC));
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
