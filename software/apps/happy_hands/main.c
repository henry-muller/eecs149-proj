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
//#include "nrf_i2s.h"
//#include "nrfx_i2s.h"

#include "buckler.h"
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

int main(void) { 
    // initialize RTT library
    APP_ERROR_CHECK(initialize_rtt());

    // initialize ADC
    initialize_adc();

    //nrf_delay_ms(2000);
    printf("RTT working...\n");

    //instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
    //printf("Line 47\n");
    //i2s_instrument_init();
    //printf("Help me pls\n");
    //i2s_instrument_play(&instrument_state);
    //printf("Line 49\n");
    //instrument_state.notes_to_play[0] = C4;
    //instrument_state.notes_to_play[1] = B3;
    //i2s_instrument_play_hal(&instrument_state);
    // while(1) {
    //     __WFE();
    // }
    // while(1) {
    //     printf("In while loop\n");
    //     i2s_instrument_play_hal(&instrument_state);
    //     //nrf_delay_ms(10);
    // }



    // Calibrate sensors
    //initialize_flex_sensors();
    //update_flex_sensor_thresholds();
    initialize_rotary_switch();
    //int i;
    // while(1) {
    //     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
    //         printf("%d ", is_sensor_flexed(i));
    //     }
    //     printf("%d\n", get_rotary_switch_position());
    // } 
    while(1) {
        printf("%d\n", get_rotary_switch_position());
        nrf_delay_ms(100);
    }
    //nrf_gpio_pin_set(S01_SEL);
    //nrf_gpio_pin_clear(S01_SEL);
    //nrf_gpio_pin_set(S23_SEL);
    //nrf_gpio_pin_clear(S23_SEL);
    //nrf_gpio_pin_set(S67_SEL);
    //nrf_gpio_pin_clear(S67_SEL);
    //nrf_gpio_pin_set(S89_SEL);
    //nrf_gpio_pin_clear(S89_SEL);


    //initialize_rotary_switch();
    
    // instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
    // i2s_instrument_init_hal();
    // while(1) {
    //     for (i = 0; i < NUMBER_OF_SENSORS; i++) {
    //         printf("%d ", is_sensor_flexed(i));
    //     }
    //     printf("%d\n", get_rotary_switch_position());
    //     update_instrument_state(&instrument_state);
    //     i2s_instrument_play_hal(&instrument_state);
    // }
    /*
    while(1) {
        __WFE();
        
        int i;
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_sensor_flexed(i));
        }
        printf("\n");
    */
        //update_instrument_state(&instrument_state);
        //i2s_instrument_play(&instrument_state);
}
