/*!
* @brief initialize BME280 sensor
*/

void initializeBME280()  {
  Serial.println("starting BME280... ");
  unsigned bmeStatus = bme280.begin();
  if (!bmeStatus) {
    stat |= 0x08;
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
    // void Adafruit_BME280::setSampling(sensor_mode mode, sensor_sampling tempSampling, sensor_sampling pressSampling, sensor_sampling humSampling, sensor_filter filter, standby_duration duration)
    bme280.setSampling(
    Adafruit_BME280::MODE_FORCED,          
    Adafruit_BME280::SAMPLING_X1,         
    Adafruit_BME280::SAMPLING_X1,         
    Adafruit_BME280::SAMPLING_X1,          
    Adafruit_BME280::FILTER_OFF,           
    Adafruit_BME280::STANDBY_MS_1000       
    );
    Serial.println("BME Connected");
    display.println("BME280 Connected");display.display();
  }
}


/*!
* @brief read temperature, pressure and relative humidity from BME280 sensor 
* @return String with readings 
**/
String readBME280(){
  if (bme280.takeForcedMeasurement()){
    Tbme = bme280.readTemperature();
    Pbme = bme280.readPressure() / 100; // for hPa
    RHbme = bme280.readHumidity();
  return (String(Tbme) + String(", ") + String(Pbme) + String(", ") + String(RHbme) + String(", "));
  }
  else{
  stat |= 0x09;
  return "None, None, None,";
  }
}
