#include "MainDemo.h"

#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON            // Background Debugger Enable


////////Audio Codec Data Declaration////////
#define 			FRAME_SIZE 1024
WM8960_CODEC *pCodecHandle;
STEREO_AUDIO_DATA Sin[FRAME_SIZE];
STEREO_AUDIO_DATA Sout[FRAME_SIZE];

////////FX File System Data Declaration////////
FSFILE* fxFile;

typedef struct {
    INT8 fuzzIsOn;
    INT16 fuzzGain;
    INT16 fuzzMix;

    INT8 overdriveIsOn;
    INT16 overdriveTreshold;

    INT8 wahIsOn;
    INT16 wahEffectRate;
    INT16 wahMinf;
    INT16 wahMaxf;
    INT16 wahQ;

    INT8 compIsOn;
    INT16 compRatio;
    INT16 compTreshold;

    INT8 bitIsOn;
    INT8 bitsToKeep;

    INT8 limitIsOn;
    INT16 limitTreshold;

    INT8 flangerIsOn;
    INT16 flangerFrequency;

} FX_DATA_STRUCT;

#define             FX_MAX_PRESETS 10
FX_DATA_STRUCT fxData[FX_MAX_PRESETS];
FX_DATA_STRUCT* fxCurrentData;
INT8 fxCurrentPreset = 0;
char charBuffer;
char charArray[100];
INT16 charArraySize = 0;

//FUZZ
const INT16 INT16_MAX = 32767;
INT32 q;
INT32 z;
//OVERDRIVE
INT16 tresholdTimesThree;
INT16 current;

//COMPRESSOR
double compCoefficient = 0;
double compCoefficientMin = 1000;

