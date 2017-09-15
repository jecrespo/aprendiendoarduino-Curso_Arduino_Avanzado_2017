//Ejercicio basado en ejemplo 3 del libro incluido en el Arduino Starter Kit
#include <MsTimer2.h>
#include <EEPROM.h>

#define MAXAddress 100
#define MINAddress 200

const int sensorPin = A0;
const float baselineTemp = 20.0;
float temp;
float max_temperature;
float min_temperature;

void setup() {
  Serial.begin(9600);
  Serial.println("Pulsa C para temperatura en grados Centigrados\nPulsa F para temperatura en grados Fahrenheit\nPulsa M para maxima\nPulsa m para minima\npulsa R para resetear max y min");
  for (int pinNumber = 3; pinNumber < 6; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

  //Actualiziar datos de la EEPROM
  EEPROM.get(MAXAddress,max_temperature);
  EEPROM.get(MINAddress,min_temperature);

  Serial.println("valores leidos de la EEPROM:");
  Serial.println("Temperatura maxima: " + (String)max_temperature);
  Serial.println("Temperatura minima: " + (String)min_temperature);

  MsTimer2::set(5000, print_temperature); // 5000ms period
  MsTimer2::start();
}

void loop() {
  int sensorVal = analogRead(sensorPin);
  float voltage = (sensorVal / 1024.0) * 5.0;
  temp = (voltage - 0.5) * 100;
  
  if (temp > max_temperature) {
    max_temperature = temp;
    EEPROM.put(MAXAddress, max_temperature);
  }

  if (temp < min_temperature) {
    min_temperature = temp;
    EEPROM.put(MINAddress, min_temperature);
  }

  if (temp < baselineTemp) {
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else if (temp >= baselineTemp + 2 && temp < baselineTemp + 4) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else if (temp >= baselineTemp + 4 && temp < baselineTemp + 6) {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
  else if (temp >= baselineTemp + 6) {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(1);

  if (Serial.available() > 0) {
    char caracterLeido = Serial.read();
    if (caracterLeido == 'c' || caracterLeido == 'C') {
      Serial.print("Sensor Value: ");
      Serial.print(sensorVal);
      Serial.print(", Volts: ");
      Serial.print(voltage);
      Serial.print(", degrees C: ");
      Serial.println(temp);
    }
    else if (caracterLeido == 'f' || caracterLeido == 'F') {
      Serial.print("Sensor Value: ");
      Serial.print(sensorVal);
      Serial.print(", Volts: ");
      Serial.print(voltage);
      Serial.print(", degrees F: ");
      Serial.println(temp * 1.8 + 32.0);
    }
    else if (caracterLeido == 'M') {
      Serial.print("Max Temperature: ");
      Serial.println(max_temperature);
    }
    else if (caracterLeido == 'm') {
      Serial.print("Min Temperature: ");
      Serial.println(min_temperature);
    }
    else if (caracterLeido == 'R') {
      Serial.println("Reset max and min values");
      EEPROM.put(MAXAddress, temp);
      EEPROM.put(MINAddress, temp);
    }
    else Serial.println("Caracter incorrecto... c o C para celsius y f o F para fahrenheit");
  }
}

void print_temperature()
{
  Serial.println(temp);
}
