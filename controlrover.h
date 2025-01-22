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
      Serial.println("Respuesta del rover: " + response);
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

String tempS;
String humS;

void handle_tcpserver()
{
 if (tcpclient.connected()) 
    {
    // Leer mensajes del servidor
    if (tcpclient.available()) {
      String response = tcpclient.readStringUntil('\n');
      Serial.println("Mensaje del rover: " + response);
      if (response.charAt(0) == 't')
        {
          response.remove(0,1);
          tempS = response;
          Serial.print("temp."); Serial.println(tempS);
        }
      else if (response.charAt(0) == 'h')
        {
          response.remove(0,1);
          humS = response;
          Serial.print("hum."); Serial.println(humS);
        }
      tft.setTextSize(2);
      tft.setCursor(10,210);
      tft.print("T:"); tft.print(tempS); tft.print(" C");
      tft.setCursor(160,210);
      tft.print("H:");tft.print(humS); tft.print(" %");
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


void control_rover()
{
  if (WiFi.status() != WL_CONNECTED)
    {
    conectar_wifi(ssidrover, passwordrover);
    }
  limpiar_pantalla(true);
  tft.setTextSize(2);
  tft.setCursor(60, 5);
  tft.print("CONTROL ROVER");

  // Intentar conectar al servidor tcserver
  if (!tcpclient.connect(host, port)) {
    Serial.println("Error al conectar con el servidor.");
    } 
  else {
    Serial.println("Conexión establecida con el servidor.");
    }

  while (BUTTON_C == 1)
    {
    handle_tcpserver();
    leer_botones();
    if (!BUTTON_U)   // adelante
      {
      tft.setCursor(100, 50);
      tft.fillRect(0,50,300,150,TFT_BLACK);
      tft.print("ADELANTE");
      send_rover_order("adelante");
      }
    if (!BUTTON_D)   // atras
      {
      tft.setCursor(100, 130);
      tft.fillRect(0,50,300,150,TFT_BLACK);
      tft.print("ATRAS");
      send_rover_order("atras");
      }
    if (!BUTTON_L)   // izquierda
      {
      tft.setCursor(10, 90);
      tft.fillRect(0,50,300,150,TFT_BLACK);
      tft.print("IZQUIERDA");
      send_rover_order("giroizda");
      }
    if (!BUTTON_R)   // derecha
      {
      tft.setCursor(170, 90);
      tft.fillRect(0,50,200,150,TFT_BLACK);
      tft.print("DERECHA");
      send_rover_order("girodcha");
      }
    if (!BUTTON_P)   // pulsar
      {
      tft.setCursor(120, 90);
      tft.fillRect(0,50,300,150,TFT_BLACK);
      tft.print("STOP");
      send_rover_order("stop");
      }
    if (!BUTTON_B)   // baja velocidad
      {
      tft.setCursor(10, 90);
      tft.fillRect(0,50,300,150,TFT_BLACK);
      tft.print("VEL-");
      send_rover_order("bajavelocidad");
      }
    if (!BUTTON_A)   // sube velocidad
      {
      tft.setCursor(170, 90);
      tft.fillRect(0,50,200,150,TFT_BLACK);
      tft.print("VEL+");
      send_rover_order("subevelocidad");
      }
    }
  tcpclient.stop();
}