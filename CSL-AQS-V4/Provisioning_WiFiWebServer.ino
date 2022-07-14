
char mdnsName[] = "wifi101"; // the MDNS name that the board will respond to
// after WiFi settings have been provisioned
// Note that the actual MDNS name will have '.local' after
// the name above, so "wifi101" will be accessible on
// the MDNS name "wifi101.local".

//WiFiServer server(80);

// Create a MDNS responder to listen and respond to MDNS name requests.
WiFiMDNSResponder mdnsResponder;

void Provisioning_Wifi() {
  WiFi.setPins(8, 7, 4, 2);
  display.clearDisplay();
  display.setCursor(0, 0);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // configure the LED pin for output mode
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  listNetworks(); //starts scanning for network


  // Start in provisioning mode:
  //  1) This will try to connect to a previously associated access point.
  //  2) If this fails, an access point named "wifi101-XXXX" will be created, where XXXX
  //     is the last 4 digits of the boards MAC address. Once you are connected to the access point,
  //     you can configure an SSID and password by visiting http://wifi101/

  WiFi.beginProvision();

  if (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0, 0);

    //to connect to WiFi connect to WiFi name wifi101... Visit http://wifi101/ 
    display.println("To connect to WiFi ");
    display.println("connect to ");
    display.println("WiFi name: wifi101... ");
    display.println("Visit http://wifi101/");
    Serial.println("Connect to AP wifi101-XXXXs");
    //display.println("If yes, connect to AP wifi101-XXXXs"); // Connect to WiFi name "wifi101..."
    //display.display();
    //display.println("Visit http://wifi101/ .Next login WiFi ");
    //Serial.println("Configure SSID and password by visiting http://wifi101/ ");
    //display.println("Login to wifi at http://wifi101/ ");
    //display.display();
    display.println("");
    display.println("Else, hold button A ");
    display.println("for 2 seconds");
    //display.println("Until blinking light stops");
    display.display();
  } 


  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LEDPIN, HIGH);     delay(500);
    digitalWrite(LEDPIN, LOW);      delay(500);
//    int buttonRead = digitalRead(BUTTON_A);
//    if (buttonRead != lastButtonState)lastDebounceTime = millis(); // reset the debouncing timer
//
//    if ((millis() - lastDebounceTime) > debounceDelay) {
//      // whatever the reading is at, it's been there for longer than the debounce
//      // delay, so take it as the actual current state:
//
//      // if the button state has changed:
//      if (buttonRead != currentButtonState) {
//        currentButtonState = buttonRead;
//
//      }
//    }
    if (toggleButton(BUTTON_A, pressState, buttonAstate, lastTimeToggle, timeDebounce)) {  //only occurs if button is pressed
      usewifi = false;
      display.clearDisplay();
      //display.println("Not Connecting");
      //display.display();
      break;
    }
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    //lastButtonState = buttonRead;
  }

  /*
    ADD CODE SUCH THAT THE SENSOR STILL RUNS WITHOUT WIFI

  */

  // Setup the MDNS responder to listen to the configured name.
  // NOTE: You _must_ call this _after_ connecting to the WiFi network and
  // being assigned an IP address.
  if (!mdnsResponder.begin(mdnsName)) {
    Serial.println("Failed to start MDNS responder!");
    while (1);
  }

  Serial.print("Server listening at http://");
  Serial.print(mdnsName);
  Serial.println(".local/");

  printWiFiStatus();
  //WiFi.end();  //uncomment this when things work as intended
}

void listNetworks() {
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  { Serial.println("Couldn't get a WiFi connection");
    while (true);
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Available Networks: ");
  display.display();
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.println(WiFi.SSID(thisNet));
    display.println(WiFi.SSID(thisNet));
    display.display();
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());


  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
