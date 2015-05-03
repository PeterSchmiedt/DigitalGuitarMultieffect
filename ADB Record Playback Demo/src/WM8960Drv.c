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

#include "WM8960Drv.h"

PRIVATE WM8960_CODEC * thisCodec;
PRIVATE INT WM8960Registers[WM8960_TOTAL_REGISTERS];


#ifndef WM8960DRV_DYNAMIC_ALLOCATION

/*************************************
 * This means that static memory allocation is
 * desired. Memory for the codec driver structure
 * is always allocated. If only playback is 
 * desired then rx buffer size should be set to
 * zero. If only record function is desired then
 * tx buffer size should be set to zero.
 ************************************/

PRIVATE STEREO_AUDIO_DATA codecTxBuffer[WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD];
PRIVATE STEREO_AUDIO_DATA codecRxBuffer[WM8960DRV_RX_BUFFER_SIZE_STEREO_WORD];
PRIVATE WM8960_CODEC codec;

#endif



PRIVATE void WM8960CodecSetRegCopyToDefault(void)
{
    /* If the codec driver needs to keep 
     * track about what was written to the
     * codec registers, the following array
     * is available.
     */

    WM8960Registers[WM8960_LEFT_INPUT_VOLUME   ] = 0x97;
    WM8960Registers[WM8960_RIGHT_INPUT_VOLUME  ] = 0x97;
    WM8960Registers[WM8960_LEFT_OUT1_VOLUME    ] = 0;
    WM8960Registers[WM8960_RIGHT_OUT1_VOLUME   ] = 0;
    WM8960Registers[WM8960_CLOCKING_1          ] = 0;
    WM8960Registers[WM8960_ADC_DAC_CTRL1       ] = 0x8;
    WM8960Registers[WM8960_ADC_DAC_CTRL2       ] = 0;
    WM8960Registers[WM8960_AUDIO_INTERFACE_1   ] = 0xA;
    WM8960Registers[WM8960_CLOCKING_2          ] = 0x1C0;
    WM8960Registers[WM8960_AUDIO_INTERFACE_2   ] = 0x0;
    WM8960Registers[WM8960_LEFT_DAC_VOLUME     ] = 0xFF;
    WM8960Registers[WM8960_RIGHT_DAC_VOLUME    ] = 0xFF;
    WM8960Registers[WM8960_RESET               ] = 0x0;
    WM8960Registers[WM8960_3D_CTRL             ] = 0x0;
    WM8960Registers[WM8960_ALC_1               ] = 0x7B;
    WM8960Registers[WM8960_ALC_2               ] = 0x100;
    WM8960Registers[WM8960_ALC_3               ] = 0x32;
    WM8960Registers[WM8960_NOISE_GATE          ] = 0x0;
    WM8960Registers[WM8960_LEFT_ADC_VOLUME     ] = 0xC3;
    WM8960Registers[WM8960_RIGHT_ADC_VOLUME    ] = 0xC3;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_1   ] = 0x1C0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_2   ] = 0x0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_3   ] = 0x0;
    WM8960Registers[WM8960_POWER_MGMT_1        ] = 0x0;
    WM8960Registers[WM8960_POWER_MGMT_2        ] = 0x0;
    WM8960Registers[WM8960_ANTI_POP_1          ] = 0x0;
    WM8960Registers[WM8960_ANTI_POP_2          ] = 0x0;
    WM8960Registers[WM8960_ADCL_SIGNAL_PATH    ] = 0x100;
    WM8960Registers[WM8960_ADCR_SIGNAL_PATH    ] = 0x100;
    WM8960Registers[WM8960_LEFT_OUT_MIX      ] = 0x50;
    WM8960Registers[WM8960_RIGHT_OUT_MIX     ] = 0x50;
    WM8960Registers[WM8960_MONO_OUT_MIX_1      ] = 0x0;
    WM8960Registers[WM8960_MONO_OUT_MIX_2      ] = 0x0;
    WM8960Registers[WM8960_LOUT2_VOLUME        ] = 0x0;
    WM8960Registers[WM8960_ROUT2_VOLUME        ] = 0x0;
    WM8960Registers[WM8960_MONO_OUT_VOLUME     ] = 0x40;
    WM8960Registers[WM8960_INPUT_BOOST_MIXER_1 ] = 0x0;
    WM8960Registers[WM8960_INPUT_BOOST_MIXER_2 ] = 0x0;
    WM8960Registers[WM8960_BYPASS_1            ] = 0x50;
    WM8960Registers[WM8960_BYPASS_2            ] = 0x50;
    WM8960Registers[WM8960_POWER_MGMT_3        ] = 0x0;
    WM8960Registers[WM8960_ADDITIONAL_CTRL_4   ] = 0x2;
    WM8960Registers[WM8960_CLASS_D_CTRL_1      ] = 0x37;
    WM8960Registers[WM8960_CLASS_D_CTRL_3      ] = 0x80;
    WM8960Registers[WM8960_PLL_N               ] = 0x8;
    WM8960Registers[WM8960_PLL_K_1             ] = 0x31;
    WM8960Registers[WM8960_PLL_K_2             ] = 0x26;
    WM8960Registers[WM8960_PLL_K_3             ] = 0xE9;


}

