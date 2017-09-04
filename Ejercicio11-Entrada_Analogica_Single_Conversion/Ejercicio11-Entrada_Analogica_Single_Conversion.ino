const int analogOutPin = 11; // Analog output pin that the LED is attached to
byte outputValue = 0;


void setup()
{

  
  bitSet(DDRB, 7);       // LED pin (13)
  bitSet(PCICR, PCIE0);     // enable pin change interrupts on bank 0
  bitSet(PCMSK0, PCINT3);  // enable PCINT3 (PB3) pin change interrupt 11
}

void loop()
{
  analogWrite(analogOutPin, outputValue);

  delay(10);
  outputValue++;
}

ISR(PCINT0_vect)  //Pin Change Interrupt Request 0 (pins D8 to D13) (PCINT0_vect)
{
  if (bitRead(PINB, 3))
  {
    bitSet(PORTB, 7);   // LED on
    Serial.println("ON");
  }
  else
  {
    bitClear(PORTB, 7); // LED off
    Serial.println("OFF");
  }
}
