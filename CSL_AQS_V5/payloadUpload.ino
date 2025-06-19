
/*!
* @brief Makes an http get request and handles the response message by calling handleResponse()
**/
void httpGet(String server, String url) {
  //Serial.println("Calling httpGet...");
  client.println("GET " + url + " HTTP/1.1");
  client.println("Host: " + String(server));
  client.println("Connection: close");
  client.println();

  response = "";
  unsigned long lastRead = millis();
  const unsigned long timeout = 1700; // 3 seconds

  while (client.connected() || client.available()) {
    if (client.available()) {
      char c = client.read();
      response += c;
      lastRead = millis(); // reset timer on data received
    }

    if (millis() - lastRead > timeout) {
      Serial.println("Response timeout. Exiting read loop.");
      break;
    }
  }

  delay(200);  // Allow TLS setup
  handleResponse();
}


/*!
* @brief Makes an http post request and handles the response message by calling handleResponse()
**/
void httpPost(String payload, char server[], String gsidg) {
  //Serial.println("Calling httpPost...");
  client.println(String("POST /macros/s/") + String(gsidg) + String("/exec HTTP/1.1"));
  client.println("Host: "+ String(server));
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);
  
  response = "";
  unsigned long lastRead = millis();
  const unsigned long timeout = 3000; // 3 seconds

  while (client.connected() || client.available()) {
    if (client.available()) {
      char c = client.read();
      response += c;
      lastRead = millis(); // reset timer on data received
    }

    if (millis() - lastRead > timeout) {
      Serial.println("Response timeout. Exiting read loop.");
      break;
    }
  }  

  delay(200);  // Allow TLS setup
  handleResponse();
}


/*!
* @brief Handles response from httpGet() and httPost()
**/
void handleResponse() {
  if (response.indexOf("200 OK") != -1) {
    //Serial.println("Successfully fetched data from google AppScript");
    int srateIndex = response.indexOf("srate:");
    if (srateIndex != -1) {
      int valueStart = srateIndex + 6; // Skip past "srate:"
      int valueEnd = response.indexOf('\n', valueStart); // Find the end of the line
      String rateStr = response.substring(valueStart, valueEnd);
      samplingRate = rateStr.toInt();

      Serial.print("Parsed sampling rate: ");
      Serial.println(samplingRate);
    
    }
  }
  // Handle Redirect (302 Moved Temporarily)
  else if (response.indexOf("302 Moved Temporarily") != -1) {
    //Serial.println("Handling 302 Moved Temporarily redirect ...");

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
      
    
      initializeClient(server_google_usercontent);
      httpGet(server_google_usercontent, pathAndQuery);
    } else {
      Serial.println("No Location header found.");
    }
  }
  else{
    Serial.println("Unrecognized response code");
    Serial.print(response);
  }
}


/*!
* @brief
* @param
*/
void payloadUpload(String payload) {
  Serial.print(payload);
  for (int i = 1; i < 4; i++) { 
    if (passcodeg != "") 
      status = WiFi.begin(ssidg, passcodeg);
    else
      status = WiFi.begin(ssidg);
    delay(500);

    if (WiFi.status() == WL_CONNECTED) {
      
      while (!client.connected()) {
        Serial.println("Connecting to server...");
        initializeClient(server_google_script);
        delay(100);
      }

      // Send data to Google Apps Script
      httpPost(payload,server_google_script, gsidg);

      // if (!client.connected()) {
      //   initializeClient();
      // }
      // Serial.print("payload: ");
      // payload = payload + String("\"}");
      // Serial.println(payload);
      // client.println(String("POST /macros/s/") + String(gsidg) + String("/exec HTTP/1.1"));
      // client.println("Host: script.google.com");
      // client.println("Content-Type: application/x-www-form-urlencoded");
      // client.print("Content-Length: ");
      // client.println(payload.length());
      // client.println();
      // client.print(payload);
      // client.println();
      // delay(200);

      // if (client.available())
      //   Serial.println("Response: ");
      // while (client.available()) {
      //   char c = client.read();
      //   Serial.write(c);
      // }

      // client.stop();
      // if (!client.connected()) {
      //   Serial.println("disconnected from server");
      // };
      WiFi.end();
      break;
    }
    else {
      Serial.print("Trying to connect to Wifi : "); 
      Serial.println(i);
    }
  }
  if (status != WL_CONNECTED)
    Serial.println("Continuing without WiFi");
}


/*!
* @brief Starts SSL connection to server
* 
*/
void initializeClient(char server[]) {
  Serial.println("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {     
    Serial.println("Connected to ");
    Serial.println(server);
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    Serial.println();
  }
  else {
    Serial.print("Not connected to ");
    Serial.println(server);
  }
}
