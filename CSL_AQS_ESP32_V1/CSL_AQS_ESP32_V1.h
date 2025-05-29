#ifndef CSL_AQI_ESP32_V1_H
#define CSL_AQI_ESP32_V1_H

#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <Wire.h>
//#include <Adafruit_SleepyDog.h>
#include <RTClib.h>
#include "SparkFun_SCD4x_Arduino_Library.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>  // OLED library
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SensirionI2CSen5x.h> 
//#include <WiFi101.h>
//#include "Storeinfo.h"
//#include <FlashStorage.h>

#define VBATPIN A13  // this is also D9 button A disable pullup to read analog
#define BUTTON_A 15  // for the adafruit Feather ESP32 v2 (ABC, 15 32 14) Oled button also A7 enable pullup to read button
//#define SD_CS 10    // Chip select for SD card default for Adalogger

/* STRUCT TO STORE ALL SENSOR DATA */
typedef struct {
  DateTime now; 
  float Tbme; // BME280 temperature C
  float Pbme; // BME280 pressure mBar
  float RHbme; // BME280 relative humidity %
  uint16_t CO2; // SCD41 C02 in ppm
  float Tco2; // SCD41 temperature C
  float RHco2; // SCD41 relative humidity %
  float mPm1_0; // SEN55 pm1.0 in ug/m^3
  float mPm2_5; // SEN55 pm2.5 in ug/m^3
  float mPm4_0; // SEN55 pm4.0 in ug/m^3
  float mPm10; // SEN55 pm10 in ug/m^3
  float RHsen; // SEN55 relative humidity %
  float Tsen; // SEN55 temperature C
  float VOCs; // SEN55 VOCs index [1..500] 
  float NOx; // SEN55 NOx index [1..500]
  float Vbat;
} data;

data sensorData; // instantiate a sensor data structure

bool force_provisioning = false; 

char header[] = "DateTime, Tbme, Pbme, RHbme, CO2, Tco2, RHco2, mPm1.0, mPm2.5, mPm4.0, mPm10, RHsen, Tsen, VOCs, NOx, Vbat";

SensirionI2CSen5x SEN55;
//WiFiSSLClient client;                                            // make SSL client
RTC_PCF8523 rtc;                                                 // Real Time Clock for RevB Adafruit logger shield
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);       // large OLED display
Adafruit_BME280 bme;                                             // the bme tprh sensor
File logfile;                                                    // the logging file
//SCD30 CO2sensor;                                                 // sensirion SCD30 CO2 NDIR
SCD4x CO2sensor(SCD4x_SENSOR_SCD41); // Tell the library we have a SCD41 connected;

/*
char server[] = "script.google.com";  // name address for Google scripts as we are communicationg with the scripg (using DNS)

// these are the commands to be sent to the google script: namely add a row to last in Sheet1 with the values TBD
String payload_base = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
String payload = "";
char header[] = "DateTime, CO2, Tco2, RHco2, Tbme, Pbme, RHbme, vbat(mV), status, mP1.0, mP2.5, mP4.0, mP10, ncP0.5, ncP1.0, ncP2.5, ncP4.0, ncP10, avgPartSize, Thsc, dPhsc";
int status = WL_IDLE_STATUS;
String ssidg, passcodeg, gsidg; */

#endif
