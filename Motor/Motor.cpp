#include <Motor/Motor.h>


Motor::Motor(MotorConfig cfg) {
  this->config = cfg;
};

void Motor::init(MotorConfig cfg) {
  this->config = cfg;
  this->init();
};

void Motor::init() {
  this->encoder = new Encoder(
    this->config.encoder_pulse_pin, 
    this->config.encoder_ctrl_pin, 
    this->config.encoder_resolution
  );
  
  this->driver  = new TB6612FNG({
    this->config.motor_standby_pin, 
    this->config.motor_in1_pin,
    this->config.motor_in2_pin, 
    this->config.motor_pwm_pin,
    _DEFAULT_PWM_FREQ,
    this->config.motor_direction
  });
  this->driver->init();
  this->encoder->init();
};



void  Motor::set(MotorConfig cfg) {
  this->config = cfg;
};

MotorConfig  Motor::get() {
  return this->config;
};


void Motor::run(float speed_rate) {
  this->driver->run(speed_rate);
};

void Motor::stop() {
  this->driver->stop();
};

void Motor::read(long *rcount, float *speed) {
  this->encoder->read(rcount, speed);
  *speed = *speed / this->config.encoder_resolution;
};

void Motor::clear() {
  this->encoder->clear();
};


void Motor::test() {
  for (float s = -1; s <=1; s+=0.1) {
    this->driver->run(s);
    delay(200);
  };
  this->stop();
};


void debug() {
  if (_MOTOR_DEBUG) {
    


  }


}