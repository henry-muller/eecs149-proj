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

#include "types.h"

#include <nrf.h>

#define PIN_MCK    (13)
#define PIN_SCK    (14) // same as BCK
#define PIN_LRCK   (15)
#define PIN_SDOUT  (16) // same as DIN
#define CHORD_ARRAY_LENGTH 600

    int16_t B3_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};

    int16_t C4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t D4_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t D4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t E4_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t E4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t F4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t G4_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t G4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t A4_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t A4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t B4_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t B4_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};

    int16_t C5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t D5_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t D5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t E5_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t E5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t F5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t G5_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t G5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t A5_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t A5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t B5_FLAT_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t B5_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};

    int16_t C6_array[CHORD_ARRAY_LENGTH] = {0, 0, 0};
    int16_t NO_NOTE_array[CHORD_ARRAY_LENGTH] = {0, 0, 0}; // TODO: Put 600 zeroes here

// int16_t wave1[CHORD_ARRAY_LENGTH] = {8, 86, 173, 266, 361, 457, 550, 635, 712, 772, 816, 844, 852, 848, 832, 812, 796, 782, 781, 790, 809, 838, 874, 916, 959, 1005, 1049, 1092, 1134, 1170, 1204, 1230, 1247, 1253, 1246, 1227, 1196, 1154, 1103, 1046, 988, 927, 871, 825, 795, 784, 800, 849, 929, 1044, 1192, 1367, 1566, 1779, 1998, 2217, 2428, 2624, 2800, 2951, 3069, 3152, 3199, 3206, 3178, 3116, 3023, 2906, 2768, 2617, 2458, 2294, 2131, 1972, 1823, 1686, 1560, 1446, 1341, 1246, 1155, 1064, 970, 868, 758, 637, 506, 371, 234, 105, -7, -97, -156, -179, -159, -96, 8, 149, 319, 514, 721, 933, 1143, 1343, 1527, 1692, 1834, 1952, 2045, 2110, 2148, 2160, 2144, 2103, 2037, 1946, 1836, 1707, 1564, 1410, 1250, 1090, 932, 781, 637, 501, 375, 252, 131, 8, -120, -253, -392, -536, -684, -836, -989, -1144, -1297, -1447, -1593, -1728, -1856, -1972, -2074, -2163, -2236, -2296, -2338, -2363, -2370, -2361, -2337, -2300, -2254, -2207, -2157, -2109, -2062, -2016, -1970, -1922, -1872, -1821, -1769, -1718, -1669, -1619, -1568, -1513, -1449, -1371, -1276, -1163, -1028, -870, -696, -504, -299, -86, 129, 344, 554, 748, 927, 1084, 1214, 1318, 1389, 1432, 1446, 1433, 1399, 1347, 1286, 1221, 1157, 1099, 1050, 1012, 983, 962, 953, 954, 966, 989, 1021, 1060, 1104, 1150, 1195, 1239, 1284, 1331, 1386, 1453, 1534, 1632, 1747, 1876, 2013, 2148, 2276, 2388, 2478, 2545, 2583, 2595, 2580, 2538, 2474, 2387, 2278, 2153, 2011, 1858, 1695, 1529, 1371, 1222, 1088, 972, 874, 796, 734, 685, 652, 628, 618, 625, 646, 684, 736, 798, 866, 937, 1005, 1066, 1119, 1163, 1200, 1227, 1245, 1253, 1249, 1232, 1199, 1154, 1097, 1029, 954, 877, 800, 722, 647, 575, 503, 434, 365, 298, 232, 166, 105, 49, -2, -48, -91, -131, -169, -208, -243, -273, -299, -316, -320, -313, -293, -263, -228, -195, -167, -146, -135, -131, -136, -144, -155, -168, -186, -215, -256, -316, -397, -503, -635, -786, -957, -1136, -1316, -1494, -1655, -1800, -1918, -2007, -2069, -2097, -2099, -2073, -2027, -1968, -1904, -1842, -1786, -1741, -1704, -1674, -1648, -1621, -1593, -1561, -1525, -1489, -1453, -1423, -1400, -1387, -1385, -1391, -1408, -1434, -1470, -1516, -1568, -1623, -1676, -1719, -1747, -1750, -1725, -1669, -1579, -1456, -1300, -1118, -919, -709, -504, -309, -131, 24, 160, 273, 368, 445, 508, 559, 596, 620, 630, 627, 610, 574, 520, 448, 354, 238, 101, -52, -221, -399, -578, -750, -906, -1040, -1144, -1217, -1258, -1265, -1245, -1198, -1127, -1041, -937, -820, -692, -553, -405, -246, -78, 101, 286, 476, 672, 869, 1067, 1261, 1447, 1621, 1775, 1902, 1996, 2054, 2073, 2053, 2001, 1920, 1817, 1706, 1591, 1483, 1389, 1312, 1254, 1213, 1189, 1183, 1189, 1207, 1232, 1262, 1293, 1325, 1354, 1382, 1412, 1448, 1492, 1549, 1619, 1699, 1789, 1880, 1967, 2044, 2105, 2144, 2159, 2151, 2119, 2064, 1992, 1903, 1801, 1689, 1570, 1447, 1319, 1187, 1055, 920, 781, 639, 495, 349, 207, 68, -66, -191, -304, -405, -491, -562, -619, -662, -699, -730, -757, -786, -816, -852, -897, -952, -1021, -1104, -1201, -1312, -1436, -1568, -1707, -1849, -1988, -2119, -2240, -2349, -2447, -2533, -2609, -2679, -2745, -2809, -2877, -2945, -3013, -3082, -3145, -3200, -3243, -3268, -3276, -3259, -3214, -3139, -3032, -2891, -2720, -2523, -2304, -2077, -1848, -1623, -1414, -1223, -1055, -914, -802, -726, -683, -674, -701, -760, -849, -961, -1096, -1247, -1409, -1580, -1753, -1924, -2083, -2227, -2351, -2448, -2520, -2567, -2589, -2593, -2583, -2563, -2537, -2508, -2477, -2448, -2421, -2394, -2369, -2346, -2321, -2291, -2256, -2214, -2161, -2100, -2029, -1949, -1857, -1758, -1650, -1532, -1409, -1279, -1144, -1006, -870, -741, -619, -516, -431, -369, -331, -312, -315, -327, -344, -358, -359, -345, -308, -252, -169, -69};


