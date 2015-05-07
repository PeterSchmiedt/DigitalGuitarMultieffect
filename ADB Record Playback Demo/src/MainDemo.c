/*****************************************************************************
 * Microchip Graphics Library Demo Application
 * This program shows how to use the Graphics Primitives Layer.
 *****************************************************************************
 * FileName:        MainDemo.c
 * Dependencies:    MainDemo.h
 * Processor:       PIC32
 * Compiler:       	MPLAB C32
 * Linker:          MPLAB LINK32
 * Company:         Microchip Technology Incorporated
 *
 * Software License Agreement
 *
 * Copyright © 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 *****************************************************************************/

#include "MainDemo.h"

/*****************************************************************************
 ******************************************************************************
 Configuration pragma.
 ******************************************************************************
 *****************************************************************************/
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

FSFILE* outData;

////////Pitch Shifter Variables Declaration////////
//#define             WINDOW_SIZE  512
//#define             HOP_SIZE    128
//#define             FRAMES      8
//INT8 step = 12; //TODO change this later on now it is defined here, should be defined elswere
const float hanningWindow[FRAME_SIZE] = {0.00000, 0.00001, 0.00004, 0.00008, 0.00015, 0.00024, 0.00034, 0.00046, 0.00060, 0.00076, 0.00094, 0.00114, 0.00136, 0.00159, 0.00185, 0.00212, 0.00241, 0.00272, 0.00305, 0.00340, 0.00377, 0.00415, 0.00456, 0.00498, 0.00542, 0.00588, 0.00636, 0.00686, 0.00738, 0.00791, 0.00846, 0.00904, 0.00963, 0.01024, 0.01086, 0.01151, 0.01217, 0.01286, 0.01356, 0.01428, 0.01501, 0.01577, 0.01654, 0.01734, 0.01815, 0.01898, 0.01982, 0.02069, 0.02157, 0.02247, 0.02339, 0.02433, 0.02528, 0.02626, 0.02725, 0.02826, 0.02928, 0.03033, 0.03139, 0.03247, 0.03357, 0.03468, 0.03582, 0.03697, 0.03813, 0.03932, 0.04052, 0.04174, 0.04298, 0.04423, 0.04550, 0.04679, 0.04810, 0.04942, 0.05076, 0.05212, 0.05349, 0.05488, 0.05629, 0.05771, 0.05915, 0.06061, 0.06208, 0.06357, 0.06508, 0.06660, 0.06814, 0.06970, 0.07127, 0.07286, 0.07446, 0.07608, 0.07772, 0.07937, 0.08104, 0.08273, 0.08443, 0.08614, 0.08787, 0.08962, 0.09138, 0.09316, 0.09495, 0.09676, 0.09858, 0.10042, 0.10227, 0.10414, 0.10603, 0.10792, 0.10984, 0.11177, 0.11371, 0.11567, 0.11764, 0.11962, 0.12162, 0.12364, 0.12567, 0.12771, 0.12977, 0.13184, 0.13392, 0.13602, 0.13813, 0.14026, 0.14240, 0.14455, 0.14672, 0.14890, 0.15109, 0.15330, 0.15552, 0.15775, 0.15999, 0.16225, 0.16452, 0.16681, 0.16910, 0.17141, 0.17373, 0.17606, 0.17841, 0.18077, 0.18314, 0.18552, 0.18791, 0.19032, 0.19273, 0.19516, 0.19760, 0.20005, 0.20252, 0.20499, 0.20748, 0.20997, 0.21248, 0.21500, 0.21752, 0.22006, 0.22261, 0.22517, 0.22774, 0.23033, 0.23292, 0.23552, 0.23813, 0.24075, 0.24338, 0.24602, 0.24867, 0.25133, 0.25400, 0.25668, 0.25937, 0.26206, 0.26477, 0.26748, 0.27020, 0.27294, 0.27568, 0.27843, 0.28118, 0.28395, 0.28672, 0.28950, 0.29229, 0.29509, 0.29789, 0.30071, 0.30353, 0.30636, 0.30919, 0.31203, 0.31488, 0.31774, 0.32060, 0.32347, 0.32635, 0.32923, 0.33212, 0.33502, 0.33792, 0.34083, 0.34374, 0.34666, 0.34959, 0.35252, 0.35545, 0.35840, 0.36135, 0.36430, 0.36726, 0.37022, 0.37319, 0.37616, 0.37914, 0.38212, 0.38511, 0.38810, 0.39109, 0.39409, 0.39710, 0.40010, 0.40311, 0.40613, 0.40915, 0.41217, 0.41519, 0.41822, 0.42125, 0.42429, 0.42732, 0.43036, 0.43341, 0.43645, 0.43950, 0.44255, 0.44560, 0.44865, 0.45171, 0.45477, 0.45782, 0.46089, 0.46395, 0.46701, 0.47008, 0.47314, 0.47621, 0.47928, 0.48235, 0.48542, 0.48848, 0.49156, 0.49463, 0.49770, 0.50077, 0.50384, 0.50691, 0.50998, 0.51305, 0.51612, 0.51919, 0.52226, 0.52532, 0.52839, 0.53146, 0.53452, 0.53758, 0.54065, 0.54371, 0.54676, 0.54982, 0.55288, 0.55593, 0.55898, 0.56203, 0.56507, 0.56812, 0.57116, 0.57420, 0.57723, 0.58026, 0.58329, 0.58632, 0.58934, 0.59236, 0.59538, 0.59839, 0.60140, 0.60441, 0.60741, 0.61040, 0.61340, 0.61639, 0.61937, 0.62235, 0.62533, 0.62830, 0.63126, 0.63422, 0.63718, 0.64013, 0.64307, 0.64601, 0.64895, 0.65188, 0.65480, 0.65772, 0.66063, 0.66353, 0.66643, 0.66933, 0.67221, 0.67509, 0.67796, 0.68083, 0.68369, 0.68654, 0.68939, 0.69223, 0.69506, 0.69788, 0.70070, 0.70351, 0.70631, 0.70910, 0.71189, 0.71467, 0.71744, 0.72020, 0.72295, 0.72569, 0.72843, 0.73116, 0.73388, 0.73659, 0.73929, 0.74198, 0.74466, 0.74734, 0.75000, 0.75265, 0.75530, 0.75794, 0.76056, 0.76318, 0.76578, 0.76838, 0.77097, 0.77354, 0.77611, 0.77866, 0.78121, 0.78374, 0.78626, 0.78878, 0.79128, 0.79377, 0.79625, 0.79872, 0.80117, 0.80362, 0.80605, 0.80848, 0.81089, 0.81329, 0.81567, 0.81805, 0.82041, 0.82276, 0.82510, 0.82743, 0.82975, 0.83205, 0.83434, 0.83661, 0.83888, 0.84113, 0.84337, 0.84559, 0.84781, 0.85001, 0.85219, 0.85437, 0.85653, 0.85867, 0.86081, 0.86292, 0.86503, 0.86712, 0.86920, 0.87126, 0.87331, 0.87535, 0.87737, 0.87938, 0.88137, 0.88335, 0.88532, 0.88727, 0.88920, 0.89112, 0.89303, 0.89492, 0.89679, 0.89865, 0.90050, 0.90233, 0.90415, 0.90595, 0.90773, 0.90950, 0.91126, 0.91300, 0.91472, 0.91643, 0.91812, 0.91979, 0.92145, 0.92310, 0.92473, 0.92634, 0.92794, 0.92952, 0.93108, 0.93263, 0.93416, 0.93567, 0.93717, 0.93866, 0.94012, 0.94157, 0.94300, 0.94442, 0.94582, 0.94720, 0.94856, 0.94991, 0.95124, 0.95256, 0.95385, 0.95513, 0.95640, 0.95764, 0.95887, 0.96008, 0.96128, 0.96245, 0.96361, 0.96475, 0.96588, 0.96698, 0.96807, 0.96914, 0.97020, 0.97123, 0.97225, 0.97325, 0.97423, 0.97520, 0.97614, 0.97707, 0.97798, 0.97887, 0.97975, 0.98060, 0.98144, 0.98226, 0.98306, 0.98385, 0.98461, 0.98536, 0.98609, 0.98680, 0.98749, 0.98816, 0.98882, 0.98945, 0.99007, 0.99067, 0.99125, 0.99182, 0.99236, 0.99288, 0.99339, 0.99388, 0.99435, 0.99480, 0.99523, 0.99565, 0.99604, 0.99642, 0.99678, 0.99711, 0.99743, 0.99774, 0.99802, 0.99828, 0.99853, 0.99875, 0.99896, 0.99915, 0.99932, 0.99947, 0.99960, 0.99971, 0.99981, 0.99988, 0.99994, 0.99998, 1.00000, 1.00000, 0.99998, 0.99994, 0.99988, 0.99981, 0.99971, 0.99960, 0.99947, 0.99932, 0.99915, 0.99896, 0.99875, 0.99853, 0.99828, 0.99802, 0.99774, 0.99743, 0.99711, 0.99678, 0.99642, 0.99604, 0.99565, 0.99523, 0.99480, 0.99435, 0.99388, 0.99339, 0.99288, 0.99236, 0.99182, 0.99125, 0.99067, 0.99007, 0.98945, 0.98882, 0.98816, 0.98749, 0.98680, 0.98609, 0.98536, 0.98461, 0.98385, 0.98306, 0.98226, 0.98144, 0.98060, 0.97975, 0.97887, 0.97798, 0.97707, 0.97614, 0.97520, 0.97423, 0.97325, 0.97225, 0.97123, 0.97020, 0.96914, 0.96807, 0.96698, 0.96588, 0.96475, 0.96361, 0.96245, 0.96128, 0.96008, 0.95887, 0.95764, 0.95640, 0.95513, 0.95385, 0.95256, 0.95124, 0.94991, 0.94856, 0.94720, 0.94582, 0.94442, 0.94300, 0.94157, 0.94012, 0.93866, 0.93717, 0.93567, 0.93416, 0.93263, 0.93108, 0.92952, 0.92794, 0.92634, 0.92473, 0.92310, 0.92145, 0.91979, 0.91812, 0.91643, 0.91472, 0.91300, 0.91126, 0.90950, 0.90773, 0.90595, 0.90415, 0.90233, 0.90050, 0.89865, 0.89679, 0.89492, 0.89303, 0.89112, 0.88920, 0.88727, 0.88532, 0.88335, 0.88137, 0.87938, 0.87737, 0.87535, 0.87331, 0.87126, 0.86920, 0.86712, 0.86503, 0.86292, 0.86081, 0.85867, 0.85653, 0.85437, 0.85219, 0.85001, 0.84781, 0.84559, 0.84337, 0.84113, 0.83888, 0.83661, 0.83434, 0.83205, 0.82975, 0.82743, 0.82510, 0.82276, 0.82041, 0.81805, 0.81567, 0.81329, 0.81089, 0.80848, 0.80605, 0.80362, 0.80117, 0.79872, 0.79625, 0.79377, 0.79128, 0.78878, 0.78626, 0.78374, 0.78121, 0.77866, 0.77611, 0.77354, 0.77097, 0.76838, 0.76578, 0.76318, 0.76056, 0.75794, 0.75530, 0.75265, 0.75000, 0.74734, 0.74466, 0.74198, 0.73929, 0.73659, 0.73388, 0.73116, 0.72843, 0.72569, 0.72295, 0.72020, 0.71744, 0.71467, 0.71189, 0.70910, 0.70631, 0.70351, 0.70070, 0.69788, 0.69506, 0.69223, 0.68939, 0.68654, 0.68369, 0.68083, 0.67796, 0.67509, 0.67221, 0.66933, 0.66643, 0.66353, 0.66063, 0.65772, 0.65480, 0.65188, 0.64895, 0.64601, 0.64307, 0.64013, 0.63718, 0.63422, 0.63126, 0.62830, 0.62533, 0.62235, 0.61937, 0.61639, 0.61340, 0.61040, 0.60741, 0.60441, 0.60140, 0.59839, 0.59538, 0.59236, 0.58934, 0.58632, 0.58329, 0.58026, 0.57723, 0.57420, 0.57116, 0.56812, 0.56507, 0.56203, 0.55898, 0.55593, 0.55288, 0.54982, 0.54676, 0.54371, 0.54065, 0.53758, 0.53452, 0.53146, 0.52839, 0.52532, 0.52226, 0.51919, 0.51612, 0.51305, 0.50998, 0.50691, 0.50384, 0.50077, 0.49770, 0.49463, 0.49156, 0.48848, 0.48542, 0.48235, 0.47928, 0.47621, 0.47314, 0.47008, 0.46701, 0.46395, 0.46089, 0.45782, 0.45477, 0.45171, 0.44865, 0.44560, 0.44255, 0.43950, 0.43645, 0.43341, 0.43036, 0.42732, 0.42429, 0.42125, 0.41822, 0.41519, 0.41217, 0.40915, 0.40613, 0.40311, 0.40010, 0.39710, 0.39409, 0.39109, 0.38810, 0.38511, 0.38212, 0.37914, 0.37616, 0.37319, 0.37022, 0.36726, 0.36430, 0.36135, 0.35840, 0.35545, 0.35252, 0.34959, 0.34666, 0.34374, 0.34083, 0.33792, 0.33502, 0.33212, 0.32923, 0.32635, 0.32347, 0.32060, 0.31774, 0.31488, 0.31203, 0.30919, 0.30636, 0.30353, 0.30071, 0.29789, 0.29509, 0.29229, 0.28950, 0.28672, 0.28395, 0.28118, 0.27843, 0.27568, 0.27294, 0.27020, 0.26748, 0.26477, 0.26206, 0.25937, 0.25668, 0.25400, 0.25133, 0.24867, 0.24602, 0.24338, 0.24075, 0.23813, 0.23552, 0.23292, 0.23033, 0.22774, 0.22517, 0.22261, 0.22006, 0.21752, 0.21500, 0.21248, 0.20997, 0.20748, 0.20499, 0.20252, 0.20005, 0.19760, 0.19516, 0.19273, 0.19032, 0.18791, 0.18552, 0.18314, 0.18077, 0.17841, 0.17606, 0.17373, 0.17141, 0.16910, 0.16681, 0.16452, 0.16225, 0.15999, 0.15775, 0.15552, 0.15330, 0.15109, 0.14890, 0.14672, 0.14455, 0.14240, 0.14026, 0.13813, 0.13602, 0.13392, 0.13184, 0.12977, 0.12771, 0.12567, 0.12364, 0.12162, 0.11962, 0.11764, 0.11567, 0.11371, 0.11177, 0.10984, 0.10792, 0.10603, 0.10414, 0.10227, 0.10042, 0.09858, 0.09676, 0.09495, 0.09316, 0.09138, 0.08962, 0.08787, 0.08614, 0.08443, 0.08273, 0.08104, 0.07937, 0.07772, 0.07608, 0.07446, 0.07286, 0.07127, 0.06970, 0.06814, 0.06660, 0.06508, 0.06357, 0.06208, 0.06061, 0.05915, 0.05771, 0.05629, 0.05488, 0.05349, 0.05212, 0.05076, 0.04942, 0.04810, 0.04679, 0.04550, 0.04423, 0.04298, 0.04174, 0.04052, 0.03932, 0.03813, 0.03697, 0.03582, 0.03468, 0.03357, 0.03247, 0.03139, 0.03033, 0.02928, 0.02826, 0.02725, 0.02626, 0.02528, 0.02433, 0.02339, 0.02247, 0.02157, 0.02069, 0.01982, 0.01898, 0.01815, 0.01734, 0.01654, 0.01577, 0.01501, 0.01428, 0.01356, 0.01286, 0.01217, 0.01151, 0.01086, 0.01024, 0.00963, 0.00904, 0.00846, 0.00791, 0.00738, 0.00686, 0.00636, 0.00588, 0.00542, 0.00498, 0.00456, 0.00415, 0.00377, 0.00340, 0.00305, 0.00272, 0.00241, 0.00212, 0.00185, 0.00159, 0.00136, 0.00114, 0.00094, 0.00076, 0.00060, 0.00046, 0.00034, 0.00024, 0.00015, 0.00008, 0.00004, 0.00001, 0.00000};

