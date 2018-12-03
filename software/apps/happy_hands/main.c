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
//#include "nrf_drv_pwm.h"
//#include "nrf_drv_clock.h"
//#include "nrfx_pwm.h"

#include "buckler.h"
// #include "pin_assignments.h"
#include "adc.h"
#include "flex_sensor_handler.h"
#include "i2s_instrument.h"
#include "rotary_switch_handler.h"
#include "instrument_state_controller.h"
#include "sensor_to_instrument_interface.h"

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

int main(void) { // Note that I renamed this to _main so that it wouldn't conflict with the main in main_i2s.c
    // initialize RTT library
    ret_code_t error_code = NRF_SUCCESS;
    error_code = initialize_rtt();
    APP_ERROR_CHECK(error_code);

    // initialize ADC
    initialize_adc();

    //nrf_delay_ms(2000);
    printf("RTT working...\n");
    nrf_delay_ms(3000);
    // Calibrate sensors
    initialize_flex_sensors();
    update_flex_sensor_thresholds();
    /*
    int i;
    while(1) {
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_sensor_flexed(i));
        }
        printf("\n");
    } */

    nrf_gpio_pin_set(S01_SEL);
    //nrf_gpio_pin_clear(S01_SEL);
    //nrf_gpio_pin_set(S23_SEL);
    //nrf_gpio_pin_clear(S23_SEL);
    //nrf_gpio_pin_set(S67_SEL);
    //nrf_gpio_pin_clear(S67_SEL);
    //nrf_gpio_pin_set(S89_SEL);
    //nrf_gpio_pin_clear(S89_SEL);


    //initialize_rotary_switch();

    /*
    instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
    i2s_instrument_init();
    i2s_instrument_play(&instrument_state);
    while(1) {
        __WFE();
        /*
        int i;
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_sensor_flexed(i));
        }
        printf("\n");
        */
        //update_instrument_state(&instrument_state);
        //i2s_instrument_play(&instrument_state);
}
