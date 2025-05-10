/*
  SCD41 Low Power Single Shot
  Edited by: 
  Originally written by: Paul Clark (SparkFun Electronics)
  License: MIT

  Functions:
  - void initializeSCD41(): 
    Initializes the SCD41 sensor for a single-shot CO2 reading. If the sensor is not connected or fails to measure, it will print an error.

  - String readSCD41(): 
    Requests and retrieves a CO2 reading, temperature, and humidity from the sensor. Returns this data as a string in the format:
      "CO2 ppm, Temperature (°C), Humidity (%)"
*/

void initializeSCD41() {
  if (!scd41.begin(false, false, false))                                              // Do not start periodic measurements
  {
    Serial.println(F("SCD41 not detected. Please check wiring. Freezing..."));
    display.println("SCD41 Not Detected");
    display.display();
  }
  else  
  {
    display.println("SCD30 Connected");
    display.display();
  }

String readSCD41() {
  int counter = 0;
  if(scd41.measureSingleShot()){                                                          // Request fresh data (should take 5 seconds)
    while (!scd41.readMeasurement()){                                                     // readMeasurement() Check for fresh data
      delay(1000);
      if (counter > 61)                                           
        break;
      else
        counter += 1;
    }

    CO2 = scd41.getCO2();
    float Tco2 = scd41.getTemperature();
    float RHco2 = scd41.getHumidity();
    return (String(CO2) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
  }
  else {
    return ("None,None,None,");
  }
}
