#include "CSL_AQS_V5.h"
#include "Arduino.h"

static const char CSL_AQS_V4p5::webpage_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Community Sensor Lab provisioning page</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h1>NGENS Air Quality Sensor</h1>
  <form action="/get">
    ssid: <input type="text" name="SSID"><br>
    <!-- <input type="submit" value="Submit">
  </form><br>
  <form action="/get"> -->
    passcode: <input type="password" name="passcode"><br>
    <!-- <input type="submit" value="Submit">
   </form><br>
 <form action="/get"> -->
    gsid: <input type="text" name="GSID"><br>
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";




/*!
*   @brief class constructor
*/
CSL_AQS_V4p5::CSL_AQS_V4p5(){}


// OLED SH1107

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::initializeOLED(){
  Serial.println("Starting 128x64 OLED... ");
  if (!display.begin(0x3C, true)) // Address 0x3C for 128x32
    Serial.println(F("SH110X  allocation failed"));
  else  {
    Serial.println("OLED Connected");
    display.println("OLED Connected");
    display.display();
    display.setRotation(1);

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.display();
  }
}

/*!
  *   @brief 
  */
bool CSL_AQS_V4p5::toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce ){
  if (digitalRead(button))  {
    buttonState = true;
    return state;
  }
  else if (buttonState && millis() - prevTime > debounce) {
    buttonState = false;
    prevTime = millis();
    return !state;
  }
  else
    return state;
}


// SCD30

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::initializeSCD30(){
  
}

