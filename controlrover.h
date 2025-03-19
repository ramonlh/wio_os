

#include "cmath"
#include <ArduinoJson.h>


WebSocketsClient webSocket;
int wsstatus = 0;

float t = 0;
float h = 0;
float a = 0;
float e = 0;
float g = 0;
int dUS1 = 0;      

void show_botones()
{
  // mostrar estado botones
  tft.drawRect(1, 1, 180, 30, TFT_WHITE);
  tft.setCursor(155, 5, 4);
  tft.setTextColor(BUTTON_A?TFT_GREY:TFT_YELLOW); tft.println("A"); 
  tft.setCursor(80, 5, 4);
  tft.setTextColor(BUTTON_B?TFT_GREY:TFT_YELLOW); tft.println("B"); 
  tft.setCursor(10, 5, 4);
  tft.setTextColor(BUTTON_C?TFT_GREY:TFT_YELLOW); tft.println("C"); 
}

void show_joystick()
{
  // mostrar estado joystick
  tft.drawCircle(260, 180, 50, TFT_WHITE);  
  tft.setCursor(250, 135, 4); 
  tft.setTextColor(BUTTON_U?TFT_GREY:TFT_YELLOW); tft.println("O"); 
  tft.setCursor(250, 205, 4);  
  tft.setTextColor(BUTTON_D?TFT_GREY:TFT_YELLOW); tft.println("O"); 
  tft.setCursor(215, 170, 4);  
  tft.setTextColor(BUTTON_L?TFT_GREY:TFT_YELLOW); tft.println("O"); 
  tft.setCursor(285, 170, 4);  
  tft.setTextColor(BUTTON_R?TFT_GREY:TFT_YELLOW); tft.println("O"); 
  tft.setCursor(250, 170, 4);  
  tft.setTextColor(BUTTON_P?TFT_GREY:TFT_YELLOW); tft.println("O"); 
}

void show_acel()
{
  tft.setTextColor(TFT_YELLOW); 
  tft.setCursor(3, 200, 4); tft.print("x: "); tft.fillRect(20,200,90,20,TFT_BLACK); tft.print(X_ACEL);
  tft.setCursor(100, 200, 4); tft.print("y: "); tft.fillRect(120,200,90,20,TFT_BLACK); tft.print(Y_ACEL);
}

void show_data_sensors()
{
  tft.setTextColor(TFT_YELLOW); 
  tft.setCursor(3, 1, 2); tft.print("temp: "); tft.fillRect(40,1,90,20,TFT_BLACK); tft.print(t);
  tft.setCursor(90, 1, 2); tft.print("hum: "); tft.fillRect(113,1,90,20,TFT_BLACK); tft.print(h);

  tft.setCursor(3, 30, 2); tft.print("rumbo: "); tft.fillRect(43,30,90,20,TFT_BLACK); tft.print(a);
  tft.setCursor(110,30, 2); tft.print("balanceo: "); tft.fillRect(173,30,90,20,TFT_BLACK); tft.print(e);
  tft.setCursor(220, 30, 2); tft.print("alabeo: "); tft.fillRect(265,30,90,20,TFT_BLACK); tft.print(g);
  
  tft.setCursor(3, 60, 2); tft.print("distancia: "); tft.fillRect(63,60,90,20,TFT_BLACK); tft.print(dUS1);
}

StaticJsonDocument<200> doc;
DeserializationError dserialerror;  

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      wsstatus = 0;
      Serial.println("Desconectado del servidor WebSocket");
      break;
    case WStype_CONNECTED:
      Serial.println("Conectado al servidor WebSocket");
      wsstatus = 1;
      break;
    case WStype_TEXT:
      Serial.print("Mensaje recibido: ");
      Serial.println((char*)payload);
      // Crear el documento JSON (ajusta el tamaño según necesidad)
      dserialerror = deserializeJson(doc, payload);    // Analizar el JSON
      // Verificar si hubo un error
      if (dserialerror) {
        Serial.print("Error JSON: ");  Serial.println(dserialerror.f_str());
        break;
        }
      // Extraer los valores
      t = doc["t"];
      h = doc["h"];
      a = doc["a"];
      e = doc["e"];
      g = doc["g"];
      dUS1 = doc["dUS1"];    
      show_data_sensors();
      break;
    case WStype_ERROR:
      wsstatus = 0;
      Serial.println("Error en la conexión WebSocket");
      break;
  }
}

void sendWebSocketCommand(const char* command) {
  if (webSocket.isConnected()) {
    webSocket.sendTXT(command);
    } 
  else {
    Serial.println("WebSocket desconectado");
    }
}

// Definir constantes de dirección
#define MFORWARD 1
#define MBACK 0

// Declaración de la función WebSocket
void sendWebSocketCommand(const char* command);

int rover_parado = 1;
bool pausado = true;

// Inicializar direcciones de las ruedas
int d1 = 0, d2 = 0, d3 = 0, d4 = 0;  // Direcciones de las ruedas
int s1 = 0, s2 = 0, s3 = 0, s4 = 0;  // Velocidades de las ruedas

void set_dir(int r1, int r2, int r3, int r4)
{
  d1 = r1;  // Dirección de la rueda 1  Trasera Dcha
  d2 = r2;  // Dirección de la rueda 2  Trasera Izda
  d3 = r3;  // Dirección de la rueda 3  Delantera Dcha
  d4 = r4;  // Dirección de la rueda 4  Delantera Izda
}

