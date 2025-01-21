// teclado.h

const int num_rows = 5;
const int num_cols = 10;
const int num_keys = num_rows * num_cols;

int caps = 0;   // o:minusculas, 1:mayusculas
const char mayus[num_keys]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
                            'U','V','W','X','Y','Z','-','_','/','*','0','1','2','3','4','5','6','7','8','9',
                            '<','<',' ',' ','A','a',' ',' ','O','K'};
const char minus[num_keys]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t',
                            'u','v','w','x','y','z','-','_','/','*','0','1','2','3','4','5','6','7','8','9',
                            '<','<',' ',' ','A','a',' ',' ','O','K'};

int x_key = 0;
int y_key = 0;

void mostrar_teclado(int tipo)    // tipo=0->mayusculas, 1= minusculas
{
  tft.setTextColor(TFT_WHITE);  
  for (int i=0; i<num_rows; i++)
    {
    for (int j=0; j<num_cols; j++)
      tft.drawRect(10+(j*30), 60+(i*30), 29, 29, ((i == y_key) && (j == x_key))?TFT_YELLOW:TFT_WHITE);
    }
  for (int i=0; i<num_rows; i++)
    {
    for (int j=0; j<num_cols; j++)
      {
      tft.setTextColor(((i == y_key) && (j == x_key))?TFT_YELLOW:TFT_WHITE);  
      tft.setCursor(15+(j*30), 65+(i*30), 4);
      tft.print(tipo==1?mayus[i*10+j]:minus[i*10+j]);
      }
    }
}

String leer_cadena(int x, int y, String iniS, int size)
{
  String auxS = iniS; 
  tft.drawRect(x, y, size*20, 30, TFT_WHITE);
  tft.setTextColor(TFT_YELLOW);  
  tft.setCursor(x+5, y+4, 4);  
  tft.print(auxS);
  leer_botones();
  while (BUTTON_C == 1)
    {
    mostrar_teclado(caps);
    leer_botones();
    if (!BUTTON_L) x_key--; if (x_key<0) x_key=num_cols-1;
    else if (!BUTTON_R) x_key++; if (x_key>=num_cols) x_key=0;
    else if (!BUTTON_U) y_key--; if (y_key<0) y_key=num_rows-1;
    else if (!BUTTON_D) y_key++; if (y_key>=num_rows) y_key=0;
    else if (!BUTTON_P) 
      {
      if (y_key<num_rows-1)
        {
        if (auxS.length()<size)
          {
          auxS = auxS + String(caps==1?mayus[y_key*10+x_key]:minus[y_key*10+x_key]);
          tft.setTextColor(TFT_YELLOW);  
          tft.setCursor(x+5, y+4, 4);  
          tft.print(auxS);
          }
        }
      else
        {
        if (x_key<=1)
          {
          auxS = auxS.substring(0,auxS.length()-1);
          tft.fillRect(x+1,y+1,size*18-2,28,TFT_BLACK);
          tft.setTextColor(TFT_YELLOW);  
          tft.setCursor(x+5, y+4, 4);  
          tft.print(auxS);
          }
        else if (x_key == 4) // mayusculas
          {
          caps = 1;
          for (int i=0; i<num_rows; i++)
            {
            for (int j=0; j<num_cols; j++)
              tft.fillRect(11+(j*30), 61+(i*30), 27, 27, TFT_BLACK);
            }
          }
        else if (x_key == 5) // minusculas
          {
          caps = 0;
          for (int i=0; i<num_rows; i++)
            {
            for (int j=0; j<num_cols; j++)
              tft.fillRect(11+(j*30), 61+(i*30), 27, 27, TFT_BLACK);
            }
          }
        else if (x_key >= 8) // OK
          {
          return auxS;
          }
        }
      }
    }
  return "";
}