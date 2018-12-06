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
#include "accelerometer_handler.h"
#include "instrument_state_controller.h"
#include "sensor_to_instrument_interface.h"

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

bool demo_mode = true;

int main(void) { 
    // initialize RTT library
    APP_ERROR_CHECK(initialize_rtt());

    // initialize ADC
    initialize_adc();

    //nrf_delay_ms(2000);
    printf("RTT working...\n");

    // for (i = 0; i < NUMBER_OF_SENSORS; i++) {
    //   printf("%d ",  get_sensor_value(i));
    // }
    // printf("\n");
    // printf("%d\n", get_rotary_switch_position());


    instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
    if (demo_mode) {
        initialize_flex_sensors();
        initialize_rotary_switch();
        i2s_instrument_init();
        while(1) {
            update_instrument_state(&instrument_state);
            i2s_instrument_play(&instrument_state);
        }
    } else {
        initialize_accelerometer();
        while(1) {
            printf("vol: %d\n", instrument_state.volume_level);
            printf("acc: %d\n", get_accelerometer_adc());
        }
    }
}
