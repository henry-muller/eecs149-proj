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

#define NUMBER_OF_INPUTS 10

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
    PITCH_BEND_DOWN = -1,
    NO_PITCH_BEND,
    PITCH_BEND_UP
} pitch_bend_t;

static bool is_user_input_present[NUMBER_OF_INPUTS];

// TODO: Refactor into bool is_note_index_input_present[8] and bool is_pitch_bend_input_present[2]

static note_index_t user_input_to_note_index(int user_input) {
    note_index_t result;
    if (user_input < 4) {
        result = (note_index_t) user_input;
    } else if (user_input > 5) {
        result = (note_index_t) (user_input - 2);
    }
    return result;
}

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
    B
} musical_key_t;

static musical_key_t key;

static void update_key(int switch_position) {
    key = (musical_key_t) switch_position;
}

static musical_note_t index_to_note(note_index_t note_index) {
    musical_note_t tonic;
    musical_note_t result;
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
            result = tonic;
        case NOTE_2:
            result = tonic + 2;
        case NOTE_3:
            result = tonic + 4;
        case NOTE_4:
            result = tonic + 5;
        case NOTE_5:
            result = tonic + 7;
        case NOTE_6:
            result = tonic + 9;
        case NOTE_7:
            result = tonic + 11;
        case NOTE_8:
            result = tonic + 12;
    }
    return result;
}

static musical_note_t pitch_bent_note(musical_note_t note, pitch_bend_t pitch_bend) {
    return (musical_note_t) (note + pitch_bend);
}

static musical_note_t output_note(note_index_t note_index, pitch_bend_t pitch_bend) {
    musical_note_t note = index_to_note(note_index);
    return pitch_bent_note(note, pitch_bend);
}

static void update_user_inputs(bool* input) {
    memcpy(&is_user_input_present, input, sizeof(bool) * NUMBER_OF_INPUTS);
}

void notes_to_play(musical_note_t* notes) {
    pitch_bend_t pitch_bend;
    if ((is_user_input_present[4] && is_user_input_present[5]) || (!is_user_input_present[4] && !is_user_input_present[5])) {
        pitch_bend = NO_PITCH_BEND;
    } else if (is_user_input_present[4]) {
        pitch_bend = PITCH_BEND_DOWN;
    } else {
        pitch_bend = PITCH_BEND_UP;
    }

    note_index_t note_index;
    musical_note_t note;
    int i;
    for (i = 0; i < 4; i++) {
        if (is_user_input_present[i]) {
            note_index = user_input_to_note_index(i);
            note = output_note(note_index, pitch_bend);
            notes[i] = note;
        } else {
            notes[i] = -1;
        }
    }
    for (i = 6; i < 10; i++) {
        if (is_user_input_present[i]) {
            note_index = user_input_to_note_index(i);
            note = output_note(note_index, pitch_bend);
            notes[i - 2] = note;
        } else {
            notes[i - 2] = -1;
        }
    }
}

//TODO: Make an "update instrument player state" function that can be called from main.c and updates this file's members.