void set_speed(int v1, int v2, int v3, int v4)
{
  s1 = v1;  // Dirección de la rueda 1  Dcha
  s2 = v2;  // Dirección de la rueda 2  Izda
  s3 = v3;  // Dirección de la rueda 3  Dcha
  s4 = v4;  // Dirección de la rueda 4  Izda
}

//////////////////////////////////////////////////////////////////////
void control_rover(float X_V, float Y_V) {
    // Definir un umbral para evitar movimientos con ruido
    const float umbral = 0.15;
    if ((X_V < umbral) && (X_V > -umbral)) X_V = 0;
    if ((Y_V < umbral) && (Y_V > -umbral)) Y_V = 0;
    if ((X_V == 0) && (Y_V == 0)) {  
      if (rover_parado == 0) {
        sendWebSocketCommand("{\"d1\":0,\"d2\":0,\"d3\":0,\"d4\":0,\"s1\":0,\"s2\":0,\"s3\":0,\"s4\":0}");
        rover_parado = 1;
        return;  // Si no hay movimiento, detenemos el rover
        }
      }
    int max_speed = 4000;     // Mapear la magnitud a una velocidad en el rango 0-4000
    // Movimiento adelante/atrás en el eje X
    int speed_x = (int)(fabs(X_V) * max_speed);  // Velocidad en el eje X (adelante/atrás)
    int speed_y = (int)(fabs(Y_V) * max_speed);  // Velocidad en el eje Y (lateral)
    if (X_V < 0)    // hacia adelante
      {
      set_dir(MFORWARD, MFORWARD, MFORWARD, MFORWARD);
      if (Y_V == 0.0) {  // Sin movimiento lateral, sólo adelante
        set_speed(speed_x, speed_x, speed_x, speed_x);
        }
      else {          // hacia adelante con componente lateral
        if (Y_V > 0) {   // hacia la derecha
          set_speed(speed_x, speed_y/2, speed_x, speed_y/2);
          }
        else {            // hacia la izquierda
          set_speed(speed_y/2, speed_x, speed_y/2, speed_x);
          }
        }
      }
    else {      // hacia atrás
      set_dir(MBACK, MBACK, MBACK, MBACK);
      if (Y_V == 0.0) {  // Sin movimiento lateral, sóloatrás
        set_speed(speed_x, speed_x, speed_x, speed_x);
        }
      else {          // hacia atrás con componente lateral
        if (Y_V > 0) {
          set_speed(speed_x, speed_y/2, speed_x, speed_y/2);
          }
        else  {
          set_speed(speed_y/2, speed_x, speed_y/2, speed_x);
          }
        }
      }
    // Enviar comando por WebSocket
    char command[128];
    snprintf(command, sizeof(command), 
        "{\"d1\":%d,\"d2\":%d,\"d3\":%d,\"d4\":%d,\"s1\":%d,\"s2\":%d,\"s3\":%d,\"s4\":%d}", 
        d1, d2, d3, d4, abs(s1), abs(s2), abs(s3), abs(s4));
    Serial.print("Enviar: ");  Serial.println(command);
    sendWebSocketCommand(command);  // Enviar el comando por WebSocket
}

///////////////////////////////////////////////////////////////////////////

void control_coche()
{
  // Configuración del servidor WebSocket
  const char* websocket_server = "192.168.11.91";
  const int websocket_port = 81;
  const char* websocket_path = "/";  // Ruta del WebSocket, puede ser "/"

  const char* websocket_rover = "192.168.4.1";

  limpiar_pantalla(true);
  // Configurar el cliente WebSocket
  webSocket.onEvent(webSocketEvent);
  if (ssid_con == 0) 
    {
    Serial.println(websocket_server);
    webSocket.begin(websocket_server, websocket_port, websocket_path);   // WiFi a router casa
    }
  else
    {
    Serial.println(websocket_rover);
    webSocket.begin(websocket_rover, websocket_port, websocket_path);   // WiFi a rover directo
    }
  tft.setTextColor(TFT_YELLOW); 
  tft.setCursor(20, 100, 4); tft.print("PAUSE");
  tft.setCursor(20, 140, 4); tft.print("Pulse P");

  while (BUTTON_A == 1)
    {
    leer_botones();    //limpiar_pantalla(true);
    if (BUTTON_P == 0) 
      {
      pausado = !pausado;
      if (pausado) {
        sendWebSocketCommand("stop");
        tft.setTextColor(TFT_YELLOW); 
        sendWebSocketCommand("stop");
        }
      else {
        tft.setTextColor(TFT_BLACK); 
        }
      tft.setCursor(20, 100, 4); tft.print("PAUSE");
      tft.setCursor(20, 140, 4); tft.print("Pulse P");
      }
    else if (BUTTON_L == 0) { 
      Serial.println("cam IZDA"); 
      sendWebSocketCommand("caml"); }
    else if (BUTTON_R == 0) { 
      Serial.println("cam RIGTH"); 
      sendWebSocketCommand("camr"); }
    else if (BUTTON_U == 0) { 
      Serial.println("cam UP"); 
      sendWebSocketCommand("camup"); }
    else if (BUTTON_D == 0) { 
      Serial.println("cam DOWN"); 
      sendWebSocketCommand("camdn"); }
    mostrar_estado_wifi();
    //show_botones();
    //show_data_sensors();
    show_joystick();
    leer_datos_acelerometro();
    show_acel();

    // Mantener la conexión WebSocket activa
    webSocket.loop();

    if (!pausado)
      {
      control_rover(X_ACEL, Y_ACEL);
      }
    //delay(1);
    }
  sendWebSocketCommand("stop");
}
