#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define POWER_PORT PORTC
#define POWER_DDR DDRC
#define GROUND_PORT PORTB
#define GROUND_DDR DDRB
#define PIN_PORT PORTD
#define PIN_DDR DDRD


// LEDS = columns, GROUNDS = rows
uint8_t row[3] = {PC0,PC1,PC2}; // rij met leds
uint8_t col[3] = {PB0,PB1,PB2}; // rij met leds
uint8_t dice = 1; // start getal

void ledsOff() {
  // zet alle leds uit
  POWER_PORT = 0; // Power lines off
  GROUND_PORT = 255; // Ground lines high
}

void dance(){
  // roll door de ledjes heen tijdens het random getal berekenen
  for (uint8_t k = 0; k < 3; k++) {
    for (uint8_t i = 0; i < 3; i++) {
      ledsOff();
      POWER_PORT = (1 << row[i]);
      GROUND_PORT ^= (1 << col[k]);
      _delay_ms(200);
    };
  };
}

ISR(INT0_vect){
  // hardware interupt op button to do things
  dance();
  dice = (rand() % 6 + 1);
}

void initINT0(){
  // innitialiseer de hardware interupt
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC00);
  sei();
}

void rollDice(uint8_t diceroll) {
  // laat het nummer zien dat gerolt is
  if(diceroll>0 && diceroll<7) {
    switch (dice) {
      case 1:
        POWER_PORT = (1 << PB1);
        GROUND_PORT ^= (1 << PC1);
        break;
      case 2:
        ledsOff();
        POWER_PORT = (1 << PB0);
        GROUND_PORT ^= (1 << PC0);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB2);
        GROUND_PORT ^= (1 << PC2);
        _delay_ms(1);
        break;
      case 3:
        ledsOff();
        POWER_PORT = (1 << PB0);
        GROUND_PORT ^= (1 << PC0);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB1);
        GROUND_PORT ^= (1 << PC1);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB2);
        GROUND_PORT ^= (1 << PC2);
        _delay_ms(1);
        break;
      case 4:
        POWER_PORT = (1 << PB0);
        GROUND_PORT ^= (1 << PC0);
        _delay_ms(1);
        POWER_PORT = (1 << PB2);
        GROUND_PORT ^= (1 << PC2);
        _delay_ms(1);
        break;
      case 5:
        ledsOff();
        POWER_PORT = (1 << PB0) | (1 << PB2);
        GROUND_PORT ^= (1 << PC0);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB1);
        GROUND_PORT ^= (1 << PC1);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB0) | (1 << PB2);
        GROUND_PORT ^= (1 << PC2);
        _delay_ms(1);
        break;
      case 6:
        ledsOff();
        POWER_PORT = (1 << PB0) | (1 << PB1) | (1 << PB2);
        GROUND_PORT ^= (1 << PC0);
        _delay_ms(1);
        ledsOff();
        POWER_PORT = (1 << PB0) | (1 << PB1) | (1 << PB2);
        GROUND_PORT ^= (1 << PC2);
        _delay_ms(1);
        break;
    };
  } else {
    dice = (rand() % 6 + 1);
  };
}

void setup() {
  POWER_DDR=0xFF; // Power bank as output
  GROUND_DDR=0xFF; // Ground bank as output
  PIN_DDR=(1 << PD2); // Button pin as output
}

int main(void) {
  // Setup
  setup();
  initINT0();
  // Turn off the leds
  ledsOff();

  while(1) {
    rollDice(dice);
  }
  return 0;
}
