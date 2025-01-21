// Wio Terminal

#include <SPI.h>
#include <TFT_eSPI.h>
#include "LIS3DHTR.h"
#include "rpcWiFi.h"
#include <HTTPClient.h>
#include <WebSocketsClient.h>
#include <Seeed_Arduino_FS.h>
#include "Arduino_JSON.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

LIS3DHTR<TwoWire> lis;
TFT_eSPI tft = TFT_eSPI();

#include "definiciones.h"
#include "sistema.h"
#include "acelerometro.h"
#include "pantalla.h"
#include "entradas.h"
#include "filesystem.h"
#include "miwifi.h"
#include "controlantena.h"
#include "controlrover.h"
#include "minichat.h"
#include "tetris.h"
#include "reloj.h"
#include "display.h"

long time1 = 0;
long timedelay1 = 1000;
long time60 = 0;
long timedelay60 = 60000;

void iniciar_serial()
{
  Serial.begin(115200);
}

void iniciar_sistema()
{
  iniciar_serial();
  init_filesystem();
  iniciar_pantalla();
  iniciar_wifi();
  //conectar_wifi(ssid, password);
  iniciar_acelerometro();
  iniciar_botones();
  iniciar_joystick();
  iniciar_sensorluz();
  iniciar_microfono();
  iniciar_buzzer();
  iniciar_control_rover();
}

const int max_menu = 11;

const String menu_0[max_menu] = 
  {"Conectar Internet", "Conectar Rover", "---", "Buscar WiFi", "Mostrar sistema", "Control Rover", "Acelerometro", "Control antena", "Mini Chat", "Tetris", "Reloj"};

int main_menu = 0;
int sub_menu = 0;

void mostrar_menu(int nivel, int pos_y, String titulo)
{
  limpiar_pantalla(false);
  //tft.setTextColor(TFT_YELLOW); 
  //tft.setCursor(10, 10, 4);  tft.print(main_menu); tft.print("  "); tft.println(titulo);  
  tft.setTextColor(TFT_WHITE); 
  tft.setCursor(10, 0, 2);  
  if (WiFi.status() == WL_CONNECTED)
    if (ssid_con = 0)
      { tft.print("Conectado a WiFi: "); tft.println(ssid); }
    else
      { tft.print("Conectado a WiFi: "); tft.println(ssidrover); }
  else
    tft.println("Para conectar WiFi pulse boton A al arrancar");  
  en_menu = 1;
  if (nivel == 0)
    {
    tft.setCursor(10, pos_y, 2); 
    for (int i=0; i< max_menu; i++)
      {
      tft.setCursor(10, pos_y+i*20, 2); 
      if (pos_menu == i)
        tft.setTextColor(TFT_YELLOW); 
      else
        tft.setTextColor(TFT_WHITE); 
      tft.print(i); tft.print(" "); tft.println(menu_0[i]);
      }
    }
}

void cambiar_menu()
{
  if (main_menu == 0)
    mostrar_menu(0, 20, "Menu principal");
}

void ejecutar_accion()
{
  if (main_menu == 0) 
    {    
    if (pos_menu == 0)
      {
      en_menu = 0; 
      ssid_con = 0;
      conectar_wifi(ssid, password);
      mostrar_menu(0, 20, "Menu principal");
      }
    if (pos_menu == 1)
      {
      en_menu = 0; 
      ssid_con = 1;
      conectar_wifi(ssidrover, passwordrover);
      mostrar_menu(0, 20, "Menu principal");
      }
    if (pos_menu == 2)
      {
      en_menu = 0; 
      conectar_wifi(ssid, password);
      mostrar_menu(0, 20, "Menu principal");
      }

    if (pos_menu == 2)
      {
      //en_menu = 0; 

      mostrar_menu(0, 20, "Menu principal");
      }
    else if (pos_menu == 3)
      {
      en_menu = 0; 
      buscar_wifi();
      }
    else if (pos_menu == 4)
      {
      en_menu = 0; 
      mostrar_sistema(true);
      }
    else if (pos_menu == 5)    // ver ficheros
      {
      en_menu = 0; 
      control_rover();
      }
    else if (pos_menu == 6)
      {
      en_menu = 0; 
      mostrar_acelerometro();
      }
    else if (pos_menu == 7)
      {
      en_menu = 0; 
      control_antena();
      }
    else if (pos_menu == 8)
      {
      en_menu = 0; 
      minichat();
      }
    else if (pos_menu == 9)
      {
      en_menu = 0; 
      setup_tetris();
      }
    else if (pos_menu == 10)
      {
      en_menu = 0; 
      actualiza_hora();
      setup_reloj();
      }
    }
}

void setup(void) {
  iniciar_sistema();
  leer_botones();
  init_filesystem();
  if (!BUTTON_A)
    conectar_wifi(ssid, password);
  mostrar_menu(0, 20, "Menu principal");
  /*
  Serial.println(WIO_5S_UP);
  Serial.println(WIO_5S_DOWN);
  Serial.println(WIO_5S_LEFT);
  Serial.println(WIO_5S_RIGHT);
  Serial.println(WIO_5S_PRESS);
  */
}

//====================================================================
void loop_main()
{
  leer_botones();
  if (!BUTTON_C) 
    cambiar_menu();    // volver a menú
  if (en_menu)
    {
    if (!BUTTON_D)
      {
      pos_menu = pos_menu + 1;
      if (pos_menu >= max_menu) pos_menu = 0;
      cambiar_menu();
      }
    if (!BUTTON_U)
      {
      pos_menu = pos_menu - 1;
      if (pos_menu < 0) pos_menu = max_menu-1;
      cambiar_menu();
      }
    if (!BUTTON_P)
      {
      ejecutar_accion();
      }
    }
  else    // se está ejecutando alguna acción
    {
    if (pos_menu == 1)
      {
      loop_wifi();
      }
    if (pos_menu == 2)
      mostrar_sistema(false);
    else if (pos_menu == 4)
      mostrar_datos_acel();     
    else if (pos_menu == 7)
      loop_tetris();     
    else if (pos_menu == 8)
      loop_reloj();     
    }
}

void actualiza_hora()
{
  timeClient.update();
  hh = timeClient.getHours() + 1;
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();
}

void display_hora()
{
  tft.fillRect(260,0,80,20,TFT_BLACK);
  tft.setCursor(260, 2, 2); tft.print(hh); tft.print(":");
  tft.setCursor(280, 2, 2); tft.print(mm); tft.print(":"); 
  tft.setCursor(300, 2, 2); tft.print(ss);
}

void cosas_cada_segundo()
{
  time1 = millis();
  display_hora();
}

void cosas_cada_segundo60()
{
  time60 = millis();
  hh = timeClient.getHours() + 1;
  mm = timeClient.getMinutes();
  ss = timeClient.getSeconds();
  actualiza_hora();
}

void loop() {
  if ((millis() - time1) > timedelay1)
    {
    cosas_cada_segundo();  
    }
  if ((millis() - time60) > timedelay60)
    {
    cosas_cada_segundo60();  
    }
  loop_main();
}

//====================================================================

