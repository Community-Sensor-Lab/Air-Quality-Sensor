/*************************************************

Microcontroller -  Adafruit Feather M0 + WiFi ATWINC1500


Small sample of dynamically updating sensor variables using an App Script

Sensor -> request(sends POST)
Sensor <- response(Appscript sends a ContentService object)

Content Service object is a redirect and needs to be handled by the sensor.

*************************************************/
// Dependencies
#include <SPI.h>
#include <WiFi101.h>


int count = 0;
// Global variables
static int samplingRate = 10000;
char server_google_script[] = "script.google.com"; 
char server_google_usercontent[] = "script.googleusercontent.com"; 

char ssidg[] = "pass";
char passcodeg[] = "pass";

//String payload = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":\"from, my, sensor, ";
char gsidg[] = "";
uint8_t status; 
WiFiSSLClient client;   


// Max number of redirects before stopping
const int maxRedirects = 3;

void httpGet(String server, String url) {
  Serial.println("Calling httpGet...");
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: " + String(server));
  client.println();  // End of headers
  delay(2000);

  // Capture Server Response
  if (client.available())
    Serial.println("GET Response: ");
  String response = "";
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
    response += c;
  }
  client.stop();
  delay(100);
  handleResponse(response);
}

// POST method to Google App Script
void httpPost(String payload,String server, char gsidg[]) {
  Serial.println("Calling httpPost...");
  // Send Request
  client.println(String("POST /macros/s/") + String(gsidg) + String("/exec HTTP/1.1"));
  client.println("Host: "+ String(server));
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);
  client.println();
  delay(2000);

  // Capture Response
  if (client.available())
    Serial.println("POST Response: ");
  String response = "";
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
    response += c;
  }
  client.stop();
  delay(100);
  handleResponse(response);
}

// Handle GET or POST response
void handleResponse(String response) {
  Serial.println("Calling handleResponse...");
  // Handle successful response (200 OK)
  if (response.indexOf("200 OK") != -1) {
    Serial.println("Successfully fetched data from google AppScript");
    Serial.println("Not good");
    Serial.println(response);
    
  }
  // Handle Redirect (302 Moved Temporarily)
  else if (response.indexOf("302 Moved Temporarily") != -1) {
    Serial.println("Handling 302 Moved Temporarily redirect ...");

    // Extract the "Location" header (URL to redirect to)
    int locIndex = response.indexOf("Location: ");
    if (locIndex != -1) {
      // Extract the URL after "Location: "
      int endIndex = response.indexOf('\n', locIndex);
      if (endIndex == -1) endIndex = response.length();  // Fallback if no newline found
      String locationURL = response.substring(locIndex + 9, endIndex);
      locationURL.trim();  // Clean up the URL
      int pathIndex = locationURL.indexOf(".com");
      if (pathIndex == -1) {
         Serial.println("Invalid URL format");
        return;
      }

      // Handle the redirect with a GET request
      String pathAndQuery = locationURL.substring(pathIndex + 4);  // Skip ".com"
      
      client.stop();  // Force fresh connection
      delay(100);

      Serial.println("\nConnecting to redirected server...");
      initializeClient(server_google_usercontent);
      delay(200);  // Allow TLS setup


      
      httpGet(server_google_usercontent, pathAndQuery);
     
    } else {
      Serial.println("No Location header found.");
      
    }
  }
  else{
    Serial.println("Unrecognized response code");
  
    
  }
    

}

// Initialize connection to the server
void initializeClient(char server[]) {
  Serial.println("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {
    Serial.println("Connected to: ");
    Serial.println(server);
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  } else {
    Serial.print("Not connected to: ");
    Serial.println(server);
  }
  Serial.println("End initializeClient");
  Serial.println();
  delay(100);
}

// Upload sensor data to the Google Apps Script
void payloadUpload(String payload) {
  Serial.println("Calling payloadUpload...");
  // Try to connect to Wi-Fi
  // for (int i = 0; i < 5; i++) {
  //   status = WiFi.begin(ssidg, passcodeg);
  //   int retry = 0;
  //   while (WiFi.status() != WL_CONNECTED && retry++ < 10) {
  //     delay(1000);
  //   }
  //   if (WiFi.status() == WL_CONNECTED) break;
  // }

  if (WiFi.status() == WL_CONNECTED) {
    while (!client.connected()) {
      Serial.println("Connecting to server...");
      initializeClient(server_google_script);
      delay(100);
    }

    // Send data to Google Apps Script
    httpPost(payload,server_google_script, gsidg);
    // WiFi.end();
  } else {
    Serial.println("Wi-Fi not connected. Continuing without Wi-Fi.");
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  WiFi.setPins(8, 7, 4, 2);  // Set pins for Feather M0 Wi-Fi

  for (int i = 0; i < 5; i++) {
    status = WiFi.begin(ssidg, passcodeg);
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry++ < 10) {
      delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED) break;
  }

  Serial.print("Connected to wifi? ");
  Serial.println(WiFi.status() == WL_CONNECTED);

  Serial.print("samplingRate: ");
  Serial.println(samplingRate);

  

}

void loop() {

 
  Serial.println(count);
  // TESTTING doPOST()

  String p = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":\"a,b,c,d," + String(count) +"\", \"srate\":" + String(samplingRate)+"}";
  Serial.print(p);
  payloadUpload(p);
  
  Serial.print("samplingRate: ");
  
  Serial.println(samplingRate);
  count += 1;
  delay(10000);  // Delay to simulate periodic uploads
  while(WiFi.status() != WL_CONNECTED);
}
