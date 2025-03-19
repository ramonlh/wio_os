// pantalla.h

void iniciar_pantalla()
{
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);    
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
  tft.setCursor(0, 0, 2);
}

void limpiar_pantalla(int iniciar_cursor)
{
  tft.fillScreen(TFT_BLACK);    
  if (iniciar_cursor) 
    tft.setCursor(0, 0);
}