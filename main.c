#include <msp430g2553.h>
#include <math.h>
#include <float.h> //avoid this if neccesary (explained above)

#include "MSP430_AD9850.h"

void main(void){
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer which can reset
							  //the device after a certain period of time.

	P1DIR |= (W_CLK | FQ_UD | DATA | RESET); //0,1,2 and 3 out

	AD9850_Init();
	AD9850_Reset();
	//puts the oscillator on 1234Hz and 0 degrees phase
	AD9850_Osc(1234, 0);

}
