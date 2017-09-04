#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/delay.h>

void setup() {
  DDRD = (1 << PD6);  //Set our pwm pin as an output, PD6 is Arduino digital pin 6 PWM

  //Timer 0 - A configuration for pin 6
  TCCR0A = ((1 << COM0A1) | (1 << WGM01) | (1 << WGM00)); //Enable pwm mode in pin PD6 and set the WGM bits to Fast pwm mode
  TCCR0B = ((1 << CS01) | (1 << CS00));        //Set prescaler to 64 - 975Hz
}

void loop() {
  //Fade up
  for (int i = 0; i < 255; i++) {
    OCR0A = i;     //Set new duty cycle value
    _delay_ms(50);    //delay a litle bit
  }

  //Fade down
  for (int i = 255; i > 0; i--) {
    OCR0A = i;      //Set new duty cycle value
    _delay_ms(50);    //delay a litle bit
  }
}
