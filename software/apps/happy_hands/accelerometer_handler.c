#include <stdbool.h>
#include <stdio.h>

#include "app_error.h"
#include "nrfx_saadc.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "adc.h"
#include "sensor_to_instrument_interface.h"

#define ACCELEROMETER_INPUT_PIN NRF_SAADC_INPUT_AIN7
#define ACCELEROMETER_ADC_CHANNEL 7
#define ACC_ADC_LOW 1000
#define ACC_ADC_HIGH 2000
#define ACC_TIMER_MS 1000

static bool is_accelerometer_initialized = false;

APP_TIMER_DEF(acc_timer);

static void acc_timer_handler(void *p_context) {
    printf("Timer Fired\n");
    nrf_saadc_event_clear(NRF_SAADC_EVENT_CH7_LIMITH);
    nrf_saadc_event_clear(NRF_SAADC_EVENT_CH7_LIMITL);
    NRFX_IRQ_ENABLE(SAADC_IRQn);
}

static void low_frequency_clk_request() {
    APP_ERROR_CHECK(nrf_drv_clock_init());
    nrf_drv_clock_lfclk_request(NULL);
}

nrf_saadc_value_t get_accelerometer_adc() {
    return sample_adc_value(ACCELEROMETER_ADC_CHANNEL);
}

void handle_accelerometer_interrupt() {
    NRFX_IRQ_DISABLE(SAADC_IRQn);
    raise_volume();
    APP_ERROR_CHECK(app_timer_start(acc_timer, APP_TIMER_TICKS(ACC_TIMER_MS), NULL));
}

void initialize_accelerometer() {
    if(!is_accelerometer_initialized) {
        initialize_adc_channel_with_limits(ACCELEROMETER_INPUT_PIN, ACCELEROMETER_ADC_CHANNEL, ACC_ADC_LOW, ACC_ADC_HIGH);
        low_frequency_clk_request();
        app_timer_init();
        APP_ERROR_CHECK(app_timer_create(&acc_timer, APP_TIMER_MODE_SINGLE_SHOT, acc_timer_handler));
        is_accelerometer_initialized = true;
    }
}
