#pragma once

#include <Arduino.h>
#include <PWM/ESP32PWM.h> 



/*
  TB6612FNG DC Motor Driver
  https://www.instructables.com/Driving-Small-Motors-With-the-TB6612FNG/

  // ========================================================
  //    Sample code:
  // ========================================================
 
  // ========================================================


 
*/



#define _TB6612FNG_DEBUG_  0

// TB6612FNG has maximum PWM switching frequency of 100kHz.
#define _DEFAULT_PWM_FREQ             50000
#define _DEFAULT_PWM_RESOLUTION_BITS  10

struct TB6612FNG_Config {
  int standby_pin;
  int in1_pin;
  int in2_pin;
  int pwm_pin;       
  int pwm_freq;
  int direction;
};

class TB6612FNG {
  public:
    TB6612FNG_Config config;
    TB6612FNG(TB6612FNG_Config s);
    void init(void);
    void run(float speed);
    void stop(void);
  private:
    void set_direction(); 
    void debug();        
    ESP32PWM *pwm;
    float speed;
};