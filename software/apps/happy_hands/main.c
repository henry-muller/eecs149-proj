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
#include "rotary_switch_handler.h"
#include "sensor_to_instrument_interface.h"

ret_code_t initialize_rtt() {
    ret_code_t error_code = NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    return error_code;
}

int main() {
    // initialize RTT library
    ret_code_t error_code = NRF_SUCCESS;
    error_code = initialize_rtt();
    APP_ERROR_CHECK(error_code);

    // initialize ADC
    initialize_adc();

    nrf_delay_ms(2000);
    printf("RTT working...\n");

    // Calibrate sensors
    // update_flex_sensor_thresholds();

    int i;
    while (1) {
        for (i = 0; i < NUMBER_OF_SENSORS; i++) {
            printf("%d ", is_sensor_flexed(i));
        }
        printf("\n");
        nrf_delay_ms(1);
    }

    initialize_rotary_switch();

    while (1) {
        printf("%d | %d\n", get_rotary_switch_position(), get_key());
        nrf_delay_ms(1000);
    }
}
