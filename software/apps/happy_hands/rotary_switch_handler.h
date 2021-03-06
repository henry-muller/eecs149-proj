#ifndef ROTARY_SWITCH_HANDLER_H
#define ROTARY_SWITCH_HANDLER_H

#include "nrfx_saadc.h"

#define ROTARY_SWITCH_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define ROTARY_SWITCH_ADC_CHANNEL 0

int get_rotary_switch_position();

void initialize_rotary_switch();

#endif /* ROTARY_SWITCH_HANDLER_H */
