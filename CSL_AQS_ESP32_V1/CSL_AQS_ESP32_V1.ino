
/*
   COMMUNITY SENSOR LAB - AIR QUALITY SENSOR

   Adafruit Feather ESP32 V2 + Featherwing Adalogger-SD-RTC + SCD30 OR SCD41 -CO2 + BME280 -TPRH + OLED display + SEN55 -PM2.5 VOC NOX

   The SCD30 has a minimum power consumption of 5mA and cannot be stop-started. It's set to 55s (30s nominal)
   sampling period and the featherM0 sleeps for 2 x 16 =32s, wakes and waits for data available.
   Button A toggles display on/off but must be held down for 16s max and then wait 16s to toggle again.

   Logs: sensor, co2, 3x t, 3x rh, press, battery voltage, pm1, pm 2.5, pm 4.0, pm 10, voc, nox 

   https://github.com/Community-Sensor-Lab/Air-Quality-Sensor

   RICARDO TOLEDO-CROW NGENS, ESI, ASRC, CUNY, May 2025

*/

#include "CSL_AQS_ESP32_V1.h"

//Interrupt Handlers
void buttonA() {
  provisionInfo.valid = false;
}

void buttonB() {
  provisionInfo.noWifi = true;
}

void connectToWiFi() {
  // try to connect to wifi or continue without wifi
  Serial.printf("Trying to connect to wifi: %s\n", provisionInfo.ssid);
  Serial.printf("To force provisioning press button A\n");
  Serial.printf("To continue without wifi press button B\n");
  display.setCursor(0, 0);
  display.clearDisplay();
  display.printf("Connecting to wifi: \n%s\n", provisionInfo.ssid);
  display.printf("Provisioning: bttn A\n");
  display.printf("No wifi: bttn B\n");
  display.display();

  while (WiFi.status() != WL_CONNECTED && !provisionInfo.noWifi) {
    delay(10000);  // wait 10 in case forced provisioning

    if (!provisionInfo.valid) { // someone pressed button A
      Serial.println("\nGoing into provisioning mode");
      display.println("provisioning mode");
      display.display();
      
      softAPprovision();
    }

    if (provisionInfo.noWifi) { // someone pressed button B
      Serial.println("\nContinuing without wifi connection");
      display.println("no wifi mode");
      display.display();
      break;
    }

    // connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(provisionInfo.ssid, provisionInfo.passcode);
    Serial.println("Waiting to connect... ");
    display.println("Waiting to connect... ");
    display.display();

    while (WiFi.status() != WL_CONNECTED && !provisionInfo.noWifi && provisionInfo.valid) {
      Serial.print(".");
      delay(400);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf("Connected to wifi %s\n", provisionInfo.ssid);
      display.printf("wifi %s\n", provisionInfo.ssid);
      display.display();
      break;
    }
  }
}

void setup() {

  Serial.begin(115200);
  delay(5000);
  Serial.println(__FILE__);

  initializeSD();     // initializeSD has to come before initializeOLED or it'll crash
  initializeOLED();   // display
  initializeSEN55();  // PM VOC NOX sensor
  initializeSCD41();  // CO2
  //initializeSCD30(25);       // CO2 sensor to 30s more stable (1 min max recommended)
  initializeBME();  // TPRH
  initializeRTC();  // clock

  logfile.println(header);
  logfile.flush();

  // Set Interrupt on button A to force provisioning
  pinMode(BUTTON_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_A), buttonA, CHANGE);
  pinMode(BUTTON_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_B), buttonB, CHANGE);

  initializeEEPROM();
  provisionInfo.noWifi = false;  // we assume we will have wifi
  delay(5000);

  connectToWiFi();
  // // try to connect to wifi or continue without wifi
  // Serial.printf("Trying to connect to wifi: %s\n", provisionInfo.ssid);
  // Serial.printf("To force provisioning press button A\n");
  // Serial.printf("To continue without wifi press button B\n");
  // display.setCursor(0, 0);
  // display.clearDisplay();
  // display.printf("Connecting to wifi: \n%s\n", provisionInfo.ssid);
  // display.printf("Provisioning: bttn A\n");
  // display.printf("No wifi: bttn B\n");
  // display.display();

  // while (WiFi.status() != WL_CONNECTED && !provisionInfo.noWifi) {
  //   delay(10000);  // wait 10 in case forced provisioning

  //   if (!provisionInfo.valid) {
  //     Serial.println("\nGoing into provisioning mode");
  //     display.println("provisioning mode");
  //     display.display();
  //     softAPprovision();
  //   }

  //   if (provisionInfo.noWifi) {
  //     Serial.println("\nContinuing without wifi connection");
  //     display.println("no wifi mode");
  //     display.display();
  //     break;
  //   }

  //   WiFi.mode(WIFI_STA);
  //   WiFi.begin(provisionInfo.ssid, provisionInfo.passcode);
  //   Serial.println("Waiting to connect... ");
  //   display.println("Waiting to connect... ");
  //   display.display();

  //   while (WiFi.status() != WL_CONNECTED && !provisionInfo.noWifi && provisionInfo.valid) {
  //     Serial.print(".");
  //     delay(400);
  //   }

  //   if (WiFi.status() == WL_CONNECTED) {
  //     Serial.printf("Connected to wifi %s\n", provisionInfo.ssid);
  //     display.printf("wifi %s\n", provisionInfo.ssid);
  //     display.display();
  //     break;
  //   }
  // }
  delay(5000);
}

void loop() {

  String bme = readBME();
  delay(300);

  String sen55 = readSEN55();
  String scd41 = readSCD41();
  DateTime now = rtc.now();  // fetch the date + time

  pinMode(VBATPIN, INPUT);  // read battery voltage
  sensorData.Vbat = float(analogReadMilliVolts(VBATPIN) * 2.0 / 1000.00);
  pinMode(BUTTON_A, INPUT_PULLUP);

  char tstring[128];
  sprintf(tstring, "%02u/%02u/%02u %02u:%02u:%02u, ",
          now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());

  String tempString = String(tstring) + bme + scd41 + sen55 + String(sensorData.Vbat);

  Serial.println(header);
  Serial.println(tempString);

  logfile.println(tempString);
  logfile.flush();

  // Serial.printf("tbme: %f, pbme: %f, rhbme %f\n", Tbme,Pbme,RHbme);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("T: %.2f C\nP: %.2f mBar\nRH: %.2f%%\n", sensorData.Tbme, sensorData.Pbme, sensorData.RHbme);
  display.printf("CO2: %d ppm\nPM2.5: %.2f ug/m^3\nVOCs: %.2f\n", sensorData.CO2, sensorData.mPm2_5, sensorData.VOCs);
  display.printf("Bat: %.2f V\n", sensorData.Vbat);
  display.display();

  if (!provisionInfo.valid) {
    softAPprovision();
    connectToWiFi();
  }

  delay(5000);
}
