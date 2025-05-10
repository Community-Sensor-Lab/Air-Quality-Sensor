/*  Adafruit BME280 PM sensor
    Functions:
    - void initializeSCD30()
    - String readBME280() where Pbme is the ambient pressure from bme sensor

    return [
      temparature,
      pressure, 
      relative humidity
    ]
*/

void initializeBME280()  {
  Serial.print("starting BME280... ");
  unsigned bmeStatus = bme280.begin();
  if (!bmeStatus) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme280.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    display.println("BME280 Not Detected");
    display.display();
  }
  else{
    Serial.println("BME Connected");
    display.println("BME280 Connected");display.display();
  }
}

String readBME280(){
  Tbme = bme280.readTemperature();
  Pbme = bme280.readPressure() / 100; // for hPa
  RHbme = bme280.readHumidity();

  String bmeString = String(Tbme) + String(", ") + String(Pbme) + String(", ") + String(RHbme) + String(", ");
  return bmeString;
}
