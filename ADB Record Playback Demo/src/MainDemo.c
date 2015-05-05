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
//const float hanningWindow[WINDOW_SIZE] = {0.000000, 0.000038, 0.000151, 0.000340, 0.000605, 0.000945, 0.001360, 0.001851, 0.002417, 0.003058, 0.003775, 0.004566, 0.005433, 0.006374, 0.007390, 0.008480, 0.009645, 0.010884, 0.012196, 0.013583, 0.015043, 0.016576, 0.018182, 0.019862, 0.021614, 0.023438, 0.025334, 0.027302, 0.029341, 0.031452, 0.033633, 0.035885, 0.038207, 0.040599, 0.043061, 0.045591, 0.048190, 0.050858, 0.053593, 0.056396, 0.059266, 0.062203, 0.065205, 0.068274, 0.071408, 0.074606, 0.077869, 0.081196, 0.084586, 0.088038, 0.091554, 0.095130, 0.098769, 0.102467, 0.106226, 0.110044, 0.113922, 0.117857, 0.121851, 0.125901, 0.130009, 0.134172, 0.138390, 0.142663, 0.146990, 0.151371, 0.155804, 0.160289, 0.164826, 0.169413, 0.174051, 0.178737, 0.183472, 0.188255, 0.193085, 0.197962, 0.202884, 0.207851, 0.212862, 0.217917, 0.223014, 0.228153, 0.233334, 0.238554, 0.243814, 0.249113, 0.254450, 0.259824, 0.265234, 0.270680, 0.276160, 0.281674, 0.287222, 0.292801, 0.298412, 0.304053, 0.309724, 0.315423, 0.321151, 0.326905, 0.332686, 0.338492, 0.344323, 0.350177, 0.356053, 0.361952, 0.367871, 0.373810, 0.379768, 0.385745, 0.391739, 0.397749, 0.403774, 0.409814, 0.415868, 0.421935, 0.428013, 0.434102, 0.440201, 0.446309, 0.452426, 0.458549, 0.464679, 0.470814, 0.476953, 0.483096, 0.489242, 0.495389, 0.501537, 0.507685, 0.513831, 0.519975, 0.526117, 0.532254, 0.538387, 0.544513, 0.550633, 0.556746, 0.562850, 0.568944, 0.575028, 0.581100, 0.587160, 0.593207, 0.599240, 0.605258, 0.611260, 0.617246, 0.623213, 0.629162, 0.635091, 0.641000, 0.646888, 0.652753, 0.658596, 0.664414, 0.670207, 0.675975, 0.681716, 0.687430, 0.693115, 0.698771, 0.704397, 0.709993, 0.715556, 0.721087, 0.726584, 0.732047, 0.737476, 0.742868, 0.748223, 0.753541, 0.758821, 0.764061, 0.769262, 0.774421, 0.779540, 0.784616, 0.789649, 0.794638, 0.799583, 0.804482, 0.809336, 0.814142, 0.818901, 0.823612, 0.828274, 0.832887, 0.837449, 0.841960, 0.846419, 0.850826, 0.855180, 0.859480, 0.863726, 0.867917, 0.872052, 0.876131, 0.880153, 0.884118, 0.888024, 0.891872, 0.895661, 0.899390, 0.903058, 0.906666, 0.910212, 0.913696, 0.917117, 0.920476, 0.923770, 0.927001, 0.930167, 0.933269, 0.936304, 0.939274, 0.942177, 0.945014, 0.947783, 0.950484, 0.953118, 0.955683, 0.958179, 0.960605, 0.962962, 0.965249, 0.967466, 0.969612, 0.971687, 0.973691, 0.975623, 0.977483, 0.979271, 0.980987, 0.982630, 0.984200, 0.985696, 0.987120, 0.988469, 0.989745, 0.990947, 0.992074, 0.993127, 0.994106, 0.995010, 0.995839, 0.996593, 0.997272, 0.997875, 0.998404, 0.998857, 0.999235, 0.999537, 0.999764, 0.999915, 0.999991, 0.999991, 0.999915, 0.999764, 0.999537, 0.999235, 0.998857, 0.998404, 0.997875, 0.997272, 0.996593, 0.995839, 0.995010, 0.994106, 0.993127, 0.992074, 0.990947, 0.989745, 0.988469, 0.987120, 0.985696, 0.984200, 0.982630, 0.980987, 0.979271, 0.977483, 0.975623, 0.973691, 0.971687, 0.969612, 0.967466, 0.965249, 0.962962, 0.960605, 0.958179, 0.955683, 0.953118, 0.950484, 0.947783, 0.945014, 0.942177, 0.939274, 0.936304, 0.933269, 0.930167, 0.927001, 0.923770, 0.920476, 0.917117, 0.913696, 0.910212, 0.906666, 0.903058, 0.899390, 0.895661, 0.891872, 0.888024, 0.884118, 0.880153, 0.876131, 0.872052, 0.867917, 0.863726, 0.859480, 0.855180, 0.850826, 0.846419, 0.841960, 0.837449, 0.832887, 0.828274, 0.823612, 0.818901, 0.814142, 0.809336, 0.804482, 0.799583, 0.794638, 0.789649, 0.784616, 0.779540, 0.774421, 0.769262, 0.764061, 0.758821, 0.753541, 0.748223, 0.742868, 0.737476, 0.732047, 0.726584, 0.721087, 0.715556, 0.709993, 0.704397, 0.698771, 0.693115, 0.687430, 0.681716, 0.675975, 0.670207, 0.664414, 0.658596, 0.652753, 0.646888, 0.641000, 0.635091, 0.629162, 0.623213, 0.617246, 0.611260, 0.605258, 0.599240, 0.593207, 0.587160, 0.581100, 0.575028, 0.568944, 0.562850, 0.556746, 0.550633, 0.544513, 0.538387, 0.532254, 0.526117, 0.519975, 0.513831, 0.507685, 0.501537, 0.495389, 0.489242, 0.483096, 0.476953, 0.470814, 0.464679, 0.458549, 0.452426, 0.446309, 0.440201, 0.434102, 0.428013, 0.421935, 0.415868, 0.409814, 0.403774, 0.397749, 0.391739, 0.385745, 0.379768, 0.373810, 0.367871, 0.361952, 0.356053, 0.350177, 0.344323, 0.338492, 0.332686, 0.326905, 0.321151, 0.315423, 0.309724, 0.304053, 0.298412, 0.292801, 0.287222, 0.281674, 0.276160, 0.270680, 0.265234, 0.259824, 0.254450, 0.249113, 0.243814, 0.238554, 0.233334, 0.228153, 0.223014, 0.217917, 0.212862, 0.207851, 0.202884, 0.197962, 0.193085, 0.188255, 0.183472, 0.178737, 0.174051, 0.169413, 0.164826, 0.160289, 0.155804, 0.151371, 0.146990, 0.142663, 0.138390, 0.134172, 0.130009, 0.125901, 0.121851, 0.117857, 0.113922, 0.110044, 0.106226, 0.102467, 0.098769, 0.095130, 0.091554, 0.088038, 0.084586, 0.081196, 0.077869, 0.074606, 0.071408, 0.068274, 0.065205, 0.062203, 0.059266, 0.056396, 0.053593, 0.050858, 0.048190, 0.045591, 0.043061, 0.040599, 0.038207, 0.035885, 0.033633, 0.031452, 0.029341, 0.027302, 0.025334, 0.023438, 0.021614, 0.019862, 0.018182, 0.016576, 0.015043, 0.013583, 0.012196, 0.010884, 0.009645, 0.008480, 0.007390, 0.006374, 0.005433, 0.004566, 0.003775, 0.003058, 0.002417, 0.001851, 0.001360, 0.000945, 0.000605, 0.000340, 0.000151, 0.000038, 0.000000,};
//INT16 *frameMatrix[FRAMES][WINDOW_SIZE];