////////Hanning Window Declaration for 1024 numbers////////
//const float hanningWindow[FRAME_SIZE] = {0.00000, 0.00001, 0.00004, 0.00008, 0.00015, 0.00024, 0.00034, 0.00046, 0.00060, 0.00076, 0.00094, 0.00114, 0.00136, 0.00159, 0.00185, 0.00212, 0.00241, 0.00272, 0.00305, 0.00340, 0.00377, 0.00415, 0.00456, 0.00498, 0.00542, 0.00588, 0.00636, 0.00686, 0.00738, 0.00791, 0.00846, 0.00904, 0.00963, 0.01024, 0.01086, 0.01151, 0.01217, 0.01286, 0.01356, 0.01428, 0.01501, 0.01577, 0.01654, 0.01734, 0.01815, 0.01898, 0.01982, 0.02069, 0.02157, 0.02247, 0.02339, 0.02433, 0.02528, 0.02626, 0.02725, 0.02826, 0.02928, 0.03033, 0.03139, 0.03247, 0.03357, 0.03468, 0.03582, 0.03697, 0.03813, 0.03932, 0.04052, 0.04174, 0.04298, 0.04423, 0.04550, 0.04679, 0.04810, 0.04942, 0.05076, 0.05212, 0.05349, 0.05488, 0.05629, 0.05771, 0.05915, 0.06061, 0.06208, 0.06357, 0.06508, 0.06660, 0.06814, 0.06970, 0.07127, 0.07286, 0.07446, 0.07608, 0.07772, 0.07937, 0.08104, 0.08273, 0.08443, 0.08614, 0.08787, 0.08962, 0.09138, 0.09316, 0.09495, 0.09676, 0.09858, 0.10042, 0.10227, 0.10414, 0.10603, 0.10792, 0.10984, 0.11177, 0.11371, 0.11567, 0.11764, 0.11962, 0.12162, 0.12364, 0.12567, 0.12771, 0.12977, 0.13184, 0.13392, 0.13602, 0.13813, 0.14026, 0.14240, 0.14455, 0.14672, 0.14890, 0.15109, 0.15330, 0.15552, 0.15775, 0.15999, 0.16225, 0.16452, 0.16681, 0.16910, 0.17141, 0.17373, 0.17606, 0.17841, 0.18077, 0.18314, 0.18552, 0.18791, 0.19032, 0.19273, 0.19516, 0.19760, 0.20005, 0.20252, 0.20499, 0.20748, 0.20997, 0.21248, 0.21500, 0.21752, 0.22006, 0.22261, 0.22517, 0.22774, 0.23033, 0.23292, 0.23552, 0.23813, 0.24075, 0.24338, 0.24602, 0.24867, 0.25133, 0.25400, 0.25668, 0.25937, 0.26206, 0.26477, 0.26748, 0.27020, 0.27294, 0.27568, 0.27843, 0.28118, 0.28395, 0.28672, 0.28950, 0.29229, 0.29509, 0.29789, 0.30071, 0.30353, 0.30636, 0.30919, 0.31203, 0.31488, 0.31774, 0.32060, 0.32347, 0.32635, 0.32923, 0.33212, 0.33502, 0.33792, 0.34083, 0.34374, 0.34666, 0.34959, 0.35252, 0.35545, 0.35840, 0.36135, 0.36430, 0.36726, 0.37022, 0.37319, 0.37616, 0.37914, 0.38212, 0.38511, 0.38810, 0.39109, 0.39409, 0.39710, 0.40010, 0.40311, 0.40613, 0.40915, 0.41217, 0.41519, 0.41822, 0.42125, 0.42429, 0.42732, 0.43036, 0.43341, 0.43645, 0.43950, 0.44255, 0.44560, 0.44865, 0.45171, 0.45477, 0.45782, 0.46089, 0.46395, 0.46701, 0.47008, 0.47314, 0.47621, 0.47928, 0.48235, 0.48542, 0.48848, 0.49156, 0.49463, 0.49770, 0.50077, 0.50384, 0.50691, 0.50998, 0.51305, 0.51612, 0.51919, 0.52226, 0.52532, 0.52839, 0.53146, 0.53452, 0.53758, 0.54065, 0.54371, 0.54676, 0.54982, 0.55288, 0.55593, 0.55898, 0.56203, 0.56507, 0.56812, 0.57116, 0.57420, 0.57723, 0.58026, 0.58329, 0.58632, 0.58934, 0.59236, 0.59538, 0.59839, 0.60140, 0.60441, 0.60741, 0.61040, 0.61340, 0.61639, 0.61937, 0.62235, 0.62533, 0.62830, 0.63126, 0.63422, 0.63718, 0.64013, 0.64307, 0.64601, 0.64895, 0.65188, 0.65480, 0.65772, 0.66063, 0.66353, 0.66643, 0.66933, 0.67221, 0.67509, 0.67796, 0.68083, 0.68369, 0.68654, 0.68939, 0.69223, 0.69506, 0.69788, 0.70070, 0.70351, 0.70631, 0.70910, 0.71189, 0.71467, 0.71744, 0.72020, 0.72295, 0.72569, 0.72843, 0.73116, 0.73388, 0.73659, 0.73929, 0.74198, 0.74466, 0.74734, 0.75000, 0.75265, 0.75530, 0.75794, 0.76056, 0.76318, 0.76578, 0.76838, 0.77097, 0.77354, 0.77611, 0.77866, 0.78121, 0.78374, 0.78626, 0.78878, 0.79128, 0.79377, 0.79625, 0.79872, 0.80117, 0.80362, 0.80605, 0.80848, 0.81089, 0.81329, 0.81567, 0.81805, 0.82041, 0.82276, 0.82510, 0.82743, 0.82975, 0.83205, 0.83434, 0.83661, 0.83888, 0.84113, 0.84337, 0.84559, 0.84781, 0.85001, 0.85219, 0.85437, 0.85653, 0.85867, 0.86081, 0.86292, 0.86503, 0.86712, 0.86920, 0.87126, 0.87331, 0.87535, 0.87737, 0.87938, 0.88137, 0.88335, 0.88532, 0.88727, 0.88920, 0.89112, 0.89303, 0.89492, 0.89679, 0.89865, 0.90050, 0.90233, 0.90415, 0.90595, 0.90773, 0.90950, 0.91126, 0.91300, 0.91472, 0.91643, 0.91812, 0.91979, 0.92145, 0.92310, 0.92473, 0.92634, 0.92794, 0.92952, 0.93108, 0.93263, 0.93416, 0.93567, 0.93717, 0.93866, 0.94012, 0.94157, 0.94300, 0.94442, 0.94582, 0.94720, 0.94856, 0.94991, 0.95124, 0.95256, 0.95385, 0.95513, 0.95640, 0.95764, 0.95887, 0.96008, 0.96128, 0.96245, 0.96361, 0.96475, 0.96588, 0.96698, 0.96807, 0.96914, 0.97020, 0.97123, 0.97225, 0.97325, 0.97423, 0.97520, 0.97614, 0.97707, 0.97798, 0.97887, 0.97975, 0.98060, 0.98144, 0.98226, 0.98306, 0.98385, 0.98461, 0.98536, 0.98609, 0.98680, 0.98749, 0.98816, 0.98882, 0.98945, 0.99007, 0.99067, 0.99125, 0.99182, 0.99236, 0.99288, 0.99339, 0.99388, 0.99435, 0.99480, 0.99523, 0.99565, 0.99604, 0.99642, 0.99678, 0.99711, 0.99743, 0.99774, 0.99802, 0.99828, 0.99853, 0.99875, 0.99896, 0.99915, 0.99932, 0.99947, 0.99960, 0.99971, 0.99981, 0.99988, 0.99994, 0.99998, 1.00000, 1.00000, 0.99998, 0.99994, 0.99988, 0.99981, 0.99971, 0.99960, 0.99947, 0.99932, 0.99915, 0.99896, 0.99875, 0.99853, 0.99828, 0.99802, 0.99774, 0.99743, 0.99711, 0.99678, 0.99642, 0.99604, 0.99565, 0.99523, 0.99480, 0.99435, 0.99388, 0.99339, 0.99288, 0.99236, 0.99182, 0.99125, 0.99067, 0.99007, 0.98945, 0.98882, 0.98816, 0.98749, 0.98680, 0.98609, 0.98536, 0.98461, 0.98385, 0.98306, 0.98226, 0.98144, 0.98060, 0.97975, 0.97887, 0.97798, 0.97707, 0.97614, 0.97520, 0.97423, 0.97325, 0.97225, 0.97123, 0.97020, 0.96914, 0.96807, 0.96698, 0.96588, 0.96475, 0.96361, 0.96245, 0.96128, 0.96008, 0.95887, 0.95764, 0.95640, 0.95513, 0.95385, 0.95256, 0.95124, 0.94991, 0.94856, 0.94720, 0.94582, 0.94442, 0.94300, 0.94157, 0.94012, 0.93866, 0.93717, 0.93567, 0.93416, 0.93263, 0.93108, 0.92952, 0.92794, 0.92634, 0.92473, 0.92310, 0.92145, 0.91979, 0.91812, 0.91643, 0.91472, 0.91300, 0.91126, 0.90950, 0.90773, 0.90595, 0.90415, 0.90233, 0.90050, 0.89865, 0.89679, 0.89492, 0.89303, 0.89112, 0.88920, 0.88727, 0.88532, 0.88335, 0.88137, 0.87938, 0.87737, 0.87535, 0.87331, 0.87126, 0.86920, 0.86712, 0.86503, 0.86292, 0.86081, 0.85867, 0.85653, 0.85437, 0.85219, 0.85001, 0.84781, 0.84559, 0.84337, 0.84113, 0.83888, 0.83661, 0.83434, 0.83205, 0.82975, 0.82743, 0.82510, 0.82276, 0.82041, 0.81805, 0.81567, 0.81329, 0.81089, 0.80848, 0.80605, 0.80362, 0.80117, 0.79872, 0.79625, 0.79377, 0.79128, 0.78878, 0.78626, 0.78374, 0.78121, 0.77866, 0.77611, 0.77354, 0.77097, 0.76838, 0.76578, 0.76318, 0.76056, 0.75794, 0.75530, 0.75265, 0.75000, 0.74734, 0.74466, 0.74198, 0.73929, 0.73659, 0.73388, 0.73116, 0.72843, 0.72569, 0.72295, 0.72020, 0.71744, 0.71467, 0.71189, 0.70910, 0.70631, 0.70351, 0.70070, 0.69788, 0.69506, 0.69223, 0.68939, 0.68654, 0.68369, 0.68083, 0.67796, 0.67509, 0.67221, 0.66933, 0.66643, 0.66353, 0.66063, 0.65772, 0.65480, 0.65188, 0.64895, 0.64601, 0.64307, 0.64013, 0.63718, 0.63422, 0.63126, 0.62830, 0.62533, 0.62235, 0.61937, 0.61639, 0.61340, 0.61040, 0.60741, 0.60441, 0.60140, 0.59839, 0.59538, 0.59236, 0.58934, 0.58632, 0.58329, 0.58026, 0.57723, 0.57420, 0.57116, 0.56812, 0.56507, 0.56203, 0.55898, 0.55593, 0.55288, 0.54982, 0.54676, 0.54371, 0.54065, 0.53758, 0.53452, 0.53146, 0.52839, 0.52532, 0.52226, 0.51919, 0.51612, 0.51305, 0.50998, 0.50691, 0.50384, 0.50077, 0.49770, 0.49463, 0.49156, 0.48848, 0.48542, 0.48235, 0.47928, 0.47621, 0.47314, 0.47008, 0.46701, 0.46395, 0.46089, 0.45782, 0.45477, 0.45171, 0.44865, 0.44560, 0.44255, 0.43950, 0.43645, 0.43341, 0.43036, 0.42732, 0.42429, 0.42125, 0.41822, 0.41519, 0.41217, 0.40915, 0.40613, 0.40311, 0.40010, 0.39710, 0.39409, 0.39109, 0.38810, 0.38511, 0.38212, 0.37914, 0.37616, 0.37319, 0.37022, 0.36726, 0.36430, 0.36135, 0.35840, 0.35545, 0.35252, 0.34959, 0.34666, 0.34374, 0.34083, 0.33792, 0.33502, 0.33212, 0.32923, 0.32635, 0.32347, 0.32060, 0.31774, 0.31488, 0.31203, 0.30919, 0.30636, 0.30353, 0.30071, 0.29789, 0.29509, 0.29229, 0.28950, 0.28672, 0.28395, 0.28118, 0.27843, 0.27568, 0.27294, 0.27020, 0.26748, 0.26477, 0.26206, 0.25937, 0.25668, 0.25400, 0.25133, 0.24867, 0.24602, 0.24338, 0.24075, 0.23813, 0.23552, 0.23292, 0.23033, 0.22774, 0.22517, 0.22261, 0.22006, 0.21752, 0.21500, 0.21248, 0.20997, 0.20748, 0.20499, 0.20252, 0.20005, 0.19760, 0.19516, 0.19273, 0.19032, 0.18791, 0.18552, 0.18314, 0.18077, 0.17841, 0.17606, 0.17373, 0.17141, 0.16910, 0.16681, 0.16452, 0.16225, 0.15999, 0.15775, 0.15552, 0.15330, 0.15109, 0.14890, 0.14672, 0.14455, 0.14240, 0.14026, 0.13813, 0.13602, 0.13392, 0.13184, 0.12977, 0.12771, 0.12567, 0.12364, 0.12162, 0.11962, 0.11764, 0.11567, 0.11371, 0.11177, 0.10984, 0.10792, 0.10603, 0.10414, 0.10227, 0.10042, 0.09858, 0.09676, 0.09495, 0.09316, 0.09138, 0.08962, 0.08787, 0.08614, 0.08443, 0.08273, 0.08104, 0.07937, 0.07772, 0.07608, 0.07446, 0.07286, 0.07127, 0.06970, 0.06814, 0.06660, 0.06508, 0.06357, 0.06208, 0.06061, 0.05915, 0.05771, 0.05629, 0.05488, 0.05349, 0.05212, 0.05076, 0.04942, 0.04810, 0.04679, 0.04550, 0.04423, 0.04298, 0.04174, 0.04052, 0.03932, 0.03813, 0.03697, 0.03582, 0.03468, 0.03357, 0.03247, 0.03139, 0.03033, 0.02928, 0.02826, 0.02725, 0.02626, 0.02528, 0.02433, 0.02339, 0.02247, 0.02157, 0.02069, 0.01982, 0.01898, 0.01815, 0.01734, 0.01654, 0.01577, 0.01501, 0.01428, 0.01356, 0.01286, 0.01217, 0.01151, 0.01086, 0.01024, 0.00963, 0.00904, 0.00846, 0.00791, 0.00738, 0.00686, 0.00636, 0.00588, 0.00542, 0.00498, 0.00456, 0.00415, 0.00377, 0.00340, 0.00305, 0.00272, 0.00241, 0.00212, 0.00185, 0.00159, 0.00136, 0.00114, 0.00094, 0.00076, 0.00060, 0.00046, 0.00034, 0.00024, 0.00015, 0.00008, 0.00004, 0.00001, 0.00000};


