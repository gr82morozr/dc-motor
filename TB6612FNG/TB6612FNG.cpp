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
  // PWM config
  this->pwm = new ESP32PWM(
    this->config.pwm_pin,
    this->config.pwm_channel, 
    this->config.pwm_freq,
    this->config.pwm_resolution,
    0
  );
};




void TB6612FNG::init(void) {
  pinMode(this->config.standby_pin, OUTPUT);
  pinMode(this->config.in1_pin, OUTPUT);
  pinMode(this->config.in2_pin, OUTPUT);
  this->pwm->init();
  // put into standby first
  digitalWrite(this->config.standby_pin, LOW);

};

void TB6612FNG::run(float speed) {
  this->speed = speed;

  // cap the value of speed (max = 1)
  if (abs(speed) > 1.0) {
    this->speed = sign(speed);
  };

  // debug info
  if (_TB6612FNG_DEBUG_) {
    Serial.println("standby_pin:" + String(this->config.standby_pin));
    Serial.println("in1_pin:" + String(this->config.in1_pin));
    Serial.println("in2_pin:" + String(this->config.in2_pin));
    Serial.println("speed:" + String(abs(this->speed)));
  };

  // run
  digitalWrite(this->config.standby_pin, HIGH);
  this->direct();
  this->pwm->run(abs(this->speed));
};

void TB6612FNG::direct() {
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
      //this->brake(); 
      break;
  };
};

void TB6612FNG::brake(void) {
  digitalWrite(this->config.in1_pin, HIGH);
  digitalWrite(this->config.in2_pin, HIGH);
  this->stop();
};

void TB6612FNG::stop(void) {
  digitalWrite(this->config.standby_pin, LOW);
};
