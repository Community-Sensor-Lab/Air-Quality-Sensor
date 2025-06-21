#ifndef CLS_AQS_V5_H
#define CLS_AQS_V5_H

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

#define VBATPIN A7                                         // This is also D9 button A disable pullup to read analog
#define BUTTON_A 9                                          // Oled button also A7 enable pullup to read button
#define BUTTON_B 6                                          // oled button
#define BUTTON_C 5                                          // oled button
#define SD_CS 10                                            // Chip select for SD card default for Adalogger
#define MAXBUF_REQUIREMENT 48

// Global status in uint8_t stat (bitwise status flags)
#define STAT_STARTING 0x00
#define STAT_SD_NOT_PRESENT 0x01
#define STAT_SD_CREATE_FAIL 0x02
#define STAT_RTC_FAIL 0x03
#define STAT_SCD30_NOT_AVAILABLE 0x04
#define STAT_SCD30_TIMEOUT 0x05
#define STAT_SCD41_NOT_AVAILABLE 0x06
#define STAT_SCD41_TIMEOUT 0x07
#define STAT_BME280_MALFUNCTION 0x08
#define STAT_BME280_TIMEOUT 0x09
#define STAT_SEN5X_MALFUNCTION 0x0A
#define STAT_SEN5X_TIMEOUT 0x0B
#define STAT_SSL_ERROR 0x0C
#define STAT_PROVISION_ERROR 0x0D
#define STAT_OTHER_ERROR 0x0E

// Function prototypes
void initializeOLED();
bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce);
void initializeSCD41(); 
String readSCD41();
void initializeBME280(); 
String readBME280();
void initializeSen5x();
String readSen5x();   
File initializeSD(); 
void payloadUpload(String payload);
void printMacAddress(byte mac[]); 
void AP_getInfo(String &ssid, String &passcode, String &gsid);
void makeMACssidAP(String startString);
void initializeClient();
void A();  // Interrupt Handler

// Global Variables
extern char outstr[160];
extern int32_t Tsleep;
extern bool displayState;
extern bool buttonAstate;
extern int lastTimeToggle;
extern int timeDebounce;
extern String response;
extern int samplingRate;
extern char server_google_script[];
extern char server_google_usercontent[];
extern String payload;
extern int status;
extern String ssidg, passcodeg, gsidg;
extern uint8_t stat; 
extern uint16_t CO2scd41;  
extern float Tbme;
extern float Pbme;
extern float RHbme;
extern float massConcentrationPm2p5;
extern float vocIndex;
extern float noxIndex;

// Sensor Components
extern Adafruit_SH1107 display;      
extern Adafruit_BME280 bme280;  
extern SensirionI2CSen5x sen5x; 
extern RTC_PCF8523 rtc; 
extern WiFiSSLClient client;                                          
extern File logfile;                                                   // For SD card logging
extern SCD4x scd41;  // SCD41 sensor instance

// CO2, Temperature, Humidity Data Header
extern char header[];

// Payload format for Google Sheets
extern String payload;

// Debug print header for SD card
extern const String headerSD;

// Force Provisioning flag
extern bool force_pro;

const char webpage_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Community Sensor Lab provisioning page</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h1>NGENS Air Quality Sensor</h1>
  <form action="/get">
    ssid: <input type="text" name="SSID"><br>
    passcode: <input type="password" name="passcode"><br>
    gsid: <input type="text" name="GSID"><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";


typedef struct {
  boolean valid;
  char saved_ssid[64];
  char saved_passcode[64];
  char saved_gsid[128];
} Secrets;

#endif // 

  
