/*!
/* @brief   starts a simple webpage with three fields to enter and send info
*/

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

/*!
*   @brief decodes url encoded values
*   
*/
String urlDecode(String input) {
  String decoded = "";
  char temp[] = "0x00"; // to hold the hex value

  for (unsigned int i = 0; i < input.length(); i++) {
    if (input[i] == '%') { // If a '%' is found, decode the following two hex characters
      if (i + 2 < input.length()) {
        temp[2] = input[i + 1];
        temp[3] = input[i + 2];
        decoded += (char) strtol(temp, NULL, 16); // Convert hex to character
        i += 2;
      }
    } else if (input[i] == '+') { // Convert '+' to space
      decoded += ' ';
    } else {
      decoded += input[i]; // Append normal characters
    }
  }
  return decoded;
}


/*!
*   @brief  prints information of the wifi the micro controller is connected to. 
*/

void printWiFiStatus() {
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connect to SSID: ");
  display.println(WiFi.SSID());
  display.display();
}

/*!
*   Prints the micro controllers mac address
* 
*   @param mac     - an array containing the microcontrollers mac address
*/

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16)
      Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i > 0)
      Serial.print(":");
  }
  Serial.println();
}

/**
*   Makes AP and, when client connected, serves the 
*   web page with entry fields. The fields are 
*   returned in the parameters.
*   
*   @param ssid     - a String to place the ssid
*   @param passcode - a String to place the passcode
*   @param gsid     - a String to place the gsid
*/
void AP_getInfo(String &ssid, String &passcode, String &gsid) {
  WiFiServer server(80);
  WiFiClient client;
  Serial.println(F("Access Point Web Server"));

  status = WiFi.status();
  if (status == WL_NO_SHIELD) {
    Serial.println(F("WiFi shield not present"));
    while (true)
      ;
  }

  makeMACssidAP("csl");
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
      Serial.println(F("new client")); 
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
              
                ssid = urlDecode(currentLine.substring(currentLine.indexOf("SSID=") + 5, currentLine.indexOf("passcode=") - 1));
                passcode = urlDecode(currentLine.substring(currentLine.indexOf("passcode=") + 9, currentLine.indexOf("GSID=") - 1));
                gsid = urlDecode(currentLine.substring(currentLine.indexOf("GSID=") + 5, currentLine.indexOf(" HTTP")));

                client.stop();
                Serial.println("client disconnected\n");
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

/**
*   Create an AP with a unique ssid formed with a string
*   and the last 2 hex digits of the board MAC address.
*   By default the local IP address of will be 192.168.1.1
*   you can override it with the following:
*   WiFi.config(IPAddress(10, 0, 0, 1));
*
*   @param startString a string to preface the ssid
*/
void makeMACssidAP(String startString) {

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