//INT16 *frameMatrix[FRAMES][WINDOW_SIZE];

////////Fast Fourier Transform Data Declaration////////
#define 			N 1024
#define 			LOGN 10
#define 			fftc fft32c1024
int32c dout[N];
int32c din[N];
int32c temp[N];
int32c scratch[N];

////////Graphics Data Declaration////////

typedef enum {
    STATE_DISPLAY_DEMO_SELECTION,
    STATE_DEMO_SELECTION,
    STATE_DISPLAY_SHOW_RECORD,
    STATE_DISPLAY_INSERT_USB_RECORD,
    STATE_INSERT_USB_RECORD,
    STATE_SHOW_CLEAR_RECORD,
    STATE_SHOW_RECORD,
    STATE_DISPLAY_SHOW_PLAYBACK_CLEAR,
    STATE_DISPLAY_SHOW_PLAYBACK,
    STATE_INSERT_USB_PLAYBACK,
    STATE_SHOW_CLEAR_PLAYBACK,
    STATE_SHOW_PLAYBACK,
    STATE_DISPLAY_SHOW_LOOPBACK,
    STATE_SHOW_CLEAR_LOOPBACK,
    STATE_SHOW_LOOPBACK,
    STATE_DISPLAY_SHOW_SETTINGS,
    STATE_SHOW_CLEAR_SETTINGS,
    STATE_SHOW_SETTINGS,
} STATES_GRAPHICS;

