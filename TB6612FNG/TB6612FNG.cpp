#include <Arduino.h>
#include <Common/Common.h> 
#include <TB6612FNG/TB6612FNG.h>


/*************************************************
 * 
 *  Constructor
 * 
 ************************************************/

TB6612FNG::TB6612FNG(TB6612FNG_Config c) {
  this->config = c;
  switch (this->config.direction) {
    case 1 :
      break;
    case -1: // swap control pins to change direction
      int t_pin;
      t_pin = this->config.in1_pin;
      this->config.in1_pin = this->config.in2_pin;
      this->config.in2_pin = t_pin;
      break;
    default:
      // pass, same as default = 1
      break;
  }
};


void TB6612FNG::init(void) {

  // create pwm instance
  this->pwm = new ESP32PWM(
    this->config.pwm_pin,
    this->config.pwm_freq,
    _DEFAULT_PWM_RESOLUTION_BITS,
    0
  );

  pinMode(this->config.standby_pin, OUTPUT);
  pinMode(this->config.in1_pin, OUTPUT);
  pinMode(this->config.in2_pin, OUTPUT);
  // put into standby mode first
  digitalWrite(this->config.standby_pin, LOW);

  this->pwm->init();
  this->debug();
};


void TB6612FNG::run(float speed) {
  this->speed = speed;

  // cap the value of speed (max = 1)
  if (abs(speed) > 1.0) {
    this->speed = sign(speed);
  };

  // run
  digitalWrite(this->config.standby_pin, HIGH);
  this->set_direction();
  this->pwm->run(abs(this->speed));
};

void TB6612FNG::set_direction() {
  // set the motor spin direction
  switch(sign(this->speed)) {
    case 1 :
      digitalWrite(this->config.in1_pin, HIGH);
      digitalWrite(this->config.in2_pin, LOW);  
      break;
    case -1 :
      digitalWrite(this->config.in1_pin, LOW);
      digitalWrite(this->config.in2_pin, HIGH);  
      break;
    case 0 :
      this->stop(); 
      break;
  };
};



void TB6612FNG::stop(void) {
  digitalWrite(this->config.standby_pin, LOW);
};



void TB6612FNG::debug() {
  // debug info
  if (_TB6612FNG_DEBUG_) {
    Serial.println("DEBUG:  standby_pin:" + String(this->config.standby_pin));
    Serial.println("DEBUG:  in1_pin:" + String(this->config.in1_pin));
    Serial.println("DEBUG:  in2_pin:" + String(this->config.in2_pin));
    Serial.println("DEBUG:  speed:" + String(abs(this->speed)));
  };
}