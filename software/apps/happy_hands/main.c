#include <stdbool.h>
#include <stdio.h>

#include "nrf_drv_uart.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "adc.h"
#include "flex_sensor_handler.h"
#include "i2s_instrument.h"
#include "rotary_switch_handler.h"
#include "accelerometer_handler.h"
#include "instrument_state_controller.h"

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

bool demo_mode = true;

int main(void) { 
    APP_ERROR_CHECK(initialize_rtt());
    printf("RTT working...\n");

    if (demo_mode) {
        initialize_adc();
        initialize_flex_sensors();
        initialize_accelerometer();
        update_flex_sensor_thresholds();
        initialize_rotary_switch();
        instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
        i2s_instrument_init();
        int i;
        while(1) {
            //get_accelerometer_adc();

            for (i = 0; i < 10; i++) {
            printf("%d ",  is_sensor_flexed(i));
            }
            printf("\n");
            printf("%d\n", get_accelerometer_adc());
            printf("VOLUME LEVEL: %d\n", instrument_state.volume_level);
            update_instrument_state(&instrument_state);
            i2s_instrument_play(&instrument_state);
        }
    } else {
        initialize_adc();
        initialize_accelerometer();
        instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
        //instrument_state_t instrument_state = {{NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE, NO_NOTE}, 2};
        //update_instrument_state(&instrument_state);
        // APP_ERROR_CHECK(nrf_drv_clock_init());
        // nrf_drv_clock_lfclk_request(NULL);
        // app_timer_init();
        // APP_ERROR_CHECK(app_timer_create(&acc_timer, APP_TIMER_MODE_SINGLE_SHOT, acc_timer_handler));
        // APP_ERROR_CHECK(app_timer_start(acc_timer, APP_TIMER_TICKS(200), NULL));
        while(1) {
            //printf("vol: %d\n", instrument_state.volume_level);
            //if(!nrfx_saadc_is_busy()) {
                get_accelerometer_adc();
                update_instrument_state(&instrument_state);
                printf("vol: %d\n", instrument_state.volume_level);
                //printf("%d", get_accelerometer_adc());
            //}
            // get_accelerometer_adc();
        }
    }
}
