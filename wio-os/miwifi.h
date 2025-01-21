
int ssid_con=0;
char ssid[30] = "MOVISTAR_EDE0";
char password[20] = "Trujillo31";
char ssidrover[30] = "roverdiego";
char passwordrover[20] = "12341234";

int en_menu_wifi = 0;
int pos_menu_wifi = 0;
int main_menu_wifi = 0;
const int max_menu_wifi = 10;

String menu_wifi[max_menu_wifi] =  {"", "", "", "", "", "", "", "", "", ""};

void leer_wifi_file(fs::FS& fs, const char* path) 
{
  File file = fs.open(path);
  if (file) 
    {
    Serial.println("Leyendo fichero datos WiFi");
    }
  else
    {
    Serial.println("Failed to open file for reading");
    return;
    }
  String auxS = file.readString();
  String auxSSID = auxS.substring(0,auxS.indexOf("\n")-1);
  String auxPASS = auxS.substring(auxS.indexOf("\n")+1,auxS.length()-2);
  String auxSSIDrover = auxS.substring(0,auxS.indexOf("\n")-1);
  String auxPASSrover = auxS.substring(auxS.indexOf("\n")+1,auxS.length()-2);
  file.close();
  auxSSID="MOVISTAR_EDE0";
  auxPASS="Trujillo31";
  auxSSIDrover="roverdiego";
  auxPASSrover="12341234";

  auxSSID.toCharArray(ssid,30);
  auxPASS.toCharArray(password,20);
  auxSSIDrover.toCharArray(ssidrover,30);
  auxPASSrover.toCharArray(passwordrover,20);
  Serial.print("SSID: ");Serial.println(ssid);
  Serial.print("PASS: ");Serial.println(password);
}

void guardar_wifi()
{
    File RootWrite = SD.open("/wifi.txt", FILE_WRITE);
    if (RootWrite) 
      {
      RootWrite.println(ssid);
      RootWrite.println(password);
      RootWrite.close();
      }
}

void iniciar_wifi()
{
  leer_wifi_file(SD, "/wifi.txt");
  WiFi.mode(WIFI_STA);
}

void conectar_wifi(char* ssid, char* password)
{
  limpiar_pantalla(true);
  tft.print("Conectando a ");  tft.println(ssid);
  tft.print("con password ");  tft.println(password);
  tft.println("WiFi.begin");
  WiFi.begin(ssid, password);
  int retry = 10;
  while ((WiFi.status() != WL_CONNECTED) && (retry>0))
    {
    retry--;
    tft.print(".");  
    WiFi.begin("MOVISTAR_EDE0", "Trujillo31");
    }
  if (WiFi.status() == WL_CONNECTED)
    {
    timeClient.begin();
    timeClient.update();
    }
}

void mostrar_estado_wifi()
{
  // mostrar estado WiFi
  tft.setCursor(185, 1, 2); 
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(2);
  if (WiFi.status() == WL_CONNECTED)
    {
    //tft.println(ssid); 
    tft.setCursor(185, 15, 2); 
    tft.println(WiFi.localIP());
    }
  else
    {
    tft.println("NO CONECTADO"); 
    }
}

void mostrar_menu_wifi(int nivel, int pos_y, String titulo)
{
  limpiar_pantalla(false);
  tft.setTextColor(TFT_YELLOW); 
  tft.setCursor(10, 10, 4); tft.println(titulo);  
  tft.setTextColor(TFT_WHITE); 
  tft.setCursor(10, 40, 2);  
  en_menu_wifi = 1;
  tft.setCursor(10, pos_y, 2); 
  for (int i=0; i< max_menu_wifi; i++)
    {
    tft.setCursor(10, pos_y+i*20, 2); 
    if (pos_menu_wifi == i)
      tft.setTextColor(TFT_YELLOW); 
    else
      tft.setTextColor(TFT_WHITE); 
    tft.print(i); tft.print(" "); tft.println(menu_wifi[i]);
    }
}

void buscar_wifi()
{
    limpiar_pantalla(false);
    tft.setCursor(1, 1, 2); 
    tft.println("Buscando redes WiFi ...");
    int n = WiFi.scanNetworks();
    for (int i=0; i<max_menu_wifi; i++)
      {
      menu_wifi[i] = String(WiFi.SSID(i)) + String(" ") + String (WiFi.RSSI(i)) + String("dbm ") + String((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?"Abierta":"*");
      }
    mostrar_menu_wifi(1, 40, "Seleccionar red WiFi");
}

void loop_wifi()
{
    if (!BUTTON_D)
      {
      pos_menu_wifi = pos_menu_wifi + 1;
      if (pos_menu_wifi >= max_menu_wifi) pos_menu_wifi = 0;
      mostrar_menu_wifi(1, 40, "Seleccionar red WiFi");
      }
    if (!BUTTON_U)
      {
      pos_menu_wifi = pos_menu_wifi - 1;
      if (pos_menu_wifi < 0) pos_menu_wifi = max_menu_wifi-1;
      mostrar_menu_wifi(1, 40, "Seleccionar red WiFi");
      }
    if (!BUTTON_P)
      {
      limpiar_pantalla(false);
      tft.setCursor(1, 1, 2); 
      WiFi.SSID(pos_menu_wifi).toCharArray(ssid,30);
      //ssid = WiFi.SSID(pos_menu_wifi);
      tft.print("Contrasena para "); tft.print(ssid);
      String auxS = leer_cadena(10, 20, "", 15);
      auxS.toCharArray(password,15);
      if (auxS.length()>0)    // guardar y conectar
        {
        guardar_wifi();
        conectar_wifi(ssid, password);
        en_menu = 1;
        pos_menu = 0;
        tft.println();
        if (WiFi.status() == WL_CONNECTED)
          tft.println("Conectado a");
        else
          tft.println("ERROR AL CONECTAR:");
        tft.println(ssid);
        tft.println(password);
        tft.println("");
        tft.println("");
        tft.println("Pulse botón IZDO para volver");
        }
      }
}
