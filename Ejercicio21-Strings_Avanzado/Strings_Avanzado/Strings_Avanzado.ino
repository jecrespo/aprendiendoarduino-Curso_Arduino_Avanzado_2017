//leer json de un servidor para tocar música

#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY  //Sustituir YY por el numero de MAC correcto
};

char url_aprendiendoarduino[] = "www.aprendiendoarduino.com";

EthernetClient client;
String webString = "";
String linea = "";
int num_lineas = 0;

void setup() {
  Serial.begin(9600);

  // start the Ethernet connection:
  Serial.println("inicializando red...");

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for (;;)
      ;
  }
  else {
    Serial.print("IP asignada por DHCP: ");
    Serial.println(Ethernet.localIP());
  }

  if (client.connect(url_aprendiendoarduino, 80)) {
    Serial.println("connected");
    client.println("GET //servicios/aprendiendoarduino/string_avanzado.json HTTP/1.1");
    client.println("Host: www.aprendiendoarduino.com");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");
  }
}

void loop() {
  if (client.available()) {
    char c = client.read();
    webString += c;
    linea += c;
    if (c == '\n') {
      num_lineas ++;
      Serial.println("linea "+ (String)num_lineas + " : " + linea);
      linea = ""; //reinicio linea
    }
  }

  if (!client.connected()) {
    //Comienza el análisis de la cadena leida
        
    Serial.println("\n\nTamaño de cadena recibida: " + (String)webString.length());

    //Payload, información recibido quitando cabeceras
    int indice_inicio = webString.indexOf('{');
    int indice_final = webString.lastIndexOf('}');
    webString = webString.substring(indice_inicio, indice_final + 1);
    Serial.println("Tamaño payload: " + (String)webString.length());
    Serial.println(webString);

    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.println("Fin de programa. Reinicia Arduino para volver a hacer una peticion");
    for (;;)
      ;
  }
}
