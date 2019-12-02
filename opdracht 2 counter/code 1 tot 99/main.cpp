#include <avr/io.h>
#include <util/delay.h>

#define SEVEN_SEGMENT_PORT PORTD
#define SEVEN_SEGMENT_DDR DDRD
#define POWER_PORT PORTB
#define POWER_DDR DDRB
#define POWERPIN_1 PB0
#define POWERPIN_2 PB1

uint8_t nummers[] = {
  0b10001000, // 0
  0b11011011, // 1
  0b10100010, // 2
  0b11000010, // 3
  0b11010001, // 4
  0b11000100, // 5
  0b10000100, // 6
  0b11011010, // 7
  0b10000000, // 8
  0b11000000  // 9
};

void segmentchanger(uint8_t n){
  SEVEN_SEGMENT_PORT=n;
}

void setup() {
  SEVEN_SEGMENT_DDR=0xFF; // Set all as output
  SEVEN_SEGMENT_PORT=0xFF; // Segments off
  POWER_DDR=0xFF; // Set our power lines as output
  POWER_PORT=0xFF; // Power lines turned off
  UCSR0B = 0; // Turns off serial pins to be used as output
}

int main(void) {
  // Setup
  setup();

  // Run main loop
  while(1) {

    for(uint8_t count1 = 0; count1 < 10; count1++) { // tientallen
      // Count
      for(uint8_t count2 = 0; count2 < 10; count2++) { // enkelen
        for(uint8_t time=0; time < 240; time++) { // doe dit 240 ms lang voor het volgende getal komt
          POWER_PORT = (1 << POWERPIN_2);
          //SevenSegment(count1);
          segmentchanger(nummers[count1]);
          _delay_ms(1);
          POWER_PORT = (1 << POWERPIN_1);
          //SevenSegment(count2);
          segmentchanger(nummers[count2]);
          _delay_ms(1);
         }
      }
    }
  }
 	return 0;
}
