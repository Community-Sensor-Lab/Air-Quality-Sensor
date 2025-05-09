
/*
   COMMUNITY SENSOR LAB - AIR QUALITY SENSOR

   Adafruit Feather ESP32 V2 + Featherwing Adalogger-SD-RTC + SCD30 OR SCD41 -CO2 + BME280 -TPRH + OLED display + SEN55 -PM2.5 VOC NOX

   The SCD30 has a minimum power consumption of 5mA and cannot be stop-started. It's set to 55s (30s nominal)
   sampling period and the featherM0 sleeps for 2 x 16 =32s, wakes and waits for data available.
   Button A toggles display on/off but must be held down for 16s max and then wait 16s to toggle again.

   Logs: sensor, co2, 3x t, 3x rh, press, battery voltage, pm1, pm 2.5, pm 4.0, pm 10, voc, nox 

   https://github.com/Community-Sensor-Lab/Air-Quality-Sensor

   RICARDO TOLEDO-CROW NGENS, ESI, ASRC, CUNY,
   

*/
// #include <FS.h>
#include <SPI.h>
// #include <SD.h>
#include <Wire.h>
#include <Adafruit_SleepyDog.h>
#include "RTClib.h"
#include "SparkFun_SCD4x_Arduino_Library.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>  // oled library
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SensirionI2CSen5x.h>
// #include <WiFi101.h>
#include <FlashStorage.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>


#define VBATPIN A7  // this is also D9 button A disable pullup to read analog
#define BUTTON_A 9  // Oled button also A7 enable pullup to read button
#define BUTTON_B 6  // oled button
#define BUTTON_C 5  // oled button
#define SD_CS 10    // Chip select for SD card default for Adalogger
#define MAXBUF_REQUIREMENT 48

#if (defined(I2C_BUFFER_LENGTH) &&            \
(I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || \
(defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
#define USE_PRODUCT_INFO
#endif

typedef struct {
  boolean valid;
  char saved_ssid[64];
  char saved_passcode[64];
  char saved_gsid[128];
} Secrets;

FlashStorage(flash_storage, Secrets);


char server[] = "script.google.com";  // name address for Google scripts as we are communicationg with the scripg (using DNS)
// these are the commands to be sent to the google script: namely add a row to last in Sheet1 with the values TBD
String payload_base = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
String payload = "";
char header[] = "DateTime, CO2, Tco2, RHco2, Tbme, Pbme, RHbme, vbat(mV), OLstatus, mP1.0, mP2.5, mP4.0, mP10, ncP0.5, ncP1.0, ncP2.5, ncP4.0, ncP10, avgPartSize, Thsc, dPhsc";
int OLstatus = -1;
String ssidg, passcodeg, gsidg;
uint16_t CO2;  // for oled display
float Pmv = 0;
float Nox = 0;
float Voc = 0;
bool force_pro = false;

SensirionI2CSen5x sen5x;
WiFiClientSecure client;                                            // make SSL client
RTC_PCF8523 rtc;                                                 // Real Time Clock for RevB Adafruit logger shield
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);       // large OLED display
Adafruit_BME280 bme;                                             // the bme tprh sensor
//File logfile;                                                    // the logging file
//SCD30 CO2sensor;                                                 // sensirion SCD30 CO2 NDIR
SCD4x CO2sensor(SCD4x_SENSOR_SCD41); // Tell the library we have a SCD41 connected;

// TruStabilityPressureSensor diffPresSens(HSC_CS, -100.0, 100.0);  // HSC differential pressure sensor for Met Eric Breunitg
//uint8_t OLstat = 0;     
int OLstat = -1;                                           // OLstatus byte

void setup(void) {
  pinMode(VBATPIN, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  delay(5000);
  Serial.println(__FILE__);

  initializeOLED();
  initializeSen5x();         // PM sensor
  initializeSCD41();
  //initializeSCD30(25);       // CO2 sensor to 30s more stable (1 min max recommended)
  initializeRTC();
  initializeBME();           // TPRH
  //logfile = initializeSD();  // SD card and RTC
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
bool displayOLstate = true;
bool buttonAOLstate = true;
int lastTimeToggle = 0;
int timeDebounce = 100;

void loop(void) {

  uint8_t ctr = 0;
  //OLstat = OLstat & 0xEF;  // clear bit 4 for CO2 sensor

  String bmeString = readBME();  // get data string from BME280 "T, P, RH, "
  String bme = readBME();

  // parsing out the t p rh float values
  float Tbme = bme.toFloat();
  bme = bme.substring(bme.indexOf(", ") + 2);
  float Pbme = bme.toFloat();
  bme = bme.substring(bme.indexOf(", ") + 2);
  float RHbme = bme.toFloat();


  String sen5xString = readSen5x();
  String sen5x = readBME();

  String co2String = readSCD41();

  DateTime now;
  now = rtc.now();  // fetch the date + time

  pinMode(VBATPIN, INPUT);  // read battery voltage
  float measuredvbat = analogRead(VBATPIN) * 0.006445;
  pinMode(BUTTON_A, INPUT_PULLUP);

  delay(5000);  // wait for the sps30 to stabilize

  //  sprintf(outstr, "%02u/%02u/%02u %02u:%02u:%02u, %.2d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %x, ",
  //          now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(),
  //          CO2, Tco2, RHco2, Tbme, Pbme, RHbme, measuredvbat, OLstat);

  sprintf(outstr, "%02u/%02u/%02u %02u:%02u:%02u, ", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  payloadUpload(String(outstr) + co2String + bmeString + String(measuredvbat) + String(", ") + String(OLstat) + String(", ") + sen5xString);

  Serial.println(header);
  //Serial.println(String(outstr) + co2String + bmeString + String(measuredvbat) + String(", ") + String(OLstat) + String(", ") + sen5xString);

  //logfile.println(String(outstr) + co2String + bmeString + String(measuredvbat) + String(", ") + String(OLstat) + String(", ") + sen5xString);
  //logfile.flush();  // Write to disk. Uses 2048 bytes of I/O to SD card, power and takes time

  // sleep cycle
  for (int i = 1; i <= 8; i++) {  // 124s = 8x16s sleep, only toggle display
    displayOLstate = toggleButton(BUTTON_A, displayOLstate, buttonAOLstate, lastTimeToggle, timeDebounce);
    if (displayOLstate) {  // turn display on with data
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
      display.print(Voc, 2);  

      display.setCursor(0, 40); 
      display.print("NOX");
      display.setCursor(40, 40); 
      display.print(Nox, 2); 

      display.setCursor(0, 48); 
      display.print("Pm 2.5");
      display.setCursor(40, 48);
      display.print(Pmv, 2); 

      display.display();
    } else {  // turn display off
      display.clearDisplay();
      display.display();
    };
    //int sleepMS = Watchdog.sleep();// remove comment for low power
    delay(16000);  // uncomment to debug because serial communication doesn't come back after sleeping
  }
}
