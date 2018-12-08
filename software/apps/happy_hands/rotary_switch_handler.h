#ifndef ROTARY_SWITCH_HANDLER_H
#define ROTARY_SWITCH_HANDLER_H

// #include <stdbool.h>
// #include <stdint.h>
// #include <stdio.h>

// #include "app_error.h"
// #include "nrf.h"
// #include "nrf_delay.h"
// #include "nrf_gpio.h"
// #include "nrf_log.h"
// #include "nrf_log_ctrl.h"
// #include "nrf_log_default_backends.h"
// #include "nrf_pwr_mgmt.h"
// #include "nrf_serial.h"
// #include "nrfx_gpiote.h"
#include "nrfx_saadc.h"
// #include "buckler.h"

// #include "adc.h"

#define ROTARY_SWITCH_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define ROTARY_SWITCH_ADC_CHANNEL 0

int get_rotary_switch_position();

void initialize_rotary_switch();

#endif /* ROTARY_SWITCH_HANDLER_H */
