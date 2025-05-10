/*
  ADD DOCUMENTATION
*/


/*
  SCD41 Low Power Single Shot
  By: Paul Clark
  Based on earlier code by: Nathan Seidle
  SparkFun Electronics
  Date: June 3rd, 2021
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.

  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/18365

  This example prints the current CO2 level, relative humidity, and temperature in C.

  Hardware Connections:
  Attach RedBoard to computer using a USB cable.
  Connect SCD40/41 to RedBoard using Qwiic cable.
  Open Serial Monitor at 115200 baud.
*/


void initializeSCD41() {
  // Serial.begin(115200);

  // delay(5000);
  // Serial.println(__FILE__);

  // Serial.println(F("SCD41 Example"));
  // Wire.begin();

  //mySensor.enableDebugging(); // Uncomment this line to get helpful debug messages on Serial

  if (scd41.begin(false, false, false) == false)  // Do not start periodic measurements
  //measBegin_________/     |     |
  //autoCalibrate__________/      |
  //skipStopPeriodicMeasurements_/
  {
    Serial.println(F("Sensor not detected. Please check wiring. Freezing..."));
    while (1)
      ;
  }

  if (!scd41.measureSingleShot()) {
    Serial.println(F("measureSingleShot failed. Are you sure you have a SCD41 connected? Freezing..."));
    while (1)
      ;
  }
}

String readSCD41()
{
  scd41.measureSingleShot();        // Request fresh data (should take 5 seconds)
  while (!scd41.readMeasurement())  // readMeasurement will return true when fresh data is available
  {
    Serial.print(F("."));
    delay(500);
  }
  CO2 = scd41.getCO2();
  float Tco2 = scd41.getTemperature();
  float RHco2 = scd41.getHumidity();

  return (String(CO2) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));

}