////////Fast Fourier Transform Data Declaration////////
#define 			N 1024
#define 			LOGN 10
#define 			fftc fft32c1024
int32c dout[N];
int32c din[N];
int32c temp[N];
int32c scratch[N];

////////Auto-wah////////
#define BP_MAX_COEFS 120
#define PI 3.1415926

short center_freq;
short samp_freq;
short counter;
short counter_limit;
short max_freq;
short min_freq;
short f_step;
struct filter filter_struct;
double a[3];
double b[3];
double x[3];
double y[3];


static struct bpcoefficients bandPassCoefficients[BP_MAX_COEFS];

//Delay
#define FLANGER_ONE_MILISECOND_SIZE 48
#define FLANGER_COUNTER_MAX 3360
INT16* flangerDelayArray;
INT32 flanger_samples_to_keep;
INT32 flanger_current_sample_no;
INT16 flanger_delay_time_ms;
INT32 flanger_counter;
INT16 flanger_direction = 1;



//STATE OF THE DEVICE

typedef enum {
    STATE_DISPLAY_SHOW_LOOPBACK,
    STATE_SHOW_CLEAR_LOOPBACK,
    STATE_SHOW_LOOPBACK,
    STATE_DISPLAY_SHOW_USB,
    STATE_SHOW_CLEAR_USB,
    STATE_SHOW_INSERT_USB,
    STATE_SHOW_USB,
    STATE_SHOW_USB_DONE
} STATES;

#define DEBOUNCE_TIME   (100 * (TICKS_PER_SECOND / 1000ull))

GOL_MSG graphicsMessage;
STATES stateScreen = STATE_DISPLAY_SHOW_LOOPBACK;
BOOL muteFlag = TRUE;
BOOL buttonsFlag = FALSE;
UINT32 tickStart;
UINT8 volADC = 100;
UINT8 volDAC = 100;
UINT8 audioIn = LINEIN;
WM8960_SAMPLE_RATE currentSampleRate = SAMPLE_RATE_48000_HZ;
extern const FONT_FLASH Font25;
extern const FONT_FLASH GOLSmallFont;
extern const FONT_FLASH GOLMediumFont;

/*
 * MAIN
 */
int main(void) {

    // Initialize hardware.
    InitializeHardware();

    // Main loop.
    while (1) {
        USBTasks();

        if (GOLDraw()) {
            CheckButtons(&graphicsMessage);
            if (graphicsMessage.uiEvent != EVENT_INVALID) {
                GOLMsg(&graphicsMessage);
            }
        }
    }


}

/**
 * GOLDrawCallback, calls right before the function GOLDraw()
 * @return 
 */
