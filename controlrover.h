// controlantena.h

String urlForward = "http://192.168.1.91:91/adelante";
String urlBack = "http://192.168.1.91:91/atras";
String urlLeft = "http://192.168.1.91:91/left";
String urlRigth = "http://192.168.1.91:91/rigth";

//WiFiClient httpClient;
//HTTPClient http;

void control_rover()
{
  limpiar_pantalla(true);
  HTTPClient http;
  tft.fillCircle(300, 20, 10, TFT_GREEN);  
  while (BUTTON_C == 1)
    {
    tft.fillCircle(300, 20, 10, TFT_GREEN);  
    leer_botones();
    if (!BUTTON_U)   // adelante
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlForward); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_D)   // atras
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlBack); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_L)   // izquierda
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlLeft); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_R)   // derecha
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlRigth); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    }
}