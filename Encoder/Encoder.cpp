#include <Arduino.h>
#include <Encoder/Encoder.h>
#include <Common/Common.h>

int Encoder::unit_channel_in_use = 0;

Encoder::Encoder(int pinA, int pinB) {
  this->get_next_unit_channel(&this->unit, &this->channel);
  this->pinA          = (gpio_num_t) pinA;   // Pulse pin
  this->pinB          = (gpio_num_t) pinB;   // Control pin
  this->resolution    = _DEFAULT_RESOLUTION_;
};

Encoder::Encoder(int unit, int channel, int pinA, int pinB, int resolution) {
  this->unit          = (pcnt_unit_t) unit;
  this->channel       = (pcnt_channel_t) channel;
  this->pinA          = (gpio_num_t) pinA;   // Pulse pin
  this->pinB          = (gpio_num_t) pinB;   // Control pin
  this->resolution    = resolution;
};

void Encoder::init() {
    gpio_pad_select_gpio(this->pinA);
    gpio_pad_select_gpio(this->pinB);
    gpio_set_direction(this->pinA, GPIO_MODE_INPUT);
    gpio_set_direction(this->pinB, GPIO_MODE_INPUT);
    gpio_pulldown_en( this->pinA);
    gpio_pulldown_en(this->pinB);

    this->config.pulse_gpio_num = this->pinA;     // Pulse PIN
	  this->config.ctrl_gpio_num  = this->pinB;     // Control PIN
	  this->config.unit           = this->unit;
	  this->config.channel        = this->channel;
	  this->config.pos_mode       = PCNT_COUNT_DIS; //Count Only On Rising-Edges
	  this->config.neg_mode       = PCNT_COUNT_INC; // Discard Falling-Edge
	  this->config.lctrl_mode     = PCNT_MODE_KEEP; // Rising A on HIGH B = CW Step
	  this->config.hctrl_mode     = PCNT_MODE_REVERSE; // Rising A on LOW B = CCW Step
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

    /* Everything is set up, now go to counting */
    
    pcnt_counter_resume(this->unit);

    this->timer = micros();
    if (_ENCODER_DEBUG_ == 1) {
      Serial.println("unit_channel_in_use:" + String(this->unit_channel_in_use) + ", unit:" + String(this->unit) + ", channel:"+ String(this->channel));
    };
};

void Encoder::read(long *count, float *rate) {
    pcnt_get_counter_value(this->unit, &this->int_count);
    pcnt_counter_clear(this->unit);
    this->count += (long) this->int_count;
    this->rate  = (float) ((this->int_count) /  (float)(micros() - timer) * 1000000.0 / (float) this->resolution) ;
    timer = micros();      
    *count = this->count;
    *rate  = this->rate;
};


void Encoder::reset() {
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