UINT16 GOLDrawCallback(void) {
    switch (stateScreen) {
        case STATE_DISPLAY_SHOW_LOOPBACK:
            muteFlag = TRUE;
            stateScreen = STATE_SHOW_CLEAR_LOOPBACK;
            break;

        case STATE_SHOW_CLEAR_LOOPBACK:
            CreateScreenLoopback();
            stateScreen = STATE_SHOW_LOOPBACK;
            break;

        case STATE_SHOW_LOOPBACK:
            AudioLoopback();
            break;

        case STATE_DISPLAY_SHOW_USB:
            stateScreen = STATE_SHOW_CLEAR_USB;
            break;

        case STATE_SHOW_CLEAR_USB:
            CreateScreenUSB();
            stateScreen = STATE_SHOW_INSERT_USB;
            break;

        case STATE_SHOW_INSERT_USB:
            if (USBHostMSDSCSIMediaDetect()) {
                if (FSInit()) {
                    fxFile = FSfopen("fx.txt", "r");
                    SetFont((void *) &Font25);
                    SetColor(WHITE);
                    OutTextXY(GetMaxX() / 2 - GetTextWidth("Insert USB device...", (void *) &Font25) / 2, 50 + GetTextHeight((void *) &Font25), "Insert USB device...");
                    SetColor(BLACK);
                    OutTextXY(GetMaxX() / 2 - GetTextWidth("USB Device inserted...", (void *) &Font25) / 2, 50 + GetTextHeight((void *) &Font25), "USB Device inserted...");

                    //reset the charArraySize to 0
                    charArraySize = 0;

                    stateScreen = STATE_SHOW_USB;
                }
            }
            break;

        case STATE_SHOW_USB:
            ReadUSBData();
            stateScreen = STATE_SHOW_USB_DONE;
            break;

        case STATE_SHOW_USB_DONE:
            ReadUSBDataDone();
            break;
    }

    return 1;
}

/****************************************************************************
  Function:
    UINT16 GOLMsgCallback( UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This callback is executed when a graphics message is being processed.
    Based on the currently displayed screen, we determine the next operation.

  Precondition:
    None

  Parameters:
    UINT16 translatedMsg  - The message as translated by the processing
                            routine for the object type.
    OBJ_HEADER* pObj    - Pointer to the graphics object header.
    GOL_MSG* pMsg       - Original graphics message.

  Return Values:
    0 - Do not call the default message processing routine for the object.
    1 - Call the default message processing routine for the object.

  Remarks:
    This function is called before the default processing.  If default
    processing is required before other processing, it must be done
    explicitly in this function, and 0 must be returned.  If default
    processing can be performed after this function, simply return 1.
 ***************************************************************************/
UINT16 GOLMsgCallback(UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg) {
    //switch (stateScreen) {
    //}

    return 1;
}

/****************************************************************************
  Function:
    BOOL USB_ApplicationEventHandler( BYTE address, USB_EVENT event,
                void *data, DWORD size )

  Summary:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.

  Description:
    This is the application event handler.  It is called when the stack has
    an event that needs to be handled by the application layer rather than
    by the client driver.  If the application is able to handle the event, it
    returns TRUE.  Otherwise, it returns FALSE.

  Precondition:
    None

  Parameters:
    BYTE address    - Address of device where event occurred
    USB_EVENT event - Identifies the event that occured
    void *data      - Pointer to event-specific data
    DWORD size      - Size of the event-specific data

  Return Values:
    TRUE    - The event was handled
    FALSE   - The event was not handled

  Remarks:
    The application may also implement an event handling routine if it
    requires knowledge of events.  To do so, it must implement a routine that
    matches this function signature and define the USB_HOST_APP_EVENT_HANDLER
    macro as the name of that function.
 ***************************************************************************/

BOOL USB_ApplicationEventHandler(BYTE address, USB_EVENT event, void *data, DWORD size) {
    switch (event) {
        case EVENT_VBUS_REQUEST_POWER:
            // The data pointer points to a byte that represents the amount of power
            // requested in mA, divided by two.  If the device wants too much power,
            // we reject it.
            return TRUE;

        case EVENT_VBUS_RELEASE_POWER:
            //This means that the device was removed
            return TRUE;
            break;

        case EVENT_HUB_ATTACH:
            return TRUE;
            break;

        case EVENT_UNSUPPORTED_DEVICE:
            return TRUE;
            break;

        case EVENT_CANNOT_ENUMERATE:
            //UART2PrintString( "\r\n***** USB Error - cannot enumerate device *****\r\n" );
            return TRUE;
            break;

        case EVENT_CLIENT_INIT_ERROR:
            //UART2PrintString( "\r\n***** USB Error - client driver initialization error *****\r\n" );
            return TRUE;
            break;

        case EVENT_OUT_OF_MEMORY:
            //UART2PrintString( "\r\n***** USB Error - out of heap memory *****\r\n" );
            return TRUE;
            break;

        case EVENT_UNSPECIFIED_ERROR: // This should never be generated.
            //UART2PrintString( "\r\n***** USB Error - unspecified *****\r\n" );
            return TRUE;
            break;

        default:
            break;
    }

    return FALSE;
}

/**
 * INIT HW
 */
