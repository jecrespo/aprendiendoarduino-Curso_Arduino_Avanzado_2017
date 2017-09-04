#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/delay.h>

byte outputValue = 0;        // valor del PWM

void setup() {
  DDRD = (1 << PB3);  //Set our pwm pin as an output. PB3 is Arduino digital pin 11 PWM

  //Timer 2 - A configuration for pin 11 (OC2A)
  TCCR2A = ((1 << COM2A1) | (1 << WGM21) | (1 << WGM20)); //Enable pwm mode in pin PD6 and set the WGM bits to Fast pwm mode
  TCCR2B = (1 << CS22);        //Set prescaler to 64 - 500Hz
}

void loop() {
  OCR2A = outputValue;     //Set new duty cycle value
  outputValue++;
  _delay_ms(50);    //delay a litle bit
}
