#pragma once
#include <Arduino.h>
#include "driver/pcnt.h"


#define Encoder_DEBUG  0
#define ENCODER_FILTER 200

class Encoder {
  public:
    Encoder(int channel, int pinA, int pinB, int resolution);
    int encoder_channel;
    gpio_num_t  encoder_pinA ;
    gpio_num_t encoder_pinB ;
    int encoder_resolution ;
    pcnt_unit_t encoder_unit ;
    pcnt_config_t encoder_config;

    int16_t thiscount  = 0;
    long    count = 0;
    float   rate  = 0.0;
    void    init();
    void    read(long *count, float *rate );
    void    clear();

  private :
    long timer = micros();
};