typedef enum {
    ID_WINDOW,
    ID_RECORD,
    ID_PLAYBACK,
    ID_LOOPBACK,
    ID_SETTINGS,
    ID_STATICTEXT1,
    ID_SLIDER1,
    ID_SLIDER2,
    ID_RADIOBUTTON1,
    ID_RADIOBUTTON2,
    ID_RADIOBUTTON3,
    ID_RADIOBUTTON4,
    ID_RADIOBUTTON5,
    ID_RADIOBUTTON6,
    ID_RADIOBUTTON7
} GRAPHIC_IDS;

typedef enum {
    TAB_RECORD,
    TAB_PLAYBACK,
    TAB_LOOPBACK,
    TAB_SETTINGS,
} GRAPHIC_TABS;

#define 			ADJUST_PIXELS 10
GOL_SCHEME *altScheme;
GOL_MSG graphicsMessage;
STATES_GRAPHICS stateScreen;
STATES_GRAPHICS stateScreen = STATE_DISPLAY_DEMO_SELECTION;
GRAPHIC_TABS screenTab = TAB_RECORD;
SLIDER *volADCIndication;
SLIDER *volDACIndication;
BUTTON *record;
BUTTON *playback;
BUTTON *loopback;
BUTTON *settings;
BOOL playbackFlag;
BOOL playFlag = TRUE;
BOOL muteFlag = TRUE;
BOOL buttonsFlag = FALSE;
BOOL recordFlag = FALSE;
BOOL playbackFlag = TRUE;
UINT32 tickStart;
UINT16 dataCount;
UINT16 secCount = 0;
UINT8 volADC = 100;
UINT8 volDAC = 100;
UINT8 tabSetting = 1;
UINT8 audioIn = MICROPHONE;
BYTE countDemo = 1;
BYTE countSetting = 2;
char secBuffer[10];
WM8960_SAMPLE_RATE currentSampleRate = SAMPLE_RATE_48000_HZ;
extern const FONT_FLASH Font25;
extern const FONT_FLASH GOLSmallFont;
extern const FONT_FLASH GOLMediumFont;

/*****************************************************************************
 ******************************************************************************
 Main
 ******************************************************************************
 *****************************************************************************/
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

/****************************************************************************
  Function:
    UINT16 GOLDrawCallback( void )

  Description:
    This callback is performed at the beginning of the GOLDraw() function.
    Processing that needs to be done on a regular basis and is not triggered
    by a message should be done here.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    All graphic control adding and removing must be done from this function,
    not from the message callback.
 ***************************************************************************/