void i2s_instrument_init() {
    // Enable transmission
    NRF_I2S->CONFIG.TXEN = (I2S_CONFIG_TXEN_TXEN_ENABLE << I2S_CONFIG_TXEN_TXEN_Pos);

    // Enable MCK generator
    NRF_I2S->CONFIG.MCKEN = (I2S_CONFIG_MCKEN_MCKEN_ENABLE << I2S_CONFIG_MCKEN_MCKEN_Pos);

    // MCKFREQ = 4 MHz
    NRF_I2S->CONFIG.MCKFREQ = I2S_CONFIG_MCKFREQ_MCKFREQ_32MDIV11  << I2S_CONFIG_MCKFREQ_MCKFREQ_Pos;

    // Ratio = 64 
    NRF_I2S->CONFIG.RATIO = I2S_CONFIG_RATIO_RATIO_64X << I2S_CONFIG_RATIO_RATIO_Pos;

    // Master mode, 16Bit, left aligned
    NRF_I2S->CONFIG.MODE = I2S_CONFIG_MODE_MODE_MASTER << I2S_CONFIG_MODE_MODE_Pos;
    NRF_I2S->CONFIG.SWIDTH = I2S_CONFIG_SWIDTH_SWIDTH_16BIT << I2S_CONFIG_SWIDTH_SWIDTH_Pos;
    NRF_I2S->CONFIG.ALIGN = I2S_CONFIG_ALIGN_ALIGN_LEFT << I2S_CONFIG_ALIGN_ALIGN_Pos;

    // Format = I2S
    NRF_I2S->CONFIG.FORMAT = I2S_CONFIG_FORMAT_FORMAT_I2S << I2S_CONFIG_FORMAT_FORMAT_Pos;

    // Use stereo 
    NRF_I2S->CONFIG.CHANNELS = I2S_CONFIG_CHANNELS_CHANNELS_LEFT << I2S_CONFIG_CHANNELS_CHANNELS_Pos;

    // Configure pins
    NRF_I2S->PSEL.MCK = (PIN_MCK << I2S_PSEL_MCK_PIN_Pos);
    NRF_I2S->PSEL.SCK = (PIN_SCK << I2S_PSEL_SCK_PIN_Pos); 
    NRF_I2S->PSEL.LRCK = (PIN_LRCK << I2S_PSEL_LRCK_PIN_Pos); 
    NRF_I2S->PSEL.SDOUT = (PIN_SDOUT << I2S_PSEL_SDOUT_PIN_Pos);

    NRF_I2S->ENABLE = 1;
}

