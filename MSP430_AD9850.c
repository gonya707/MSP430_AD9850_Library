#include  <msp430g2553.h>
#include "MSP430_AD9850.h"

/* Starts AD9850 operation changing its value to "all zeros".
 * Refreshes previous status from the* microcontroller.
 * */
void AD9850_Init(void){
	P1OUT = 0x00;
}


/* Reset operation for the AD9850. This function must be called before using AD9850_Osc
 * first time in the execution (check page 12 on datasheet)
 * */
void AD9850_Reset(void){
	P1OUT &= ~W_CLK;
	P1OUT |= W_CLK;
	P1OUT &= ~W_CLK;

	P1OUT &= ~RESET;
	P1OUT |= RESET;
	P1OUT &= ~RESET;

	P1OUT &= ~FQ_UD;
	P1OUT |= FQ_UD;
	P1OUT &= ~FQ_UD;

	AD9850_Osc(0, 0);
}


/* Sets the DDS sine and square oscillator to the detailed "frequency" and "phase" variables.
 * "frequency" will be turned into a 32 bit word, so the frequency resolution of 0.0291 Hz
 * with a 125 MHz reference clock. "phase" will be a 5 bit word instead so the resolution is
 * 11.5 degrees, or pi/32 radians.
 */
void AD9850_Osc(double frequency, double phase){
	int i;
	long  y = frequency * FREQ_FACTOR / XTAL_MHZ;

	while(phase > 360)
		phase -= 360;
	long z = phase / 11.5;

	//Frequency 32-bit word
	for (i = 31; i >= 0; i--){
		if(((y >> (31 - i)) & 0x01))
			P1OUT |= DATA;
		else
			P1OUT &= ~DATA;
		P1OUT |= W_CLK;
		P1OUT &= ~W_CLK;
	}
	
	//control bit #1, control bit #2 and Power off, all to low
	P1OUT &= ~DATA;
	P1OUT |= W_CLK; //1
	P1OUT &= ~W_CLK;
	P1OUT |= W_CLK; //2
	P1OUT &= ~W_CLK;
	P1OUT |= W_CLK; //po
	P1OUT &= ~W_CLK;

	//phase 5-bit word
	for (i = 5; i >= 0; i--){
		if((z >> (5 - i)) & 0x01)
			P1OUT |= DATA;
		else
			P1OUT &= ~DATA;
		P1OUT |= W_CLK;
		P1OUT &= ~W_CLK;
	}

	P1OUT |= FQ_UD;
	P1OUT &= ~FQ_UD;
}

/* Enables power down mode. This method is used for a quick "all outputs" disable.
 * The effect is the same as AD9850_Osc(0,0), but it takes less clock cycles
 */
void AD9850_PowerDown(void){
	int PDword = 0x04;
	int i;
	P1OUT |= FQ_UD;
	P1OUT &= ~FQ_UD;

	for (i = 0; i < 8; i++){
		if((PDword >> i) & 0x01)
			P1OUT |= DATA;
		else
			P1OUT &= ~DATA;
		P1OUT |= W_CLK;
		P1OUT &= ~W_CLK;
	}

	P1OUT |= FQ_UD;
	P1OUT &= ~FQ_UD;

}


/* Performs a frequency sweep increased in "inc"Hz steps. The frequency order is from low to high
 * and resets to the lower frequency when maximum frequency is reached.
 */
void AD9850_Sweep_Up(double minFreq, double maxFreq, double inc, int cyclesPerDelay){
	double f = minFreq;
	int i;
	while (1){
		AD9850_Osc(f, 0);
		f += inc;
		if (f > maxFreq)
			f = minFreq;

		for(i = 0; i < cyclesPerDelay; i++){}
	}
}

/* Performs a frequency sweep decreased in "inc"Hz steps. The frequency order is from high to low
 * and resets to the higher frequency when minimum frequency is reached.
 */
void AD9850_Sweep_Down(double minFreq, double maxFreq, double inc, int cyclesPerDelay){
	double f = maxFreq;
	int i;
	while (1){
		AD9850_Osc(f, 0);
		f -= inc;
		if (f < minFreq)
			f = maxFreq;
		for(i = 0; i < cyclesPerDelay; i++){}
	}
}

/* Performs a frequency sweep increased in "inc"Hz steps. The frequency order is from low to high,
 * but it changes to from high to low when the maximum frequency is reached and so on.
 */
void AD9850_Sweep_Loop(double minFreq, double maxFreq, double inc, int cyclesPerDelay){
	double f = minFreq;
	int i;
	while (1){
		AD9850_Osc(f, 0);
		f += inc;
		if (f > maxFreq || f < minFreq)
			inc = -inc;
		for(i = 0; i < cyclesPerDelay; i++){}
	}
}




