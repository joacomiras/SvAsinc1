#include <Arduino.h>
#include <WiFi.h>
#include <AsyncWebServer.h>
#include <AsyncTCP.h>

const char* ssid = "";
const char* password = "";
AsyncWebServer server(80);
const int ldrPin =34;

const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<meta charset="UTF-8">
<title>Monitor de luz ambiental</title>
<style> /* el style siempre lo hago una vez termino el html, así se como nombro cada cosa /*
body{
  background-color: COLOR-FONDO; /* color-fondo como un placeholder */
  text-align: center;
  font-family:'segoe ui';
  }
  .titulo{
  text-align: center;
  font-family:'segoe ui';
  }
  .lectura{
  border: 3px dotted white;
  padding: 25px;
  border-radius:20px;
  }
  .porcentaje{
  font-weight:bold;
  font-size:40px;
  }
</style>
</head>
  <body>
    <h1 class="titulo">Monitor de luz ambiental</h1>
    <div class="lectura">
      <span class="porcentaje"> Nivel de luz: PORCENTAJE% </span>
    </div>
  </body>
</html>
)rawliteral";

String generarPagina(){ 
  int lectura=analogRead(ldrPin);
  int porcentaje=map(lectura,0,4095,0,100);
  String color;
  if (porcentaje>70){
    color="#f1c40f";
  } else if (porcentaje>30 && porcentaje<70){
    color="#e67e22";
  } else {
    color="#2c3e50";
  }
}
  String pagina = pagina_template; //hay casos donde el string este de pagina_template se pone adentro del void handleRootrequest, no sé bien en qué casos va ahí o por separado
pagina.replace("PORCENTAJE",String(porcentaje));
pagina.replace("COLOR-FONDO",color);
return pagina;
 
void setup(){
  Serial.begin(115200);
  Serial.print("Conectando a...");
  serial.println(ssid);
  WiFi.begin(ssid,password);
  int timeout=20;
  while (WiFi.status() != WL_CONNECTED && timeout>0){
    delay(500);
    Serial.print(".");
    timeout--;
  }
  if (WiFi.Status()!WL_CONNECTED){
  }
  Serial.println("\nFallo la conexion. Reiniciando..");
  delay(1000);
  ESP.restart();
  }
  Serial.println("\nConexion WIFI exitosa!");
  Serial.print("IP del servidor: http://");
  serial.println(WiFi.localIP());

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200,"text/html",generarPagina);
});
server.begin();

void loop(){}