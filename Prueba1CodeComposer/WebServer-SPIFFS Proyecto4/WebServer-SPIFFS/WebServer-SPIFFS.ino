//Cristopher Sagastume 18640

/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
  https://github.com/luisllamasbinaburo/ESP32-Examples
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//definimos los pines de UART 2
#define RXD0 3
#define TXD0 1
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password Seteamos nuestro Wifi
const char* ssid = "TURBONETT_668E97";  // Enter your SSID here
const char* password = "JFCESG*8485";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)
//variables que nos sevirán para estados de parqueos etc.
char Led[20];
uint8_t LED1pin = 2;
uint8_t segm = 0;
uint8_t count = 4;
bool LED1status = LOW;
int Led2 = 0;
bool Parqueo1 = 0;
bool Parqueo2 = 0;
bool Parqueo3 = 0;
bool Parqueo4 = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200, SERIAL_8N1, RXD0, TXD0);
  //Serial2.begin(115200, SERIAL_8N1, RXD0, TXD0);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  Serial.println("Try Connecting to ");
  Serial.println(ssid);
  //configuramos las salidas para el 7 segmentos
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound([]() {                  // If the client requests any URI
    if (!HandleFileRead(server.uri()))      // send it if it exists
      handleNotFound();             // otherwise, respond with a 404 (Not Found) error
  });
  //server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
  //colocamos en el 7 segmentos un 4 que equivale a 4 parqueos libres
  digitalWrite(23, LOW);//a
  digitalWrite(22, LOW);//b
  digitalWrite(21, HIGH);//c
  digitalWrite(19, LOW);//d
  digitalWrite(18, HIGH);//e
  digitalWrite(5, HIGH);//f
  digitalWrite(15, HIGH);//g
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  
  //se recibio algo por Uart?
  if (Serial.available() > 0) {
    //guardamos el valor recibido en la variable Led y en la variable Led2 obtenemos el largo de la cadena recibida 
    Led2 = Serial.readBytes(Led, 20);
    //verificamos si lo recibido equivale a los datos del parqueo ocupado para restarle 1 al contador y así modificar el 7 segmentos y el web server
    if (Led2 == 2 or Led2 == 4 or Led2 == 6 or Led2 == 8 ) {
      count--;

     
    }//verificamos si lo recibido equivale a los datos del parqueo libre para sumarle 1 al contador y así modificar el 7 segmentos y el web server
    else if ( Led2 == 1 or Led2 == 3 or Led2 == 5 or Led2 == 7 ) {
      count++;
    }
    //en todo este bloque verificamos que dato se recibio para modificar las banderas de estado de parqueo que nos servirá para identificar en el web server que parqueo tuvo un cambio
    if (Led2 == 2) { //parqueo 1 ocupado
      Parqueo1 = 1;


    }
    else if (Led2 == 1) {//parqueo 1 disponible
      Parqueo1 = 0;
    }
    if (Led2 == 4) {//parqueo 2 libre
      Parqueo2 = 1;


    }
    else if (Led2 == 3) {//parqueo 2 ocupado
      Parqueo2 = 0;
    }
    if (Led2 == 6) { //paruqueo 3 libre
      Parqueo3 = 1;


    }
    else if (Led2 == 5) { //parqueo 3 ocupado
      Parqueo3 = 0;
    }
    if (Led2 == 8) { //parqueo 4 libre
      Parqueo4 = 1;


    }
    else if (Led2 == 7) { //parqueo 4 ocupado
      Parqueo4 = 0;
    }
    server.send(200, "text/html", SendHTML(count, Parqueo1, Parqueo2, Parqueo3, Parqueo4)); //enviamos los datos al web server inyectandola por medio de la funcion SendHTML

    //verificamos el valor de count para asignarle un valor al 7 segmentos
    switch (count) {
      case 0:
        digitalWrite(23, HIGH);//a
        digitalWrite(22, HIGH);//b
        digitalWrite(21, HIGH);//c
        digitalWrite(19, HIGH);//d
        digitalWrite(18, HIGH);//e
        digitalWrite(5, HIGH);//f
        digitalWrite(15, LOW);//g
        
        break;
      case 1:
        digitalWrite(23, LOW);//a
        digitalWrite(22, LOW);//b
        digitalWrite(21, LOW);//c
        digitalWrite(19, LOW);//d
        digitalWrite(18, HIGH);//e
        digitalWrite(5, HIGH);//f
        digitalWrite(15, LOW);//g

        break;
      case 2:
        digitalWrite(23, HIGH);//a
        digitalWrite(22, HIGH);//b
        digitalWrite(21, LOW);//c
        digitalWrite(19, HIGH);//d
        digitalWrite(18, HIGH);//e
        digitalWrite(5, LOW);//f
        digitalWrite(15, HIGH);//g

        break;
      case 3:
        digitalWrite(23, HIGH);//a
        digitalWrite(22, LOW);//b
        digitalWrite(21, LOW);//c
        digitalWrite(19, HIGH);//d
        digitalWrite(18, HIGH);//e
        digitalWrite(5, HIGH);//f
        digitalWrite(15, HIGH);//g
        break;
      case 4:
        digitalWrite(23, LOW);//a
        digitalWrite(22, LOW);//b
        digitalWrite(21, HIGH);//c
        digitalWrite(19, LOW);//d
        digitalWrite(18, HIGH);//e
        digitalWrite(5, HIGH);//f
        digitalWrite(15, HIGH);//g

        break;
    }
    delay(400);
  }
}
//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {

  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(count, Parqueo1, Parqueo2, Parqueo3, Parqueo4));
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t count, uint8_t FlagParqueo1, uint8_t FlagParqueo2, uint8_t FlagParqueo3, uint8_t FlagParqueo4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Control de Parqueos</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP32 Web Server &#128664</h1>\n";
  ptr += "<h3>Control de Parqueos</h3>\n";
