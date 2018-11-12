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
#include "nrf_drv_pwm.h"
//#include "nrf_drv_clock.h"
//#include "nrfx_pwm.h"

#include "buckler.h"
// #include "pin_assignments.h"
#include "adc.h"
#include "flex_sensor_handler.h"
#include "pwm_instrument.h"


ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

int main() {
    //----------Initialization stuff--------------------------------------------------------------------------------
    // initialize RTT library
    ret_code_t error_code = NRF_SUCCESS;
    error_code = initialize_rtt();
    APP_ERROR_CHECK(error_code);

    // initialize ADC
    initialize_adc();

    // initialize analog inputs
    // nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
    //channel_config.gain = NRF_SAADC_GAIN1_6; // GAIN = 1.6
    //channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // REFERENCE = 0.6

    // NOTE IN CASE THINGS BREAK WITH THE ADC: I commented out the .gain and .reference things because
    // it LOOKS like those are already set with the default channel config thing. If that turns out to have
    // been a mistake, will have to rework.

    initialize_flex_sensors();
    //----------End initialization stuff-----------------------------------------------------------------------------
    
    // // Start clock for accurate frequencies
    // NRF_CLOCK->TASKS_HFCLKSTART = 1; 
    // // Wait for clock to start
    // while(NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    // int i;
    // for (i = 0; i < 10; i++) {
    //     play_C_scale(500);
    // }

    
    nrf_delay_ms(2000);
    printf("RTT working...\n");

    // Calibrate sensors
    //update_flex_sensor_thresholds();

    nrf_delay_ms(5000);

    int i;
    while (1) {
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_sensor_flexed(i));
        }
        printf("\n");
        nrf_delay_ms(1);
    }
}
