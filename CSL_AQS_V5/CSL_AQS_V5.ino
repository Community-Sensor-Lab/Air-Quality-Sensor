/*!
   COMMUNITY SENSOR LAB - AIR QUALITY SENSOR

   featherM0-Wifi + featherwing adalogger-SD-RTC +  OLED display 

   The SCD30 has a minimum power consumption of 5mA and cannot be stop-started. It's set to 55s (30s nominal)
   sampling period and the featherM0 sleeps for 2 x 16 =32s, wakes and waits for data available.
   Button A toggles display on/off but must be held down for 16s max and then wait 16s to toggle again.

   Logs: 
   RTC - DateTime,
   SCD30 - Carbon Dioxide Concentration , Temperature , Relative Humidity, 
   SCD41 - Carbon Dioxide Concentration, Temperature , Relative Humidity, 
   BME 280 - Temperature , Pressure, Relative Humidity, 
   Microcontroller, Program  - Battery Voltage mV, stat
   Sen5X - Mass Concentration Pm1, Mass Concentration Pm2.5, Mass Concentration Pm4, Mass Concentration Pm10,
   Number Concentration Pm0.5, Number Concentration Pm1, Number Concentration Pm2.5, Number Concentration Pm4, Number Concentration Pm10,
   Typical Particle Size, Ambient Relative Humidity, Ambient Temperature, 
   VOC Index, NOx Index

   https://github.com/Community-Sensor-Lab/Air-Quality-Sensor

   Global status is in uint8_t stat in bit order:
   0-  0000 0000 0x00Starting state 
   1-  0000 0001 0x01 SD card not present
   2-  0000 0010 0x02 SD could not create file
   3-  0000 0011 0x03 RTC failed
   4-  0000 0100 0x04 SCD30 sensor not available
   5-  0000 0101 0x05 SCD30 sensor timeout
   6-  0000 0110 0x06 SCD41 sensor not available
   7-  0000 0111 0x07 SCD41 sensor timeout
   8-  0010 1000 0x08 BME280 sensor malfunction
   9-  0100 1001 0x09 BME280 sensor timeout
   10- 0000 1010 0x0A SEN5X sensor malfunction
   11- 0000 1011 0x0B SEN5X sensor timeout
   12- 0000 1100 0x0C SSL Connection: google.com ssl connection error
   13- 0000 1101 0x0D Provisioning error 
   14- 0000 1110 0x0E
   15- 0000 1111 0x0F
 
   RICARDO TOLEDO-CROW NGENS, ESI, ASRC, CUNY,
   AMALIA TORRES, CUNY, July 2021

*/

#include "CSL_AQS_V5.h"

#define VBATPIN A7                                           // this is also D9 button A disable pullup to read analog
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

// Global Variables

// Force Provisioning
bool force_pro = false;

char outstr[160];
int32_t Tsleep = 0;
bool displayState = true;
bool buttonAstate = true;
int lastTimeToggle = 0;
int timeDebounce = 100;

String response = "";
int samplingRate = 10000;
char server_google_script[] = "script.google.com"; 
char server_google_usercontent[] = "script.googleusercontent.com"; 
String payload = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":";
// Alternative: CO2_scd30, T_scd30, RH_scd30 
char header[] = "DateTime, CO2_scd41, T_scd41, RH_scd41, T_bme280, P_bme280, RH_bme280, dvbat(mV), status, \
 mC_Pm1_sen5x, mC_Pm2_sen5x, mC_Pm4_sen5x, mC_Pm10_sen5x, nC_Pm0_5_sen5x, nC_Pm1_sen5x, nC_Pm2_sen5x, nC_Pm4_sen5x, nC_Pm10_sen5x, typPartSize_sen5x, \
 ambientRH_sen5x, ambientTemp_sen5x, vocIndex_sen5x, noxIndex_sen5x";
int status = WL_IDLE_STATUS;
String ssidg, passcodeg, gsidg;
uint8_t stat = 0; 
// Alternative: 
// SCD30 & SCD40
//uint16_t CO2scd30;
uint16_t CO2scd41;  

// BME
float Tbme = 0;
float Pbme = 0;
float RHbme= 0;

//SEN 55
float massConcentrationPm2p5 = 0;
float vocIndex = 0;
float noxIndex = 0;

// Sensor Instances                                                                
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);      
Adafruit_BME280 bme280;  
SensirionI2CSen5x sen5x; 
RTC_PCF8523 rtc; 
WiFiSSLClient client;                                          
File logfile;                                                  
// SCD30 scd30;                                             
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
  // initializeSCD30(25);                                                                          
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
  Watchdog.sleep(16000);
}

void loop(void) {

  uint8_t ctr = 0;            
 
  // String scd30String = readSCD30(100);
  String scd41String = readSCD41();
  String bmeString   = readBME280();  
  String sen5xString  = readSen5x();
  DateTime now;
  now = rtc.now();                                                    

  pinMode(VBATPIN, INPUT);  // read battery voltage
  float measuredvbat = analogRead(VBATPIN) * 0.006445;
  pinMode(BUTTON_A, INPUT_PULLUP);
  //delay(5000);  // wait for the sps30 to stabilize

  sprintf(outstr, "%02u/%02u/%02u %02u:%02u:%02u", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  payloadUpload(payload + "\"" + outstr + "," + scd41String + "," + bmeString + "," + String(measuredvbat) + "," + String(stat) + "," + sen5xString + "\",\"srate\":" + String(samplingRate) + "}"
);

  Serial.println(header);

  // print sd card
  //logfile.println(String(outstr) + scd41String + bmeString + String(measuredvbat) + String(", ") + String(stat) + String(", ") + sen5xString);
  //logfile.flush();                                                // Write to disk. Uses 2048 bytes of I/O to SD card, power and takes time
  
  // sleep cycle
  int sleepMS = 0;
  while ( sleepMS <= samplingRate ) {                                  // 124s = 8x16s sleep, only toggle display
    displayState = toggleButton(BUTTON_A, displayState, buttonAstate, lastTimeToggle, timeDebounce);
    if (displayState) { // On
      displaySensorData(measuredvbat);
    } else {       
      display.clearDisplay();
      display.display();
    };
    sleepMS += Watchdog.sleep();// remove comment for low power
    //delay(samplingRate);  // uncomment to debug because serial communication doesn't come back after sleeping
  }
}