UINT16 GOLDrawCallback(void) {
    switch (stateScreen) {
        case STATE_DISPLAY_DEMO_SELECTION:
            ShowScreenDemoSelection();
            stateScreen = STATE_DEMO_SELECTION;
            break;

        case STATE_DEMO_SELECTION:
            if (buttonsFlag)
                CheckScreenDemoSelection();
            break;

        case STATE_DISPLAY_SHOW_RECORD:
            stateScreen = STATE_DISPLAY_INSERT_USB_RECORD;
            break;

        case STATE_DISPLAY_INSERT_USB_RECORD:
            ShowInsertUSBRecord();
            stateScreen = STATE_INSERT_USB_RECORD;
            break;

        case STATE_INSERT_USB_RECORD:
            if (USBHostMSDSCSIMediaDetect()) {
                if (FSInit()) {
                    outData = FSfopen("output2.pcm", "w");
                    SetFont((void *) &Font25);
                    SetColor(BLACK);
                    OutTextXY(20, 40, "Insert USB Drive!");
                    OutTextXY(20, 70, "Status: Stopped...");
                    SetColor(BRIGHTBLUE);
                    OutTextXY(20, 40, "USB Drive Plugged-in!");
                    OutTextXY(20, 70, "Status: Recording");
                    if (audioIn == MICROPHONE)
                        OutTextXY(85, 90, "on MIC...");
                    else if (audioIn == LINEIN)
                        OutTextXY(85, 90, "on Line-IN...");
                    stateScreen = STATE_SHOW_CLEAR_RECORD;
                }
            }
            break;

        case STATE_SHOW_CLEAR_RECORD:
            SetFont((void *) &Font25);
            SetColor(BRIGHTBLUE);
            OutTextXY(20, 110, "Time Elapsed: ");
            secCount = 0;
            uitoa2(secCount, secBuffer);
            strcat(secBuffer, " sec");
            SetColor(BRIGHTRED);
            OutTextXY(145, 110, secBuffer);
            tickStart = TickGet();
            stateScreen = STATE_SHOW_RECORD;
            break;

        case STATE_SHOW_RECORD:
            if (!recordFlag)
                ShowScreenRecord();
            break;

        case STATE_DISPLAY_SHOW_PLAYBACK:
            playFlag = TRUE;
            playbackFlag = TRUE;
            stateScreen = STATE_DISPLAY_SHOW_PLAYBACK_CLEAR;
            break;

        case STATE_DISPLAY_SHOW_PLAYBACK_CLEAR:
            ShowInsertUSBPlayback();
            stateScreen = STATE_INSERT_USB_PLAYBACK;
            break;

        case STATE_INSERT_USB_PLAYBACK:
            if (USBHostMSDSCSIMediaDetect()) {
                if (FSInit()) {
                    outData = FSfopen("output2.pcm", "r");
                    SetFont((void *) &Font25);
                    SetColor(BLACK);
                    OutTextXY(20, 40, "Insert USB Drive!");
                    OutTextXY(20, 70, "Status: Stopped...");
                    SetColor(BRIGHTBLUE);
                    OutTextXY(20, 40, "USB Drive Plugged-in!");
                    OutTextXY(20, 70, "Status: Playing...");
                    stateScreen = STATE_SHOW_CLEAR_PLAYBACK;
                }
            }
            break;

        case STATE_SHOW_CLEAR_PLAYBACK:
            SetFont((void *) &Font25);
            SetColor(BRIGHTBLUE);
            OutTextXY(20, 110, "Time Elapsed: ");
            secCount = 0;
            uitoa2(secCount, secBuffer);
            strcat(secBuffer, " sec");
            SetColor(BRIGHTRED);
            OutTextXY(145, 110, secBuffer);
            tickStart = TickGet();
            stateScreen = STATE_SHOW_PLAYBACK;
            break;

        case STATE_SHOW_PLAYBACK:
            if (playbackFlag && playFlag)
                ShowScreenPlayback();
            break;

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
            //PerformFFT();
            break;

        case STATE_DISPLAY_SHOW_SETTINGS:
            stateScreen = STATE_SHOW_CLEAR_SETTINGS;
            break;

        case STATE_SHOW_CLEAR_SETTINGS:
            tabSetting = 1;
            CreateSettingsScreen();
            stateScreen = STATE_SHOW_SETTINGS;
            break;

        case STATE_SHOW_SETTINGS:
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
    switch (stateScreen) {
        case STATE_DISPLAY_DEMO_SELECTION:
            break;

        case STATE_DEMO_SELECTION:
            return ProcessMessageDemoSelection(translatedMsg, pObj, pMsg);
            break;
    }

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

/****************************************************************************
  Function:
     void InitializeHardware( void )

  Description:
    This function initializes board hardware.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

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

    // Turn VBUSON ON.
    AD1PCFGbits.PCFG5 = 1;
    PORTSetPinsDigitalOut(IOPORT_D, BIT_5);
    PORTSetBits(IOPORT_D, BIT_5);

    // Initialize audio codec.	
    pCodecHandle = WM8960CodecOpen(O_RDWR);
    WM8960CodecStartAudio(pCodecHandle, TRUE);
    WM8960CodecConfigVolume(pCodecHandle, 0, 0);
    ResetCodec(SAMPLE_RATE_48000_HZ, LINEIN);
    WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);

    // Graphics init.
    InitGraph();

    // Start the general timer.
    TickInit();

    // Initialize graphics scheme used.
    altScheme = GOLCreateScheme();
    altScheme->Color0 = RGB565CONVERT(0x4C, 0x8E, 0xFF);
    altScheme->Color1 = BLACK;
    altScheme->EmbossDkColor = RGB565CONVERT(0x1E, 0x00, 0xE5);
    altScheme->EmbossLtColor = RGB565CONVERT(0xA9, 0xDB, 0xEF);
    altScheme->ColorDisabled = RGB565CONVERT(0xD4, 0xE1, 0xF7);
    altScheme->TextColor1 = BRIGHTBLUE;
    altScheme->TextColor0 = RGB565CONVERT(255, 102, 0);
    altScheme->TextColorDisabled = RGB565CONVERT(0xB8, 0xB9, 0xBC);
    altScheme->CommonBkColor = BLACK;
    altScheme->pFont = (void *) &Font25;

    // Initialize the USB stack
    USBInitialize(0);

    // Start-up splash.
    SetColor(BLACK);
    ClearDevice();
    SetColor(BLACK);
    ClearDevice();


    //WAIT_UNTIL_FINISH(PutImage(0, 5, (void *) &icon_pic32, IMAGE_NORMAL));




    SetColor(BRIGHTGREEN);
    SetFont((void *) &Font25);
    OutTextXY(70, 65, "DIGITAL");
    OutTextXY(5, 65 + GetTextHeight((void *) &Font25) - 5, "  GUITAR MULTIEFFECT");
    SetColor(GREEN);
    SetFont((void *) &GOLMediumFont);
    OutTextXY(65, 65 + 2 * GetTextHeight((void *) &Font25) - 10, "Peter Schmiedt");
    //WAIT_UNTIL_FINISH(PutImage(30, 65 + 3*GetTextHeight( (void *)&GOLMediumFont ), (void *) &icon_intro, IMAGE_NORMAL));
    DelayMs(3000);

    BUTTON_VOLUME_UP_TRIS = 1;
    BUTTON_VOLUME_DOWN_TRIS = 1;
    BUTTON_NEXT_TRIS = 1;
    BUTTON_PREVIOUS_TRIS = 1;

}

/****************************************************************************
  Function:
     void ShowScreenDemoSelection( void )

  Description:
    This function creates demo selection screen.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void ShowScreenDemoSelection(void) {
    GOLFree();
    SetColor(BLACK);
    ClearDevice();

    record = BtnCreate(ID_RECORD,
            45 + ADJUST_PIXELS, 40, 55 + ADJUST_PIXELS, 50, 3,
            BTN_DRAW,
            (void *) &icon_record,
            NULL,
            altScheme
            );


    playback = BtnCreate(ID_PLAYBACK,
            160 - ADJUST_PIXELS, 40, 170 - ADJUST_PIXELS, 50, 3,
            BTN_DRAW,
            (void *) &icon_playback,
            NULL,
            altScheme
            );

    loopback = BtnCreate(ID_LOOPBACK,
            45 + ADJUST_PIXELS, 125, 55 + ADJUST_PIXELS, 135, 3,
            BTN_DRAW,
            (void *) &icon_loopback,
            NULL,
            altScheme
            );

    settings = BtnCreate(ID_SETTINGS,
            160 - ADJUST_PIXELS, 125, 170 - ADJUST_PIXELS, 135, 3,
            BTN_DRAW,
            (void *) &icon_settings,
            NULL,
            altScheme
            );


    if (screenTab == TAB_RECORD) {
        SetColor(BLACK);
        Arc(
                128 - ADJUST_PIXELS, 93, 202 - ADJUST_PIXELS, 167,
                3,
                7,
                0xFF
                );
        SetColor(BRIGHTRED);
        Arc(
                13 + ADJUST_PIXELS, 8, 87 + ADJUST_PIXELS, 82,
                3,
                7,
                0xFF
                );
        screenTab = TAB_PLAYBACK;
    } else if (screenTab == TAB_PLAYBACK) {
        SetColor(BLACK);
        Arc
                (
                13 + ADJUST_PIXELS, 8, 87 + ADJUST_PIXELS, 82,
                3,
                7,
                0xFF
                );
        SetColor(BRIGHTRED);
        Arc
                (
                128 - ADJUST_PIXELS, 8, 202 - ADJUST_PIXELS, 82,
                3,
                7,
                0xFF
                );
        screenTab = TAB_LOOPBACK;
    } else if (screenTab == TAB_LOOPBACK) {
        SetColor(BLACK);
        Arc
                (
                128 - ADJUST_PIXELS, 8, 202 - ADJUST_PIXELS, 82,
                3,
                7,
                0xFF
                );
        SetColor(BRIGHTRED);
        Arc
                (
                13 + ADJUST_PIXELS, 93, 87 + ADJUST_PIXELS, 167,
                3,
                7,
                0xFF
                );
        screenTab = TAB_SETTINGS;
    } else if (screenTab == TAB_SETTINGS) {
        SetColor(BLACK);
        Arc
                (
                13 + ADJUST_PIXELS, 93, 87 + ADJUST_PIXELS, 167,
                3,
                7,
                0xFF
                );
        SetColor(BRIGHTRED);
        Arc
                (
                128 - ADJUST_PIXELS, 93, 202 - ADJUST_PIXELS, 167,
                3,
                7,
                0xFF
                );
        screenTab = TAB_RECORD;
    }

    buttonsFlag = TRUE;

}

/****************************************************************************
  Function:
     void CheckScreenDemoSelection( void )

  Description:
    This function checks if demo selection screen is created and updates titles.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void CheckScreenDemoSelection() {
    if (!(IsObjUpdated(record) && IsObjUpdated(playback) && IsObjUpdated(loopback) && IsObjUpdated(settings))) {
        SetFont((void *) &GOLSmallFont);
        SetColor(BRIGHTGREEN);
        OutTextXY(28 + ADJUST_PIXELS, 67, "RECORD");
        OutTextXY(135 - ADJUST_PIXELS, 67, "PLAYBACK");
        OutTextXY(20 + ADJUST_PIXELS, 152, "LOOPBACK");
        OutTextXY(137 - ADJUST_PIXELS, 152, "SETTINGS");
        buttonsFlag = FALSE;
    }
}

/****************************************************************************
  Function:
     void ShowInsertUSBRecord( void )

  Description:
    This function creates record demo screen with USB thumbdrive information.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void ShowInsertUSBRecord(void) {
    SetColor(BLACK);
    ClearDevice();
    PutImage(5, 5, (void*) &icon_trecord, 1);
    PutImage(80, GetMaxY() - 30, (void*) &icon_tback, 1);
    PutImage(160, GetMaxY() - 30, (void*) &icon_trecord_p, 1);
    SetFont((void *) &Font25);
    SetColor(BRIGHTGREEN);
    OutTextXY(37, 5, "Record");
    SetColor(BRIGHTRED);
    OutTextXY(20, 40, "Insert USB Drive!");
    OutTextXY(20, 70, "Status: Stopped...");
}

/****************************************************************************
  Function:
     void ShowScreenRecord( void )

  Description:
    This function creates and updates record demo screen while recording audio.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

inline void ShowScreenRecord(void) {
    int i;

    PORTSetBits(IOPORT_C, BIT_14);
    dataCount = FSfwrite(Sin, sizeof (STEREO_AUDIO_DATA), FRAME_SIZE, outData);
    if (dataCount != FRAME_SIZE) {
        SetFont((void *) &Font25);
        SetColor(BLACK);
        OutTextXY(20, 40, "USB Drive Plugged-in!");
        OutTextXY(20, 70, "Status: Recording...");
        if (audioIn == MICROPHONE)
            OutTextXY(85, 90, "on MIC...");
        else if (audioIn == LINEIN)
            OutTextXY(85, 90, "on Line-IN...");
        SetColor(BRIGHTRED);
        OutTextXY(20, 40, "USB Drive Full!");
        OutTextXY(20, 70, "Status: Stopped!");
        //FSfclose(outData); 
    } else {
        if ((TickGet() - tickStart) > TICK_SECOND) {
            SetFont((void *) &Font25);
            SetColor(BLACK);
            OutTextXY(145, 110, secBuffer);
            uitoa2(secCount, secBuffer);
            strcat(secBuffer, " sec");
            SetColor(BRIGHTRED);
            OutTextXY(145, 110, secBuffer);
            secCount++;
            tickStart = TickGet();
        }
    }
    PORTClearBits(IOPORT_C, BIT_14);
    WM8960CodecRead(pCodecHandle, Sin, FRAME_SIZE);

    if (audioIn == MICROPHONE)
        for (i = 0; i < FRAME_SIZE; i++) {
            Sin[i].rightChannel = Sin[i].leftChannel;
        }
}

/****************************************************************************
  Function:
     void ShowInsertUSBPlayback( void )

  Description:
    This function creates playback demo screen with USB thumbdrive information.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void ShowInsertUSBPlayback(void) {
    SetColor(BLACK);
    ClearDevice();
    PutImage(5, 5, (void*) &icon_tplayback, 1);
    PutImage(80, GetMaxY() - 30, (void*) &icon_tback, 1);
    PutImage(160, GetMaxY() - 30, (void*) &icon_tpause, 1);
    SetFont((void *) &Font25);
    SetColor(BRIGHTGREEN);
    OutTextXY(37, 5, "Playback");
    SetColor(BRIGHTRED);
    OutTextXY(20, 40, "Insert USB Drive!");
    OutTextXY(20, 70, "Status: Stopped...");
}

/****************************************************************************
  Function:
     void ShowScreenRecord( void )

  Description:
    This function creates and updates playback demo screen while playing audio.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

inline void ShowScreenPlayback(void) {
    PORTSetBits(IOPORT_C, BIT_13);
    dataCount = FSfread(Sin, sizeof (STEREO_AUDIO_DATA), FRAME_SIZE, outData);
    if (dataCount != FRAME_SIZE) {
        playbackFlag = FALSE;
        SetFont((void *) &Font25);
        SetColor(BLACK);
        OutTextXY(20, 40, "USB Drive Plugged-in!");
        OutTextXY(20, 70, "Status: Playing...");
        SetColor(BRIGHTRED);
        OutTextXY(20, 40, "Audio file ended!");
        OutTextXY(20, 70, "Status: Stopped!");
        //FSfclose(outData);		    			
        ResetCodec((countDemo == 1) ? 4 : countDemo - 1, audioIn);
    } else {
        PORTClearBits(IOPORT_C, BIT_13);
        WM8960CodecWrite(pCodecHandle, Sin, FRAME_SIZE);
        if (dataCount == FRAME_SIZE) {
            if ((TickGet() - tickStart) > TICK_SECOND) {
                SetFont((void *) &Font25);
                SetColor(BLACK);
                OutTextXY(145, 110, secBuffer);
                uitoa2(secCount, secBuffer);
                strcat(secBuffer, " sec");
                SetColor(BRIGHTRED);
                OutTextXY(145, 110, secBuffer);
                secCount++;
                tickStart = TickGet();
            }
        }
    }
}

/****************************************************************************
  Function:
     void CreateScreenLoopback( void )

  Description:
    This function creates loopback demo screen.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void CreateScreenLoopback(void) {
    SetColor(WHITE);
    ClearDevice();
    //PutImage(5, 5, (void*) &icon_tloopback, 1);
    PutImage(80, GetMaxY() - 30, (void*) &icon_tback, 1);
    PutImage(160, GetMaxY() - 30, (void*) &icon_tunmute, 1);
    SetFont((void *) &Font25);
    SetColor(BLACK);
    OutTextXY(5, 5, "Preset: ");
    OutTextXY(GetTextWidth("Preset: ", (void *) &Font25), 5, "20");



    SetColor(WHITE);
    //Line(30, GetMaxY() - 125, 30, GetMaxY() - 33);
    //Line(30, GetMaxY() - 33, 160, GetMaxY() - 33);
    //ResetCodec(SAMPLE_RATE_48000_HZ, MICROPHONE);
    ResetCodec(SAMPLE_RATE_48000_HZ, LINEIN);
}

/****************************************************************************
  Function:
     void AudioLoopback(void);

  Description:
    This function captures and plays back audio.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

inline void AudioLoopback(void) {
    WM8960CodecRead(pCodecHandle, Sin, FRAME_SIZE);
    PORTSetBits(IOPORT_D, BIT_0);

    //------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //Overdrive();

    Fuzz();


    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        Sin[i].rightChannel = Sin[i].leftChannel;
    }

    WM8960CodecWrite(pCodecHandle, Sin, FRAME_SIZE);
    PORTClearBits(IOPORT_D, BIT_0);
}

/****************************************************************************
  Function:
     void CreateSettingsScreen( void )

  Description:
    This function creates settings screen.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void CreateSettingsScreen(void) {
    SetColor(BLACK);
    ClearDevice();
    PutImage(5, 5, (void*) &icon_tsettings, 1);
    PutImage(80, GetMaxY() - 30, (void*) &icon_tback, 1);
    SetFont((void *) &Font25);
    SetColor(BRIGHTGREEN);
    OutTextXY(37, 5, "Settings");

    RbCreate
            (
            ID_RADIOBUTTON5,
            7, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT), 7 + 25, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 25,
            RB_DRAW | ((countSetting == 1) ? RB_CHECKED : 0) | RB_GROUP,
            " ", //" LI",        
            altScheme
            );

    RbCreate
            (
            ID_RADIOBUTTON6,
            7, 13 + 2 * GetTextHeight((void *) &FONTDEFAULT), 7 + 25, 13 + 2 * GetTextHeight((void *) &FONTDEFAULT) + 25,
            RB_DRAW | ((countSetting == 2) ? RB_CHECKED : 0),
            " ", //" MIC",        
            altScheme
            );


    RbCreate
            (
            ID_RADIOBUTTON1,
            GetMaxX() / 2 + 34, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT), GetMaxX() - 5, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 20,
            RB_DRAW | ((countDemo == 1) ? RB_CHECKED : 0) | RB_GROUP,
            "48K",
            altScheme
            );

    RbCreate
            (
            ID_RADIOBUTTON2,
            GetMaxX() / 2 + 34, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 25, GetMaxX() - 5, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 45,
            RB_DRAW | ((countDemo == 2) ? RB_CHECKED : 0),
            "44.1K",
            altScheme
            );

    RbCreate
            (
            ID_RADIOBUTTON3,
            GetMaxX() / 2 + 34, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 50, GetMaxX() - 5, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 70,
            RB_DRAW | ((countDemo == 3) ? RB_CHECKED : 0),
            "32K",
            altScheme
            );

    RbCreate
            (
            ID_RADIOBUTTON4,
            GetMaxX() / 2 + 34, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 75, GetMaxX() - 5, 13 + 1 * GetTextHeight((void *) &FONTDEFAULT) + 95,
            RB_DRAW | ((countDemo == 4) ? RB_CHECKED : 0),
            "16K",
            altScheme
            );


    volADCIndication = SldCreate(
            ID_SLIDER1,
            GetMaxX() / 2 - 28, 14 + 2 * GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 18, GetMaxY() - 40,
            SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL,
            95,
            5,
            volADC,
            altScheme);

    volDACIndication = SldCreate(
            ID_SLIDER2,
            GetMaxX() / 2 + 6, 14 + 2 * GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 16, GetMaxY() - 40,
            SLD_DRAW | SLD_SCROLLBAR | SLD_VERTICAL,
            95,
            5,
            volDAC,
            altScheme);


    PutImage(7 + 2 + 25, 13 + 2 + 1 * GetTextHeight((void *) &FONTDEFAULT), (void*) &icon_tlinein, 1);
    PutImage(7 + 2 + 25, 13 + 2 + 2 * GetTextHeight((void *) &FONTDEFAULT), (void*) &icon_tmic, 1);
    PutImage(GetMaxX() / 2 - 36, 15 + GetTextHeight((void *) &FONTDEFAULT), (void*) &icon_tmic, 1);
    PutImage(GetMaxX() / 2 - 1, 15 + GetTextHeight((void *) &FONTDEFAULT), (void*) &icon_theadphone, 1);
    SetColor(BRIGHTRED);
    SetLineType(DASHED_LINE);
    Line(GetMaxX() / 2 - 42, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 42, GetMaxY() - 35);
    Line(GetMaxX() / 2 - 6, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 6, GetMaxY() - 35);
    Line(GetMaxX() / 2 + 30, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 30, GetMaxY() - 35);
    PutImage(160, GetMaxY() - 30, (void*) &icon_tselect, 1);
    SetLineType(NORMAL_LINE);

    switch (tabSetting) {

        case 1:
            SetColor(BLACK);
            Arc
                    (
                    GetMaxX() / 2 + 38, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() - 4, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            SetColor(BRIGHTGREEN);
            Arc
                    (
                    10, 15 + GetTextHeight((void *) &FONTDEFAULT), 60, GetMaxY() / 2 + 10,
                    3,
                    4,
                    0xFF
                    );
            break;

        case 2:
            SetColor(BLACK);
            Arc
                    (
                    10, 15 + GetTextHeight((void *) &FONTDEFAULT), 60, GetMaxY() / 2 + 10,
                    3,
                    4,
                    0xFF
                    );
            SetColor(BRIGHTGREEN);
            Arc
                    (
                    GetMaxX() / 2 - 35, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 13, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            break;

        case 3:
            SetColor(BLACK);
            Arc
                    (
                    GetMaxX() / 2 - 35, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 13, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            SetColor(BRIGHTGREEN);
            Arc
                    (
                    GetMaxX() / 2, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 22, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            break;

        case 4:
            SetColor(BLACK);
            Arc
                    (
                    GetMaxX() / 2, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 22, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            SetColor(BRIGHTGREEN);
            Arc
                    (
                    GetMaxX() / 2 + 38, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() - 4, GetMaxY() - 40,
                    3,
                    4,
                    0xFF
                    );
            break;

        default: break;

    }


}

/****************************************************************************
  Function:
     UINT16 ProcessMessageDemoSelection( UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg )

  Description:
    This function processes graphics messages received for the demo selection screen.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

UINT16 ProcessMessageDemoSelection(UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg) {
    UINT16 controlID;
    controlID = GetObjID(pObj);

    switch (controlID) {
        case ID_RECORD:
            if (translatedMsg == BTN_MSG_PRESSED) {
                stateScreen = STATE_DISPLAY_SHOW_RECORD;
            }
            break;

        case ID_PLAYBACK:
            if (translatedMsg == BTN_MSG_PRESSED) {
                stateScreen = STATE_DISPLAY_SHOW_PLAYBACK;
            }
            break;

        case ID_LOOPBACK:
            if (translatedMsg == BTN_MSG_PRESSED) {
                stateScreen = STATE_DISPLAY_SHOW_LOOPBACK;
            }
            break;

        case ID_SETTINGS:
            if (translatedMsg == BTN_MSG_PRESSED) {
                stateScreen = STATE_DISPLAY_SHOW_SETTINGS;
            }
            break;
    }
    return 1;
}


/****************************************************************************
  Function:
     inline void CheckButtons( GOL_MSG *message );

  Description:
    This function checks user buttons for press and takes appropriate 
    action based.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

#define DEBOUNCE_TIME   (100 * (TICKS_PER_SECOND / 1000ull))

void CheckButtons(GOL_MSG *message) {
    message->uiEvent = EVENT_INVALID;
    message->type = TYPE_KEYBOARD;

    // Process BUTTON_VOLUME_DOWN(S2) presses, including debouncing
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
                case STATE_SHOW_SETTINGS:
                    switch (tabSetting) {
                        case 1:
                            if (countSetting == 2) {
                                RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON5), ID_RADIOBUTTON5);
                                audioIn = LINEIN;
                                countSetting = 1;
                            } else if (countSetting == 1) {
                                RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON6), ID_RADIOBUTTON6);
                                audioIn = MICROPHONE;
                                countSetting = 2;
                            }
                            ResetCodec(SAMPLE_RATE_NO_CHANGE, audioIn);
                            break;

                        case 2:
                            volADC -= 5;
                            if (volADC == 0)
                                volADC += 5;
                            WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);
                            volADCIndication->pos = volADC;
                            SetState(volADCIndication, SLD_DRAW);
                            break;

                        case 3:
                            volDAC -= 5;
                            if (volDAC == 0)
                                volDAC += 5;
                            WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);
                            volDACIndication->pos = volDAC;
                            SetState(volDACIndication, SLD_DRAW);
                            break;

                        case 4:
                            countDemo = (countDemo == 4) ? 1 : countDemo + 1;
                            switch (countDemo) {
                                case 1:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON1), ID_RADIOBUTTON1);
                                    ResetCodec(SAMPLE_RATE_48000_HZ, audioIn);
                                    break;

                                case 2:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON2), ID_RADIOBUTTON2);
                                    ResetCodec(SAMPLE_RATE_44100_HZ, audioIn);
                                    break;

                                case 3:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON3), ID_RADIOBUTTON3);
                                    ResetCodec(SAMPLE_RATE_32000_HZ, audioIn);
                                    break;

                                case 4:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON4), ID_RADIOBUTTON4);
                                    ResetCodec(SAMPLE_RATE_16000_HZ, audioIn);
                                    break;

                                default:
                                    break;
                            }
                            break;


                        default:
                            break;

                    }
                    break;

            }
            switchBouncing = TRUE;

        }
    }

    // Process BUTTON_VOLUME_UP(S3) presses, including debouncing
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


                case STATE_SHOW_SETTINGS:

                    switch (tabSetting) {
                        case 1:
                            if (countSetting == 2) {
                                RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON5), ID_RADIOBUTTON5);
                                audioIn = LINEIN;
                                countSetting = 1;
                            } else if (countSetting == 1) {
                                RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON6), ID_RADIOBUTTON6);
                                audioIn = MICROPHONE;
                                countSetting = 2;
                            }
                            ResetCodec(SAMPLE_RATE_NO_CHANGE, audioIn);
                            break;

                        case 2:
                            volADC += 5;
                            if (volADC == 95)
                                volADC -= 5;
                            WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);
                            volADCIndication->pos = volADC;
                            SetState(volADCIndication, SLD_DRAW);
                            break;

                        case 3:
                            volDAC += 5;
                            if (volDAC == 95)
                                volDAC -= 5;
                            WM8960CodecConfigVolume(pCodecHandle, volADC, volDAC);
                            volDACIndication->pos = volDAC;
                            SetState(volDACIndication, SLD_DRAW);
                            break;

                        case 4:

                            countDemo = (countDemo == 1) ? 4 : countDemo - 1;

                            switch (countDemo) {
                                case 1:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON1), ID_RADIOBUTTON1);
                                    ResetCodec(SAMPLE_RATE_48000_HZ, audioIn);
                                    break;

                                case 2:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON2), ID_RADIOBUTTON2);
                                    ResetCodec(SAMPLE_RATE_44100_HZ, audioIn);
                                    break;

                                case 3:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON3), ID_RADIOBUTTON3);
                                    ResetCodec(SAMPLE_RATE_32000_HZ, audioIn);
                                    break;

                                case 4:
                                    RbSetCheck((RADIOBUTTON *) GOLFindObject(ID_RADIOBUTTON4), ID_RADIOBUTTON4);
                                    ResetCodec(SAMPLE_RATE_16000_HZ, audioIn);
                                    break;

                                default:
                                    break;
                            }
                            break;

                        default: break;
                    }
                    break;

                default: break;
            }
            switchBouncing = TRUE;
        }
    }

    // Process BUTTON_NEXT(S4) presses, including debouncing
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
                case STATE_DEMO_SELECTION:
                    if (screenTab == TAB_RECORD) {
                        SetColor(BLACK);
                        Arc
                                (
                                128 - ADJUST_PIXELS, 93, 202 - ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                        SetColor(BRIGHTRED);
                        Arc
                                (
                                13 + ADJUST_PIXELS, 8, 87 + ADJUST_PIXELS, 82,
                                3,
                                7,
                                0xFF
                                );
                        screenTab = TAB_PLAYBACK;
                    } else if (screenTab == TAB_PLAYBACK) {
                        SetColor(BLACK);
                        Arc
                                (
                                13 + ADJUST_PIXELS, 8, 87 + ADJUST_PIXELS, 82,
                                3,
                                7,
                                0xFF
                                );
                        SetColor(BRIGHTRED);
                        Arc
                                (
                                128 - ADJUST_PIXELS,
                                8,
                                202 - ADJUST_PIXELS,
                                82,
                                3,
                                7,
                                0xFF
                                );
                        screenTab = TAB_LOOPBACK;
                    } else if (screenTab == TAB_LOOPBACK) {
                        SetColor(BLACK);
                        Arc
                                (
                                128 - ADJUST_PIXELS,
                                8,
                                202 - ADJUST_PIXELS,
                                82,
                                3,
                                7,
                                0xFF
                                );
                        SetColor(BRIGHTRED);
                        Arc
                                (
                                13 + ADJUST_PIXELS, 93, 87 + ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                        screenTab = TAB_SETTINGS;
                    } else if (screenTab == TAB_SETTINGS) {
                        SetColor(BLACK);
                        Arc
                                (
                                13 + ADJUST_PIXELS, 93, 87 + ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                        SetColor(BRIGHTRED);
                        Arc
                                (
                                128 - ADJUST_PIXELS, 93, 202 - ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                        screenTab = TAB_RECORD;
                    }
                    break;

                case STATE_SHOW_RECORD:
                    if (recordFlag) {
                        PutImage(160, GetMaxY() - 30, (void*) &icon_trecord_p, 1);
                        recordFlag = FALSE;
                    } else {
                        PutImage(160, GetMaxY() - 30, (void*) &icon_trecord_d, 1);
                        recordFlag = TRUE;
                    }

                    break;

                case STATE_SHOW_PLAYBACK:
                    if (playFlag) {
                        PutImage(160, GetMaxY() - 30, (void*) &icon_tplay, 1);
                        playFlag = FALSE;
                    } else {
                        PutImage(160, GetMaxY() - 30, (void*) &icon_tpause, 1);
                        playFlag = TRUE;
                    }
                    WM8960CodecClear(pCodecHandle);
                    break;


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

                case STATE_SHOW_SETTINGS:
                    tabSetting++;
                    if (tabSetting > 4)
                        tabSetting = 1;
                    PutImage(160, GetMaxY() - 30, (void*) &icon_tunselect, 1);

                    switch (tabSetting) {

                        case 1:
                            SetColor(BLACK);
                            Arc
                                    (
                                    GetMaxX() / 2 + 38, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() - 4, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            SetColor(BRIGHTGREEN);
                            Arc
                                    (
                                    10, 15 + GetTextHeight((void *) &FONTDEFAULT), 60, GetMaxY() / 2 + 10,
                                    3,
                                    4,
                                    0xFF
                                    );
                            break;

                        case 2:
                            SetColor(BLACK);
                            Arc
                                    (
                                    10, 15 + GetTextHeight((void *) &FONTDEFAULT), 60, GetMaxY() / 2 + 10,
                                    3,
                                    4,
                                    0xFF
                                    );
                            SetColor(BRIGHTGREEN);
                            Arc
                                    (
                                    GetMaxX() / 2 - 35, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 13, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            break;

                        case 3:
                            SetColor(BLACK);
                            Arc
                                    (
                                    GetMaxX() / 2 - 35, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 - 13, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            SetColor(BRIGHTGREEN);
                            Arc
                                    (
                                    GetMaxX() / 2, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 22, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            break;

                        case 4:
                            SetColor(BLACK);
                            Arc
                                    (
                                    GetMaxX() / 2, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() / 2 + 22, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            SetColor(BRIGHTGREEN);
                            Arc
                                    (
                                    GetMaxX() / 2 + 38, 15 + GetTextHeight((void *) &FONTDEFAULT), GetMaxX() - 4, GetMaxY() - 40,
                                    3,
                                    4,
                                    0xFF
                                    );
                            break;

                        default: break;

                    }
                    DelayMs(50);
                    PutImage(160, GetMaxY() - 30, (void*) &icon_tselect, 1);
                    break;

                default:
                    break;
            }
            switchBouncing = TRUE;
        }
    }

    // Process BUTTON_PREVIOUS(S4) presses, including debouncing
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

                case STATE_DEMO_SELECTION:
                    message->uiEvent = EVENT_KEYSCAN;
                    message->param2 = SCAN_CR_PRESSED;
                    switchBouncing = TRUE;
                    SetColor(BRIGHTGREEN);

                    if (screenTab == TAB_PLAYBACK) {
                        ResetCodec(SAMPLE_RATE_8000_HZ, audioIn);
                        message->param1 = ID_RECORD;
                        Arc
                                (
                                13 + ADJUST_PIXELS, 8, 87 + ADJUST_PIXELS, 82,
                                3,
                                7,
                                0xFF
                                );
                    } else if (screenTab == TAB_LOOPBACK) {
                        ResetCodec(SAMPLE_RATE_8000_HZ, audioIn);
                        message->param1 = ID_PLAYBACK;
                        Arc
                                (
                                128 - ADJUST_PIXELS, 8, 202 - ADJUST_PIXELS, 82,
                                3,
                                7,
                                0xFF
                                );
                    } else if (screenTab == TAB_SETTINGS) {
                        ResetCodec(currentSampleRate, audioIn);
                        message->param1 = ID_LOOPBACK;
                        Arc
                                (
                                13 + ADJUST_PIXELS, 93, 87 + ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                    } else if (screenTab == TAB_RECORD) {
                        message->param1 = ID_SETTINGS;
                        Arc
                                (
                                128 - ADJUST_PIXELS, 93, 202 - ADJUST_PIXELS, 167,
                                3,
                                7,
                                0xFF
                                );
                    }

                    DelayMs(200);
                    break;

                case STATE_SHOW_RECORD:
                    FSfclose(outData);
                case STATE_INSERT_USB_RECORD:
                    recordFlag = FALSE;
                    screenTab = TAB_RECORD;
                    stateScreen = STATE_DISPLAY_DEMO_SELECTION;
                    ResetCodec((countDemo == 1) ? 4 : countDemo - 1, audioIn);
                    break;

                case STATE_SHOW_PLAYBACK:
                    FSfclose(outData);
                case STATE_INSERT_USB_PLAYBACK:
                    screenTab = TAB_PLAYBACK;
                    stateScreen = STATE_DISPLAY_DEMO_SELECTION;
                    ResetCodec((countDemo == 1) ? 4 : countDemo - 1, audioIn);
                    break;

                case STATE_SHOW_LOOPBACK:
                    muteFlag = FALSE;
                    screenTab = TAB_LOOPBACK;
                    stateScreen = STATE_DISPLAY_DEMO_SELECTION;
                    ResetCodec((countDemo == 1) ? 4 : countDemo - 1, audioIn);
                    break;

                case STATE_SHOW_SETTINGS:
                    screenTab = TAB_SETTINGS;
                    stateScreen = STATE_DISPLAY_DEMO_SELECTION;
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

/****************************************************************************
  Function:
     void ResetCodec(unsigned int sample_rate, int audioIn);

  Description:
    This function resets with a sampling rate and audio in choice.

  Precondition:
    None

  Parameters:
    sampleRate - Audio sampling rate of the codec.
    audioIn - Line-in or MIC audio in

  Returns:
    None

  Remarks:
    None.
 ***************************************************************************/

void ResetCodec(unsigned int sampleRate, int audioIn) {
    currentSampleRate = sampleRate;
    WM8960CodecClear(pCodecHandle);
    if (sampleRate != SAMPLE_RATE_NO_CHANGE)
        WM8960CodecConfigSampleRate(pCodecHandle, sampleRate);

    DelayMs(10);
    if (audioIn == LINEIN) {
        WM8960CodecEnableMicrophone(pCodecHandle, FALSE);
        WM8960CodecEnableLineIn(pCodecHandle, TRUE);
    } else if (audioIn == MICROPHONE) {
        WM8960CodecEnableLineIn(pCodecHandle, FALSE);
        WM8960CodecEnableMicrophone(pCodecHandle, TRUE);
    }
}

/****************************************************************************
  Function:
     void PerformFFT(void);

  Description:
    This function performs Fast Fourier Transform on captured audio 
    and creates/updates frequency spectrum on the graphics display.

  Precondition:
    None

  Parameters:
    None

  Returns:
    None

  Remarks:
    FFT is not applied to the full frame. It is recommended to use 
    pre-windowing on the frame before applying FFT.
 ***************************************************************************/
/*
inline void PerformFFT() {
    int i, j;

    for (i = 0, j = 0; i < REAL_N; i++, j++) {
        din[i].re = Sin[j].rightChannel;
        din[i].im = 0;
    }

    mips_fft16(dout, din, (int16c *) fftc, scratch, REAL_LOGN);
    //SetColor(WHITE);

    for (i = 1, j = 1; i < REAL_N >> 1; i++, j = j + 1) {
        if (fftro[i] > 12)
            fftro[i] = 12;
        //Bar(30 + j, GetMaxY()-((fftro[i] << 3) - 10) - 35, 30 + j, GetMaxY() - 35);
        fftro[i] = dout[i].re;
        fftio[i] = dout[i].im;
    }
                    //out, in,     in,    N
    mips_vec_mul16(fftro, fftro, fftro, REAL_N >> 1);
    mips_vec_mul16(fftio, fftio, fftio, REAL_N >> 1);
    mips_vec_add16(fftro, fftro, fftio, REAL_N >> 1);

    //SetColor(BRIGHTRED);
    for (i = 1, j = 1; i < REAL_N >> 1; i++, j = j + 1) {
        if (fftro[i] > 12)
            fftro[i] = 12;
        //Bar(30 + j, GetMaxY()-((fftro[i] << 3) - 10) - 35, 30 + j, GetMaxY() - 35);
    }
}
 */

//inline void fft();

void uitoa2(WORD Value, BYTE* Buffer) {
    BYTE i;
    WORD Digit;
    WORD Divisor;
    BOOL Printed = FALSE;
    if (Value) {
        for (i = 0, Divisor = 10000; i < 5u; i++) {
            Digit = Value / Divisor;
            if (Digit || Printed) {
                *Buffer++ = '0' + Digit;
                Value -= Digit*Divisor;
                Printed = TRUE;
            }
            Divisor /= 10;
        }
    } else {
        *Buffer++ = '0';
    }
    *Buffer = '\0';
}

void Fuzz() {
    INT16 INT16_MAX = 32767;
    INT16 gain = 100; //gain - amount of distortion, gain>1
    INT16 mix = 20; //mix - mix of original and distorted sound, 1=only distorted mix>=1
    INT32 q[FRAME_SIZE];
    INT32 z[FRAME_SIZE];

    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        q[i] = gain * Sin[i].leftChannel;
        if (q[i] > 0) {
            z[i] = (1 - exp(-abs(q[i] / INT16_MAX))) * INT16_MAX;
        } else {
            z[i] = -(1 - exp(-abs(q[i] / INT16_MAX))) * INT16_MAX;
        }
        Sin[i].leftChannel = mix * z[i] + (1 - mix) * Sin[i].leftChannel;
    }

}

void Overdrive() {
    INT16 treshold = 2000;
    INT16 tresholdTimesThree = treshold * 3;
    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        INT16 current = abs(Sin[i].leftChannel);

        //Sin[i].leftChannel = (3 - pow(2 - 3 * Sin[i].leftChannel / tresholdTimesThree, 2)) / (3 / tresholdTimesThree); //(3-(2-3x/32767)^2)/3/32767

        if (current < treshold) {
            Sin[i].leftChannel *= 2;
        } else if (current < treshold * 2) {
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
}

/*
void Octave() {
    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        din[i].re = 64 * Sin[i].leftChannel;
        din[i].im = 0;
    }

    mips_fft32(temp, din, fftc, scratch, LOGN);

    
      double LinearInterpolate(
            double y1,double y2,
            double mu)
        {
            return(y1*(1-mu)+y2*mu);
        }
     

    //DO the phase shift
    int j;
    for (j = FRAME_SIZE - 1; j >= 0; j--) {
        if ((j % 2 == 0) && (j < 512)) {
            temp[j * 2] = temp[j];
        }

        temp[j].re = 32 * temp[j].re;
        temp[j].im = 32 * (0 - temp[j].im);
    }


    //    int l;
    //    for (l = 1; l < FRAME_SIZE - 2; l = l + 2) {
    //        temp[l].re = (temp[l - 1].re + temp[l + 1].re) / 2;
    //        temp[l].im = (temp[l - 1].im + temp[l + 1].im) / 2;
    //    }

    mips_fft32(dout, temp, fftc, scratch, LOGN);


    int k;
    for (k = 0; k < FRAME_SIZE; k++) {
        Sin[k].leftChannel = dout[k].re; // * hanningWindow[i];
    }

    
    char print[30];
    sprintf(print, "%i", din[0].re);
    SetColor(WHITE);
    Bar(100, 20, 160, 40);
    SetColor(BLACK);
    OutTextXY(100, 20, print);
    
}
 */