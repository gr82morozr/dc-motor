#pragma once

#include <Arduino.h>
#include <ESP32PWM.h> 

#define TB6612FNG_DEBUG  0


// TB6612FNG has maximum PWM switching frequency of 100kHz.
#define DEFAULT_PWM_FREQ       50000
#define DEFAULT_PWM_RESOLUTION 10


struct TB6612FNGConfig {
  int channel;
  int standby_pin;
  int in1_pin;
  int in2_pin;
  int pwm_pin;
  int pwm_freq;
  int pwm_resolution_bits;
};

class TB6612FNG_Driver {
  public:
    TB6612FNGConfig config;
    TB6612FNG_Driver(TB6612FNGConfig s);
    void init(void);
    void run(float speed);
    void rotate(float angle);
    void brake(void);
    void stop(void);
  private:
    void direct();      
    ESP32PWM *pwm;
    float speed;
};