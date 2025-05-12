#ifndef CSL_AQS_V5_h
#define CSL_AQS_V5_h

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_SleepyDog.h>
#include "RTClib.h"                              
#include "SparkFun_SCD30_Arduino_Library.h"                    
#include "SparkFun_SCD4x_Arduino_Library.h"                    
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>  
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SensirionI2CSen5x.h>
#include <WiFi101.h>
#include <FlashStorage.h>

#define VBATPIN A7                                              // this is also D9 button A disable pullup to read analog
#define BUTTON_A 9                                              // Oled button also A7 enable pullup to read button
#define BUTTON_B 6                                              // oled button
#define BUTTON_C 5                                              // oled button
#define SD_CS 10                                                // Chip select for SD card default for Adalogger
#define MAXBUF_REQUIREMENT 48

#if (defined(I2C_BUFFER_LENGTH) &&            \
(I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || \
(defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
#define USE_PRODUCT_INFO


class CSL_AQS_V4p5
{

public:

  CSL_AQS_V4p5(void)

  void initializeOLED();

  bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce );
  
  void initializeSCD30();
  
  void initializeSCD41(); 
  
  void initializeBME280(); 
  
  void initializeSen5x();

  void initializeRTC();

  void printWiFiStatus();
  void printMacAddress(byte mac[]); 
  void AP_getInfo(String &ssid, String &passcode, String &gsid);
  void makeMACssidAP(String startString);
  

  void initializeClient();
  void payloadUpload(String payload);

private
  const char webpage_html[] PROGMEM;

}

#endif
  