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
#include <nrf_i2s.h>

#define PIN_MCK    (13)
#define PIN_SCK    (14) // same as BCK
#define PIN_LRCK   (15)
#define PIN_SDOUT  (16) // same as DIN

    int16_t B3_array[368] = {104, 300, 493, 688, 875, 1069, 1257, 1448, 1634, 1823, 2008, 2193, 2371, 2552, 2726, 2901, 3070, 3239, 3400, 3565, 3723, 3882, 4036, 4190, 4340, 4491, 4637, 4786, 4932, 5078, 5221, 5364, 5503, 5643, 5778, 5917, 6052, 6191, 6329, 6471, 6610, 6750, 6884, 7019, 7148, 7275, 7397, 7519, 7639, 7762, 7883, 8008, 8131, 8256, 8379, 8503, 8624, 8748, 8868, 8989, 9106, 9222, 9332, 9442, 9547, 9652, 9753, 9858, 9962, 10068, 10172, 10279, 10384, 10492, 10598, 10709, 10816, 10926, 11029, 11130, 11224, 11313, 11396, 11478, 11558, 11641, 11722, 11808, 11890, 11977, 12061, 12146, 12227, 12310, 12389, 12469, 12543, 12618, 12687, 12754, 12817, 12879, 12936, 12993, 13045, 13098, 13146, 13193, 13236, 13279, 13317, 13355, 13388, 13420, 13445, 13467, 13481, 13491, 13493, 13492, 13484, 13476, 13463, 13448, 13428, 13408, 13383, 13357, 13327, 13297, 13262, 13226, 13183, 13136, 13081, 13020, 12952, 12880, 12803, 12726, 12646, 12568, 12486, 12401, 12309, 12209, 12095, 11972, 11837, 11695, 11547, 11397, 11248, 11097, 10945, 10791, 10632, 10471, 10302, 10130, 9953, 9769, 9579, 9383, 9179, 8969, 8755, 8530, 8300, 8062, 7818, 7568, 7310, 7047, 6784, 6516, 6251, 5985, 5720, 5454, 5182, 4905, 4621, 4322, 4018, 3706, 3391, 3075, 2763, 2452, 2147, 1838, 1531, 1221, 908, 592, 274, -44, -362, -685, -1002, -1321, -1637, -1954, -2272, -2593, -2915, -3248, -3581, -3919, -4257, -4594, -4928, -5258, -5582, -5909, -6227, -6545, -6855, -7161, -7461, -7756, -8045, -8340, -8629, -8924, -9217, -9510, -9799, -10085, -10361, -10637, -10902, -11166, -11418, -11667, -11903, -12138, -12359, -12579, -12787, -12997, -13199, -13403, -13599, -13798, -13986, -14173, -14347, -14517, -14674, -14827, -14967, -15107, -15235, -15363, -15481, -15595, -15700, -15800, -15887, -15972, -16044, -16112, -16168, -16221, -16262, -16300, -16325, -16348, -16362, -16374, -16380, -16384, -16382, -16384, -16381, -16372, -16350, -16323, -16285, -16242, -16187, -16130, -16063, -15992, -15912, -15829, -15736, -15641, -15535, -15427, -15309, -15188, -15060, -14929, -14793, -14658, -14516, -14376, -14229, -14080, -13923, -13763, -13595, -13425, -13247, -13069, -12887, -12703, -12514, -12325, -12132, -11940, -11744, -11545, -11345, -11144, -10939, -10730, -10515, -10296, -10071, -9840, -9605, -9368, -9130, -8894, -8658, -8426, -8192, -7959, -7729, -7499, -7270, -7044, -6817, -6594, -6370, -6144, -5919, -5693, -5466, -5241, -5013, -4787, -4560, -4331, -4103, -3876, -3652, -3432, -3215, -3003, -2793, -2582, -2373, -2162, -1949, -1735, -1520, -1309, -1099, -890, -687, -485, -286, -89};
    int16_t C4_array[347] = {33, 239, 523, 796, 1079, 1392, 1715, 2045, 2374, 2710, 3055, 3421, 3804, 4197, 4581, 4917, 5244, 5578, 5868, 6133, 6398, 6646, 6865, 7064, 7247, 7397, 7542, 7698, 7827, 7933, 8057, 8187, 8311, 8438, 8583, 8776, 9007, 9258, 9527, 9804, 10066, 10321, 10606, 10894, 11164, 11430, 11678, 11886, 12035, 12104, 12093, 12006, 11861, 11655, 11384, 11043, 10638, 10155, 9617, 9063, 8496, 7934, 7411, 6938, 6516, 6148, 5849, 5594, 5388, 5254, 5188, 5182, 5216, 5269, 5309, 5332, 5338, 5355, 5389, 5419, 5427, 5395, 5309, 5169, 5006, 4801, 4517, 4148, 3695, 3179, 2601, 1968, 1278, 543, -241, -1079, -1917, -2746, -3561, -4344, -5080, -5776, -6408, -6977, -7474, -7901, -8259, -8535, -8715, -8814, -8843, -8799, -8680, -8493, -8244, -7949, -7624, -7286, -6930, -6561, -6180, -5814, -5483, -5171, -4890, -4658, -4472, -4338, -4261, -4243, -4275, -4372, -4541, -4788, -5115, -5524, -6003, -6530, -7117, -7764, -8463, -9204, -9962, -10716, -11446, -12148, -12824, -13448, -14021, -14538, -14986, -15373, -15709, -15987, -16196, -16325, -16381, -16373, -16299, -16157, -15965, -15723, -15431, -15119, -14823, -14521, -14205, -13884, -13569, -13278, -13007, -12749, -12505, -12268, -12026, -11782, -11545, -11299, -11040, -10749, -10427, -10069, -9667, -9228, -8755, -8248, -7713, -7139, -6543, -5933, -5319, -4706, -4084, -3454, -2823, -2195, -1561, -923, -302, 308, 908, 1494, 2072, 2642, 3204, 3754, 4277, 4778, 5254, 5723, 6180, 6601, 6996, 7370, 7710, 8010, 8295, 8554, 8753, 8912, 9040, 9110, 9125, 9117, 9070, 8975, 8864, 8749, 8621, 8494, 8382, 8273, 8176, 8141, 8157, 8188, 8247, 8338, 8464, 8616, 8793, 8979, 9161, 9343, 9513, 9644, 9737, 9795, 9805, 9756, 9625, 9432, 9207, 8912, 8544, 8114, 7636, 7124, 6555, 5933, 5292, 4656, 4037, 3464, 2972, 2478, 1981, 1527, 1118, 754, 447, 198, 12, -144, -304, -464, -616, -735, -830, -926, -1026, -1129, -1197, -1246, -1323, -1401, -1454, -1501, -1545, -1577, -1620, -1687, -1728, -1735, -1762, -1801, -1833, -1863, -1882, -1824, -1755, -1794, -1802, -1694, -1563, -1422, -1263, -1109, -973, -780, -540, -335, -154, 22, 171, 281, 378, 443, 454, 431, 385, 287, 145, -26, -209, -393, -593, -803, -1012, -1208, -1371, -1475, -1516, -1537, -1534, -1508, -1441, -1343, -1259, -1170, -1043, -915, -796, -641, -454, -254, -86};
    int16_t D4_FLAT_array[328] = {34, 255, 556, 842, 1150, 1484, 1829, 2178, 2528, 2888, 3264, 3664, 4076, 4490, 4856, 5200, 5554, 5864, 6144, 6424, 6683, 6909, 7117, 7300, 7452, 7612, 7768, 7886, 8007, 8146, 8278, 8412, 8559, 8757, 8999, 9266, 9550, 9843, 10116, 10392, 10700, 10997, 11280, 11555, 11798, 11985, 12090, 12104, 12031, 11888, 11680, 11398, 11039, 10607, 10088, 9514, 8926, 8323, 7740, 7207, 6732, 6312, 5965, 5680, 5441, 5279, 5195, 5180, 5213, 5268, 5311, 5333, 5340, 5362, 5398, 5425, 5420, 5363, 5241, 5077, 4884, 4613, 4249, 3788, 3252, 2647, 1980, 1249, 469, -370, -1258, -2143, -3016, -3870, -4677, -5436, -6140, -6768, -7322, -7794, -8191, -8500, -8702, -8811, -8843, -8791, -8659, -8448, -8173, -7848, -7500, -7135, -6752, -6354, -5955, -5592, -5256, -4948, -4693, -4490, -4344, -4261, -4243, -4283, -4397, -4593, -4877, -5251, -5716, -6246, -6833, -7492, -8210, -8982, -9781, -10582, -11359, -12103, -12820, -13479, -14081, -14618, -15078, -15474, -15811, -16080, -16265, -16362, -16384, -16334, -16208, -16017, -15775, -15473, -15144, -14829, -14510, -14174, -13835, -13505, -13204, -12921, -12654, -12400, -12148, -11888, -11636, -11381, -11112, -10813, -10478, -10105, -9682, -9219, -8716, -8177, -7603, -6989, -6355, -5706, -5058, -4405, -3743, -3075, -2410, -1742, -1066, -405, 241, 878, 1497, 2109, 2711, 3305, 3881, 4428, 4948, 5447, 5941, 6407, 6834, 7240, 7613, 7940, 8244, 8525, 8742, 8911, 9045, 9113, 9125, 9111, 9047, 8938, 8820, 8692, 8555, 8428, 8314, 8202, 8143, 8153, 8183, 8241, 8335, 8469, 8631, 8821, 9017, 9209, 9400, 9569, 9688, 9771, 9807, 9784, 9677, 9488, 9259, 8964, 8582, 8132, 7626, 7083, 6473, 5811, 5131, 4464, 3818, 3249, 2738, 2206, 1704, 1254, 853, 512, 236, 31, -136, -304, -474, -633, -752, -853, -954, -1064, -1162, -1218, -1283, -1371, -1437, -1487, -1535, -1572, -1612, -1683, -1728, -1736, -1765, -1807, -1839, -1872, -1873, -1788, -1762, -1814, -1749, -1612, -1469, -1305, -1138, -994, -800, -546, -330, -138, 47, 196, 307, 404, 453, 446, 413, 338, 204, 33, -159, -352, -560, -782, -1003, -1210, -1382, -1482, -1521, -1538, -1529, -1490, -1400, -1300, -1215, -1098, -957, -832, -681, -488, -276, -94};
    int16_t D4_array[3] = {0, 0, 0};
    int16_t E4_FLAT_array[3] = {0, 0, 0};
    int16_t E4_array[3] = {0, 0, 0};
    int16_t F4_array[3] = {0, 0, 0};
    int16_t G4_FLAT_array[3] = {0, 0, 0};
    int16_t G4_array[3] = {0, 0, 0};
    int16_t A4_FLAT_array[3] = {0, 0, 0};
    int16_t A4_array[3] = {0, 0, 0};
    int16_t B4_FLAT_array[3] = {0, 0, 0};
    int16_t B4_array[3] = {0, 0, 0};

    int16_t C5_array[3] = {0, 0, 0};
    int16_t D5_FLAT_array[3] = {0, 0, 0};
    int16_t D5_array[3] = {0, 0, 0};
    int16_t E5_FLAT_array[3] = {0, 0, 0};
    int16_t E5_array[3] = {0, 0, 0};
    int16_t F5_array[3] = {0, 0, 0};
    int16_t G5_FLAT_array[3] = {0, 0, 0};
    int16_t G5_array[3] = {0, 0, 0};
    int16_t A5_FLAT_array[3] = {0, 0, 0};
    int16_t A5_array[3] = {0, 0, 0};
    int16_t B5_FLAT_array[3] = {0};

    int16_t B5_array[3] = {0};        // B5 = 0002

    int16_t C6_array[3] = {0};
    int16_t NO_NOTE_array[3] = {0, 0, 0}; // TODO: Put 600 zeroes here