void WM8960CodecClose(WM8960_CODEC * pCodecHandle)
{
#ifdef WM8960DRV_DYNAMIC_ALLOCATION
    /* Only memory allocated dynamically can be
     * freed. */
    if(pCodecHandle == NULL) return;

    if(pCodecHandle->txBuffer != NULL) 
    {
        free(pCodecHandle->txBuffer);
    }

    if(pCodecHandle->rxBuffer != NULL)
    {
        free(pCodecHandle->rxBuffer);
    }

    free(pCodecHandle);
#endif
}

WM8960_CODEC * WM8960CodecOpen(O_MODE codecOpenMode)
{

    SpiOpenFlags spiFlags;
	int i;

    /* This function allocates memory for the 
     * codec driver structure and return a 
     * pointer to the structure. Returns NULL
     * if the allocation failed for any reason.
     */

#ifdef WM8960DRV_DYNAMIC_ALLOCATION
    thisCodec = (WM8960_CODEC *)malloc(sizeof(WM8960_CODEC));
    if(thisCodec == NULL)
    {
        /* Memory allocation failed */
        return(NULL);
    }

#else
    /* Use static allocation */
    thisCodec = &codec;
#endif

	
    /* Enable the DMA module */
    DmaWriteGlobalFlags(DMA_GFLG_ON);

    /* Check if playback is required */
    if((codecOpenMode == O_WRONLY ) ||
            (codecOpenMode == O_RDWR))
    {
        /* This means that TX function is desired
         * and the codec will perform playback 
         * function.
         */

#ifdef WM8960DRV_DYNAMIC_ALLOCATION
        thisCodec->txBuffer = (STEREO_AUDIO_DATA *)malloc(WM8960DRV_TX_BUFFER_SIZE_BYTES);
        if(thisCodec->txBuffer == NULL)
        {
            /* If memory could not be allocated then
             * de-allocate memory allocated so far and
             * exit.
             */
            free(thisCodec);
            return(NULL);
        }
        for(i=0;i<WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD;i++){
		thisCodec->txBuffer[i].audioWord=0;
		}
#else
        thisCodec->txBuffer = codecTxBuffer;
#endif
        thisCodec->txReadPtr = 0;
        thisCodec->txWritePtr = 0;

#ifdef DMA_PING_PONG
        DmaChnConfigure(WM8960DRV_SPI_TX_DMA_CHANNEL, DMA_CHN_PRI0, DMA_CONFIG_DEFAULT);
#else
        /* Configure the DMA channel. Channel priority is
         * zero. The channel is placed in continuous mode
         */

        DmaChnConfigure(WM8960DRV_SPI_TX_DMA_CHANNEL, DMA_CHN_PRI0, DMA_CONFIG_AUTO);
#endif
        DmaChnSetEventControl(WM8960DRV_SPI_TX_DMA_CHANNEL, 
                DMA_EV_START_IRQ_EN|
                DMA_EV_START_IRQ(WM8960DRV_SPI_MODULE_TX_IRQ));

#ifdef  DMA_PING_PONG

        /* If ping pong buffering is used, then enable
         * the block done event. We will switch ping pong
         * buffers in the event done interrupt. Set the
         * block count to half the size of the buffer
         * and then 
         */

        DmaChnSetEvEnableFlags(WM8960DRV_SPI_TX_DMA_CHANNEL,DMA_EV_BLOCK_DONE);
        INTSetVectorPriority(WM8960DRV_SPI_TX_DMA_VECTOR, WM8960DRV_SPI_TX_DMA_INT_PRI_LEVEL);
        INTSetSubPriority(WM8960DRV_SPI_TX_DMA_VECTOR, WM8960DRV_SPI_TX_DMA_INT_SPRI_LEVEL);
        INTClearFlag(WM8960DRV_SPI_TX_DMA_INTERRUPT);

        DmaChnSetTxfer(WM8960DRV_SPI_TX_DMA_CHANNEL,
                (void*)thisCodec->txBuffer,
                (void *)(&WM8960DRV_SPI_MODULE_BUFFER),
                (DMA_PP_BUFFER_SIZE * sizeof(STEREO_AUDIO_DATA)), 
                sizeof(UINT32),sizeof(UINT32));
#else

        DmaChnSetTxfer(WM8960DRV_SPI_TX_DMA_CHANNEL,
                (void*)thisCodec->txBuffer,
                (void *)(&WM8960DRV_SPI_MODULE_BUFFER),
                WM8960DRV_TX_BUFFER_SIZE_BYTES, 
                sizeof(UINT32),sizeof(UINT32));
#endif
    }

    if((codecOpenMode == O_RDONLY ) ||
            (codecOpenMode == O_RDWR))
    {
        /* This means that RX function is desired
         * and the codec will perform record 
         * function.
         */

#ifdef WM8960DRV_DYNAMIC_ALLOCATION
        thisCodec->rxBuffer = (STEREO_AUDIO_DATA *)malloc(WM8960DRV_RX_BUFFER_SIZE_BYTES);
        if(thisCodec->rxBuffer == NULL)
        {
            /* If memory could not be allocated then
             * de-allocate memory allocated so far and
             * exit.
             */

            if(thisCodec->txBuffer != NULL) 
            {
                free(thisCodec->txBuffer);
            }
            free(thisCodec);
            return(NULL);
        }

		for(i=0;i<WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD;i++){
		thisCodec->rxBuffer[i].audioWord=0;
		}
#else
        /* Static memory allocation */
        thisCodec->rxBuffer = codecRxBuffer;
#endif
        thisCodec->rxReadPtr = 0;
        thisCodec->rxWritePtr = 0;

        /* Configure RX DMA channel */
        /* Configure the DMA channel. Channel priority is
         * zero. The channel is placed in continuous mode
         * */

#ifdef DMA_PING_PONGR
        DmaChnConfigure(WM8960DRV_SPI_RX_DMA_CHANNEL, DMA_CHN_PRI0, DMA_CONFIG_DEFAULT);
#else
        DmaChnConfigure(WM8960DRV_SPI_RX_DMA_CHANNEL, DMA_CHN_PRI0, DMA_CONFIG_AUTO);
#endif

        DmaChnSetEventControl(WM8960DRV_SPI_RX_DMA_CHANNEL, 
                DMA_EV_START_IRQ_EN|
                DMA_EV_START_IRQ(WM8960DRV_SPI_MODULE_RX_IRQ));

#ifdef  DMA_PING_PONGR

        /* If ping pong buffering is used, then enable
         * the block done event. We will switch ping pong
         * buffers in the event done interrupt. Set the
         * block count to half the size of the buffer
         * and then 
         */

        DmaChnSetEvEnableFlags(WM8960DRV_SPI_RX_DMA_CHANNEL,DMA_EV_BLOCK_DONE);
        INTSetVectorPriority(WM8960DRV_SPI_RX_DMA_VECTOR, WM8960DRV_SPI_RX_DMA_INT_PRI_LEVEL);
        INTSetSubPriority(WM8960DRV_SPI_RX_DMA_VECTOR, WM8960DRV_SPI_RX_DMA_INT_SPRI_LEVEL);
        INTClearFlag(WM8960DRV_SPI_RX_DMA_INTERRUPT);

        DmaChnSetTxfer(WM8960DRV_SPI_RX_DMA_CHANNEL,
        		(void *)(&WM8960DRV_SPI_MODULE_BUFFER),
                (void*)thisCodec->rxBuffer,
                sizeof(UINT32),
                (DMA_PP_BUFFER_SIZE * sizeof(STEREO_AUDIO_DATA)), 
                sizeof(UINT32));
#else
        DmaChnSetTxfer(WM8960DRV_SPI_RX_DMA_CHANNEL,
                (void *)(&WM8960DRV_SPI_MODULE_BUFFER),
                (void*)thisCodec->rxBuffer,
                sizeof(UINT32),
                WM8960DRV_RX_BUFFER_SIZE_BYTES, 
                sizeof(UINT32));
#endif
    }

    /* Intialize the the codec control 
     * I2C module.
     */

    I2CConfigure(WM8960DRV_I2C_MODULE,0);
    I2CSetFrequency(WM8960DRV_I2C_MODULE, GetPeripheralClock(), WM8960DRV_I2C_BAUD);
    I2CEnable(WM8960DRV_I2C_MODULE, TRUE);

    /* Configure the SPI module for operation with 
     * the codec. Use the DSP/PCM Mode Audio Interface
     * as detailed in figure 30 in the codec data sheet.
     * Since SPI is slave clock div should not matter.
     */

    spiFlags =  SPI_OPEN_SLVEN      // Slave mode enable 
        | SPI_OPEN_SSEN     // Enable slave select function
        | SPI_OPEN_CKP_HIGH // Clock polarity Idle High Actie Low
        | SPI_OPEN_MODE32   // Character size is 32 bit
        | SPI_OPEN_FRMEN    // Enable Framed SPI
        | SPI_OPEN_FSP_IN   // Frame Sync Pulse is input
        | SPI_OPEN_FSP_HIGH; // Frame Sync Pulse is active high

    SpiChnConfigure(WM8960DRV_SPI_MODULE, spiFlags);

    /* Setup default state of codec registers */
    WM8960CodecSetRegCopyToDefault();

    INT command = 0;
    WM8960Codec_ioctl(thisCodec, WM8960_RESET, &command);
    
 
    return(thisCodec);

}

