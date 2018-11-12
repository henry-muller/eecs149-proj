#ifndef ADC_H
#define ADC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"

bool is_adc_initialized();

void initialize_adc();

void initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel);

nrf_saadc_value_t sample_adc_value(uint8_t channel);

#endif /* ADC_H */
