#include <avr/io.h>
int adc_value;    //Variable used to store the value read from the ADC converter

void setup() {
  DDRB |= (1 << PB5); ///PB5/digital 13 is an output
  ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); //Prescaler at 128 so we have an 125Khz clock source
  ADMUX |= ~(1 << REFS0);
  ADMUX &= ~(1 << REFS1);     //Avcc(+5v) as voltage reference
  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); //ADC in free-running mode
  ADCSRA |= (1 << ADATE);     //Signal source, in this case is the free-running
  ADCSRA |= (1 << ADEN);      //Power up the ADC
  ADCSRA |= (1 << ADSC);      //Start converting
}

void loop() {
  adc_value = ADCW;  //Read the ADC value, really that's just it
  if (adc_value > 512) {
    PORTB |= (1 << PB5); //If ADC value is above 512 turn led on
  }
  else {
    PORTB &= ~(1 << PB5); //Else turn led off
  }
}