INT WM8960Codec_ioctl(WM8960_CODEC * pCodecHandle, WM8960_REGISTER commandRegister, void * data)
{
    /* This function writes a value to the
     * codec register. It returns 1 if the
     * function is successful. -1 otherwise
     */

    INT command = *(INT *)data;
    BYTE byte1  = (BYTE) (commandRegister << 1) | (command >> 8);
    BYTE byte2  = (BYTE) (command & 0xFF);

    /* Wait for the bus to go idle */
    while(I2CBusIsIdle(WM8960DRV_I2C_MODULE) == FALSE);

    /* Send a start */

    if(I2CStart(WM8960DRV_I2C_MODULE) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  

        return(-1);
    }

    /* Wait for start to complete */
    while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_START));  

    /* Now send the codec I2C address */
    if(I2CSendByte(WM8960DRV_I2C_MODULE,WM8960_I2C_ADDRESS) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the address was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  
        return(-1);
    }

    /* Now send the first byte. This includes bits
     * 7 through 0 of the codec register address and
     * bit 8 of the data to be written to the 
     * register */

    if(I2CSendByte(WM8960DRV_I2C_MODULE,byte1) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the address was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  
        return(-1);
    }

    /* Now send the second byte. This includes bits
     * 7 through 0 of the codec register address and
     * bit 8 of the data to be written to the 
     * register */

    if(I2CSendByte(WM8960DRV_I2C_MODULE,byte2) != I2C_SUCCESS)
    {
        /* This means there was a bus collision
         * Send a stop condition and exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  

        return(-1);
    }

    /* Wait till transmission is completed */
    while(!I2CTransmissionHasCompleted(WM8960DRV_I2C_MODULE));

    /* Check if the byte2 was acknowledged */
    if(I2CByteWasAcknowledged(WM8960DRV_I2C_MODULE) == FALSE)
    {
        /* This means some thing went wrong. The address
         * was not acknowledged. Send a stop condition and
         * exit. */

        I2CStop(WM8960DRV_I2C_MODULE);

        /* Wait for stop to complete */
        while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  
        return(-1);
    }

    /* Send the stop condition */
    I2CStop(WM8960DRV_I2C_MODULE);

    /* Wait for stop to complete */
    while(!(I2CGetStatus(WM8960DRV_I2C_MODULE) & I2C_STOP));  

    return(1);    

}

