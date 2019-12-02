#include <avr/io.h>
#include <util/delay.h>

int main(void){
	// initialiseer pb0 en pb1
	DDRB = 0b00000011;

	// endless loop
	while(1){
		PORTB = 0b0000010; //zet led 1 aan en 2 uit
		_delay_ms(250);
		PORTB = 0b00000001; //zet led 1 uit en 2 aan
		_delay_ms(250);
	}
	return 0;
}
