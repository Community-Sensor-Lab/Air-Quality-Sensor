// Uncoment if using SCD30 instead of SCD41

/**
void initializeSCD30(int samplingInterval)  {
  Serial.println("starting SCD30... ");
  if (!scd30.begin()) {
    stat |= 0x04; 
    Serial.println(F("SCD30 not detected. Please check wiring. Freezing..."));
    display.println("SCD30 not detected");
    display.display();
  }
  else  {
    Serial.println(F("SCD30 Connected"));
    display.println("SCD30 Connected");
    display.display();
    scd30.setAmbientPressure(0);                            // 0=deactivate press. correction. Default 1013.25 mBar
    scd30.setAutoSelfCalibration(false);                    // de-activate Automatic Self-Calibraton
    //scd30.setForcedRecalibrationFactor(453);
    scd30.setMeasurementInterval(samplingInterval);         // Change measurement interval seconds 2-1800
    delay(1000);
  }
}

String readSCD30(float Pbme) {
  int counter = 0;
  if (scd30.isConnected()) { 
    scd30.setAmbientPressure(Pbme);                             // update CO2 sensor to current pressure
    while (!scd30.dataAvailable()) {                            // dataAvailable() returns true when data is available
      delay(1000);
      if (counter > 61){ 
        stat |= 0x05;                                       
        break;
      }
      else
        counter += 1;
    }

    CO2scd30 = scd30.getCO2();
    float Tco2 = scd30.getTemperature();
    float RHco2 = scd30.getHumidity();
    return (String(CO2scd30) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
  }
  else {
    stat |= 0x04;
    return ("None,None,None,");
  }
}
**/