void WM8960CodecClear(WM8960_CODEC * codecHandle){
	int i;

		for(i=0;i<WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD;i++){
		codecHandle->rxBuffer[i].audioWord=0;
		codecHandle->txBuffer[i].audioWord=0;
		}
}

inline UINT WM8960Codec_write(WM8960_CODEC * codecHandle, STEREO_AUDIO_DATA * data, UINT nStereoSamples)
{
    /*************************************
     * This is a non blocking write call.
     * The function will copy stereo data 
     * from the application buffer to the 
     * codec driver buffer. The amount of
     * data to be copied will be determined
     * by the txBufferWritePtr and 
     * txBufferReadPtr
     *************************************/

    UINT nWrittenSamples = 0;
    UINT i;

    /* Check if there is nothing to write */

    if(nStereoSamples == 0) return(0);

#ifdef DMA_PING_PONG

    UINT ppFlag;
    UINT usePPBuffer;
    STEREO_AUDIO_DATA * dest;

    /* Check which png pong buffer is being
     * consumed. */

    ppFlag = codecHandle->ppFlag;
    usePPBuffer = (ppFlag == 0) ? 1 : 0;


    if(codecHandle->ppBufferIsBusy[usePPBuffer] == TRUE)
    {
        /* This means that the buffer that we want to 
         * write is not available. It is waiting to be
         * consumed by the DMA. This can happen if the
         * write function is called too often. */

        return(0);

    }

    /* Check the size of the audio frame to written */

    nWrittenSamples = (nStereoSamples > DMA_PP_BUFFER_SIZE) ? 
        DMA_PP_BUFFER_SIZE : nStereoSamples;

    /* Write the data to the correct PP buffer*/

    dest = (usePPBuffer == 0) ? &codecHandle->txBuffer[0] : 
        &codecHandle->txBuffer[DMA_PP_BUFFER_SIZE];

    for(i = 0; i < nWrittenSamples; i ++ )
    {
        dest[i].audioWord = data[i].audioWord;
    }

    codecHandle->ppBufferCount[usePPBuffer] = nWrittenSamples;        
    codecHandle->ppBufferIsBusy[usePPBuffer] = TRUE;

#else

    /* Compute the amount of space available
     * in the codec driver transmit buffer.
     */

    UINT txReadPtr = DmaChnGetSrcPnt(WM8960DRV_SPI_TX_DMA_CHANNEL)/sizeof(UINT32);
    UINT txWritePtr = codecHandle->txWritePtr;
    STEREO_AUDIO_DATA * dest = codecHandle->txBuffer;

    if(txWritePtr == txReadPtr)
    {
        /* This means that tx buffer is empty
        */

        nWrittenSamples = WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD;
    }
    else if(txReadPtr > txWritePtr)
    {
        /* This means that application write is
         * lagging the codec driver read but no 
         * wrap around has occured.
         */

        nWrittenSamples = txReadPtr - txWritePtr;
    }
    else
    {
        /* This means that application write is
         * lagging the codec driver read but  
         * wrap around has occured.
         */

        nWrittenSamples = WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD - txWritePtr
            + txReadPtr;
    }  

    /* Make sure we only write the number of samples
     * that need to be written
     */

    nWrittenSamples = (nWrittenSamples > nStereoSamples) 
        ? nStereoSamples : nWrittenSamples;


    /* At this point nWrittenSamples contains the number
     * of stereo samples that can be actual written
     */

    for(i = 0; i < nWrittenSamples; i++)
    {
        /* Copy data from the application buffer
         * to the codec driver buffer. Also implement
         * driver buffer wrap around.
         */

        dest[txWritePtr].audioWord = data[i].audioWord;

        txWritePtr ++;
        if(txWritePtr >= WM8960DRV_TX_BUFFER_SIZE_STEREO_WORD)
        {
            txWritePtr = 0;
        }

    }

    /* Update the codec driver structure
    */

    codecHandle->txWritePtr = txWritePtr;

    /* Enable the channel */

    DmaChnEnable(WM8960DRV_SPI_TX_DMA_CHANNEL);

#endif

    return(nWrittenSamples);    
}

