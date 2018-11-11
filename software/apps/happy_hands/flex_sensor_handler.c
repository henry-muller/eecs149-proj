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
#include "adc.h"

#include "buckler.h"

// #include "instrument_player.h"

#define NUMBER_OF_SENSORS 5

#define SENSOR_0_INPUT_PIN NRF_SAADC_INPUT_AIN0
#define SENSOR_1_INPUT_PIN NRF_SAADC_INPUT_AIN1
#define SENSOR_2_INPUT_PIN NRF_SAADC_INPUT_AIN2
#define SENSOR_3_INPUT_PIN NRF_SAADC_INPUT_AIN3
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN4

#define SENSOR_0_ADC_CHANNEL 0
#define SENSOR_1_ADC_CHANNEL 1
#define SENSOR_2_ADC_CHANNEL 2
#define SENSOR_3_ADC_CHANNEL 3
#define SENSOR_4_ADC_CHANNEL 4

static nrf_saadc_value_t flex_sensor_thresholds[NUMBER_OF_SENSORS];

static nrf_saadc_value_t get_sensor_value(int sensor) {
    nrf_saadc_value_t result = 0;
    switch(sensor) {
        case 0:
            result = sample_adc_value(SENSOR_0_ADC_CHANNEL);
        case 1:
            result = sample_adc_value(SENSOR_1_ADC_CHANNEL);
        case 2:
            result = sample_adc_value(SENSOR_2_ADC_CHANNEL);
        case 3:
            result = sample_adc_value(SENSOR_3_ADC_CHANNEL);
        case 4:
            result = sample_adc_value(SENSOR_4_ADC_CHANNEL);
    }
    return result;
}

static bool is_above_threshold(int sensor_number, nrf_saadc_value_t sensor_value) {
    return sensor_value >= 0.8 * flex_sensor_thresholds[sensor_number];
}

void update_flex_sensor_thresholds() {
    int i;
    int j;
    nrf_saadc_value_t sums[NUMBER_OF_SENSORS];
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        sums[i] = (nrf_saadc_value_t) 0;
    }
    int32_t count = 10; // number of samples to take
    printf("Preparing to calibrate sensors. Please bend all fingers.\n");
    nrf_delay_ms(2000);
    printf("Calibrating...\n");
    for (i = 0; i < count; i++) {
        for (j = 0; j < NUMBER_OF_SENSORS; j++) {
            sums[j] += get_sensor_value(j);
            nrf_delay_ms(10);
        }
    }
    for (i = 0; i < NUMBER_OF_SENSORS; i++) {
        flex_sensor_thresholds[i] = (nrf_saadc_value_t) (sums[i]/count);
    }
}

void initialize_flex_sensors() {
    if (!is_adc_initialized()) {
        initialize_adc();
    }
    initialize_adc_channel(SENSOR_0_INPUT_PIN, SENSOR_0_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_1_INPUT_PIN, SENSOR_1_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_2_INPUT_PIN, SENSOR_2_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_3_INPUT_PIN, SENSOR_3_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_4_INPUT_PIN, SENSOR_4_ADC_CHANNEL);
}

bool is_sensor_flexed(int sensor_number) {
    return is_above_threshold(sensor_number, get_sensor_value(sensor_number));
}