void InitializeHardware(void) {
    // Initialize and configure clock for PIC32.
    INTEnableSystemMultiVectoredInt();
    SYSTEMConfigPerformance(GetSystemClock());

    TRISB = 0xffff;
    PORTB = 0xffff;
    // Intialize IO ports used.	
    BUTTON_VOLUME_UP_TRIS = 1;
    BUTTON_VOLUME_DOWN_TRIS = 1;
    BUTTON_NEXT_TRIS = 1;
    BUTTON_PREVIOUS_TRIS = 1;
    PORTSetPinsDigitalOut(IOPORT_D, BIT_0);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_13);
    PORTSetPinsDigitalOut(IOPORT_C, BIT_14);
    PORTClearBits(IOPORT_D, BIT_0);
    PORTClearBits(IOPORT_C, BIT_13);
    PORTClearBits(IOPORT_C, BIT_14);

    // Turn VBUSON ON. I DONT NEED THIS BECAUSE PC HAS POWER
    AD1PCFGbits.PCFG5 = 1; //RB5
    //LCD
    PORTSetPinsDigitalOut(IOPORT_D, BIT_5);
    PORTSetBits(IOPORT_D, BIT_5);

    //INITIALIZE the default FX data
    fxData[0].fuzzIsOn = FALSE;
    fxData[0].fuzzGain = 100;
    fxData[0].fuzzMix = 20;
    fxData[0].overdriveIsOn = FALSE;
    fxData[0].overdriveTreshold = 2000;
    fxData[0].wahIsOn = FALSE;
    fxData[0].wahEffectRate = 2000;
    fxData[0].wahMinf = 300;
    fxData[0].wahMaxf = 1000;
    fxData[0].wahQ = 5;
    fxData[0].bitIsOn = FALSE;
    fxData[0].bitsToKeep = 7;
    fxData[0].compIsOn = FALSE;
    fxData[0].compRatio = 4;
    fxData[0].compTreshold = 2000;
    fxData[0].limitIsOn = FALSE;
    fxData[0].limitTreshold = 2000;
    fxData[0].flangerIsOn = FALSE;
    fxData[0].flangerFrequency = 1;


    //point to the current preset
    fxCurrentData = &fxData[0];


    //Audio
    pCodecHandle = WM8960CodecOpen(O_RDWR);
    WM8960CodecStartAudio(pCodecHandle, TRUE);
    WM8960CodecConfigVolume(pCodecHandle, 0, 0);
    ResetCodec(SAMPLE_RATE_48000_HZ, LINEIN);
    WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);

    // Graphics.
    InitGraph();

    // TIMER ON
    TickInit();

    // Initialize the USB
    USBInitialize(0);

    // Start-up splash.
    SetColor(BLACK);
    ClearDevice();
    SetColor(BLACK);
    ClearDevice();



    SetColor(BLUE);
    SetFont((void *) &GOLMediumFont);
    OutTextXY((GetMaxX() - GetTextWidth("Ceske Vysoke Uceni Technick", (void *) &GOLMediumFont)) / 2, 10, "Ceske Vysoke Uceni Technicke");
    OutTextXY((GetMaxX() - GetTextWidth("v Praze", (void *) &GOLMediumFont)) / 2, 10 + GetTextHeight((void *) &GOLMediumFont), "v Praze");
    OutTextXY((GetMaxX() - GetTextWidth("Fakulta Elektrotechnicka", (void *) &GOLMediumFont)) / 2, 10 + 2 * GetTextHeight((void *) &GOLMediumFont), "Fakulta Elektrotechnicka");

    SetColor(BRIGHTGREEN);
    SetFont((void *) &Font25);
    OutTextXY((GetMaxX() - GetTextWidth("Prototyp Kytaroveho", (void *) &Font25)) / 2, 110, "Prototyp Kytaroveho");
    OutTextXY((GetMaxX() - GetTextWidth("Multiefektu", (void *) &Font25)) / 2, 110 + GetTextHeight((void *) &Font25) - 5, "Multiefektu");
    SetColor(GREEN);
    SetFont((void *) &GOLMediumFont);
    OutTextXY((GetMaxX() - GetTextWidth("Peter Schmiedt", (void *) &GOLMediumFont)) / 2, 110 + 2 * GetTextHeight((void *) &Font25) - 10, "Peter Schmiedt");
    DelayMs(3000);

    BUTTON_VOLUME_UP_TRIS = 1;
    BUTTON_VOLUME_DOWN_TRIS = 1;
    BUTTON_NEXT_TRIS = 1;
    BUTTON_PREVIOUS_TRIS = 1;

}

/**
 * Create screen USB
 */
void CreateScreenUSB(void) {
    GOLFree();
    SetColor(WHITE);
    ClearDevice();
    SetFont((void *) &Font25);
    SetColor(BLACK);
    OutTextXY(90, GetMaxY() - 25, "FX");
    OutTextXY(GetMaxX() / 2 - (GetTextWidth("USB Device", (void *) &Font25) / 2), 50, "USB Device");
    OutTextXY(GetMaxX() / 2 - GetTextWidth("Insert USB device...", (void *) &Font25) / 2, 50 + GetTextHeight((void *) &Font25), "Insert USB device...");

    SetColor(WHITE);
}

/**
 * Read and process the data
 */
inline void ReadUSBData(void) {
    PORTSetBits(IOPORT_C, BIT_13);

    while (!FSfeof(fxFile)) {
        FSfread((char*) &charBuffer, sizeof (char), 1, fxFile);
        charArray[charArraySize] = charBuffer;
        charArraySize++;
    }

    FSfclose(fxFile);

    //PARSE THE DATA
    char* token = strtok(charArray, "\n");
    int i;
    for (i = 0; i < FX_MAX_PRESETS; i++) {
        fxData[i].fuzzIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].fuzzGain = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].fuzzMix = atoi(token);
        token = strtok(NULL, "\n");

        fxData[i].overdriveIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].overdriveTreshold = atoi(token);
        token = strtok(NULL, "\n");

        fxData[i].wahIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].wahEffectRate = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].wahMinf = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].wahMaxf = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].wahQ = atoi(token);
        token = strtok(NULL, "\n");

        fxData[i].bitIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].bitsToKeep = atoi(token);
        token = strtok(NULL, "\n");


        fxData[i].compIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].compRatio = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].compTreshold = atoi(token);
        token = strtok(NULL, "\n");

        fxData[i].limitIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].limitTreshold = atoi(token);
        token = strtok(NULL, "\n");

        fxData[i].flangerIsOn = atoi(token);
        token = strtok(NULL, "\n");
        fxData[i].flangerFrequency = atoi(token);
        token = strtok(NULL, "\n");

        token = strtok(NULL, "\n");

    }

    SetColor(WHITE);
    OutTextXY(GetMaxX() / 2 - GetTextWidth("USB Device inserted...", (void *) &Font25) / 2, 50 + GetTextHeight((void *) &Font25), "USB Device inserted...");

    PORTClearBits(IOPORT_C, BIT_13);
}

/**
 * USB DONE
 */
inline void ReadUSBDataDone(void) {
    PORTSetBits(IOPORT_C, BIT_13);

    SetColor(BLACK);
    OutTextXY(GetMaxX() / 2 - GetTextWidth("DONE", (void *) &Font25) / 2, 50 + GetTextHeight((void *) &Font25), "DONE");

    fxCurrentPreset = 0;
    PORTClearBits(IOPORT_C, BIT_13);
}

/**
 * Create screen loopback
 */
void CreateScreenLoopback(void) {
    GOLFree();
    SetColor(WHITE);
    ClearDevice();

    PutImage(160, GetMaxY() - 30, (void*) &icon_tunmute, 1);


    SetFont((void *) &Font25);
    SetColor(BLACK);
    OutTextXY(80, GetMaxY() - 25, "USB");
    OutTextXY(5, 0, "Preset: ");

    RedrawScreenLoopback();

    ResetCodec(SAMPLE_RATE_48000_HZ, LINEIN);
}

/**
 * Redraw the main display and init the filters
 */
