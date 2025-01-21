// controlantena.h

String urlForward = "http://192.168.1.91:91/adelante";
String urlBack = "http://192.168.1.91:91/atras";
String urlLeft = "http://192.168.1.91:91/giroizquierda";
String urlRigth = "http://192.168.1.91:91/giroderecha";

WiFiClient tcpclient;

const uint16_t tcpserver_port = 92;            // Puerto del servidor tcp

//const char* host = "192.168.1.91"; // Dirección IP del servidor
const char* host = "192.168.4.1"; // Dirección IP del servidor
const uint16_t port = 8080;

void send_rover_order(String comando)
{
 if (tcpclient.connected()) {
    String message = comando;  // Mensaje a enviar
    tcpclient.println(message);  // Enviar el mensaje
     // Serial.println("Enviado: " + message);

    // Leer la respuesta del servidor
    if (tcpclient.available()) {
      String response = tcpclient.readStringUntil('\n');
      Serial.println("Respuesta del servidor: " + response);
      }
    } 
  else {
    Serial.println("Desconectado. Intentando reconectar...");
    if (tcpclient.connect(host, port)) {
      Serial.println("Reconexión exitosa.");
    } else {
      Serial.println("Error al reconectar.");
    }
  }
}

void handle_tcpserver()
{
 if (tcpclient.connected()) {

    String message = "adelante";  // Mensaje a enviar
    tcpclient.println(message);  // Enviar el mensaje
    Serial.println("Enviado: " + message);

    // Leer la respuesta del servidor
    if (tcpclient.available()) {
      String response = tcpclient.readStringUntil('\n');
      Serial.println("Respuesta del servidor: " + response);
    }
    delay(2000);  // Esperar antes de enviar el próximo mensaje
    } 
  else {
    Serial.println("Desconectado. Intentando reconectar...");
    if (tcpclient.connect(host, port)) {
      Serial.println("Reconexión exitosa.");
    } else {
      Serial.println("Error al reconectar.");
      delay(5000);
    }
  }
}

void iniciar_control_rover()
{
  // Intentar conectar al servidor tcpserver
  if (!tcpclient.connect(host, port)) {
    Serial.println("Error al conectar con el servidor.");
  } else {
    Serial.println("Conexión establecida con el servidor.");
  }

}

void control_rover()
{
  limpiar_pantalla(true);
  iniciar_control_rover();
  HTTPClient http;
  tft.setTextSize(2);
  tft.setCursor(60, 5);
  tft.print("CONTROL ROVER");

  while (BUTTON_C == 1)
    {
    //handle_tcpserver();
    leer_botones();
    if (!BUTTON_U)   // adelante
      {
      tft.setCursor(100, 50);
      tft.fillRect(0,50,300,180,TFT_BLACK);
      tft.print("ADELANTE");
      send_rover_order("adelante");
      }
    if (!BUTTON_D)   // atras
      {
      tft.setCursor(100, 200);
      tft.fillRect(0,50,300,180,TFT_BLACK);
      tft.print("ATRAS");
      send_rover_order("atras");
      }
    if (!BUTTON_L)   // izquierda
      {
      tft.setCursor(10, 125);
      tft.fillRect(0,50,300,180,TFT_BLACK);
      tft.print("IZQUIERDA");
      send_rover_order("giroizda");
      }
    if (!BUTTON_R)   // derecha
      {
      tft.setCursor(170, 125);
      tft.fillRect(0,50,200,180,TFT_BLACK);
      tft.print("DERECHA");
      send_rover_order("girodcha");
      }
    if (!BUTTON_P)   // pulsar
      {
      tft.setCursor(120, 125);
      tft.fillRect(0,50,300,180,TFT_BLACK);
      tft.print("STOP");
      send_rover_order("stop");
      }
    if (!BUTTON_B)   // baja velocidad
      {
      tft.setCursor(10, 125);
      tft.fillRect(0,50,300,180,TFT_BLACK);
      tft.print("VEL-");
      send_rover_order("bajavelocidad");
      }
    if (!BUTTON_A)   // sube velocidad
      {
      tft.setCursor(170, 125);
      tft.fillRect(0,50,200,180,TFT_BLACK);
      tft.print("VEL+");
      send_rover_order("subevelocidad");
      }
    }
  tcpclient.stop();
}