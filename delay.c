#include "delay.h"


void delay_ms(unsigned int ms) {
	// we wait 800 loops per millisecond
	unsigned volatile int i,j;
	for(i=0; i<ms; i++) {
		for(j=0; j<800; j++) { // was 800
			; // do nothing, we are just waiting
		}
	}
}


void delay_us (unsigned int us) {
	unsigned int i;
	
	for (i=0; i<us; i++) {
		// no need for inner loop because it is just a delay of one?
		;
	}
}




