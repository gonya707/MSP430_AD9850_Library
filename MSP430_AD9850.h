/*	Library for MSP430G2553 and compatible devices.
 *
 * 	This library contains several functions for using with a DDS generator
 * 	AD9850 module. All the functions are intended to work with this device
 * 	in SERIAL mode, so you need to connect the parallel pins D0 and D1 to VCC
 * 	and D2 GND, as written in the AD9850 datasheet. For further information
 * 	check the Analog Devices website.
 *
 * 	Coded by Gonzalo Recio
 */

#ifndef MSP430_AD9850_H
	 #define MSP430_AD9850_H


	//Interchangeable pins
	#define W_CLK  BIT0 //P1.0
	#define FQ_UD  BIT1 //P1.1
	#define DATA   BIT2 //P1.2
	#define RESET  BIT3 //P1.3

	//Frequency of your crystal oscillator (CLKIN input pin 9 in datasheet), measured in MHz
	// This reference frequency must be higher than 1MHz.
	#define XTAL_MHZ 125

	//Relationship value between actual frequency and 32-bit word sent in the serial streaming
	#define FREQ_FACTOR 4294.967295

	//function prototypes
		void AD9850_Init(void);
		void AD9850_Reset(void);
		void AD9850_Osc(double frequency, double phase);
		void AD9850_Sweep_Up(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
		void AD9850_Sweep_Down(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
		void AD9850_Sweep_Loop(double minFreq, double maxFreq, double inc, int cyclesPerDelay);
		void AD9850_PowerDown(void);

#endif