////////Fast Fourier Transform Data Declaration////////
#define 			N 1024
#define 			LOGN 10
#define 			fftc fft16c1024
int16c dout[N];
int16c din[N];
int16c temp[N];
int16c scratch[N];
//int16 fftro[REAL_N];
//int16 fftio[REAL_N];

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

    //Distortion();

    Octave();


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

void Distortion() {
    //Int 16 -- (-32,768 to +32,767)
    int i = 0;
    INT16 treshold = 2000;
    INT16 clamp = 3000;
    while (i < FRAME_SIZE) {
        if (Sin[i].leftChannel > treshold) {
            Sin[i].leftChannel = clamp;
        }
        if (Sin[i].leftChannel < -treshold) {
            Sin[i].leftChannel = -clamp;
        }

        if (Sin[i].leftChannel > treshold) {
            Sin[i].leftChannel = clamp;
        }
        if (Sin[i].leftChannel < -treshold) {
            Sin[i].leftChannel = -clamp;
        }
        i++;
    }
}
//fft16 strasne utlmuje signal, aby som mohol pracovat musim si vsetko prerobit na 32 bit
void Octave() {
    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        din[i].re = 20 * Sin[i].leftChannel;
        din[i].im = 20 * 0;
    }

    //mips_fft16(temp, din, (int16c *) fftc, scratch, LOGN);
    mips_fft16(temp, din, fftc, scratch, LOGN);



    int j;
    for (j = 0; j < FRAME_SIZE; j++) {
        temp[j].re = 20 * temp[j].re;
        temp[j].im = 20 * (0 - temp[j].im);
    }

    //mips_fft16(dout, temp, (int16c *) fftc, scratch, LOGN);
    mips_fft16(dout, temp, fftc, scratch, LOGN);


    int k;
    for (k = 0; k < FRAME_SIZE; k++) {
        Sin[k].leftChannel = 100 * dout[k].re;
    }

    /*
    char print[30];
    sprintf(print, "%i", din[0].re);
    SetColor(WHITE);
    Bar(100, 20, 160, 40);
    SetColor(BLACK);
    OutTextXY(100, 20, print);
    */
}