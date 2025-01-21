// display.h

void mostrar_estado_botones()
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

void mostrar_estado_joystick()
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

void mostrar_datos_acelerometro()
{
  leer_datos_acelerometro();
  tft.setTextColor(TFT_YELLOW); 
  tft.setCursor(2, 35, 4); tft.print("x: "); tft.fillRect(20,35,90,20,TFT_BLACK); tft.print(X_ACEL);
  tft.setCursor(100, 35, 4); tft.print("y: "); tft.fillRect(120,35,90,20,TFT_BLACK); tft.print(Y_ACEL);
  tft.setCursor(200, 35, 4); tft.print("z: "); tft.fillRect(220,35,90,20,TFT_BLACK); tft.println(Z_ACEL);
}

void mostrar_datos_lux()
{
  // mostrar datos sensor de luz
  int light = analogRead(WIO_LIGHT);
  tft.setCursor(2, 60, 4);
  tft.print("Lux: "); tft.fillRect(50,60,80,20,TFT_BLACK);  tft.println(light);  
}

void mostrar_datos_mic()
{
  // mostrar datos micrófono
  int val = analogRead(WIO_MIC);
  tft.setCursor(2, 85, 4);
  tft.print("Mic: "); tft.fillRect(50,85,80,20,TFT_BLACK);  tft.println(WIO_MIC);  
}


void mostrar_sistema(int borrar_pantalla)
{
  if (borrar_pantalla)
    limpiar_pantalla(true);
  mostrar_estado_botones();
  mostrar_estado_wifi();
  mostrar_datos_acelerometro();
  mostrar_datos_lux();
  mostrar_datos_mic();
  mostrar_estado_joystick();
  delay(100);
}

