// minichat.h

String urlSendMsg = "https://dweet.io/dweet/for/minichat?";
String urlLastMsg = "https://dweet.io/get/latest/dweet/for/minichat";
String urlAllMsg = "https://dweet.io/get/dweets/for/minichat";
//String urlChunkMsg = "https://dweet.io/listen/for/dweets/from/minichat";

/**
//void leerMensajes() {
void minichat() {
  HTTPClient http;

  // Construir URL completa
//  String url = dweetURL + thingName;
  String url = urlAllMsg;

  Serial.println("Recuperando mensajes de dweet.io...");
  Serial.println("URL: " + url);

  http.begin(url); // Inicializar HTTPClient
  int httpResponseCode = http.GET(); // Hacer solicitud GET

  if (httpResponseCode > 0) {
    String payload = http.getString(); // Respuesta en formato JSON
    Serial.println("Respuesta de dweet.io recibida:");
    Serial.println(payload);

    // Procesar el JSON
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("Error al analizar JSON: " + String(error.c_str()));
      mostrarError("Error al leer JSON");
      return;
    }

    // Mostrar todos los mensajes en pantalla
    mostrarMensajes(doc["with"]);
  } else {
    Serial.println("Error en la conexión: " + String(httpResponseCode));
    mostrarError("Error de Conexion");
  }

  http.end(); // Terminar la conexión
}

void mostrarMensajes(JsonArray mensajes) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.println("Mensajes de minichat:");

  int y = 40; // Posición inicial para los mensajes
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  for (JsonObject mensaje : mensajes) {
    JsonObject content = mensaje["content"];
    String fecha = mensaje["created"];

    // Mostrar fecha
    tft.setCursor(10, y);
    tft.println("Fecha:");
    tft.setCursor(20, y + 20);
    tft.println(fecha);
    y += 40;

    // Mostrar cada clave-valor del contenido
    for (JsonPair kv : content) {
      tft.setCursor(10, y);
      tft.println(String(kv.key().c_str()) + ": " + String(kv.value().as<String>()));
      y += 20;

      // Si la pantalla se llena, pausar y limpiar
      if (y > 220) {
        tft.setCursor(10, y);
        tft.println("Presione reset para mas.");
        delay(5000);
        tft.fillScreen(TFT_BLACK);
        y = 40;
      }
    }
    y += 20; // Espacio extra entre mensajes
  }
}

void mostrarError(String mensaje) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 20);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.println("ERROR:");
  tft.setCursor(10, 50);
  tft.println(mensaje);
  delay(5000);
}

***/
void minichat()
{
  limpiar_pantalla(true);
  tft.println("MINI CHAT");
  if((WiFi.status() == WL_CONNECTED)) 
    {
    HTTPClient http;
//    http.begin(urlLastMsg); //HTTP
    http.begin(urlAllMsg); //HTTP
    int httpCode = http.GET();
    if(httpCode > 0) 
      {
      //tft.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) 
        {
        String payload = http.getString();
        JSONVar myObject = JSON.parse(payload);
        String datetime = myObject["with"][0]["created"]; tft.print(": ");
        tft.println(myObject["with"][0]["content"]["msg"]);
      } 
    else 
      {
      tft.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    http.end();
    }
  }
}