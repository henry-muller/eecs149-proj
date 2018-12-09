#ifndef ADC_H
#define ADC_H

#include <stdbool.h>
#include <stdint.h>

#include "nrfx_saadc.h"

bool is_adc_initialized();

void initialize_adc();

void initialize_adc_channel(nrf_saadc_input_t pin, uint8_t channel);

void initialize_adc_channel_with_limits(nrf_saadc_input_t pin, uint8_t channel, int16_t limit_low, int16_t limit_high);

nrf_saadc_value_t sample_adc_value(uint8_t channel);

#endif /* ADC_H */
