/*
  Menu interactivo
*/
float voltios;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  String titulo = "Menu Interactivo con Arduino";
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.print("\n" + titulo + "\n");
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.println();
}

void loop() {
  int respuesta;
  do {
    respuesta = menu();
    switch (respuesta) {
      case 1:
        Serial.println("----------------> Led Encendido");
        digitalWrite(9, HIGH);
        break;
      case 2:
        Serial.println("----------------> Led Apagado");
        digitalWrite(9, LOW);
        break;
      case 3:
        voltios = analogRead(A0) * 5.0 / 1023;
        Serial.println("----------------> En el pin 0 he leido " + (String)voltios + "V");
        break;
      case 4:
        Serial.println("Adios...");
        while (true) {
          //se queda aqui el programa
        }
        break;
      default:
        Serial.println("Opcion de menu incorrecta!!!!!");
        break;
    }
    //limpia_terminal();ç
    Serial.println();
  } while ((respuesta < 1) || (respuesta > 4));
}

int menu() {
  String leido = "";

  Serial.println("Elige opcion a ejecutar:");
  Serial.println("1 - Enciende Led en Pin 9");
  Serial.println("2 - Apaga Led en Pin 9");
  Serial.println("3 - Leer Voltaje en pin A0");
  Serial.println("4 - Fin");

  while (Serial.available() == 0) {
    //Ho hago nada
  }

  do {
    char caracter_leido;
    caracter_leido = Serial.read();
    leido += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);

  if (leido.toInt() != 0)
    return leido.toInt();
  else return 0;
}

void limpia_terminal() {		//Solo valido para usar con putty
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}
