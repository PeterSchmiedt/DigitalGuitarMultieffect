				PIC32 Audio Development Board Demo
***********************************************************************************************
This simple example demonstrates the usage of various componets on the Audio Development Board
such as the 24-bit audio codec, 16bpp TFT display, USB OTG and serial interfaces.
It has a playback application for the user to evaluate the audio input and output 
interfaces and samplerates(8K-48KHz). Record and playback can be used to record 
raw audio on to USB thumbdrive and playback the recorded audio. The audio codec uses the
Serial Peripheral Interface (SPI) interface. The TFT display demonstrates the 
Parallel Master Port (PMP) interface. USB OTG module demonstrates the USB capability.
The PICtail interface enables MFi capability for authorized MFi licensees.

This demo works on Audio Development Board for PIC32 (DM320011).

The Audio Development Board is preprogrammed with an audio demo. Use the following
steps to run this demo:

Step 1: Power the ADB by connecting the 9V power brick provided with the ADB 
	in the power connector (J1).

Step 2: Connect a pair of headphones or powered speakers into the jack 
	labeled HP OUT (J6).

Step 3: A line-in input to the Line IN (J5) jack can be connected or the 
	on-board microphone (MIC) can be used for audio input. 
	MIC is the default audio input, but it can be changed to Line-in in the settings.

Step 4: A USB Flash drive must be plugged into the USB receptacle (J7) 
	to use the record and playback features in the demonstration.

Step 5: Press switch S3 to navigate between the individual applications. 
	Navigation is indicated by the red bevel around the application icon. 
	Record application records audio at 8KHz sample rateover MIC or line-in on to the USB thumbdrive.
	Playback application plays the previously recorded audio over Headphones. 
	If using for first time, it is required to run Record to record audio before Playback.
	Loopback application is a loopback of audio input to output. Codec can be
	configured to different sample rates for loopback for evaluation. It also displays
	the FFT of the audio signal.

Step 6: Press switch S2 to select the application. The selection is confirmed 
	by the red bevel turning green before the next application screen is presented.

Step 7: While the application is running, switch S1 can be pressed to return 
	to the main menu, which closes the application. 
	Or alternatively, switch S4 can be used to perform a specific action
	depending on the application, as described in Table 1.


NOTE:
THE PRE-COMPILED LIBRARY USED IN THIS DEMO WAS SOURCED FROM THE MICROCHIP APPLICATIONS LIBRARY (MAL). 
IF YOU ARE INTERESTED IN DEVELOPING WITH THE STACK RATHER THAN USING THIS PRECOMPILED LIBRARY, 
PLEASE USE THE LATEST SOURCE FILES PROVIDED IN THE MICROCHIP APPLICATIONS LIBRARY.
