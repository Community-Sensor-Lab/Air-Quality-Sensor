
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
    Serial.println("Connect to AP wifi101-XXXXs");
    Serial.println("Configure SSID and password by visiting http://wifi101/ ");
  }

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LEDPIN, HIGH);     delay(500);
    digitalWrite(LEDPIN, LOW);      delay(500);
  }

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
}

void listNetworks() {
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  { Serial.println("Couldn't get a WiFi connection");
    while (true);
  }
  for (int thisNet = 0; thisNet < numSsid; thisNet++)
    Serial.println(WiFi.SSID(thisNet));
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