static void play_wave(int16_t wave_table[], size_t wave_table_size) {
    printf("got to line 62\n");
    // Configure data pointer
    NRF_I2S->TXD.PTR = (uint32_t)wave_table;
    NRF_I2S->RXTXD.MAXCNT = wave_table_size/2;

    // Start transmitting I2S data
    NRF_I2S->TASKS_START = 1;
    printf("Finished\n");
}

static int16_t[] generate_chord(musical_note_t notes_to_play[]) {
    int16_t* chord = malloc(CHORD_ARRAY_LENGTH * sizeof(int16_t));
    int i;
    int16_t* current_note_array;
    for (i = 0; i < NUMBER_OF_NOTE_INDICES; i++) {
        chord[i] = 0;
        switch(notes_to_play[i]) {
            case B3:
                current_note_array = B3_array;
                break;
            case C4:
                current_note_array = C4_array;
                break;
            case D4_FLAT:
                current_note_array = D4_FLAT_array;
                break;
            case D4:
                current_note_array = D4_array;
                break;
            case E4_FLAT:
                current_note_array = E4_FLAT_array;
                break;
            case E4:
                current_note_array = E4_array;
                break;
            case F4:
                current_note_array = F4_array;
                break;
            case G4_FLAT:
                current_note_array = G4_FLAT_array;
                break;
            case G4:
                current_note_array = G4_array;
                break;
            case A4_FLAT:
                current_note_array = A4_FLAT_array;
                break;
            case A4:
                current_note_array = A4_array;
                break;
            case B4_FLAT:
                current_note_array = B4_FLAT_array;
                break;
            case B4:
                current_note_array = B4_array;
                break;
            case C5:
                current_note_array = C5_array;
                break;
            case D5_FLAT:
                current_note_array = D5_FLAT_array;
                break;
            case D5:
                current_note_array = D5_array;
                break;
            case E5_FLAT:
                current_note_array = E5_FLAT_array;
                break;
            case E5:
                current_note_array = E5_array;
                break;
            case F5:
                current_note_array = F5_array;
                break;
            case G5_FLAT:
                current_note_array = G5_FLAT_array;
                break;
            case G5:
                current_note_array = G5_array;
                break;
            case A5_FLAT:
                current_note_array = A5_FLAT_array;
                break;
            case A5:
                current_note_array = A5_array;
                break;
            case B5_FLAT:
                current_note_array = B5_FLAT_array;
                break;
            case B5:
                current_note_array = B5_array;
                break;
            case C6:
                current_note_array = C6_array;
                break;
        } 
    }
}

void i2s_instrument_play(instrument_state_t* state) {
    /*
    // Configure data pointer
    NRF_I2S->TXD.PTR = (uint32_t)&wave1[0];
    NRF_I2S->RXTXD.MAXCNT = sizeof(wave1) / sizeof(uint32_t);

    // Start transmitting I2S data
    NRF_I2S->TASKS_START = 1;
    */
   play_wave(wave1, ARRAY_SIZE(wave1));
}
