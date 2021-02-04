#include <Arduino.h>
#include <Encoder.h>


Encoder::Encoder(int channel, int pinA, int pinB, int resolution) {
  this->encoder_unit          = (pcnt_unit_t) channel;
  this->encoder_channel       = PCNT_CHANNEL_0;
  this->encoder_pinA          = (gpio_num_t) pinA;   // Pulse pin
  this->encoder_pinB          = (gpio_num_t) pinB;   // Control pin
  this->encoder_resolution    = resolution;
    
};

void Encoder::init() {
    gpio_pad_select_gpio(this->encoder_pinA);
    gpio_pad_select_gpio(this->encoder_pinB);
    gpio_set_direction(this->encoder_pinA, GPIO_MODE_INPUT);
    gpio_set_direction(this->encoder_pinB, GPIO_MODE_INPUT);
    gpio_pulldown_en( this->encoder_pinA);
    gpio_pulldown_en(this->encoder_pinB);

    this->encoder_config.pulse_gpio_num = this->encoder_pinA; //Rotary Encoder Chan A
	  this->encoder_config.ctrl_gpio_num = this->encoder_pinB;    //Rotary Encoder Chan B
	  this->encoder_config.unit    = this->encoder_unit;
	  this->encoder_config.channel = PCNT_CHANNEL_0;
	  this->encoder_config.pos_mode = PCNT_COUNT_DIS; //Count Only On Rising-Edges
	  this->encoder_config.neg_mode = PCNT_COUNT_INC;   // Discard Falling-Edge
	  this->encoder_config.lctrl_mode = PCNT_MODE_KEEP;    // Rising A on HIGH B = CW Step
	  this->encoder_config.hctrl_mode = PCNT_MODE_REVERSE; // Rising A on LOW B = CCW Step
	  this->encoder_config.counter_h_lim = INT16_MAX;
	  this->encoder_config.counter_l_lim = INT16_MIN ;


    /* Initialize PCNT unit */
    pcnt_unit_config(&this->encoder_config);
    /* Configure and enable the input filter */
    pcnt_set_filter_value(this->encoder_unit, ENCODER_FILTER); 
    pcnt_filter_enable(this->encoder_unit);
    pcnt_filter_disable(this->encoder_unit);
    /* Initialize PCNT's counter */
    pcnt_counter_pause(this->encoder_unit);
    pcnt_counter_clear(this->encoder_unit);

    /* Everything is set up, now go to counting */
    
    pcnt_counter_resume(this->encoder_unit);

    this->timer = micros();
    if (Encoder_DEBUG == 1) {
      Serial.println(this->encoder_unit);
    };
};

void Encoder::read(long *count, float *rate) {
    pcnt_get_counter_value(this->encoder_unit, &this->thiscount);
    pcnt_counter_clear(this->encoder_unit);
    this->count += (long) this->thiscount;
    this->rate  = (float) ((this->thiscount) /  (float)(micros() - timer) * 1000000.0 / (float) this->encoder_resolution) ;
    timer = micros();      
    *count = this->count;
    *rate  = this->rate;
};


void Encoder::clear() {
  this->count = 0;
};