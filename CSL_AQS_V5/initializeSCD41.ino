/*!
* @brief Initialize SCD41 
**/
void initializeSCD41() {
  Serial.println("starting SCD41... ");
  if (!scd41.begin(false, false, false)){                                                 // Do not start periodic measurements
    stat |= 0x06;
    Serial.println(F("SCD41 not detected. Please check wiring. Freezing..."));
    display.println("SCD41 not detected");
    display.display();
  }
  else {
    Serial.println(F("SCD41 Connected"));
    display.println("SCD41 Connected");
    display.display();
  }
}


/*!
* @brief read CO2, temperature and relative humidity from SCD41 sensor 
* @return String with readings 
**/

String readSCD41() {
  int counter = 0;
  if(scd41.measureSingleShot()){                                                          // Request fresh data (should take 5 seconds)
    while (!scd41.readMeasurement()){                                                     // readMeasurement() Check for fresh data
      delay(100);
      if (counter > 61){ 
        stat |= 0x07;
        break;
      }
      else
        counter += 1;
    } 
  }
  CO2scd41 = scd41.getCO2();
  return (String(scd41.getCO2()) + String(", ") + String(scd41.getTemperature()) + String(", ") + String(scd41.getHumidity()));
  
}
