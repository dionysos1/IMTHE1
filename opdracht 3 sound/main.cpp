#include <avr/io.h>
#include <util/delay.h>

#define SPEAKER PD2
#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
#define POT PC5
#define POT_PORT PORTC
#define POT_PIN PINC
#define POT_DDR DDRC

// homemade delay function
void Playtime(uint16_t count) {
  while(count--) {
    _delay_us(1);
  }
}

// Play sound for t time using PWM
void playSound(uint16_t t) {
  SPEAKER_PORT = (1<< SPEAKER); // Speaker on
  Playtime(t); // Delay for input t
  SPEAKER_PORT = 0; // Speaker off
  Playtime(t); // Delay for input t
}

// Read values from potentiometer
uint16_t readMeter(uint8_t channel) {
  ADMUX = (0xf0 & ADMUX) | channel; // nieuw kanaal
  ADCSRA |= (1 << ADSC); // conversie start
  loop_until_bit_is_clear(ADCSRA, ADSC); // wacht to alle data binnen is

  return (ADC);
}

void setupSpeaker() {
  SPEAKER_DDR |= (1 << SPEAKER); //speaker aanzetten
}

void setupMeter() {
  ADMUX |= (1 << REFS0); // Reference voltage = 5v
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0); // Turn on ADC
  ADCSRA |= (1 << ADEN); // Conversion start
}

int main(void) {
  // Setup
  setupSpeaker();
  setupMeter();

  // Run main loop
  while(1){
    playSound(readMeter(POT)); // Play sound using the pot value.
  }
  return 0;
}
