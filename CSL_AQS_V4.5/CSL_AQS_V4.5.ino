/*
   COMMUNITY SENSOR LAB - AIR QUALITY SENSOR

   featherM0-Wifi + featherwing adalogger-SD-RTC + SCD30-CO2 + BME280-TPRH + OLED display + SPS30-PM2.5

   The SCD30 has a minimum power consumption of 5mA and cannot be stop-started. It's set to 55s (30s nominal)
   sampling period and the featherM0 sleeps for 2 x 16 =32s, wakes and waits for data available.
   Button A toggles display on/off but must be held down for 16s max and then wait 16s to toggle again.

   Logs: date time, co2, t, rh, t2, press, rh2, battery voltage, status

   https://github.com/Community-Sensor-Lab/Air-Quality-Sensor

   Global status is in uint8_t stat in bit order:
   0- 0000 0001 0x01 SD card not present
   1- 0000 0010 0x02 SD could not create file
   2- 0000 0100 0x04 RTC failed
   3- 0000 1000 0x08 SCD30 CO2 sensor not available
   4- 0001 0000 0x10 SCD30 CO2 sensor timeout
   5- 0010 0000 0x20 SPS30 PM2.5 sensor malfunction
   6- 0100 0000 0x40 HSC differential pressure sensor absent or malfunction
   7- 1000 0000 0x80 future: google.com ssl connection error

   RICARDO TOLEDO-CROW NGENS, ESI, ASRC, CUNY,
   AMALIA TORRES, CUNY, July 2021

*/
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
#endif


// Function prototypes
void initializeOLED();
bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce );

void initializeSCD30();
String readSCD30();

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

void payloadUpload(String payload);
void initializeClient();
  

// Global Variables
typedef struct {
  boolean valid;
  char saved_ssid[64];
  char saved_passcode[64];
  char saved_gsid[128];
} Secrets;

FlashStorage(flash_storage, Secrets);

char server[] = "script.google.com"; 

String payload = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
char header[] = "DateTime, CO2, Tco2, RHco2, Tbme, Pbme, RHbme, vbat(mV), status, mP1.0, mP2.5, mP4.0, mP10, ncP0.5, ncP1.0, ncP2.5, ncP4.0, ncP10, avgPartSize, Thsc, dPhsc";
int status = WL_IDLE_STATUS;
String ssidg, passcodeg, gsidg;


// SCD
uint16_t CO2; 

// BME
float Tbme = 0;
float Pbme = 0;
float RHbme= 0;

//SEN 55
// float massConcentrationPm1p0 = 0;
float massConcentrationPm2p5 = 0;
// float massConcentrationPm4p0 = 0;
// float massConcentrationPm10p0 = 0;
// float numberConcentrationPm0p5 = 0;
// float numberConcentrationPm1p0 = 0;
// float numberConcentrationPm2p5 = 0;
// float numberConcentrationPm4p0 = 0;
// float numberConcentrationPm10p0 = 0;
// float typicalParticleSize = 0;
// float ambientHumidity = 0;
// float ambientTemperature = 0;
float vocIndex = 0;
float noxIndex = 0;

// Force Provisioning
bool force_pro = false;

// Sensor Componentns                                                                 
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);      
Adafruit_BME280 bme280;  
SensirionI2CSen5x sen5x; 
RTC_PCF8523 rtc; 
WiFiSSLClient client;                                          
File logfile;                                                  
SCD30 scd30;                                             
SCD4x scd41(SCD4x_SENSOR_SCD41);                          


void setup() {
  pinMode(VBATPIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
  delay(5000);
  Serial.println(__FILE__);
  WiFi.setPins(8, 7, 4, 2);

  initializeOLED();
  initializeSCD41();                                      
  initializeSCD30(25);                                                                          
  initializeBME280();  
  initializeSen5x(); 
  initializeRTC();                                          
  logfile = initializeSD();

  delay(3000);

  //Set Interrupt
  pinMode(BUTTON_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_A), A, CHANGE);


  if (!check_valid()) {
    AP_getInfo(ssidg, passcodeg, gsidg);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to previously saved network");
    display.display();
    storeinfo(ssidg, passcodeg, gsidg);
  }
}

//Interrupt Handler
void A() {
  force_pro = true;
}

char outstr[160];
int32_t Tsleep = 0;
bool displayState = true;
bool buttonAstate = true;
int lastTimeToggle = 0;
int timeDebounce = 100;

void loop(void) {

  uint8_t ctr = 0;
  String bmeString   = readBME280();                                   
  String sen5xString = readSen5x();
  String scd41String = readSCD41();
  String scd30String = readSCD30(100);
  DateTime now;
  now = rtc.now();                                                    

  pinMode(VBATPIN, INPUT);  // read battery voltage
  float measuredvbat = analogRead(VBATPIN) * 0.006445;
  pinMode(BUTTON_A, INPUT_PULLUP);

  delay(5000);  // wait for the sps30 to stabilize

  /**
    Payload Format: 
    outstr, "%02u/%02u/%02u %02u:%02u:%02u, %.2d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %x"
    "now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),CO2, Tco2, RHco2, Tbme, Pbme, RHbme, measuredvbat, stat";
  **/

  sprintf(outstr, "%02u/%02u/%02u %02u:%02u:%02u, ", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  payloadUpload( payload + String("\"") + String(outstr) + scd41String + bmeString + String(measuredvbat) + String(", ") + String(1) + String(", ") + sen5xString);

  Serial.println(header);
  Serial.println(String(outstr) + scd41String + bmeString + String(measuredvbat) + String(", ") + String(1) + String(", ") + sen5xString);

  logfile.println(String(outstr) + scd41String + bmeString + String(measuredvbat) + String(", ") + String(1) + String(", ") + sen5xString);
  logfile.flush();                                                // Write to disk. Uses 2048 bytes of I/O to SD card, power and takes time

  // sleep cycle
  for (int i = 1; i <= 8; i++) {                                  // 124s = 8x16s sleep, only toggle display
    displayState = toggleButton(BUTTON_A, displayState, buttonAstate, lastTimeToggle, timeDebounce);
    if (displayState) { // On
      display.clearDisplay();
     
      display.setCursor(0, 0); 
      display.print("CO2 ");
      display.setCursor(40, 0); 
      display.print(CO2); 
      display.print(" ppm");
      display.print("  ");
      display.print(measuredvbat, 2); 
      display.print("V");


      display.setCursor(0, 8); 
      display.print("T");
      display.setCursor(40, 8); 
      display.print(Tbme, 2); 
      display.print("C");

      display.setCursor(0, 16);
      display.print("P");
      display.setCursor(40, 16);  
      display.print(Pbme, 2);  
      display.print(" mBar");

      display.setCursor(0, 24);  
      display.print("RH");
      display.setCursor(40, 24); 
      display.print(RHbme, 0);  
      display.print("%");


      display.setCursor(0, 32);  
      display.print("VOC");
      display.setCursor(40, 32); 
      display.print(vocIndex, 2);  

      display.setCursor(0, 40); 
      display.print("NOX");
      display.setCursor(40, 40); 
      display.print(noxIndex, 2); 

      display.setCursor(0, 48); 
      display.print("Pm 2.5");
      display.setCursor(40, 48);
      display.print(massConcentrationPm2p5, 2); 

      display.display();
    } else {       
      display.clearDisplay();
      display.display();
    };
    //int sleepMS = Watchdog.sleep();// remove comment for low power
    delay(6000);  // uncomment to debug because serial communication doesn't come back after sleeping
  }
}