INT WM8960CodecEnableMicrophone(WM8960_CODEC * pCodecHandle, BOOL enable)
{
    INT command;
    INT result;

    if(enable == TRUE)
    {
        /* Connects the LINPUT1 to PGA input */
        command = 0x138;
        WM8960Registers[WM8960_ADCL_SIGNAL_PATH] = command;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADCL_SIGNAL_PATH, &command);
        if(result < 0) return (-1);
    }
}

INT WM8960CodecEnableLineIn(WM8960_CODEC * pCodecHandle,BOOL enable)
{
    INT command;
    INT result;

    if(enable == TRUE)
    {
        /* Connect Line input to the PGA */
        command = 0x048;
        WM8960Registers[WM8960_ADCL_SIGNAL_PATH] = command;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADCL_SIGNAL_PATH, &command);
        if(result < 0) return (-1);

        command = 0x048;
        WM8960Registers[WM8960_ADCR_SIGNAL_PATH] = command;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADCR_SIGNAL_PATH, &command);
        if(result < 0) return (-1);

        /* Line input 2 gain set to 0 db */        
        command = 0x0A;
        WM8960Registers[WM8960_INPUT_BOOST_MIXER_1] = command;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_INPUT_BOOST_MIXER_1, &command);
        if(result < 0) return (-1);

        command = 0x0A;
        WM8960Registers[WM8960_INPUT_BOOST_MIXER_2] = command;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_INPUT_BOOST_MIXER_2, &command);
        if(result < 0) return (-1);
    }

}

inline UINT WM8960Codec_read(WM8960_CODEC * codecHandle, 
                STEREO_AUDIO_DATA * data, UINT nStereoSamples)
{
    /*************************************
     * This is a non blocking read call.
     * The function will copy stereo data 
     * from the codec driver buffer to the
     * application buffer . The amount of
     * data to be copied will be determined
     * by the rxBufferWritePtr and 
     * rxBufferReadPtr
     *************************************/

    UINT nReadSamples = 0;
    UINT i;

    /* Check if nothing should be read */

    if(nStereoSamples == 0) return(0);



#ifdef DMA_PING_PONGR

    UINT ppFlagR;
    UINT usePPBufferR;
    STEREO_AUDIO_DATA * src;

    /* Check which png pong buffer is being
     * consumed. */

    ppFlagR = codecHandle->ppFlagR;
    usePPBufferR = (ppFlagR == 0) ? 1 : 0;


    if(codecHandle->ppBufferIsBusyR[usePPBufferR] == TRUE)
    {
        /* This means that the buffer that we want to 
         * write is not available. It is waiting to be
         * consumed by the DMA. This can happen if the
         * write function is called too often. */

        return(0);

    }

    /* Check the size of the audio frame to written */

    nReadSamples = (nStereoSamples > DMA_PP_BUFFER_SIZE) ? 
        DMA_PP_BUFFER_SIZE : nStereoSamples;

    /* Write the data to the correct PP buffer*/

    src = (usePPBufferR == 0) ? &codecHandle->rxBuffer[0] : 
        &codecHandle->rxBuffer[DMA_PP_BUFFER_SIZE];

    for(i = 0; i < nReadSamples; i ++ )
    {
        data[i].audioWord = src[i].audioWord;
    }

    codecHandle->ppBufferCountR[usePPBufferR] = nReadSamples;        
    codecHandle->ppBufferIsBusyR[usePPBufferR] = TRUE;

#else


    /* Compute the amount of date available
     * in the codec driver receive buffer.
     */

    UINT rxBufferReadPtr = codecHandle->rxReadPtr;
    UINT rxBufferWritePtr = DmaChnGetDstPnt(WM8960DRV_SPI_RX_DMA_CHANNEL)/sizeof(STEREO_AUDIO_DATA);
    STEREO_AUDIO_DATA * src = codecHandle->rxBuffer;

    if(rxBufferWritePtr == rxBufferReadPtr)
    {
        /* This means that the buffer is
         * empty.
         */

        return(0);
    }
    else if( rxBufferWritePtr > rxBufferReadPtr)
    {
        /* This means that the codec driver is
         * ahead of the application read */

        nReadSamples = rxBufferWritePtr - rxBufferReadPtr;
    }
    else
    {
        /* This means the codec driver is ahead
         * of the application read but the write
         * has wrapped around.
         */

        nReadSamples = WM8960DRV_RX_BUFFER_SIZE_STEREO_WORD  - rxBufferReadPtr
            + rxBufferWritePtr;

    }

    /* If the number of available samples is greater
     * than those required, then only read the number
     * of required samples.
     */

    nReadSamples = (nReadSamples > nStereoSamples) 
        ? nStereoSamples : nReadSamples;

    /* Store the samples in the application buffer 
     * and handle the read pointer rollover
     */


    for(i = 0; i < nReadSamples; i ++)
    {
        data[i].audioWord = src[rxBufferReadPtr].audioWord;
        rxBufferReadPtr ++;
        if(rxBufferReadPtr  >= WM8960DRV_RX_BUFFER_SIZE_STEREO_WORD)  
        {
            rxBufferReadPtr = 0;
        }  
    }

    codecHandle->rxReadPtr = rxBufferReadPtr;
#endif
    return(nReadSamples);

}

