

/*
 *  function:   test dfr0008
 *  by:         lisper (leyapin@gmail.com)
 *  created:    2013-11-04
 *
 */
#include <Arduino.h>
#include "CytronMotorDriver.h"
#include <Bounce2.h>



#define up_button   8
#define down_button A1
#define left_button 9
#define right_button    12

#define stick_button A0
#define level_stick A3
#define vertical_stick A2

int vertical_value=0;   //current value
int level_value=0;      //current value
int vertical_valuep=0;  //previous value
int level_valuep=0;     //previous value
bool joystick_mode=false;
bool clicking_mode=false;
uint16_t key_delay_time = 20;   // for ispressed ()

//
// Configure the motor driver.
CytronMD motor(PWM_DIR, 3, 2);  // PWM = Pin 3, DIR = Pin 2.
CytronMD motor2(PWM_DIR, 5, 4);  // PWM = Pin 5, DIR = Pin 4.
Bounce debouncer = Bounce(); 
void setup () {
    pinMode (stick_button, INPUT_PULLUP);
    pinMode (level_stick, INPUT);
    pinMode (vertical_stick, INPUT);

    pinMode (up_button, INPUT);
    pinMode (down_button, INPUT);
    pinMode (left_button, INPUT);
    pinMode (right_button, INPUT);
    debouncer.attach(stick_button);
    debouncer.interval(100);
    Serial.begin (9600);
    motor.setSpeed(0);
    motor2.setSpeed(0);
}

//
void loop () { 
    debouncer.update();
    vertical_value = ( analogRead (vertical_stick) - 524 ) /2.2;
    level_value = ( analogRead (level_stick) - 524 ) / 2.2;

    if (level_value > -20 && level_value < 20) level_value=0; 
    if (vertical_value != vertical_valuep) {
        //Serial.print ("vertical=");
        //Serial.println (vertical_value);
    }
    if (level_value != level_valuep) {
        //Serial.print ("level=");
        //Serial.println (level_value);
    }
    
    vertical_valuep = vertical_value;
    level_valuep = level_value;
    
    int stick_button_val = debouncer.read();
    if (stick_button_val == LOW){
        joystick_mode = !joystick_mode;
        if (joystick_mode) Serial.println ("Joystick mode turned on");
        else Serial.println ("Joystick mode turned off");
        motor.setSpeed(0);
        motor2.setSpeed(0);
    }
    if (joystick_mode){ 
      motor.setSpeed(-level_value);
      motor2.setSpeed(-level_value);
      Serial.print ("Speed=");
      Serial.println (level_value);
    }
   
    else {
      if (ispressed (up_button)){
          motor.setSpeed(0);
          motor2.setSpeed(0);
          //Serial.println ("up pressed");
          }
      if (clicking_mode){
          motor.setSpeed(0); 
          motor2.setSpeed(0);
          clicking_mode = false;
        }
      if (ispressed (down_button)){
          motor.setSpeed(-255);
          motor2.setSpeed(-255);
          clicking_mode = true;
          //Serial.println ("down pressed");
          }
      if (ispressed (left_button)){ 
          motor.setSpeed(-255);
          motor2.setSpeed(-255);
          //Serial.println ("left pressed");
          }
      if (ispressed (right_button)){
          motor.setSpeed(255);
          motor2.setSpeed(255);
          //Serial.println ("right pressed");
          }

    }
    delay (10);
}

//check button
boolean ispressed (uint8_t key) {
    if (digitalRead (key) == 0) {
        delay (key_delay_time);
        if (digitalRead (key) == 0)
            return true;
    }
    return false;
}
