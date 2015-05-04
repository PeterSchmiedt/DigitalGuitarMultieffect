/*********************************************************************
 * Microchip Graphic Library Demo Application
 * The header file joins all header files used in the project.
 *********************************************************************
 * FileName:        MainDemo.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC32
 * Compiler:        C32
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).  
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY
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
 ********************************************************************/
#ifndef _MAINDEMO_H
#define _MAINDEMO_H

////////////////////////////// INCLUDES //////////////////////////////

#include <plib.h>
#include "GenericTypeDefs.h"
#include "Graphics\Graphics.h"
#include "icons_adb.h"
#include "Tick.h"
#include "fftc.h"    
#include "WM8960Drv.h"
#include "./h/usb.h"
#include "./h/usb/usb_host_msd.h"
#include "./h/usb/usb_host_msd_scsi.h"
#include "./h/MDD File System/FSIO.h"
#include <math.h>


void InitializeHardware(void);
void CheckButtons(GOL_MSG *message);
inline void ShowScreenRecord(void);
inline void ShowScreenPlayback(void);
inline void AudioLoopback(void);
void CheckScreenDemoSelection(void);
void ShowInsertUSBRecord(void);
void CreateScreenLoopback(void);
void ShowInsertUSBPlayback(void);
UINT16 GOLDrawCallback(void);
UINT16 GOLMsgCallback(UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ShowScreenDemoSelection(void);
UINT16 ProcessMessageDemoSelection(UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
inline void PerformFFT(void);
void ResetCodec(unsigned int sample_rate, int audio_in);
void CreateSettingsScreen(void);
void uitoa2(WORD Value, BYTE* Buffer);
void Distortion();
void PitchShift();
inline void CreateFrames();

#endif // _MAINDEMO_H