void WM8960CodecStartAudio(WM8960_CODEC * pCodecHandle, BOOL enable)
{
    if(enable == TRUE)
    {
#ifdef DMA_PING_PONG
        /* Initialize the ping pong buffer system.
         * The DMA will first process buffer 0.
         * The initial size of each buffer is set
         * at half the buffer size. Clear the event
         * flag and then enable the interrupt. */

        pCodecHandle->ppFlag = 0;
        pCodecHandle->ppBufferIsBusy[0] = TRUE;
        pCodecHandle->ppBufferIsBusy[1] = FALSE;
        pCodecHandle->ppBufferCount[0] = DMA_PP_BUFFER_SIZE;
        pCodecHandle->ppBufferCount[1] = DMA_PP_BUFFER_SIZE;
        DmaChnClrEvFlags(WM8960DRV_SPI_TX_DMA_CHANNEL, DMA_EV_BLOCK_DONE);
        INTEnable( WM8960DRV_SPI_TX_DMA_INTERRUPT, INT_ENABLED);


        pCodecHandle->ppFlagR = 0;
        pCodecHandle->ppBufferIsBusyR[0] = TRUE;
        pCodecHandle->ppBufferIsBusyR[1] = FALSE;
        pCodecHandle->ppBufferCountR[0] = DMA_PP_BUFFER_SIZE;
        pCodecHandle->ppBufferCountR[1] = DMA_PP_BUFFER_SIZE;
        DmaChnClrEvFlags(WM8960DRV_SPI_RX_DMA_CHANNEL, DMA_EV_BLOCK_DONE);
        INTEnable( WM8960DRV_SPI_RX_DMA_INTERRUPT, INT_ENABLED);
#endif
        SpiChnEnable(WM8960DRV_SPI_MODULE, TRUE);
        DmaChnEnable(WM8960DRV_SPI_TX_DMA_CHANNEL);
        DmaChnEnable(WM8960DRV_SPI_RX_DMA_CHANNEL);
        //SpiChnEnable(WM8960DRV_SPI_MODULE, TRUE);
    }
    else
    {
	    DmaChnDisable(WM8960DRV_SPI_TX_DMA_CHANNEL);
        DmaChnDisable(WM8960DRV_SPI_RX_DMA_CHANNEL);
        SpiChnEnable(WM8960DRV_SPI_MODULE, FALSE);
        DmaChnDisable(WM8960DRV_SPI_TX_DMA_CHANNEL);
        DmaChnDisable(WM8960DRV_SPI_RX_DMA_CHANNEL);
    }
}


INT WM8960CodecConfigVolume(WM8960_CODEC * pCodecHandle, UINT8 volumePerA, UINT8 volumePerD)
{
    INT result;	
    UINT8 volumeA;
	UINT8 volumeD;
	
	volumePerA=(volumePerA>90)?90 : volumePerA;
	volumePerD=(volumePerD>90)?90 : volumePerD;
	
	volumeA=0x7F + (UINT8)((long)(0x7F*volumePerA)/100);
	volumeD=0x7F + (UINT8)((long)(0x7F*volumePerD)/100);


    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);
    		
	
    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */ 

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);
    
    
    
    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x100|volumeA;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x100|volumeA;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);
    		
	
    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x100|volumeD;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */ 

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x100|volumeD;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);
 
}


INT WM8960CodecConfigDACMute(WM8960_CODEC * pCodecHandle, BOOL enable){
	INT command;
	INT result;
	command=WM8960Registers[WM8960_ADC_DAC_CTRL1]|((enable==TRUE)?0x8:0x0);
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &command);
                if(result < 0) return(-1);
}

