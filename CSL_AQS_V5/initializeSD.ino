/*!
* @brief Initialize SD card
*/
File initializeSD()  {
  File logFile;
  if (!SD.begin(SD_CS)) { 
    Serial.println("Card failed, or not present.");
    stat |= 0x01;
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
      stat |= 0x02;
    }
    else  {
      Serial.print("logging to file: ");
      Serial.println(filename);
      logFile.println(String(header) + "Sampling Rate, WiFi Status");
      logFile.flush();
    }
  }

  return logFile;
}


/*!
* @brief Writes the sensors data to SD card along with the WiFi status
* @details knowing the wifi status helps us know if the data was also uploaded to Appscript 
*/

void writeToSD_With_WiFiStat(String data) {
  String WifiStatus;

  // Get the current Wi-Fi status
  switch(WiFi.status()) {
    case WL_CONNECTED:
      WifiStatus = "WL_CONNECTED";
      break;
    
    case WL_NO_SHIELD:
      WifiStatus = "WL_NO_SHIELD";
      break;

    case WL_CONNECT_FAILED:
      WifiStatus = "WL_CONNECT_FAILED";
      break;

    case WL_IDLE_STATUS:
      WifiStatus = "WL_IDLE_STATUS";
      break;
      
    case WL_DISCONNECTED:
      WifiStatus = "WL_DISCONNECTED";
      break;
      
    default:
      WifiStatus = "UNKNOWN_STATUS";
      break;
  }

  // Write the data and the Wi-Fi status to the SD card
  logfile.println(data + ", " + WifiStatus);
  logfile.flush();  // Write to disk, this is I/O intensive
}

