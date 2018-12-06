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
#include "nrf_i2s.h"
#include "nrfx_i2s.h"
#include "nrf_drv_i2s.h"

#include "types.h"
#include "piano.h"

#define SCK_PIN 14 // same as BCK
#define LRCK_PIN 15
#define MCK_PIN 13
#define SDOUT_PIN 16 // same as DIN

#define BUFFER_LENGTH 7500

static int16_t* note_arrays[NUMBER_OF_NOTES] = {
    B3_array,
    C4_array,
    D4_FLAT_array,
    D4_array,
    E4_FLAT_array,
    E4_array,
    F4_array,
    G4_FLAT_array,
    G4_array,
    A4_FLAT_array,
    A4_array,
    B4_FLAT_array,
    B4_array,
    C5_array,
    D5_FLAT_array,
    D5_array,
    E5_FLAT_array,
    E5_array,
    F5_array,
    G5_FLAT_array,
    G5_array,
    A5_FLAT_array,
    A5_array,
    B5_FLAT_array,
    B5_array,
    C6_array,
    NO_NOTE_array
};

static const int note_lengths[NUMBER_OF_NOTES] = {
    B3_LENGTH,
    C4_LENGTH,
    D4_FLAT_LENGTH,
    D4_LENGTH,
    E4_FLAT_LENGTH,
    E4_LENGTH,
    F4_LENGTH,
    G4_FLAT_LENGTH,
    G4_LENGTH,
    A4_FLAT_LENGTH,
    A4_LENGTH,
    B4_FLAT_LENGTH,
    B4_LENGTH,
    C5_LENGTH,
    D5_FLAT_LENGTH,
    D5_LENGTH,
    E5_FLAT_LENGTH,
    E5_LENGTH,
    F5_LENGTH,
    G5_FLAT_LENGTH,
    G5_LENGTH,
    A5_FLAT_LENGTH,
    A5_LENGTH,
    B5_FLAT_LENGTH,
    B5_LENGTH,
    C6_LENGTH,
    NO_NOTE_LENGTH
};

static bool is_note_present[NUMBER_OF_NOTES] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

