#define PIN_SERVO 9

#include<Wire.h>
#include <Servo.h>

// Ratios de conversion especificados en la documentacion
// Deberemos dividir los valores que nos da el Giroscopio y el
// Acelerometro entre estas constantes para obtener un valor
// coherente. RAD_A_DEG es la conversion de radianes a grados.
#define A_R 16384.0  // aceleracion
#define G_R 131.0    // giroscopo

//Conversion de radianes a grados 180/PI
#define RAD_A_DEG = 57.295779

//MPU-6050 da los valores en enteros de 16 bits

const int MPU_addr = 0x68; // I2C address of the MPU-6050

//Valores sin refinar
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//Angulos
float Acc[2];
float Gy[2];
float Angle[2] = {0, 0};

Servo myservo;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  myservo.attach(PIN_SERVO);

  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //Se calculan los ngulos Y, X respectivamente.
  Acc[1] = atan(-1 * (AcX / A_R) / sqrt(pow((AcY / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;
  Acc[0] = atan((AcY / A_R) / sqrt(pow((AcX / A_R), 2) + pow((AcZ / A_R), 2))) * RAD_TO_DEG;

  Serial.println("Angulo Acelerometro X: " + String(Acc[0]));
  Serial.println("Angulo Acelerometro Y: " + String(Acc[1]));

  //Indico posicion al servo
  myservo.write(map(Acc[0], -90, 90, 0, 180));
  Serial.println("Angulo servo: " + String(map(Acc[0], -90, 90, 0, 180)));

  delay(500);
}
