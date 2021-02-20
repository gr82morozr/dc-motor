#pragma once
#include <Arduino.h>
#include "driver/pcnt.h"

#define _ENCODER_DEBUG_       1
#define _ENCODER_FILTER_      200
#define _DEFAULT_RESOLUTION   15000 


/*
  ESP32 pulse counter (DC Motor Encoder).


  // ========================================================
  //    Sample code:
  // ========================================================
  long count;
  float speed;
  Encoder encoder=Encoder(32, 35);
  // --- or ---
  // Encoder encoder=Encoder(1, 0, 32, 35, 15000);
  void setup() {
    encoder.init();
  }
  void loop() {
    encoderL.read(&count, &speed);
  }
  // ========================================================


  Note:
  - Pulse Counter, unit = 0 ~ 7, channel = 0, 1, total 16 counters
 

  Change Log:
    - 2021-01-31 : updated to simplify the constructors 
*/


class Encoder {
  public:
    Encoder(int pulse_pin, int ctrl_pin);
    Encoder(int pulse_pin, int ctrl_pin, int resolution);
    Encoder(int unit, int channel, int pulse_pin, int ctrl_pin, int resolution);
    void    init();
    void    read(long *count, float *rate);
    long    read_count();
    float   read_rate();
    void    clear();

  private :
    pcnt_unit_t       unit ;
    pcnt_channel_t    channel;
    gpio_num_t        pulse_pin ;
    gpio_num_t        ctrl_pin ;
    pcnt_config_t     config;  
    int               resolution ;
    long              timer       = micros();
    int16_t           int_count   = 0;
    long              count       = 0;
    float             rate        = 0.0; 
    static  int       unit_channel_in_use;
    static void       get_next_unit_channel(pcnt_unit_t *unit, pcnt_channel_t *channel);
    void              debug();
};