static int current_note_array_locations[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static int16_t tx_buffer_0[BUFFER_LENGTH] = {0};
static int16_t tx_buffer_1[BUFFER_LENGTH] = {0};

static bool is_buffer_1_tx = false;
static bool swing = true;
static int shrink = 5; // Don't let me forget to use this HAHAHAHAHAHA KMS

int16_t get_next_note_in_array(musical_note_t note) {
    const int16_t* note_array = note_arrays[note];
    int note_array_length = note_lengths[note];
    int current_note_array_location = current_note_array_locations[note];
    int16_t result = note_array[current_note_array_location];
    current_note_array_locations[note] = (current_note_array_locations[note] + 1) % note_array_length;
    return result / shrink;
}

static void update_tx_buffer(int16_t *buffer) {
    //printf("update_tx_buffer called\n");
    int i;
    for (i = 0; i < BUFFER_LENGTH; i++) {
        buffer[i] = 0;
        if (is_note_present[B3]) {
            buffer[i] += get_next_note_in_array(B3);
        }
        if (is_note_present[C4]) {
            buffer[i] += get_next_note_in_array(C4);
        }
        if (is_note_present[D4_FLAT]) {
            buffer[i] += get_next_note_in_array(D4_FLAT);
        }
        if (is_note_present[D4]) {
            buffer[i] += get_next_note_in_array(D4);
        }
        if (is_note_present[E4_FLAT]) {
            buffer[i] += get_next_note_in_array(E4_FLAT);
        }
        if (is_note_present[E4]) {
            buffer[i] += get_next_note_in_array(E4);
        }
        if (is_note_present[F4]) {
            buffer[i] += get_next_note_in_array(F4);
        }
        if (is_note_present[G4_FLAT]) {
            buffer[i] += get_next_note_in_array(G4_FLAT);
        }
        if (is_note_present[G4]) {
            buffer[i] += get_next_note_in_array(G4);
        }
        if (is_note_present[A4_FLAT]) {
            buffer[i] += get_next_note_in_array(A4_FLAT);
        }
        if (is_note_present[A4]) {
            buffer[i] += get_next_note_in_array(A4);
        }
        if (is_note_present[B4_FLAT]) {
            buffer[i] += get_next_note_in_array(B4_FLAT);
        }
        if (is_note_present[B4]) {
            buffer[i] += get_next_note_in_array(B4);
        }
        if (is_note_present[C5]) {
            buffer[i] += get_next_note_in_array(C5);
        }
        if (is_note_present[D5_FLAT]) {
            buffer[i] += get_next_note_in_array(D5_FLAT);
        }
        if (is_note_present[D5]) {
            buffer[i] += get_next_note_in_array(D5);
        }
        if (is_note_present[E5_FLAT]) {
            buffer[i] += get_next_note_in_array(E5_FLAT);
        }
        if (is_note_present[E5]) {
            buffer[i] += get_next_note_in_array(E5);
        }
        if (is_note_present[F5]) {
            buffer[i] += get_next_note_in_array(F5);
        }
        if (is_note_present[G5_FLAT]) {
            buffer[i] += get_next_note_in_array(G5_FLAT);
        }
        if (is_note_present[G5]) {
            buffer[i] += get_next_note_in_array(G5);
        }
        if (is_note_present[A5_FLAT]) {
            buffer[i] += get_next_note_in_array(A5_FLAT);
        }
        if (is_note_present[A5]) {
            buffer[i] += get_next_note_in_array(A5);
        }
        if (is_note_present[B5_FLAT]) {
            buffer[i] += get_next_note_in_array(B5_FLAT);
        }
        if (is_note_present[B5]) {
            buffer[i] += get_next_note_in_array(B5);
        }
        if (is_note_present[C6]) {
            buffer[i] += get_next_note_in_array(C6);
        }
    }
    // int i;
    // for (i = 0; i < BUFFER_LENGTH; i++) {
    //     buffer[i] = get_next_note_in_array(C4)/50 + get_next_note_in_array(E4)/50;// + get_next_note_in_array(G4)/10;
    //     printf("%d\n", buffer[i]);
    // }
}

static nrfx_i2s_buffers_t i2s_buffers = {NULL, (uint32_t*) tx_buffer_0};

static void data_handler(nrfx_i2s_buffers_t const *p_released, uint32_t status) {
    //printf("data_handler called\n");
    if(swing) {
        is_buffer_1_tx  = !is_buffer_1_tx;
        i2s_buffers.p_tx_buffer = (uint32_t*) (is_buffer_1_tx ? tx_buffer_0 : tx_buffer_1);
    }
    update_tx_buffer((int16_t *) (p_released->p_tx_buffer));
    nrfx_i2s_next_buffers_set(&i2s_buffers);
    
}

void i2s_instrument_init() {
    nrfx_i2s_config_t config = NRF_DRV_I2S_DEFAULT_CONFIG;
    config.sck_pin = SCK_PIN;
    config.lrck_pin = LRCK_PIN;
    config.sdout_pin = SDOUT_PIN;
    config.sdin_pin = NRFX_I2S_PIN_NOT_USED;
    config.channels = NRF_I2S_CHANNELS_LEFT;
    config.mck_setup = NRF_I2S_MCK_32MDIV11; // 2.909 MHz
    config.ratio = NRF_I2S_RATIO_32X; // Divide by 32
    APP_ERROR_CHECK(nrfx_i2s_init(&config, data_handler));
    //printf("In\n");
    //nrfx_err_t start_err_code;
    nrfx_i2s_start(&i2s_buffers, BUFFER_LENGTH/2, 0);
    //printf("nrfx_i2s_start error code: %ld\n", start_err_code);
    //printf("Out\n");
}

void i2s_instrument_init_hal() {
    // Enable transmission
    NRF_I2S->CONFIG.TXEN = (I2S_CONFIG_TXEN_TXEN_ENABLE << I2S_CONFIG_TXEN_TXEN_Pos);

    // Enable MCK generator
    NRF_I2S->CONFIG.MCKEN = (I2S_CONFIG_MCKEN_MCKEN_ENABLE << I2S_CONFIG_MCKEN_MCKEN_Pos);

    // MCKFREQ = 2.909 MHz
    NRF_I2S->CONFIG.MCKFREQ = I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV11 << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;

    // Ratio = 32 
    NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_32X << I2S_CONFIG_RATIO_RATIO_Pos;

    // Master mode, 16Bit, left aligned
    NRF_I2S->CONFIG.MODE = I2S_CONFIG_MODE_MODE_MASTER << I2S_CONFIG_MODE_MODE_Pos;
    NRF_I2S->CONFIG.SWIDTH = I2S_CONFIG_SWIDTH_SWIDTH_16BIT << I2S_CONFIG_SWIDTH_SWIDTH_Pos;
    NRF_I2S->CONFIG.ALIGN = I2S_CONFIG_ALIGN_ALIGN_LEFT << I2S_CONFIG_ALIGN_ALIGN_Pos;

    // Format = I2S
    NRF_I2S->CONFIG.FORMAT = I2S_CONFIG_FORMAT_FORMAT_I2S << I2S_CONFIG_FORMAT_FORMAT_Pos;

    // Use left channel 
    NRF_I2S->CONFIG.CHANNELS = I2S_CONFIG_CHANNELS_CHANNELS_LEFT << I2S_CONFIG_CHANNELS_CHANNELS_Pos;

    // Configure pins
    NRF_I2S->PSEL.MCK = (MCK_PIN << I2S_PSEL_MCK_PIN_Pos);
    NRF_I2S->PSEL.SCK = (SCK_PIN << I2S_PSEL_SCK_PIN_Pos); 
    NRF_I2S->PSEL.LRCK = (LRCK_PIN << I2S_PSEL_LRCK_PIN_Pos); 
    NRF_I2S->PSEL.SDOUT = (SDOUT_PIN << I2S_PSEL_SDOUT_PIN_Pos);

    NRF_I2S->ENABLE = 1;

    // Configure data pointer
    NRF_I2S->TXD.PTR = (uint32_t)NO_NOTE_array;
    NRF_I2S->RXTXD.MAXCNT = 1;

    // Start transmitting I2S data
    NRF_I2S->TASKS_START = 1;
}

void i2s_instrument_play(instrument_state_t *state) {
    int i;
    for (i = 0; i < NUMBER_OF_NOTES; i++) {
       is_note_present[i] = false;
    }
    for (i = 0; i < NUMBER_OF_NOTE_INDICES; i++) {
       is_note_present[state->notes_to_play[i]] = true;
    }
    switch(state->volume_level) {
        case 0:
            shrink = 8;
            break;
        case 1:
            shrink = 7;
            break;
        case 2:
            shrink = 6;
            break;
        case 3:
            shrink = 5;
            break;
    }
    //printf("line 251\n");
    //nrf_delay_ms(1000);
}

static void play_wave_hal(musical_note_t note) {
    NRF_I2S->TXD.PTR = (uint32_t)&note_arrays[note][0];
    NRF_I2S->RXTXD.MAXCNT = (int)(note_lengths[note]/2);
}

// static void play_wave(int16_t wave_array[], size_t array_size) {
//     // Configure data pointer
//     NRF_I2S->TXD.PTR = (uint32_t)wave_array;
//     NRF_I2S->RXTXD.MAXCNT = (int)(array_size/2);
// }

void i2s_instrument_play_hal(instrument_state_t *state) {
    //printf("In play fn\n");
    int i = 0;
    //bool still_looking = true;
    while (i < NUMBER_OF_NOTE_INDICES) {
        if (state->notes_to_play[i]!= NO_NOTE) {
            printf("note: %d\n", state->notes_to_play[i]);
            play_wave_hal(state->notes_to_play[i]);
            break;
        }
        if (i == NUMBER_OF_NOTE_INDICES - 1) {
            play_wave_hal(NO_NOTE);
        }
        i++;
    }

    //play_wave_hal(B3);
    //play_wave_hal(G4_FLAT_array, ARRAY_SIZE(G4_FLAT_array));
    //NRF_I2S->TXD.PTR = (uint32_t)C4_array;
    //NRF_I2S->RXTXD.MAXCNT = C4_LENGTH/2;
    //play_wave(B3_array, ARRAY_SIZE(B3_array));
}
