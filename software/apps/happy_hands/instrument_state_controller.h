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
#include "nrf_drv_pwm.h"

// typedef enum {
//     NOTE_1 = 1,
//     NOTE_2,
//     NOTE_3,
//     NOTE_4,
//     NOTE_5,
//     NOTE_6,
//     NOTE_7,
//     NOTE_8
// } note_index_t;

// typedef enum {
//     PITCH_BEND_DOWN = -1,
//     NO_PITCH_BEND,
//     PITCH_BEND_UP
// } pitch_bend_t;

typedef enum {
    B3,

    C4,
    D4_FLAT,
    D4,
    E4_FLAT,
    E4,
    F4,
    G4_FLAT,
    G4,
    A4_FLAT,
    A4,
    B4_FLAT,
    B4,

    C5,
    D5_FLAT,
    D5,
    E5_FLAT,
    E5,
    F5,
    G5_FLAT,
    G5,
    A5_FLAT,
    A5,
    B5_FLAT,
    B5,

    C6
} musical_note_t;

// typedef enum {
//     C,
//     D_FLAT,
//     D,
//     E_FLAT,
//     E,
//     F,
//     G_FLAT,
//     G,
//     A_FLAT,
//     A,
//     B_FLAT,
//     B
// } musical_key_t;

typedef struct {
    musical_note_t notes_to_play[NUMBER_OF_NOTE_INDICES];
} instrument_state_t;

void play_instrument(instrument_state_t* state);


