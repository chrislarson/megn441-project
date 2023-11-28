#include <Arduino.h>

// Pin setup
#define PWM_A 9
#define DIR_A1 4
#define DIR_A2 5

#define BUTTON_A 2

void run_motor(int pwm, int dir);
void motor_setup();

boolean buttonPushed = false;
int buttonState = 0;
long starttime, stoptime, deltatime;

void setup()
{
    pinMode(BUTTON_A, INPUT_PULLUP);
    motor_setup();
    Serial.begin(9600);
}

void loop()
{
    buttonState = digitalRead(BUTTON_A);
    if (buttonState == 0)
    {
        run_motor(255 * 1.0, 1);
        if (buttonPushed == false)
        {
            buttonPushed = true;
            starttime = millis();
            Serial.print(starttime);
        }
    }
    else
    {
        run_motor(0, 1);
        if (buttonPushed == true)
        {
            stoptime = millis();
            Serial.print(" ");
            Serial.print(stoptime);
            buttonPushed = false;
            deltatime = stoptime - starttime;
            // print results
            Serial.print("\t");
            Serial.println(deltatime);
        }
    }
}

void motor_setup()
{
    // Define motor driver pins as outputs
    pinMode(PWM_A, OUTPUT);
    pinMode(DIR_A1, OUTPUT);
    pinMode(DIR_A2, OUTPUT);
    // Initialize all motor pins to zero
    digitalWrite(PWM_A, 0);
    digitalWrite(DIR_A1, 0);
    digitalWrite(DIR_A2, 0);
    return;
}

void run_motor(int pwm, int dir)
{
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
    return;
}
