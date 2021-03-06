#pragma once
#include <Arduino.h>
#include <PWM/ESP32PWM.h>
#include <Encoder/Encoder.h>
#include <TB6612FNG/TB6612FNG.h>
#include <PIDController/PIDController.h>


#define _MOTOR_DEBUG        1
#define _MOTOR_TRACE        1
#define _DEFAULT_PWM_FREQ   50000

/**********************************************************************************************
 *
 *  Wrapper class for general DC motor control 
 *  ESP32 Arduino Core + PCNT + TB6612FNG + Motor with Encodeder
 *
 *********************************************************************************************/
 
struct MotorConfig {
  int motor_standby_pin;
  int motor_in1_pin;
  int motor_in2_pin;
  int motor_pwm_pin;       
  int motor_direction;
  int encoder_pulse_pin;
  int encoder_ctrl_pin;
  int encoder_resolution;
};



class Motor {
  public:
    Motor(MotorConfig cfg);
    void init();
    void init(MotorConfig cfg);
    
    void run(double speed_rate);   // -1 <= speed_rate<= 1 , relative speed, not acturate
    void drive(double speed);      // run at acturate speed
    void rotate(double degrees);   // rotate acturate degrees
    void stop();


    void read(long *rcount, double *speed);
    void clear();
    void test();
    void set(MotorConfig cfg);
    MotorConfig get();
    
  private :
    MotorConfig   config        ;
    TB6612FNG     *driver       ;
    Encoder       *encoder      ;
    double         max_speed     ; 
    double         target_speed  ;
    void          debug()       ;
    void          trace()       ;
};