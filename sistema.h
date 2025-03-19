// sistema.h

int en_menu = 0;
int pos_menu = 0;

void iniciar_botones()
{
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
}

void iniciar_joystick()
{
  pinMode(WIO_5S_UP, INPUT_PULLUP);     // 31
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);   // 34
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);   // 32
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);  // 33
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);  // 35
}

void iniciar_sensorluz()
{
  pinMode(WIO_LIGHT, INPUT);    
  tft.fillScreen(TFT_BLACK);  
}

void iniciar_microfono()
{
  pinMode(WIO_MIC, INPUT);
}

void iniciar_buzzer()
{
  pinMode(WIO_BUZZER, OUTPUT);
}

void leer_botones()
{
  BUTTON_A = digitalRead(WIO_KEY_A);
  BUTTON_B = digitalRead(WIO_KEY_B);
  BUTTON_C = digitalRead(WIO_KEY_C);
  BUTTON_U = digitalRead(WIO_5S_UP);
  BUTTON_D = digitalRead(WIO_5S_DOWN);
  BUTTON_L = digitalRead(WIO_5S_LEFT);
  BUTTON_R = digitalRead(WIO_5S_RIGHT);
  BUTTON_P = digitalRead(WIO_5S_PRESS);
  if (pos_menu != 7)
    if ((!BUTTON_A) || (!BUTTON_B) || (!BUTTON_C) || (!BUTTON_U) || (!BUTTON_D) || (!BUTTON_L) || (!BUTTON_R) || (!BUTTON_P))
      delay(200);
}