//verificacion del valor de contador y de las banderas de estado de los parqueos para así asignarlas al web server el estado actual del parqueo y el numero de parqueos disponibles
  if (count == 0)
  {
    ptr += "<h4>Todos los parqueos estan ocupados<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 1 and FlagParqueo1 == 0)
  {
    ptr += "<h4>Existe 1 parqueo disponible<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";

  }
  else if (count == 1 and FlagParqueo2 == 0)
  {
    ptr += "<h4>Existe 1 parqueo disponible<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";

  }
  else if (count == 1 and FlagParqueo3 == 0)
  {
    ptr += "<h4>Existe 1 parqueo disponible<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";

  }
  else if (count == 1 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existe 1 parqueo disponible<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";

  }

  else if (count == 2 and FlagParqueo1 == 0 and FlagParqueo2 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 2 and FlagParqueo1 == 0 and FlagParqueo3 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 2 and FlagParqueo1 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 2 and FlagParqueo2 == 0 and FlagParqueo3 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 2 and FlagParqueo2 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 2 and FlagParqueo3 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 2 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }

  else if (count == 3 and FlagParqueo1 == 0 and FlagParqueo2 == 0 and FlagParqueo3 == 0)
  {
    ptr += "<h4>Existen 3 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 3 and FlagParqueo1 == 0 and FlagParqueo2 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 3 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 3 and FlagParqueo2 == 0 and FlagParqueo3 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 3 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 3 and FlagParqueo1 == 0 and FlagParqueo3 == 0 and FlagParqueo4 == 0)
  {
    ptr += "<h4>Existen 3 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else if (count == 4)
  {
    ptr += "<h4>Existen 4 parqueos disponibles<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "    <td>&#10062;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  else
  {
    ptr += "<h4>Todos los parqueos estan ocupados<h4>\n";
    ptr += "\t\t\t\t<table style=\"width:100%\">\n";
    ptr += "  <tr>\n";
    ptr += "    <th>Parqueo1</th>\n";
    ptr += "    <th>Parqueo2</th>\n";
    ptr += "    <th>Parqueo3</th>\n";
    ptr += "    <th>Parqueo4</th>\n";
    ptr += "  </tr>\n";
    ptr += "  <tr>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "    <td>&#10060;</td>\n";
    ptr += "  </tr>\n";
    ptr += "</table>";
  }
  ptr += "<script>\n";
  ptr += "<!--\n";
  ptr += "function timedRefresh(timeoutPeriod) {\n";
  ptr += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  ptr += "}\n";
  ptr += "\n";
  ptr += "window.onload = timedRefresh(3000);\n";
  ptr += "\n";
  ptr += "//   -->\n";
  ptr += "</script>";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
