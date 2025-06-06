/*!
* @brief 
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
      delay(1000);
      if (counter > 61){ 
        stat |= 0x07;
        break;
      }
      else
        counter += 1;
    }

    CO2scd41 = scd41.getCO2();
    float Tscd41 = scd41.getTemperature();
    float RHscd41 = scd41.getHumidity();
    return (String(CO2scd41) + String(", ") + String(Tscd41) + String(", ") + String(RHscd41) + String(", "));
  }
  else {
    return ("None,None,None,");
  }
}
