/*
  Web impresa:
  <!DOCTYPE html><html>
  <head><title>LOGIN</title>
  <style>.auto-width {display: inline-block; width:50%;}</style></head>
  <body align="center">
  <form name="login" action="http://192.168.1.179/arduino" method="post">
  <fieldset class="auto-width">
  <legend>ARDUINO LOGIN</legend><br>
  User:<br><input type="text" name="user" value=><br><br>
  Password:<br><input type="password" name="password" value=><br><br>
  <input type="submit" value="Send">
  </fieldset>
  </form>
  <h3 align="center"><font size="1">(c)www.aprendiendoarduino.com</font></h3>
  </body>
  </html>
*/

#include <SPI.h>
#include <Ethernet.h>

#define USUARIO "admin"
#define PASSWORD "aprendiendoarduino"

//Configuración Ethernet
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY};  //Sustituir YY por el numero de MAC correcto

EthernetServer server = EthernetServer(80);

boolean led_state = LOW;

void setup()
{
  Serial.begin(9600);
  //Inicializar Ethernet
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

  //Comienzo a escuchar cliente ethernet
  server.begin();
}

void loop()
{
  String recibido = ""; //Buffer

  //Si hay in cliente entrante, hay caracteres por leer
  EthernetClient client = server.available();
  if (client) {
    while (client.available()) {
      recibido += char(client.read());
      //Serial.println(client.read());  //Para hacer debug
    }
    Serial.println("He recibido por ethernet: ");
    Serial.println(recibido);

    if (recibido.startsWith("GET / HTTP/1.1")) {
      Serial.println("------> Muestro web");
      muestraLogin(client);
    }
    else if (recibido.startsWith("POST /arduino HTTP/1.1")) {
      Serial.println("------> Compruebo usuario y contraseña");
      String usuario_recibido = recibido.substring(recibido.indexOf("user=") + 5, recibido.indexOf("&password="));
      String password_recibido = recibido.substring(recibido.indexOf("&password=") + 10);
      Serial.println("usuario = " + usuario_recibido);
      Serial.println("password = " + password_recibido);
      if ((usuario_recibido == USUARIO) && (password_recibido == PASSWORD)) {
        muestraWeb(client);
        Serial.println("Acceso Correcto");
      }
      else {
        muestraLogin(client);
        Serial.println("Acceso Incorrecto");
      }
    }
    else {
      client.println("HTTP/1.0 200K");
      client.println();
      client.println("ERROR!!!");
      client.stop();
    }
  }
}

void muestraWeb(EthernetClient client) {
  client.println("HTTP/1.0 200K");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<head><style>table, th, td {border: 1px solid black;}</style></head>");
  client.println("<body>");
  client.println("<table style=\"width:auto\">");
  client.println("<tr><th>Pin</th><th>Valor</th></tr>");
  for (int i = 0; i < 6 ; i++) {
    client.println("<tr><td>A" + (String)i + "</td><td>" + (String)analogRead(i) + "</td></tr>");
  }
  client.println("</table>");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
}

void muestraLogin(EthernetClient client) {
  client.println("HTTP/1.0 200K");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<head><title>LOGIN</title>");
  client.println("<style>.auto-width {display: inline-block; width:50%;}</style></head>");
  client.println("<body align=\"center\">");
  client.print("<form name=\"login\" action=\"http://");
  client.print(Ethernet.localIP());
  client.print("/arduino\" method=\"post\">");
  client.println("<fieldset class=\"auto-width\">");
  client.println("<legend>ARDUINO LOGIN</legend><br>");
  client.println("User:<br><input type=\"text\" name=\"user\" value=><br><br>");
  client.println("Password:<br><input type=\"password\" name=\"password\" value=><br><br>");
  client.println("<input type=\"submit\" value=\"Send\">");
  client.println("</fieldset>");
  client.println("</form>");
  client.println("<h3 align=\"center\"><font size=\"1\">(c)www.aprendiendoarduino.com</font></h3>");
  client.println("</body></html>");
  client.stop();
}