String CSL_AQS_V4p5::readSCD30(float Pbme) {
  int counter = 0;
  if (scd30.isConnected()) { 
    scd30.setAmbientPressure(Pbme);                             // update CO2 sensor to current pressure
    while (!scd30.dataAvailable()) {                            // dataAvailable() returns true when data is available
      delay(1000);
      if (counter > 61){ 
        stat |= 0x05;                                       
        break;
      }
      else
        counter += 1;
    }

    CO2scd30 = scd30.getCO2();
    float Tco2 = scd30.getTemperature();
    float RHco2 = scd30.getHumidity();
    return (String(CO2scd30) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
  }
  else {
    stat |= 0x04;
    return ("None,None,None,");
  }
}

// SCD41

/*!
  *   @brief 
  */  
void CSL_AQS_V4p5::initializeSCD41(){
  Serial.println("Starting SCD41... ");
  if (!scd41.begin(false, false, false)){                                                 // Do not start periodic measurements
    stat |= 0x06;
    Serial.println(F("SCD41 not detected. Please check wiring. Freezing..."));
    display.println("SCD41 not detected");
    display.display();
  }
  else {
    Serial.println(F("SCD41 Connected"));
    display.println("SCD41 Connected");
    display.display();
  }
}


String CSL_AQS_V4p5::readSCD41(){
  int counter = 0;
  if(scd41.measureSingleShot()){                                                          // Request fresh data (should take 5 seconds)
    while (!scd41.readMeasurement()){                                                     // readMeasurement() Check for fresh data
      delay(1000);
      if (counter > 61){ 
        stat |= 0x07;
        break;
      }
      else
        counter += 1;
    }

    CO2scd41 = scd41.getCO2();
    float Tco2 = scd41.getTemperature();
    float RHco2 = scd41.getHumidity();
    return (String(CO2scd41) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
  }
  else {
    return ("None,None,None,");
  }
}
 
// BME280

/*!
  *   @brief
  */
void CSL_AQS_V4p5::initializeBME280(){

}

String CSL_AQS_V4p5::readBME280(){
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
  
// SEN 5x

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::initializeSen5x(){

}

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::initializeRTC(){
  Serial.println("Starting RTC... ");
  Wire.begin();  
  if (!rtc.begin()) {
    stat |= 0x03;
    Serial.println("RTC Failed");
    display.println("RTC Not Started..");
    display.display();
  }
  else
    Serial.println("RTC Connected");
    display.println("RTC Connected");
    display.display();
}

// Provisioning 

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::printWiFiStatus(){
   Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print(F("Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connect to SSID: ");
  display.println(WiFi.SSID());
  display.display();
}

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::printMacAddress(byte mac[]){
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16)
      Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0)
      Serial.print(":");
  }
  Serial.println();
}

/*!
*   Makes AP and, when client connected, serves the 
*   web page with entry fields. The fields are 
*   returned in the parameters.
*   
*   @param ssid     - a String to place the ssid
*   @param passcode - a String to place the passcode
*   @param gsid     - a String to place the gsid
*/
void CSL_AQS_V4p5::AP_getInfo(String &ssid, String &passcode, String &gsid){
  WiFiServer server(80);
  WiFiClient client;
  Serial.println(F("Access Point Web Server"));

  status = WiFi.status();
  if (status == WL_NO_SHIELD) {
    Serial.println(F("WiFi shield not present"));
    while (true)
      ;
  }

  makeMACssidAP("ngens_sensor");
  delay(1000);
  printWiFiStatus();

  while (true) {  
   
    if (status != WiFi.status()) { 
      status = WiFi.status();
      if (status == WL_AP_CONNECTED) {
        byte remoteMac[6];
        Serial.print(F("Device connected to AP, MAC address: "));
        WiFi.APClientMacAddress(remoteMac);
        printMacAddress(remoteMac);

        Serial.println(F("Starting server"));
        server.begin();
        // print where to go in a browser:
        IPAddress ip = WiFi.localIP();
        Serial.print(F("To provide provisioning info, open a browser at http://"));
        Serial.println(ip);
        Serial.println(ip);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("Provision access");
        display.print("at http://");
        display.println(ip);
        display.display();


      } else {
        // a device has disconnected from the AP, and we are back in listening mode
        Serial.println(F("Device disconnected from AP"));
        display.println("Device disconnected");
        display.display();
        client.stop();
      }
    }

    client = server.available();  

    if (client) {                   
      Serial.println(F("New client")); 
      String currentLine = "";      
      while (client.connected()) {  

        if (client.available()) { 
          char c = client.read();  
          Serial.write(c);         

          if (c == '\n') {  
           
            if (currentLine.length() == 0) {
              client.println(F("HTTP/1.1 200 OK"));
              client.println(F("Content-type:text/html"));
              client.println();
              client.print(webpage_html);  
              client.println();
              break;                   
            } else {                                   
              if (currentLine.startsWith("GET /get?")) {  
                int ssidIndx = currentLine.indexOf("SSID=");
                int passcodeIndx = currentLine.indexOf("passcode=");
                int gsidIndx = currentLine.indexOf("GSID=");
                int httpIndx = currentLine.indexOf(" HTTP");
                ssid = currentLine.substring(ssidIndx + 5, passcodeIndx - 1);
                passcode = currentLine.substring(passcodeIndx + 9, gsidIndx - 1);
                gsid = currentLine.substring(gsidIndx + 5, httpIndx);

                client.stop();
                Serial.println("Client disconnected\n");
                WiFi.end();
                delay(5000);
                status = WiFi.status();
                storeinfo(ssid, passcode, gsid);
                return; 
              }
              currentLine = ""; 
            }
          } else if (c != '\r') {  
            currentLine += c;     
          }
        }  
      }    
      client.stop();
      Serial.println(F("Client disconnected"));

    }  
  }    
}

/*!
*   Create an AP with a unique ssid formed with a string
*   and the last 2 hex digits of the board MAC address.
*   By default the local IP address of will be 192.168.1.1
*   you can override it with the following:
*   WiFi.config(IPAddress(10, 0, 0, 1));
*
*   @param startString a string to preface the ssid
*/
void CSL_AQS_V4p5::makeMACssidAP(String startString){
 byte localMac[6];

  Serial.print(F("Device MAC address: "));
  WiFi.macAddress(localMac);
  printMacAddress(localMac);

  char myHexString[3];
  sprintf(myHexString, "%02X%02X", localMac[1], localMac[0]);
  // EDIT LOCAL SERVER NAME BY CHANGING SSID
  String ssid = startString + String((char *)myHexString);

  Serial.print(F("Creating access point: "));
  Serial.println(ssid);
  status = WiFi.beginAP(ssid.c_str());

  if (status != WL_AP_LISTENING) {
    Serial.println(F("Creating access point failed"));
    while (true)
      ;
  }
}

// WIFI

/*!
  *   @brief  
  *   @param
  *   @param
  */  
void CSL_AQS_V4p5::initializeClient(){
  Serial.println("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {     
    Serial.println("Connected to ");
    Serial.println(server);
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }
  else {
    Serial.print("Not connected to ");
    Serial.println(server);
  }
  Serial.println("End intializeClient");
}

/*!
  *   @brief 
  */
void CSL_AQS_V4p5::payloadUpload(String payload){
  for (int i = 1; i < 4; i++) { 
    if (passcodeg != "") 
      status = WiFi.begin(ssidg, passcodeg);
    else
      status = WiFi.begin(ssidg);
    delay(500);

    if (WiFi.status() == WL_CONNECTED) {
      if (!client.connected()) {
        initializeClient();
      }
      Serial.print("Payload: ");
      payload = payload + String("\"}");
      Serial.println(payload);
      client.println(String("POST /macros/s/") + String(gsidg) + String("/exec HTTP/1.1"));
      client.println("Host: script.google.com");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(payload.length());
      client.println();
      client.print(payload);
      client.println();
      delay(200);

      if (client.available())
        Serial.println("Response: ");
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }

      client.stop();
      if (!client.connected()) {
        Serial.println("Disconnected from server");
      };
      WiFi.end();
      break;
    }
    else {
      Serial.print("Trying to connect to Wifi : "); Serial.println(i);
    }
  }
  if (status != WL_CONNECTED)
    Serial.println("Continuing without WiFi");
}

