#include <stdbool.h>
// #include <stdint.h>
#include <stdio.h>

// #include "app_error.h"
// #include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
// #include "nrf_log.h"
// #include "nrf_log_ctrl.h"
// #include "nrf_log_default_backends.h"
// #include "nrf_pwr_mgmt.h"
// #include "nrf_serial.h"
#include "nrfx_gpiote.h"
// #include "buckler.h"

#include "adc.h"
#include "flex_sensor_handler.h"

#define NUMBER_OF_SENSORS 10

#define SENSOR_01_INPUT_PIN NRF_SAADC_INPUT_AIN2 //left pink and ring
#define SENSOR_23_INPUT_PIN NRF_SAADC_INPUT_AIN3 //left middle and pointer
#define SENSOR_4_INPUT_PIN NRF_SAADC_INPUT_AIN1 //left thumb
#define SENSOR_5_INPUT_PIN NRF_SAADC_INPUT_AIN6 //right thumb
#define SENSOR_67_INPUT_PIN NRF_SAADC_INPUT_AIN4 // right pointer and middle
#define SENSOR_89_INPUT_PIN NRF_SAADC_INPUT_AIN5 // right ring and pinky

#define SENSOR_01_ADC_CHANNEL 2 //left pink and ring
#define SENSOR_23_ADC_CHANNEL 3 //left middle and pointer
#define SENSOR_4_ADC_CHANNEL 1 //left thumb
#define SENSOR_5_ADC_CHANNEL 6 //right thumb
#define SENSOR_67_ADC_CHANNEL 4 // right pointer and middle
#define SENSOR_89_ADC_CHANNEL 5 // right ring and pinky

#define S01_SEL NRF_GPIO_PIN_MAP(0,6) //pin 8
#define S23_SEL NRF_GPIO_PIN_MAP(0,7) //pin 9
#define S67_SEL NRF_GPIO_PIN_MAP(0,8) //pin 10
#define S89_SEL NRF_GPIO_PIN_MAP(0,11) //pin 11

#define THRESHOLD_SLACK_FACTOR 0.98

static nrf_saadc_value_t flex_sensor_thresholds[NUMBER_OF_SENSORS] = {2800, 2500, 2700, 2400, 2900, 2300, 2400, 2300, 2600, 2600};

static nrf_saadc_value_t get_sensor_value(int sensor) {
    nrf_saadc_value_t result = 0;
    switch(sensor) {
        case 0:
            nrf_gpio_pin_set(S01_SEL);
            result = sample_adc_value(SENSOR_01_ADC_CHANNEL);
            break;
        case 1:
            nrf_gpio_pin_clear(S01_SEL);
            result = sample_adc_value(SENSOR_01_ADC_CHANNEL);
            break;
        case 2:
            nrf_gpio_pin_set(S23_SEL);
            result = sample_adc_value(SENSOR_23_ADC_CHANNEL);
            break;
        case 3:
            nrf_gpio_pin_clear(S23_SEL);
            result = sample_adc_value(SENSOR_23_ADC_CHANNEL);
            break;
        case 4:
            result = sample_adc_value(SENSOR_4_ADC_CHANNEL);
            break;
        case 5:
            result = sample_adc_value(SENSOR_5_ADC_CHANNEL);
            break;
        case 6:
            nrf_gpio_pin_set(S67_SEL);
            result = sample_adc_value(SENSOR_67_ADC_CHANNEL);
            break;
        case 7:
            nrf_gpio_pin_clear(S67_SEL);
            result = sample_adc_value(SENSOR_67_ADC_CHANNEL);
            break;
        case 8:
            nrf_gpio_pin_set(S89_SEL);
            result = sample_adc_value(SENSOR_89_ADC_CHANNEL);
            break;
        case 9:
            nrf_gpio_pin_clear(S89_SEL);
            //nrf_delay_ms(1);
            result = sample_adc_value(SENSOR_89_ADC_CHANNEL);
            break;
    }
    // printf("According to sample_adc_value, sensor %d reading is %d\n", sensor, result);
    return result;
}

static bool is_above_threshold(int sensor_number, nrf_saadc_value_t sensor_value) {
    // printf("Threshold for sensor %d is %d\n", sensor_number, flex_sensor_thresholds[sensor_number]);
    return sensor_value >= THRESHOLD_SLACK_FACTOR * flex_sensor_thresholds[sensor_number];
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
    nrf_gpio_cfg_output(S01_SEL);
    nrf_gpio_cfg_output(S23_SEL);
    nrf_gpio_cfg_output(S67_SEL);
    nrf_gpio_cfg_output(S89_SEL);

    initialize_adc_channel(SENSOR_01_INPUT_PIN, SENSOR_01_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_23_INPUT_PIN, SENSOR_23_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_4_INPUT_PIN, SENSOR_4_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_5_INPUT_PIN, SENSOR_5_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_67_INPUT_PIN, SENSOR_67_ADC_CHANNEL);
    initialize_adc_channel(SENSOR_89_INPUT_PIN, SENSOR_89_ADC_CHANNEL);
}

bool is_sensor_flexed(int sensor_number) {
    return is_above_threshold(sensor_number, get_sensor_value(sensor_number));
}
