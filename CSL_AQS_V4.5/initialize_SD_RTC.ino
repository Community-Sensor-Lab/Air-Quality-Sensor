
File initialize_SD_RTC()  {
  File logFile;
  if (!SD.begin(SD_CS)) { 
    Serial.println("Card failed, or not present.");
    stat = stat | 0x01; // set bit 1 if SD error
  }
  else  {
    Serial.print("starting SD card... ");
    char filename[] = "LOG0000.TXT";  // create a new file//increase 0000
    for (uint16_t i = 0; i <= 9999; i++) {
      filename[3] = i / 1000 + '0';    //48 ascii character value of 0 //integer division by
      filename[4] = i / 100 - 10 * (i / 1000) + '0';
      filename[5] = i / 10 - 10 * (i / 100) + '0'; //integer division 100
      filename[6] = i % 10 + '0';   //modulo 10
      if (!SD.exists(filename)) {  
        logFile = SD.open(filename, FILE_WRITE);
        break;  
      }
    }
    if (!logFile) {
      Serial.println("Couldn't create file");
      stat = stat | 0x02;
    }
    else  {
      Serial.print("logging to file: ");
      Serial.println(filename);
      logFile.println(header);
      logFile.flush();
    }
  }

  Serial.print("starting RTC... ");
  Wire.begin();  
  if (!rtc.begin()) {
    Serial.println("RTC Failed");
    stat = stat | 0x04; // 3rd bit set 'rtc not started'
    display.println("RTC Not Started..");
    display.display();
  }
  else
    Serial.println("RTC Connected");
    display.println("RTC Connected");
    display.display();

  /***
  TO SET TIME at compile: run once to syncro then run again with line commented out.

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  ***/

  return logFile;
}