void RedrawScreenLoopback(void) {
    //init overdrive here
    tresholdTimesThree = fxCurrentData->overdriveTreshold * 3;
    //init autowah here
    AutoWah_init(fxCurrentData->wahEffectRate, 24000, fxCurrentData->wahMaxf, fxCurrentData->wahMinf, fxCurrentData->wahQ, 0.707, 10);

    //Init the compressor LPF for envelope smoothing
    EnvelopeInit();

    //initialize the delay
    FlangerInit();


    char print[30];

    SetFont((void *) &Font25);
    SetColor(WHITE);
    Bar(GetTextWidth("Preset:  ", (void *) &Font25), 0, GetTextWidth("Preset:  ", (void *) &Font25) + 30, GetTextHeight((void *) &Font25) + 5);

    SetColor(BLACK);
    sprintf(print, "%i", fxCurrentPreset + 1);
    OutTextXY(GetTextWidth("Preset:  ", (void *) &Font25), 0, print);

    SetLineType(SOLID_LINE);
    SetLineThickness(1);
    Line(0, GetTextHeight((void *) &Font25), GetMaxX(), GetTextHeight((void *) &Font25));

    //create a blank
    SetColor(WHITE);
    Bar(0, GetTextHeight((void*) &Font25), GetMaxX(), GetMaxY() - 30);
    //Print the actual data on the device
    SetFont((void*) &GOLSmallFont);
    SetColor(BLACK);
    SHORT topOffset = GetTextHeight((void*) &Font25);
    SHORT textOffset = GetTextHeight((void*) &GOLSmallFont);
    SHORT blockOffset = 45;
    //FUZZ--------------------------------------------------------------------

    if (fxCurrentData->fuzzIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5, topOffset, "FZ");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5, topOffset, "FZ");
    }
    SetColor(BLACK);

    OutTextXY(5, topOffset + textOffset, "G: ");
    sprintf(print, "%i", fxCurrentData->fuzzGain);
    OutText(print);

    OutTextXY(5, topOffset + textOffset * 2, "M: ");
    sprintf(print, "%i", fxCurrentData->fuzzMix);
    OutText(print);

    //LIMITER--------------------------------------------------------------------
    if (fxCurrentData->limitIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5, topOffset + 3 * textOffset, "LIM");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5, topOffset + 3 * textOffset, "LIM");
    }
    SetColor(BLACK);

    OutTextXY(5, topOffset + 4 * textOffset, "T: ");
    sprintf(print, "%i", fxCurrentData->limitTreshold);
    OutText(print);

    //OVERDRIVE--------------------------------------------------------------------

    if (fxCurrentData->overdriveIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5 + blockOffset, topOffset, "OD");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5 + blockOffset, topOffset, "OD");
    }
    SetColor(BLACK);

    OutTextXY(5 + blockOffset, topOffset + textOffset, "T: ");
    sprintf(print, "%i", fxCurrentData->overdriveTreshold);
    OutText(print);

    //DELAY--------------------------------------------------------------------

    if (fxCurrentData->flangerIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5 + blockOffset, 2 * textOffset + topOffset, "DLY");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5 + blockOffset, 2 * textOffset + topOffset, "DLY");
    }
    SetColor(BLACK);

    OutTextXY(5 + blockOffset, topOffset + textOffset * 3, "T: ");
    sprintf(print, "%i", fxCurrentData->flangerFrequency);
    OutText(print);

    //AUTO-WAH--------------------------------------------------------------------
    if (fxCurrentData->wahIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5 + blockOffset * 2, topOffset, "WAH");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5 + blockOffset * 2, topOffset, "WAH");
    }
    SetColor(BLACK);

    OutTextXY(5 + blockOffset * 2, topOffset + textOffset, "R: ");
    sprintf(print, "%i", fxCurrentData->wahEffectRate);
    OutText(print);

    OutTextXY(5 + blockOffset * 2, topOffset + textOffset * 2, "m: ");
    sprintf(print, "%i", fxCurrentData->wahMinf);
    OutText(print);

    OutTextXY(5 + blockOffset * 2, topOffset + textOffset * 3, "M: ");
    sprintf(print, "%i", fxCurrentData->wahMaxf);
    OutText(print);

    OutTextXY(5 + blockOffset * 2, topOffset + textOffset * 4, "Q: ");
    sprintf(print, "%i", fxCurrentData->wahQ);
    OutText(print);

    //BitCrush--------------------------------------------------------------------
    if (fxCurrentData->bitIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5 + blockOffset * 3, topOffset, "BC");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5 + blockOffset * 3, topOffset, "BC");
    }
    SetColor(BLACK);

    OutTextXY(5 + blockOffset * 3, topOffset + textOffset, "B: ");
    sprintf(print, "%i", fxCurrentData->bitsToKeep);
    OutText(print);

    //Compressor--------------------------------------------------------------------
    if (fxCurrentData->compIsOn == 1) {
        SetColor(BRIGHTGREEN);
        OutTextXY(5 + blockOffset * 3, topOffset + textOffset * 2, "CP");
    } else {
        SetColor(BRIGHTRED);
        OutTextXY(5 + blockOffset * 3, topOffset + textOffset * 2, "CP");
    }
    SetColor(BLACK);

    OutTextXY(5 + blockOffset * 3, topOffset + textOffset * 3, "T: ");
    sprintf(print, "%i", fxCurrentData->compTreshold);
    OutText(print);

    OutTextXY(5 + blockOffset * 3, topOffset + textOffset * 4, "R: ");
    sprintf(print, "%i", fxCurrentData->compRatio);
    OutText(print);
    OutText(":1");
}