void i2s_instrument_init() {
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
    NRF_I2S->PSEL.MCK = (PIN_MCK << I2S_PSEL_MCK_PIN_Pos);
    NRF_I2S->PSEL.SCK = (PIN_SCK << I2S_PSEL_SCK_PIN_Pos); 
    NRF_I2S->PSEL.LRCK = (PIN_LRCK << I2S_PSEL_LRCK_PIN_Pos); 
    NRF_I2S->PSEL.SDOUT = (PIN_SDOUT << I2S_PSEL_SDOUT_PIN_Pos);

    NRF_I2S->ENABLE = 1;
}

static void play_wave(int16_t wave_array[], size_t array_size) {
    // Configure data pointer
    NRF_I2S->TXD.PTR = (uint32_t)wave_array;
    NRF_I2S->RXTXD.MAXCNT = (int)(array_size/2); // Figure out why you have to divide by 2.
    // Can this be fixed by the settings we initialize I2S with? Maybe mono vs. stereo or something?
    // The code I copied this from used stereo, that might explain the differences

    // Start transmitting I2S data
    NRF_I2S->TASKS_START = 1;
}

static int16_t* generate_chord(musical_note_t notes_to_play[]) {
    int16_t* chord_array = calloc(3, sizeof(int16_t));
    int i;
    int j;
    int16_t* current_note_array;
    for (i = 0; i < NUMBER_OF_NOTE_INDICES; i++) {
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
            case NO_NOTE:
                current_note_array = NO_NOTE_array;
                break;
        }
        for (j = 0; j < 3; j++) {
            chord_array[j] +=  current_note_array[j]; // Assumes everything starts at 0
        }
    }
    return chord_array;
}

void i2s_instrument_play(instrument_state_t* state) {
    /*
    // Configure data pointer
    NRF_I2S->TXD.PTR = (uint32_t)&wave1[0];
    NRF_I2S->RXTXD.MAXCNT = sizeof(wave1) / sizeof(uint32_t);

    // Start transmitting I2S data
    NRF_I2S->TASKS_START = 1;
    */
   while(1) {
        play_wave(B3_array, ARRAY_SIZE(B3_array));
        nrf_delay_ms(2000);
        play_wave(C4_array, ARRAY_SIZE(C4_array));
        nrf_delay_ms(2000);
        play_wave(D4_FLAT_array, ARRAY_SIZE(D4_FLAT_array));
        nrf_delay_ms(2000);
   }
}
