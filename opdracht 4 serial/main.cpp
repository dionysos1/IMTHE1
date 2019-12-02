#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000
#define BAUD 9600
#define BAUD_RATE_CALC ((F_CPU/16/BAUD) - 1)

char rec;
char read;

void serialSetup() {
  // Register settings
  UBRR0H = (BAUD_RATE_CALC >> 8);
  UBRR0L = BAUD_RATE_CALC;
  // Enable transmit and receive
  UCSR0B = (1 << TXEN0)| (1 << TXCIE0) | (1 << RXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Data format = 8 bit
}

// Function that allows sending of a string of chars
void sendString(char* sendString) {
  for (int i = 0; i < strlen(sendString); i++){
    while (( UCSR0A & (1<<UDRE0)) == 0){}; // Wait for buffer to empty
    UDR0 = sendString[i];
  }
}

// Function that allows sending of a single char
void sendChar(char sendChar) {
  while (( UCSR0A & (1<<UDRE0)) == 0){}; // Wait for buffer to empty
  UDR0 = sendChar;
}

// Data received via serial needs to be read and returned in a char
char readData(void) {
  while((UCSR0A & (1<<RXC0)) == 0){}; // wait until read buffer is empty
  rec = UDR0; // Set our last received value
  return rec;
}

int main(void) {
  // Setup internal LED
  DDRB = (1 << PB1);
  // Setup serial
  serialSetup();
  // Print a message that shows the serial interface is ready
  sendString("Sent + to turn on the LED\n");
  sendString("Sent - to turn off the LED\n");

  // Run main loop
  while(1){
    // Wait until input is read
    read = readData();
    // Print input
    sendString("Input: ");
    sendChar(read);
    sendString(" => ");

    // Turn the led on, off or do nothing, depending on the input
    if (read == '+'){
      // led on
      sendString("LED ON");
      PORTB = (255 << PB1);
    } else if (read == '-'){
      // led off
      sendString("LED OFF");
      PORTB = 0;
    }
    // Make sure next messages get printed on a new line
    sendString("\n");
  }
  return 0;
}