INT WM8960CodecConfigSampleRate(WM8960_CODEC * pCodecHandle, WM8960_SAMPLE_RATE sampleRate)
{
    /* This function configures the codec
     * for 48KHz sample rate. This function 
     * should be the first function that is called
     * after codec driver is opened. It will 
     * configure the digital interface but will 
     * not enable it.
     */

    INT command;
    INT result;

    /* Reset the codec */
 //   command = 0;
 //   result = WM8960Codec_ioctl(pCodecHandle, WM8960_RESET, &command);
 //   if (result < 0) return(0);

    /* Disable Left and Right PGA Mute. Other register
     * bits are default. */

    WM8960Registers[WM8960_LEFT_INPUT_VOLUME] = 0x13F;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_INPUT_VOLUME, 
            &WM8960Registers[WM8960_LEFT_INPUT_VOLUME]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_RIGHT_INPUT_VOLUME] = 0x13F;
    WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_INPUT_VOLUME, 
            &WM8960Registers[WM8960_RIGHT_INPUT_VOLUME]);
    if(result < 0) return(-1);

    /* Configure output for the 0dB level. Set bit
     * so that the volume is updated. */

    WM8960Registers[WM8960_LEFT_OUT1_VOLUME] = 0x179;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_OUT1_VOLUME, 
            &WM8960Registers[WM8960_LEFT_OUT1_VOLUME]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_RIGHT_OUT1_VOLUME] = 0x179;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_OUT1_VOLUME, 
            &WM8960Registers[WM8960_RIGHT_OUT1_VOLUME]);
    if(result < 0) return(-1);


    /* More ADC and DAC configuration */

    WM8960Registers[WM8960_ADC_DAC_CTRL2] = 0xc;//0;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL2, 
            &WM8960Registers[WM8960_ADC_DAC_CTRL2]);
    if(result < 0) return(-1);

    /* Audio Interface */

    WM8960Registers[WM8960_AUDIO_INTERFACE_1] = 0x43;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_AUDIO_INTERFACE_1,
            &WM8960Registers[WM8960_AUDIO_INTERFACE_1]);
    if(result < 0) return(-1);

    /* Configure bit clock to support a word length
     * of 16 bits. This will allow 32 bits to  be 
     * transmitted in one frame.
     */

    WM8960Registers[WM8960_CLOCKING_2] = 0x1C7;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_CLOCKING_2,
            &WM8960Registers[WM8960_CLOCKING_2]);
    if(result < 0) return(-1);

    /* Default values */
    WM8960Registers[WM8960_AUDIO_INTERFACE_2] = 0x0;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_AUDIO_INTERFACE_2,
            &WM8960Registers[WM8960_AUDIO_INTERFACE_2]);
    if(result < 0) return(-1);

    /* Left DAC volume. Set at 0 db*/
    WM8960Registers[WM8960_LEFT_DAC_VOLUME] = 0x1D7;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_DAC_VOLUME,
            &WM8960Registers[WM8960_LEFT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Right DAC volume. Set at 0 dB */ 

    WM8960Registers[WM8960_RIGHT_DAC_VOLUME] = 0x1D7;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_DAC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_DAC_VOLUME]);
    if(result < 0) return(-1);

    /* Left ADC volume. Set at zero dB */

    WM8960Registers[WM8960_LEFT_ADC_VOLUME] = 0x1C3;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_ADC_VOLUME,
            &WM8960Registers[WM8960_LEFT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Right ADC volume. Set at zero dB */

    WM8960Registers[WM8960_RIGHT_ADC_VOLUME] = 0x1C3;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_ADC_VOLUME,
            &WM8960Registers[WM8960_RIGHT_ADC_VOLUME]);
    if(result < 0) return(-1);

    /* Left Output Mixer */

    WM8960Registers[WM8960_LEFT_OUT_MIX] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_LEFT_OUT_MIX,
            &WM8960Registers[WM8960_LEFT_OUT_MIX]);
    if(result < 0) return(-1);

    /* Right Output Mixer */

    WM8960Registers[WM8960_RIGHT_OUT_MIX] = 0x100;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_RIGHT_OUT_MIX,
            &WM8960Registers[WM8960_RIGHT_OUT_MIX]);
    if(result < 0) return(-1);

    /* Set up the Microphone Bias voltage level */

    WM8960Registers[WM8960_ADDITIONAL_CTRL_4   ] = 0x3;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADDITIONAL_CTRL_4 ,
            &WM8960Registers[WM8960_ADDITIONAL_CTRL_4 ]);

    if(result < 0) return(-1);

    if(sampleRate != SAMPLE_RATE_44100_HZ)
    {

        /* If the required sampling rate is 48,32,24,16 or 8
         * KHz, the SYSCLK should ne 12.288MHz.*/ 

        /* Configure the PLL to provide a 12.288 MHz clock */

        WM8960Registers[WM8960_PLL_N] = 0x28;
        WM8960Registers[WM8960_PLL_K_1] = 0x31;
        WM8960Registers[WM8960_PLL_K_2] = 0x26;
        WM8960Registers[WM8960_PLL_K_3] = 0xE9;

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_N, 
                &WM8960Registers[WM8960_PLL_N]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_1, 
                &WM8960Registers[WM8960_PLL_K_1]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_2, 
                &WM8960Registers[WM8960_PLL_K_2]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_3, 
                &WM8960Registers[WM8960_PLL_K_3]);
        if(result < 0) return(-1);

        /* Configure the ADC and DAC clock dividers */

        switch(sampleRate)
        {
            case SAMPLE_RATE_48000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x5;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x6;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            case SAMPLE_RATE_32000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x4D;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x2;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
/*                
            case SAMPLE_RATE_24000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x95;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
*/
            case SAMPLE_RATE_16000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0xDD;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            case SAMPLE_RATE_8000_HZ:
                WM8960Registers[WM8960_CLOCKING_1] = 0x1B5;
                result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                        &WM8960Registers[WM8960_CLOCKING_1]);
                if(result < 0) return(-1);
                WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x0;
                result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                        &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
                if(result < 0) return(-1);
                break;
            default:
                /* sample rate is not supported */
                return(-1);
                break;
        }

    }
    else if(sampleRate == SAMPLE_RATE_44100_HZ)
    {
        /* The sampling rate is 44100Hz and this needs a
         * SYSCLK of 11.2896 MHz */

        WM8960Registers[WM8960_PLL_N] = 0x27;
        WM8960Registers[WM8960_PLL_K_1] = 0x86;
        WM8960Registers[WM8960_PLL_K_2] = 0xC2;
        WM8960Registers[WM8960_PLL_K_3] = 0x26;

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_N, 
                &WM8960Registers[WM8960_PLL_N]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_1, 
                &WM8960Registers[WM8960_PLL_K_1]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_2, 
                &WM8960Registers[WM8960_PLL_K_2]);
        if(result < 0) return(-1);

        result = WM8960Codec_ioctl(pCodecHandle,WM8960_PLL_K_3, 
                &WM8960Registers[WM8960_PLL_K_3]);
        if(result < 0) return(-1);

        WM8960Registers[WM8960_CLOCKING_1] = 0x05;
        result = WM8960Codec_ioctl(pCodecHandle,  WM8960_CLOCKING_1, 
                &WM8960Registers[WM8960_CLOCKING_1]);
        if(result < 0) return(-1);
        WM8960Registers[WM8960_ADC_DAC_CTRL1] = 0x4;
        result = WM8960Codec_ioctl(pCodecHandle, WM8960_ADC_DAC_CTRL1, 
                &WM8960Registers[WM8960_ADC_DAC_CTRL1]);
        if(result < 0) return(-1);


    }

    /* Power up all the modules */

    WM8960Registers[WM8960_POWER_MGMT_1] = 0xFE;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_POWER_MGMT_1,
            &WM8960Registers[WM8960_POWER_MGMT_1]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_POWER_MGMT_2] = 0x1E1;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_POWER_MGMT_2,
            &WM8960Registers[WM8960_POWER_MGMT_2]);
    if(result < 0) return(-1);

    WM8960Registers[WM8960_POWER_MGMT_3] = 0x03C;
    result = WM8960Codec_ioctl(pCodecHandle, WM8960_POWER_MGMT_3,
            &WM8960Registers[WM8960_POWER_MGMT_3]);
    if(result < 0) return(-1);

    return(1);


}

