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
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "buckler.h"
#include "adc.h"

#define ACCELEROMETER_INPUT_PIN NRF_SAADC_INPUT_AIN7
#define ACCELEROMETER_ADC_CHANNEL 7

static bool is_accelerometer_initialized = false;

APP_TIMER_DEF(acc_timer);

static void acc_timer_handler(void *p_context) {
    NRFX_IRQ_ENABLE(SAADC_IRQn);
}

static void low_frequency_clk_request() {
    APP_ERROR_CHECK(nrf_drv_clock_init());
    nrf_drv_clock_lfclk_request(NULL);
}

nrf_saadc_value_t get_accelerometer_adc() {
    // if (count == 100) {
    return sample_adc_value(ACCELEROMETER_ADC_CHANNEL);
    //     last_value = adc_value;
    //     count = 0;
    // } else {
    //     count++;
    // }
    // return last_value;
}

void handle_accelerometer_low() {
    // something with state controller
    NRFX_IRQ_DISABLE(SAADC_IRQn);
    APP_ERROR_CHECK(app_timer_start(acc_timer, APP_TIMER_TICKS(2000), NULL));
}

void initialize_accelerometer() {
    if(!is_accelerometer_initialized) {
        initialize_adc_channel_with_limits(ACCELEROMETER_INPUT_PIN, ACCELEROMETER_ADC_CHANNEL, 1400, 1800);
        low_frequency_clk_request();
        app_timer_init();
        APP_ERROR_CHECK(app_timer_create(&acc_timer, APP_TIMER_MODE_SINGLE_SHOT, acc_timer_handler));
        is_accelerometer_initialized = true;
    }
}
