// filesystem.h

#ifdef _SAMD21_
  #define SDCARD_SS_PIN 1
  #define SDCARD_SPI SPI
#endif 

void readFile(fs::FS& fs, const char* path) 
{
  File file = fs.open(path);
  if (!file) 
    {
    Serial.println("Failed to open file for reading");
    return;
    }
  Serial.println("Read from file: ");
  while (file.available()) 
    {
    Serial.println(file.readString());
    }
  file.close();
}

void init_filesystem()
{
  while (!SD.begin(SDCARD_SS_PIN,SDCARD_SPI,4000000UL)) 
    {
    Serial.println("Card Mount Failed");
    return;
    }  
  //readFile(SD, "/wifi.txt");
}

