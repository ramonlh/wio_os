// controlantena.h

String urlArriba = "http://192.168.1.91:91/arriba";
String urlAbajo = "http://192.168.1.91:91/abajo";
String urlIzquierda = "http://192.168.1.91:91/giroizda";
String urlDerecha = "http://192.168.1.91:91/girodcha";
String urlIraorigen = "http://192.168.1.91:91/iraorigen";
String urlReset = "http://192.168.1.91:91/reset";
String urlBasicdata = "http://192.168.1.91:91/basicdata";

long satid;
String satname, satdate, sattime, sattle;
float sataz, satel, antaz, antel;

WiFiClient httpClient;
HTTPClient http;

void leer_datos_antena()
{
  if (http.begin(httpClient, urlBasicdata)) //HTTP
    {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) 
      {
      String payload = http.getString();
      JSONVar myObject = JSON.parse(payload);
      satid = long(myObject["satid"]);
      String auxS0 = (myObject[myObject.keys()[0]]);    // date
      String auxS1 = (myObject[myObject.keys()[1]]);    // time
      String auxS7 = (myObject[myObject.keys()[7]]);    // name
      String auxS8 = (myObject[myObject.keys()[8]]);    // tle
      satdate = auxS0;
      sattime = auxS1;
      sattle = auxS8;
      satname = auxS7;
      sataz = double(myObject["sataz"]);
      satel = double(myObject["satel"]);
      antaz = double(myObject["antaz"]);
      antel = double(myObject["antel"]);
      } 
    else 
      {
      tft.print("GET failed:"); tft.println(httpCode);
      }
    http.end();
    }
}

void control_antena()
{
  limpiar_pantalla(true);
  tft.println("CONTROL ANTENA");
  leer_datos_antena();
  tft.print(" Satelite "); 
  tft.print(" ID: "); tft.print(satid);
  tft.print(" Nombre: "); tft.println(satname);
  tft.print(" Fecha: "); tft.print(satdate);
  tft.print("  Hora: "); tft.println(sattime);
  tft.setCursor(0, 60, 4);
  tft.print("SAT Az.: "); tft.print(sataz);
  tft.print(" El.: "); tft.println(satel);
  tft.print("ANT Az.: "); tft.print(antaz);
  tft.print("  El.: "); tft.println(antel);
  delay(200);
  HTTPClient http;
  tft.fillCircle(300, 20, 10, TFT_GREEN);  
  while (BUTTON_C == 1)
    {
    tft.fillCircle(300, 20, 10, TFT_GREEN);  
    leer_botones();
    if (!BUTTON_U)   // arriba
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlArriba); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_D)   // abajo
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlAbajo); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_L)   // izquierda
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlIzquierda); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_R)   // derecha
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlDerecha); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_P)   // Press
      {
      tft.fillCircle(300, 20, 10, TFT_RED);  
      http.begin(urlIraorigen); //HTTP
      int httpCode = http.GET();
      http.end();
      }
    if (!BUTTON_B)   // boton B
      {
      if (!BUTTON_A)
        {
        tft.fillCircle(300, 20, 10, TFT_RED);  
        http.begin(urlReset); //HTTP
        int httpCode = http.GET();
        http.end();
        }
      }
    }
}