#include <Arduino.h>
#include <Common/Common.h>
#include <Encoder/Encoder.h>


int Encoder::unit_channel_in_use = 0;

Encoder::Encoder(int pulse_pin, int ctrl_pin) {
  this->get_next_unit_channel(&this->unit, &this->channel);
  this->pulse_pin     = (gpio_num_t) pulse_pin;   // Pulse pin
  this->ctrl_pin      = (gpio_num_t) ctrl_pin;    // Control pin
  this->resolution    = _DEFAULT_RESOLUTION;
};

Encoder::Encoder(int pulse_pin, int ctrl_pin, int resolution) {
  this->get_next_unit_channel(&this->unit, &this->channel);
  this->pulse_pin     = (gpio_num_t) pulse_pin;   // Pulse pin
  this->ctrl_pin      = (gpio_num_t) ctrl_pin;    // Control pin
  this->resolution    = resolution;
};


Encoder::Encoder(int unit, int channel, int pulse_pin, int ctrl_pin, int resolution) {
  this->unit          = (pcnt_unit_t) unit;
  this->channel       = (pcnt_channel_t) channel;
  this->pulse_pin     = (gpio_num_t) pulse_pin;   // Pulse pin
  this->ctrl_pin      = (gpio_num_t) ctrl_pin;    // Control pin
  this->resolution    = resolution;
};

void Encoder::init() {
  gpio_pad_select_gpio(this->pulse_pin);
  gpio_pad_select_gpio(this->ctrl_pin);
  gpio_set_direction(this->pulse_pin, GPIO_MODE_INPUT);
  gpio_set_direction(this->ctrl_pin, GPIO_MODE_INPUT);
  gpio_pulldown_en(this->pulse_pin);
  gpio_pulldown_en(this->ctrl_pin);

  this->config.pulse_gpio_num = this->pulse_pin;     // Pulse PIN
	this->config.ctrl_gpio_num  = this->ctrl_pin;     // Control PIN
	this->config.unit           = this->unit;
	this->config.channel        = this->channel;
	this->config.pos_mode       = PCNT_COUNT_DIS;     //Count Only On Rising-Edges
	this->config.neg_mode       = PCNT_COUNT_INC;     // Discard Falling-Edge
	this->config.lctrl_mode     = PCNT_MODE_KEEP;     // Rising A on HIGH B = CW Step
	this->config.hctrl_mode     = PCNT_MODE_REVERSE;  // Rising A on LOW B = CCW Step
	this->config.counter_h_lim  = INT16_MAX;
	this->config.counter_l_lim  = INT16_MIN ;


  /* Initialize PCNT unit */
  pcnt_unit_config(&this->config);
  /* Configure and enable the input filter */
  pcnt_set_filter_value(this->unit, _ENCODER_FILTER_); 
  pcnt_filter_enable(this->unit);
  pcnt_filter_disable(this->unit);
  /* Initialize PCNT's counter */
  pcnt_counter_pause(this->unit);
  pcnt_counter_clear(this->unit);

  /* Everything is set up, now start counting */
  pcnt_counter_resume(this->unit);

  this->timer = micros();
  if (_ENCODER_DEBUG_ == 1) {
    Serial.println("unit_channel_in_use:" + String(this->unit_channel_in_use) + ", unit:" + String(this->unit) + ", channel:"+ String(this->channel));
  };
};

void Encoder::read(long *count, double *rate) {
    pcnt_get_counter_value(this->unit, &this->int_count);
    pcnt_counter_clear(this->unit);
    this->count += (long) this->int_count;
    this->rate  = (double) ((this->int_count) /  (double)(micros() - timer) * 1000000.0 ) ;
    timer = micros();      
    *count = this->count;
    *rate  = this->rate;
};


void Encoder::clear() {
  pcnt_counter_clear(this->unit);
  this->count = 0;
};

long Encoder::read_count() {
  return (this->count);
}

void Encoder::get_next_unit_channel(pcnt_unit_t *unit, pcnt_channel_t *channel) {
  if ( Encoder::unit_channel_in_use >=0 && Encoder::unit_channel_in_use <7 ) {
    Encoder::unit_channel_in_use ++;
    *unit = (pcnt_unit_t) Encoder::unit_channel_in_use;
    *channel = PCNT_CHANNEL_0;
  } else {
    Encoder::unit_channel_in_use = 0;
    *unit = (pcnt_unit_t) 0;
    *channel = PCNT_CHANNEL_0;
  } 
}