void CheckButtons(GOL_MSG *message) {

    //BUTTON_VOLUME_DOWN(S2)
    {
        static BOOL switchBouncing = TRUE;
        static UINT32 switchTime = 0;

        if (switchBouncing) {
            if (BUTTON_VOLUME_DOWN_IO == 0) {
                switchTime = TickGet();
            } else if (TickGet() - switchTime >= DEBOUNCE_TIME) {
                switchBouncing = FALSE;
            }
        } else if (BUTTON_VOLUME_DOWN_IO == 0) {
            switch (stateScreen) {
                case STATE_SHOW_LOOPBACK:
                    if (fxCurrentPreset == 0) {
                        fxCurrentPreset = FX_MAX_PRESETS - 1;
                    } else {
                        fxCurrentPreset--;
                    }

                    fxCurrentData = &fxData[fxCurrentPreset];
                    //redraw the loopback screen with new FX data
                    RedrawScreenLoopback();
                    break;
                default: break;

            }
            switchBouncing = TRUE;

        }
    }

    //BUTTON_VOLUME_UP
    {
        static BOOL switchBouncing = TRUE;
        static UINT32 switchTime = 0;

        if (switchBouncing) {
            if (BUTTON_VOLUME_UP_IO == 0) {
                switchTime = TickGet();
            } else if (TickGet() - switchTime >= DEBOUNCE_TIME) {
                switchBouncing = FALSE;
            }
        } else if (BUTTON_VOLUME_UP_IO == 0) {

            switch (stateScreen) {
                case STATE_SHOW_LOOPBACK:
                    if (fxCurrentPreset == FX_MAX_PRESETS - 1) {
                        fxCurrentPreset = 0;
                    } else {
                        fxCurrentPreset++;
                    }

                    fxCurrentData = &fxData[fxCurrentPreset];
                    //redraw the loopback screen with new FX data
                    RedrawScreenLoopback();
                    break;
                default: break;

            }
            switchBouncing = TRUE;
        }
    }

    //BUTTON_NEXT
    {
        UINT8 *pCommandData;
        static BOOL switchBouncing = TRUE;
        static UINT32 switchTime = 0;

        if (switchBouncing) {
            if (BUTTON_NEXT_IO == 0) {
                switchTime = TickGet();
            } else if (TickGet() - switchTime >= DEBOUNCE_TIME) {
                switchBouncing = FALSE;
            }
        } else if (BUTTON_NEXT_IO == 0) {
            switch (stateScreen) {
                case STATE_SHOW_LOOPBACK:
                    if (muteFlag) {
                        WM8960CodecConfigDACMute(pCodecHandle, TRUE);
                        PutImage(160, GetMaxY() - 30, (void*) &icon_tmute, 1);
                        muteFlag = FALSE;
                    } else {
                        WM8960CodecConfigDACMute(pCodecHandle, FALSE);
                        PutImage(160, GetMaxY() - 30, (void*) &icon_tunmute, 1);
                        muteFlag = TRUE;
                    }
                    break;
                default: break;

            }
            switchBouncing = TRUE;
        }
    }

    //BUTTON_PREVIOUS
    {
        UINT8 *pCommandData;
        static BOOL switchBouncing = TRUE;
        static UINT32 switchTime = 0;

        if (switchBouncing) {
            if (BUTTON_PREVIOUS_IO == 0) {
                switchTime = TickGet();
            } else if (TickGet() - switchTime >= DEBOUNCE_TIME) {
                switchBouncing = FALSE;
            }
        } else if (BUTTON_PREVIOUS_IO == 0) {

            switch (stateScreen) {
                case STATE_SHOW_LOOPBACK:
                    muteFlag = FALSE;
                    stateScreen = STATE_DISPLAY_SHOW_USB;
                    ResetCodec();
                    break;

                case STATE_SHOW_USB:
                    stateScreen = STATE_DISPLAY_SHOW_LOOPBACK;
                    break;
                case STATE_SHOW_INSERT_USB:
                    stateScreen = STATE_DISPLAY_SHOW_LOOPBACK;
                    break;
                case STATE_SHOW_USB_DONE:
                    stateScreen = STATE_DISPLAY_SHOW_LOOPBACK;
                    break;

                default: break;

            }

            PORTClearBits(IOPORT_D, BIT_0);
            PORTClearBits(IOPORT_C, BIT_13);
            PORTClearBits(IOPORT_C, BIT_14);
            switchBouncing = TRUE;
        }
    }


}

/**
 * RESET CODEC
 */
void ResetCodec() {
    WM8960CodecClear(pCodecHandle);
    if (currentSampleRate != SAMPLE_RATE_NO_CHANGE)
        WM8960CodecConfigSampleRate(pCodecHandle, currentSampleRate);

    DelayMs(10);

    WM8960CodecEnableMicrophone(pCodecHandle, FALSE);
    WM8960CodecEnableLineIn(pCodecHandle, TRUE);

}

/**
 * MAIN AUDIO LOOP
 */
inline void AudioLoopback(void) {
    WM8960CodecRead(pCodecHandle, Sin, FRAME_SIZE);
    PORTSetBits(IOPORT_D, BIT_0);

    compCoefficientMin = 1000;

    int i;
    for (i = 0; i < FRAME_SIZE; i++) {

        if (fxCurrentData->limitIsOn == 1) {
            Limiter(i);
        }

        Envelope(Sin[i].leftChannel);

        if (fxCurrentData->wahIsOn == 1) {
            Sin[i].leftChannel = AutoWah_process(Sin[i].leftChannel) * 20;
            AutoWah_sweep();
        }

        if (fxCurrentData->overdriveIsOn == 1) {
            Overdrive(i);
        }
        if (fxCurrentData->fuzzIsOn == 1) {
            Fuzz(i);
        }

        if (fxCurrentData->bitIsOn == 1) {
            BitCrusher(i);
        }

        if (fxCurrentData->flangerIsOn == 1) {
            FlangerProcess(i);
            FlangerSweep();
        }


        if (fxCurrentData->compIsOn == 1) {
            Compressor(i);
        }


    }

    for (i = 0; i < FRAME_SIZE; i++) {
        Sin[i].rightChannel = Sin[i].leftChannel;
    }

    WM8960CodecWrite(pCodecHandle, Sin, FRAME_SIZE);
    PORTClearBits(IOPORT_D, BIT_0);
}

/**
 * Flanger Init
 */
inline void FlangerInit() {
    flanger_samples_to_keep = 960; //ONE_MILISECOND_SIZE * fxCurrentData->delayTime;
    flanger_current_sample_no = 0;
    flanger_delay_time_ms = 1;
    flanger_counter = FLANGER_COUNTER_MAX;
    flanger_direction = 1;

    free(flangerDelayArray);
    flangerDelayArray = malloc(960 * sizeof (INT16));

    int i;
    for (i = 0; i < flanger_samples_to_keep; i++) {
        flangerDelayArray[i] = 0;
    }

    flanger_samples_to_keep = flanger_delay_time_ms * FLANGER_ONE_MILISECOND_SIZE;

}

/**
 * Processes the Flanger effect
 * @param i
 */
inline void FlangerProcess(int i) {
    Sin[i].leftChannel += flangerDelayArray[flanger_current_sample_no];
    flangerDelayArray[flanger_current_sample_no] = (flangerDelayArray[flanger_current_sample_no] + Sin[i].leftChannel) * 0.45f; // fxCurrentData->delayDecay; 
    //720 forward feed
    if (flanger_direction == 1 && flanger_current_sample_no < FLANGER_ONE_MILISECOND_SIZE) {
        flangerDelayArray[flanger_current_sample_no + (flanger_delay_time_ms * FLANGER_ONE_MILISECOND_SIZE)] = Sin[i].leftChannel * 0.45f;
    }


    flanger_current_sample_no++;
    if (flanger_current_sample_no >= flanger_samples_to_keep) {
        flanger_current_sample_no = 0;
    }
}
/**
 * Emulate the LFO of 1HZ
 */
