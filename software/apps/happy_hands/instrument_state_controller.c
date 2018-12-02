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

#define MIN_VOLUME 0
#define MAX_VOLUME 5

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
            break;
        case 2:
            result = tonic + 2;
            break;
        case 3:
            result = tonic + 4;
            break;
        case 4:
            result = tonic + 5;
            break;
        case 5:
            result = tonic + 7;
            break;
        case 6:
            result = tonic + 9;
            break;
        case 7:
            result = tonic + 11;
            break;
        case 8:
            result = tonic + 12;
            break;
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
        if (is_note_index_input_present(i + 1)) {
            state->notes_to_play[i] = output_note(i + 1, pitch_bend);
        } else {
            state->notes_to_play[i] = NO_NOTE;
        }
    }
}

static void update_volume_level(instrument_state_t* state) {
    volume_command_t volume_command = get_volume_command();
    switch(volume_command) 
    // TODO: Switch on volume_command, which is DOWN, HOLD, or UP.
    // Update state->volume_level accordingly.
    // Use the constants MIN_VOLUME and MAX_VOLUME to avoid overrun
    
}

static void update_key() {
    key = get_key();
}

void update_instrument_state(instrument_state_t* state) {
    update_key();
    update_notes_to_play(state);
    update_volume_level(state);
}

// Rule: all instrument files have a "play" function that takes in an instrument_state_t struct.
// The state struct should be created in our MAIN() function and updated using instrument_state_controller. Then
// we actually play the instrument using the play() function.