inline UINT WM8960CodecWrite(WM8960_CODEC * codecHandle, STEREO_AUDIO_DATA * data, UINT nSamples) 
{
    /* This is a blocking implementation of the
     * codec write function. This function will not
     * return unless nSamples have been written
     */

    UINT written = 0;

    while(written < nSamples)
    {
        written += WM8960Codec_write(codecHandle, &data[written], (nSamples - written));
    }

    return(nSamples);
}

inline UINT WM8960CodecRead(WM8960_CODEC * codecHandle, STEREO_AUDIO_DATA * data, UINT nSamples) 
{
    /* This is a blocking implementation of the
     * codec read function. This function will not
     * return unless nSamples have been written
     */

    UINT read = 0;

    while(read < nSamples)
    {
        read += WM8960Codec_read(codecHandle, &data[read], (nSamples - read));
    }

    return(nSamples);
}


#ifdef DMA_PING_PONG

void __attribute__((vector(36),
                interrupt(ipl5), nomips16))DmaInterruptHandler(void)
{
    STEREO_AUDIO_DATA * dest = thisCodec->txBuffer;

    INT size;

    /* Clear the interrupt and event flag */
    INTClearFlag(WM8960DRV_SPI_TX_DMA_INTERRUPT);
    DmaChnClrEvFlags(WM8960DRV_SPI_TX_DMA_CHANNEL, DMA_EV_BLOCK_DONE);

    /* Make the consumed buffer available
     * to the application */

    UINT ppFlag = thisCodec->ppFlag;
    thisCodec->ppBufferIsBusy[ppFlag] = FALSE;
    /* Prepare the other buffer for
     * processing */

    ppFlag = (ppFlag == 0) ? 1 : 0;
    thisCodec->ppBufferIsBusy[ppFlag] = TRUE;
    dest = (ppFlag == 0) ? dest : &dest[DMA_PP_BUFFER_SIZE];
    size = thisCodec->ppBufferCount[ppFlag] * sizeof(STEREO_AUDIO_DATA);

    DmaChnSetTxfer(WM8960DRV_SPI_TX_DMA_CHANNEL,
            (void*)dest,
            (void *)(&WM8960DRV_SPI_MODULE_BUFFER),
            size, 
            sizeof(UINT32),sizeof(UINT32));

    thisCodec->ppFlag = ppFlag;
    DmaChnEnable(WM8960DRV_SPI_TX_DMA_CHANNEL);

}
#endif


#ifdef DMA_PING_PONGR

void __attribute__((vector(37),
                interrupt(ipl5), nomips16))DmaInterruptHandlerR(void)
{
    STEREO_AUDIO_DATA * src = thisCodec->rxBuffer;

    INT sizeR;



    /* Make the consumed buffer available
     * to the application */

    UINT ppFlagR = thisCodec->ppFlagR;
    thisCodec->ppBufferIsBusyR[ppFlagR] = FALSE;
    /* Prepare the other buffer for
     * processing */

    ppFlagR = (ppFlagR == 0) ? 1 : 0;
    thisCodec->ppBufferIsBusyR[ppFlagR] = TRUE;
    src = (ppFlagR == 0) ? src : &src[DMA_PP_BUFFER_SIZE];
    sizeR = thisCodec->ppBufferCountR[ppFlagR] * sizeof(STEREO_AUDIO_DATA);

    /* Clear the interrupt and event flag */
    INTClearFlag(WM8960DRV_SPI_RX_DMA_INTERRUPT);
    DmaChnClrEvFlags(WM8960DRV_SPI_RX_DMA_CHANNEL, DMA_EV_BLOCK_DONE);
 
    DmaChnSetTxfer(WM8960DRV_SPI_RX_DMA_CHANNEL,
    		(void *)(&WM8960DRV_SPI_MODULE_BUFFER),
            (void*)src,
            sizeof(UINT32),
            sizeR, 
            sizeof(UINT32));
                
    thisCodec->ppFlagR = ppFlagR;
    DmaChnEnable(WM8960DRV_SPI_RX_DMA_CHANNEL);

}
#endif
