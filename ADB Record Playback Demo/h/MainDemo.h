#ifndef _MAINDEMO_H
#define _MAINDEMO_H

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

struct bpcoefficients {
    double e;
    double p;
    double d[3];
};

struct filter {
    double e;
    double p;
    double d[3];
    double x[3];
    double y[3];
};

void InitializeHardware(void);
void CheckButtons(GOL_MSG *message);
inline void AudioLoopback(void);
void CreateScreenLoopback(void);
void RedrawScreenLoopback(void);
void CreateScreenUSB(void);
inline void ReadUSBData(void);
inline void ReadUSBDataDone(void);
UINT16 GOLDrawCallback(void);
UINT16 GOLMsgCallback(UINT16 translatedMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);
void ResetCodec();

void Compressor(int i);
void Limiter(int i);
void Fuzz(int i);
void Overdrive(int i);

void BitCrusher(int i);

inline void EnvelopeInit();
inline void Envelope(double xin);

inline void FlangerInit();
inline void FlangerProcess(int i);

void BandPassInitialize(double fsfilt, double gb, double Q, short fstep, short fmin);
void BandPassSetup(struct filter * filter_struct, int ind);
double BandPassProcess(double yin, struct filter * filter_struct);

void AutoWah_init(short effect_rate, short sampling, short maxf, short minf, short Q, double gainfactor, short freq_step);
double AutoWah_process(int xin);
void AutoWah_sweep();

#endif // _MAINDEMO_H
