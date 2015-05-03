/**********************************************************************
* © 2010 Microchip Technology Inc.
*
* FileName:        WM8960Drv.h
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32MX
* Compiler:        MPLAB® C32 
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

#ifndef _WM8960DRV_H_
#define _WM8960DRV_H_

#include <p32xxxx.h>
#include <GenericTypeDefs.h>
#include <plib.h>

#ifndef GetPeripheralClock
#define GetPeripheralClock() 80000000
#endif

/**************************************
 * Specifies that the Codec driver should
 * use malloc for allocating buffer memory
 * and driver data structure. Commenting
 * this out will use static allocation.
 *************************************/
#define WM8960DRV_DYNAMIC_ALLOCATION

/**************************************
 * WM8960 Codec Driver TX buffer in size
 * in stereo samples
 *************************************/
#define WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD   2048

/**************************************
 * WM8960 Codec Driver RX buffer in size
 * in stereo samples.
 *************************************/
#define WM8960DRV_RX_BUFFER_SIZE_STEREO_WORD   2048//256

/**************************************
 * WM8960 Codec Driver TX buffer in size
 * in bytes
 *************************************/
#define WM8960DRV_TX_BUFFER_SIZE_BYTES  (WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD * sizeof(UINT32))

/**************************************
 * WM8960 Codec Driver RX buffer in size
 * in bytes
 *************************************/
#define WM8960DRV_RX_BUFFER_SIZE_BYTES  (WM8960DRV_RX_BUFFER_SIZE_STEREO_WORD * sizeof(UINT32))

/**************************************
 * WM8960 Codec I2C Address
 *************************************/

#define WM8960_I2C_ADDRESS 0x34

/**********************
 * Stereo Data Type 
 **********************/

typedef union {
    struct
    {
        INT16 rightChannel;
        INT16 leftChannel;
    };
    UINT32 audioWord;

}STEREO_AUDIO_DATA;

enum{
	LINEIN=0,
	MICROPHONE
}AUDIO_IN;

/**************************************
 * This macro instructs the driver
 * to use ping pong buffering.
 *************************************/

#define DMA_PING_PONG
#define DMA_PING_PONGR
#define DMA_PP_BUFFER_SIZE (WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD/2)

/**************************************
 * WM8960 Codec Driver Data Strcuture
 *************************************/
typedef struct
{
    STEREO_AUDIO_DATA * txBuffer;  /* Data which will be sent to the DAC */
    STEREO_AUDIO_DATA * rxBuffer;  /* Data which is read from the ADC */
    UINT   txReadPtr;  /* TX head pointer  */
    UINT   txWritePtr; /* TX tail pointer  */
    UINT   rxReadPtr;  /* RX tail pointer  */
    UINT   rxWritePtr; /* RX head pointer  */
    
#ifdef DMA_PING_PONG
    volatile UINT ppFlag;   /* Indicates which buffer is active */
    BOOL ppBufferIsBusy[2]; /* Indicates the status of the buffer */
    UINT ppBufferCount[2];  /* Indicates the amount of data in a buffer */

    volatile UINT ppFlagR;   /* Indicates which buffer is active */
    BOOL ppBufferIsBusyR[2]; /* Indicates the status of the buffer */
    UINT ppBufferCountR[2];  /* Indicates the amount of data in a buffer */    
#endif
    
    
}WM8960_CODEC;


/**************************************
 * WM8960 I2C Control channel used on this
 * particular hardware.
 *************************************/

#define WM8960DRV_I2C_MODULE    I2C2
#define WM8960DRV_I2C_BAUD      100000

/**************************************
 * WM8960 I2C Data Communication SPI 
 * channel used on this particular 
 * hardware.
 *************************************/

#define WM8960DRV_SPI_MODULE            SPI_CHANNEL2A
#define WM8960DRV_SPI_MODULE_BUFFER     SPI2ABUF
#define WM8960DRV_SPI_MODULE_TX_IRQ     _SPI2A_TX_IRQ
#define WM8960DRV_SPI_MODULE_RX_IRQ     _SPI2A_RX_IRQ


/**************************************
 * WM8960 DMA channel for reading
 * and writing data to the SPI module.
 *************************************/

#define WM8960DRV_SPI_TX_DMA_CHANNEL        DMA_CHANNEL0
#define WM8960DRV_SPI_TX_DMA_VECTOR         INT_DMA_0_VECTOR
#define WM8960DRV_SPI_TX_DMA_INTERRUPT      INT_DMA0
#define WM8960DRV_SPI_TX_DMA_INT_PRI_LEVEL  INT_PRIORITY_LEVEL_5
#define WM8960DRV_SPI_TX_DMA_INT_SPRI_LEVEL INT_SUB_PRIORITY_LEVEL_0

/**************************************
 * WM8960 DMA channel for reading
 * and reading data from the SPI module.
 *************************************/

#define WM8960DRV_SPI_RX_DMA_CHANNEL    DMA_CHANNEL1
#define WM8960DRV_SPI_RX_DMA_VECTOR         INT_DMA_1_VECTOR
#define WM8960DRV_SPI_RX_DMA_INTERRUPT      INT_DMA1
#define WM8960DRV_SPI_RX_DMA_INT_PRI_LEVEL  INT_PRIORITY_LEVEL_5
#define WM8960DRV_SPI_RX_DMA_INT_SPRI_LEVEL INT_SUB_PRIORITY_LEVEL_1
/**************************************
 * WM8960 Registers and addresses.
 *************************************/

