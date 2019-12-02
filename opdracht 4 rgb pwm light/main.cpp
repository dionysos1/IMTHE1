#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_PORT PORTB
#define LED_PIN PINB
#define LED_DDR DDRB
#define LEDr OCR2A //PB3
#define LEDg OCR1B //PB2
#define LEDb OCR1A //PB1

// timer function from Make: AVR book
static inline void initTimers(void) {
  // Timer 1
  TCCR1A |= (1 << WGM10); // Fast PWM mode, 8-bit
  TCCR1A |= (1 << COM1A1); // PWM output on OCR1A
  TCCR1A |= (1 << COM1B1); // PWM output on OCR1B
  TCCR1B |= (1 << WGM12); // Fast PWM mode, pt.2
  TCCR1B |= (1 << CS11); // PWM Freq = F_CPU/8/256
  // Timer 2
  TCCR2A |= (1 << WGM20); // Fast PWM mode
  TCCR2A |= (1 << WGM21); // Fast PWM mode, pt.2
  TCCR2A |= (1 << COM2A1); // PWM output on OCR2A
  TCCR2B |= (1 << CS21); // PWM Freq = F_CPU/8/256
}

void setColourRgb(uint8_t r, uint8_t g, uint8_t b) {
  LEDr = r;
  LEDg = g;
  LEDb = b;
}

ISR(TIMER2_OVF_vect) {

  uint8_t rgbColour[3];
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;

      // set leds to specified brightness
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      _delay_ms(10);
    }
  }
}

void initInterruptTimer() {
  // Overflow interrupt enabled when timers overflow
  TIMSK2 |= (1 << TOIE2);
  // Switch interrupts on
  sei();
}

int main(void) {
  // init
  initTimers();
  initInterruptTimer();
  LED_DDR = (1 << PB1) | (1 << PB2) | (1 << PB3);

  while (1) {} //infinite while loop to keep arduino running
  return (0);
}