inline void FlangerSweep() {
    if (!--flanger_counter) {
        flanger_delay_time_ms += flanger_direction;
        flanger_samples_to_keep = flanger_delay_time_ms * FLANGER_ONE_MILISECOND_SIZE;

        if (flanger_delay_time_ms >= 15) {
            flanger_direction = -1;
        }

        if (flanger_delay_time_ms <= 1) {
            flanger_direction = 1;
        }

        flanger_counter = fxCurrentData->flangerFrequency * 3200;
    }
}
/**
 * Compressor
 * @param i
 */
void Compressor(int i) {

    if (y[2] > fxCurrentData->compTreshold) {
        compCoefficient = pow((y[2] / fxCurrentData->compTreshold), (1 / fxCurrentData->compRatio - 1));
        Sin[i].leftChannel *= compCoefficient;
    }
}
/*
 * Limiter
 */
void Limiter(int i) {

    if (y[2] > fxCurrentData->limitTreshold) {
        compCoefficient = pow((y[2] / fxCurrentData->limitTreshold), (1 / 10 - 1));
        Sin[i].leftChannel *= compCoefficient;
    }
}

/*
 * BitCrush
 */
void BitCrusher(int i) {
    Sin[i].leftChannel = Sin[i].leftChannel & (-1 << fxCurrentData->bitsToKeep);
}

/*
 * Fuzz
 */
void Fuzz(int i) {
    q = fxCurrentData->fuzzGain * Sin[i].leftChannel;
    if (q > 0) {
        z = (1 - exp(-abs(q / INT16_MAX))) * INT16_MAX;
    } else {
        z = -(1 - exp(-abs(q / INT16_MAX))) * INT16_MAX;
    }
    Sin[i].leftChannel = fxCurrentData->fuzzMix * z + (1 - fxCurrentData->fuzzMix) * Sin[i].leftChannel;
}

/*
 * Overdrive
 */
void Overdrive(int i) {

    INT16 current = abs(Sin[i].leftChannel);

    if (current < fxCurrentData->overdriveTreshold) {
        Sin[i].leftChannel *= 2;
    } else if (current < fxCurrentData->overdriveTreshold * 2) {
        if (Sin[i].leftChannel > 0) {
            Sin[i].leftChannel = (3 - pow(2 - 3 * Sin[i].leftChannel / tresholdTimesThree, 2)) / (3 / tresholdTimesThree);
        } else {
            Sin[i].leftChannel = -(3 - pow(2 - 3 * Sin[i].leftChannel / tresholdTimesThree, 2)) / (3 / tresholdTimesThree);
        }
    } else {
        if (Sin[i].leftChannel > 0) {
            Sin[i].leftChannel = tresholdTimesThree;
        } else {
            Sin[i].leftChannel = -tresholdTimesThree;
        }
    }
}


/*
 * Load Coefficients for the LP filter
 */
inline void EnvelopeInit() {
    double C = 1018.59;

    b[0] = 1 / (1 + 2 * 0.7071 * C + pow(C, 2));
    b[1] = 2 * b[0];
    b[2] = b[0];

    a[1] = 2 * b[0]*(1 - pow(C, 2));
    a[2] = b[0]*(1 - 2 * 0.7071 * C + pow(C, 2));
}

/*
 * Simple LP filter to process the envelope
 */
inline void Envelope(double in) {
    x[0] = x[1];
    x[1] = x[2];

    x[2] = abs(in);

    y[0] = y[1];
    y[1] = y[2];

    y[2] = b[0] * x[2];
    y[2] += b[1] * x[1];
    y[2] += b[2] * x[0];
    y[2] -= a[1] * y[1];
    y[2] -= a[2] * y[0];
}

/*
 * Initalize the AutoWah filter
 */
void AutoWah_init(short effect_rate, short sampling, short maxf, short minf, short Q, double gainfactor, short freq_step) {
    center_freq = 0;
    samp_freq = sampling;
    counter = effect_rate;


    counter_limit = effect_rate;

    min_freq = 0;
    max_freq = (maxf - minf) / freq_step;

    BandPassInitialize(sampling, gainfactor, Q, freq_step, minf);
    f_step = freq_step;

}

/*
 * Process the signal here
 */
double AutoWah_process(int xin) {
    double yout;

    yout = BandPassProcess(xin, &filter_struct);

    return yout;
}

/*
 * Do a change in the Band-pass filter acording to the signal envelope
 */
void AutoWah_sweep() {
    unsigned int filter_index;
    double out;

    if (!--counter) {
        filter_index = (double) min_freq + y[2] * 0.1; //--------------------------------------------------------------------------------------------------------------------

        if (filter_index > max_freq) {
            filter_index = max_freq;
        }
        if (filter_index < 0) {
            filter_index = 0;
        }

        BandPassSetup(&filter_struct, filter_index);

        counter = counter_limit;
    }
}

/*
 * This function initializes the band-pass filter
 */
void BandPassInitialize(double fsfilt, double gb, double Q, short fstep, short fmin) {
    int i;
    double damp;
    double wo;

    damp = gb / sqrt(1 - pow(gb, 2));

    for (i = 0; i < BP_MAX_COEFS; i++) {
        wo = 2 * PI * (fstep * i + fmin) / fsfilt;
        bandPassCoefficients[i].e = 1 / (1 + damp * tan(wo / (Q * 2)));
        bandPassCoefficients[i].p = cos(wo);
        bandPassCoefficients[i].d[0] = (1 - bandPassCoefficients[i].e);
        bandPassCoefficients[i].d[1] = 2 * bandPassCoefficients[i].e * bandPassCoefficients[i].p;
        bandPassCoefficients[i].d[2] = (2 * bandPassCoefficients[i].e - 1);
    }
}

/*
 * This function loads the new coefficients for the Band-pass filter
 */
void BandPassSetup(struct filter * filter_struct, int ind) {
    filter_struct->e = bandPassCoefficients[ind].e;
    filter_struct->p = bandPassCoefficients[ind].p;
    filter_struct->d[0] = bandPassCoefficients[ind].d[0];
    filter_struct->d[1] = bandPassCoefficients[ind].d[1];
    filter_struct->d[2] = bandPassCoefficients[ind].d[2];
}

/*
 * Processes the Band-pass filter
 */
double BandPassProcess(double yin, struct filter * filter_struct) {
    double yout;

    filter_struct->x[0] = filter_struct->x[1];
    filter_struct->x[1] = filter_struct->x[2];
    filter_struct->x[2] = yin;

    filter_struct->y[0] = filter_struct->y[1];
    filter_struct->y[1] = filter_struct->y[2];

    filter_struct->y[2] = filter_struct->d[0] * filter_struct->x[2] - filter_struct->d[0] * filter_struct->x[0] + (filter_struct->d[1] * filter_struct->y[1]) - filter_struct->d[2] * filter_struct->y[0];

    yout = filter_struct->y[2];

    return yout;
}