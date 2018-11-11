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

#include "sensor_to_instrument_interface.h"

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

// static bool is_note_index_input_present[8];
// static bool is_pitch_bend_input_present[2];

// TODO: Refactor into bool is_note_index_input_present[8] and bool is_pitch_bend_input_present[2]

// static note_index_t user_input_to_note_index(int user_input) {
//     note_index_t result;
//     if (user_input < 4) {
//         result = (note_index_t) user_input;
//     } else if (user_input > 5) {
//         result = (note_index_t) (user_input - 2);
//     }
//     return result;
// }



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

static musical_key_t key;

static musical_note_t index_to_note(int note_index) {
    musical_note_t tonic = -1;
    musical_note_t result = -1;
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
        case 1:
            result = tonic;
        case 2:
            result = tonic + 2;
        case 3:
            result = tonic + 4;
        case 4:
            result = tonic + 5;
        case 5:
            result = tonic + 7;
        case 6:
            result = tonic + 9;
        case 7:
            result = tonic + 11;
        case 8:
            result = tonic + 12;
    }
    return result;
}

static musical_note_t pitch_bent_note(musical_note_t note, pitch_bend_t pitch_bend) {
    return (musical_note_t) (note + pitch_bend);
}

static musical_note_t output_note(int note_index, pitch_bend_t pitch_bend) {
    musical_note_t note = index_to_note(note_index);
    return pitch_bent_note(note, pitch_bend);
}

static void update_notes_to_play(instrument_state_t* state) {
    pitch_bend_t pitch_bend = get_pitch_bend();
    int i;
    for (i = 0; i < NUMBER_OF_NOTE_INDICES; i++) {
        if (is_note_index_input_present(i)) {
            state->notes_to_play[i] = output_note(i, pitch_bend);
        } else {
            state->notes_to_play[i] = -1;
        }
    }
}

static void update_key() {
    key = get_key();
}

void update_instrument_state(instrument_state_t* state) {
    update_key();
    update_notes_to_play(state);
    // guess we update volume here too?
}

// Rule: all instrument files have a "play" function that takes in an instrument_state_t struct.
// The state struct should be created in our MAIN() function and updated using instrument_state_controller. Then
// we actually play the instrument using the play() function.
