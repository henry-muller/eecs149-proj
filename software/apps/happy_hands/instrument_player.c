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

typedef enum {
    NOTE_1 = 1,
    NOTE_2,
    NOTE_3,
    NOTE_4,
    NOTE_5,
    NOTE_6,
    NOTE_7,
    NOTE_8
} note_index_t;

typedef enum {
    PITCH_BEND_DOWN,
    PITCH_BEND_UP
} pitch_bend_t;

static bool is_input_on[NUMBER_OF_INPUTS];

typedef enum {
    // A0,
    // B0_FLAT,
    // B0,

    // C1,
    // D1_FLAT,
    // D1,
    // E1_FLAT,
    // E1,
    // F1,
    // G1_FLAT,
    // G1,
    // A1_FLAT,
    // A1,
    // B1_FLAT,
    // B1,
    
    // C2,
    // D2_FLAT,
    // D2,
    // E2_FLAT,
    // E2,
    // F2,
    // G2_FLAT,
    // G2,
    // A2_FLAT,
    // A2,
    // B2_FLAT,
    // B2,

    // C3,
    // D3_FLAT,
    // D3,
    // E3_FLAT,
    // E3,
    // F3,
    // G3_FLAT,
    // G3,
    // A3_FLAT,
    // A3,
    // B3_FLAT,
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

    C6,
    // D6_FLAT,
    // D6,
    // E6_FLAT,
    // E6,
    // F6,
    // G6_FLAT,
    // G6,
    // A6_FLAT,
    // A6,
    // B6_FLAT,
    // B6,

    // C7,
    // D7_FLAT,
    // D7,
    // E7_FLAT,
    // E7,
    // F7,
    // G7_FLAT,
    // G7,
    // A7_FLAT,
    // A7,
    // B7_FLAT,
    // B7,

    // C8,

    NUMBER_OF_NOTES
} musical_note_t;

typedef enum {
    C,
    D_FLAT,
    D,
    E_FLAT,
    E,
    F,
    G_FLAT,
    G,
    A_FLAT,
    A,
    B_FLAT,
    B,
    NUMBER_OF_KEYS
} musical_key_t;

musical_note_t note(note_index_t note_index, musical_key_t key) {
    musical_note_t tonic;
    switch(key) {
        case C:
            tonic = C4;
            break;
        case D_FLAT:
            tonic = D4_FLAT;
            break;
        case D:
            tonic = D4;
            break;
        case E_FLAT:
            tonic = E4_FLAT;
            break;
        case E:
            tonic = E4;
            break;
        case F:
            tonic = F4;
            break;
        case G_FLAT:
            tonic = G4_FLAT;
            break;
        case G:
            tonic = G4;
            break;
        case A_FLAT:
            tonic = A4_FLAT;
            break;
        case A:
            tonic = A4;
            break;
        case B_FLAT:
            tonic = B4_FLAT;
            break;
        case B:
            tonic = B4;
            break;
    }
    switch(note_index) {
        case NOTE_1:
            return tonic;
        case NOTE_2:
            return tonic + 2;
        case NOTE_3:
            return tonic + 4;
        case NOTE_4:
            return tonic + 5;
        case NOTE_5:
            return tonic + 7;
        case NOTE_6:
            return tonic + 9;
        case NOTE_7:
            return tonic + 11;
        case NOTE_8:
            return tonic + 12;
    }
}