typedef enum
{
    WM8960_LEFT_INPUT_VOLUME    = 0x00,
    WM8960_RIGHT_INPUT_VOLUME   = 0x01,
    WM8960_LEFT_OUT1_VOLUME     = 0x02 ,
    WM8960_RIGHT_OUT1_VOLUME    = 0x03 ,
    WM8960_CLOCKING_1           = 0x04 ,
    WM8960_ADC_DAC_CTRL1        = 0x05,
    WM8960_ADC_DAC_CTRL2        = 0x06,
    WM8960_AUDIO_INTERFACE_1    = 0x07,
    WM8960_CLOCKING_2           = 0x08,
    WM8960_AUDIO_INTERFACE_2    = 0x09,
    WM8960_LEFT_DAC_VOLUME      = 0x0A,
    WM8960_RIGHT_DAC_VOLUME     = 0x0B,
    WM8960_RESET                = 0x0F,
    WM8960_3D_CTRL              = 0x10,
    WM8960_ALC_1                = 0x11,
    WM8960_ALC_2                = 0x12,
    WM8960_ALC_3                = 0x13,
    WM8960_NOISE_GATE           = 0x14,
    WM8960_LEFT_ADC_VOLUME      = 0x15,
    WM8960_RIGHT_ADC_VOLUME     = 0x16,
    WM8960_ADDITIONAL_CTRL_1    = 0x17,
    WM8960_ADDITIONAL_CTRL_2    = 0x18,
    WM8960_ADDITIONAL_CTRL_3    = 0x1B,
    WM8960_POWER_MGMT_1         = 0x19,
    WM8960_POWER_MGMT_2         = 0x1A,
    WM8960_ANTI_POP_1           = 0x1C,
    WM8960_ANTI_POP_2           = 0x1D,
    WM8960_ADCL_SIGNAL_PATH     = 0x20,
    WM8960_ADCR_SIGNAL_PATH     = 0x21,
    WM8960_LEFT_OUT_MIX         = 0x22,
    WM8960_RIGHT_OUT_MIX        = 0x25,
    WM8960_MONO_OUT_MIX_1       = 0x26,
    WM8960_MONO_OUT_MIX_2       = 0x27,
    WM8960_LOUT2_VOLUME         = 0x28,
    WM8960_ROUT2_VOLUME         = 0x29,
    WM8960_MONO_OUT_VOLUME      = 0x2A,
    WM8960_INPUT_BOOST_MIXER_1  = 0x2B,
    WM8960_INPUT_BOOST_MIXER_2  = 0x2C,
    WM8960_BYPASS_1             = 0x2D,
    WM8960_BYPASS_2             = 0x2E,
    WM8960_POWER_MGMT_3         = 0x2F,
    WM8960_ADDITIONAL_CTRL_4    = 0x30,
    WM8960_CLASS_D_CTRL_1       = 0x31,
    WM8960_CLASS_D_CTRL_3       = 0x33,
    WM8960_PLL_N                = 0x34,
    WM8960_PLL_K_1              = 0x35,
    WM8960_PLL_K_2              = 0x36,
    WM8960_PLL_K_3              = 0x37

}WM8960_REGISTER;

#define WM8960_TOTAL_REGISTERS 56


/**********************************
 * Defines the possible operation
 * modes for the codec driver.
 *********************************/
typedef enum
{
    O_RDONLY = 0x1,   /* Open the codec driver for a read operation only */
    O_WRONLY = 0x2,   /* Open the codec driver for write operation only   */
    O_RDWR = 0x4,     /* Open the codec driver for both read and write   */

}O_MODE;

typedef enum
{
    SAMPLE_RATE_48000_HZ,
    SAMPLE_RATE_44100_HZ,
    SAMPLE_RATE_32000_HZ,
    //SAMPLE_RATE_24000_HZ,
    SAMPLE_RATE_16000_HZ,
    SAMPLE_RATE_8000_HZ,
    SAMPLE_RATE_NO_CHANGE
    
}WM8960_SAMPLE_RATE;

/* Low level functions */

INT WM8960Codec_ioctl(WM8960_CODEC * pCodecHandle, WM8960_REGISTER commandRegister, void * data);
inline UINT WM8960CodecRead(WM8960_CODEC * codecHandle, STEREO_AUDIO_DATA * data, UINT nSamples);
inline UINT WM8731Codec_read(WM8960_CODEC * codecHandle,STEREO_AUDIO_DATA * data, UINT nStereoSamples);
inline UINT WM8960CodecWrite(WM8960_CODEC * codecHandle, STEREO_AUDIO_DATA * data, UINT nSamples); 
inline UINT WM8731Codec_write(WM8960_CODEC * codecHandle,STEREO_AUDIO_DATA * data, UINT nStereoSamples);

WM8960_CODEC * WM8960CodecOpen(O_MODE codecOpenMode);
void WM8960CodecClose(WM8960_CODEC * pCodecHandle);
INT WM8960CodecConfigSampleRate(WM8960_CODEC * pCodecHandle, WM8960_SAMPLE_RATE sampleRate);
INT WM8960CodecConfigVolume(WM8960_CODEC * pCodecHandle, UINT8 volumePerA, UINT8 volumePerD);
INT WM8960CodecEnableMicrophone(WM8960_CODEC * pCodecHandle, BOOL enable);
INT WM8960CodecEnableLineIn(WM8960_CODEC * pCodecHandle,BOOL enable);
void WM8960CodecStartAudio(WM8960_CODEC * pCodecHandle, BOOL enable);
void WM8960CodecClear(WM8960_CODEC * codecHandle);
INT WM8960CodecConfigDACMute(WM8960_CODEC * pCodecHandle, BOOL enable);

#endif
