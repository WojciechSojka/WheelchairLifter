#include <Arduino.h>

#define level_stick A0
#define vertical_stick A1
#define stick_button 8
int up_button = 2;
int right_button = 3;
int down_button = 6;
int left_button = 5;
int vertical_value=0;   
int level_value=0;      
int vertical_valuep=0;  
int level_valuep=0;    
bool joystick_mode=false;
bool clicking_mode=false;
uint16_t key_delay_time = 100; 
int motor_1_PWM = 11;
int motor_1_DIR = 13;
int motor_2_PWM = 9;
int motor_2_DIR = 4;

void setup () {
    pinMode (stick_button, INPUT_PULLUP);
    pinMode (level_stick, INPUT);
    pinMode (vertical_stick, INPUT);

    pinMode (up_button, INPUT);
    pinMode (down_button, INPUT);
    pinMode (left_button, INPUT);
    pinMode (right_button, INPUT);
    pinMode (motor_1_DIR, OUTPUT);
    pinMode (motor_2_DIR, OUTPUT);
    pinMode (motor_1_PWM, OUTPUT);
    pinMode (motor_2_PWM, OUTPUT);
    Serial.begin (9600);
    stop_motor(motor_1_PWM);
    stop_motor(motor_2_PWM);
}

void loop () { 
    vertical_value = ( analogRead (vertical_stick) - 524 ) /2.2;
    level_value = ( analogRead (level_stick) - 524 ) / 2.2;

    if (level_value > -20 && level_value < 20) level_value=0; 

    vertical_valuep = vertical_value;
    level_valuep = level_value;
    
    if (ispressed(stick_button)){
        joystick_mode = !joystick_mode;
        if (joystick_mode) Serial.println ("Joystick mode turned on");
        else Serial.println ("Joystick mode turned off");
        stop_motor(motor_1_PWM);
        stop_motor(motor_2_PWM);
    }
    if (joystick_mode){ 
      run_motor_joystick(motor_1_DIR, motor_1_PWM, -value);
      run_motor_joystick(motor_2_DIR, motor_2_PWM, -value);
      Serial.print ("Speed=");
      Serial.println (level_value);
    }
   
    else {, 
      if (ispressed (up_button)){
          run_motor_forward(motor_1_DIR, motor_1_PWM);
          run_motor_forward(motor_2_DIR, motor_2_PWM);
          Serial.println ("up pressed");
          }
      if (clicking_mode){
          stop_motor(motor_1_PWM);
          stop_motor(motor_2_PWM);
          clicking_mode = false;
        }
      if (ispressed (down_button)){
          run_motor_backward(motor_1_DIR, motor_1_PWM);
          run_motor_backward(motor_2_DIR, motor_2_PWM);
          clicking_mode = true;
          Serial.println ("down pressed");
          }
      if (ispressed (left_button)){ 
          run_motor_backward(motor_1_DIR, motor_1_PWM);
          run_motor_backward(motor_2_DIR, motor_2_PWM);
          Serial.println ("left pressed");
          }
      if (ispressed (right_button)){
          run_motor_backward(motor_1_DIR, motor_1_PWM);
          run_motor_backward(motor_2_DIR, motor_2_PWM);
          Serial.println ("right pressed");
          }

    }
    delay (10);
}


void run_motor_joystick(int pin_dir, int pwm_dir, value){
   if (value > 0) digitalWrite(pin_dir, HIGH);
   else digitalWrite(pin_dir, LOW);
   analogWrite(pwm_dir, value);
   
  }
void run_motor_forward(int pin_dir, int pin_pwm){
    digitalWrite(pin_dir, HIGH);
    digitalWrite(pin_pwm, HIGH);
  }

void run_motor_backward(int pin_dir, int pin_pwm){
    digitalWrite(pin_dir, LOW);
    digitalWrite(pin_pwm, HIGH);
  }
  
void stop_motor(int pin_pwm){
    digitalWrite(pin_pwm, LOW);
  }
  
boolean ispressed (uint8_t key) {
    if (digitalRead (key) == 0) {
        delay (key_delay_time);
        if (digitalRead (key) == 0)
            return true;
    }
    return false;
}
