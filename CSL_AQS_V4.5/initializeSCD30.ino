/*
  Sensirion SPS20 PM sensor.

  Functions:
  - void initializeSCD30(int samplingInterval): 
    Initializes the Sensirion SCD30 CO2 sensor. This function checks if the sensor is connected, 
    and if so, configures it with the given sampling interval. It also deactivates automatic self-calibration 
    and sets the ambient pressure to 0 (no pressure correction).
    - samplingInterval (int): The measurement interval in seconds. The default value is 2 seconds, 
      but it can be adjusted between 2 and 1800 seconds.

  - String readSCD30(float Pbme): 
    Reads the current CO2 concentration, temperature, humidity, and particulate matter (PM) data from the SCD30 sensor.
    The ambient pressure from the BME sensor is passed as a parameter to adjust the CO2 readings based on atmospheric conditions.
    Returns a string containing the CO2 concentration (ppm), temperature (°C), humidity (%), and 
    the size of particles in micrometers (0.5µm, 1µm, 2.5µm, 10µm) with the average particle size in µm. 
    - Pbme (float): The ambient pressure from the BME sensor, used to adjust the CO2 reading for current pressure conditions.
  
    Example return: "400, 22.5, 45.3, "
    The string includes the CO2 concentration (ppm), temperature (°C), humidity (%), and PM data in the format of 
    "CO2 ppm, Temperature (°C), Humidity (%), "

*/

void initializeSCD30(int samplingInterval)  {
  Serial.print("starting SCD30... ");
  if (!scd30.begin()) {
    Serial.println(F("SCD30 not detected. Please check wiring. Freezing..."));
    display.println("SCD30 Not Detected");
    display.display();
  }
  else  {
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
      if (counter > 61)                                         
        break;
      else
        counter += 1;
    }

    CO2 = scd30.getCO2();
    float Tco2 = scd30.getTemperature();
    float RHco2 = scd30.getHumidity();
    return (String(CO2) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
  }
  else {
    return ("None,None,None,");
  }
}
