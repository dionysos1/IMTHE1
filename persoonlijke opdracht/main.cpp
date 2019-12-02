#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hd44780.h"
#include <string.h>
#include "max6675.h"

#define F_CPU 16000000
#define BAUD 9600
#define BAUD_RATE_CALC ((F_CPU/16/BAUD) - 1)

#define SPEAKER PD3
#define SPEAKER_PORT PORTD
#define SPEAKER_DDR DDRD
#define POT PC5
#define POT_PORT PORTC
#define POT_PIN PINC
#define POT_DDR DDRC

char rec;
double temp = 0;
char printbuff[100];
// thermocouple library needs to know the pin numbers
int thermoDO = 5;
int thermoCS = 6;
int thermoCLK = 7;
int sound;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

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

void setup() {
  // wait for MAX chip to stabilize
  _delay_ms(500);
}

ISR(TIMER1_OVF_vect){
  temp = thermocouple.readCelsius();
  _delay_ms(50);
}

void initTimer1(){
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << TOIE1); //timer overflow interupt enable
  sei();
}

int main(void) {
  setupSpeaker();
  setupMeter();
  initTimer1();
  setup();
  lcd_init();
  lcd_clrscr();
  // go to start of line 1
  lcd_goto(0);
  lcd_puts("Temp of Tea");
  // skip to second line
  lcd_goto(0x40);
  lcd_puts("Curr temp: ");
  while(1){
    dtostrf(temp, 10, 2, printbuff);
     lcd_clrscr();
     lcd_goto(0);
     lcd_puts("Temp of Tea");
     lcd_goto(0x40);
     lcd_puts(printbuff);
     //playSound(75);
     while (temp >= 100.0){
       playSound(100);
       lcd_clrscr();
       lcd_goto(0);
       lcd_puts("Temp of Tea");
       lcd_goto(0x40);
       lcd_puts("Tea Ready!");
       //temp = thermocouple.readCelsius();
     }
   }
   return 0;
}
