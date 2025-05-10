/*
  Write to Google Sheets via Wi-Fi using an HTTP 1.1 request to a Google Apps Script.

  Functions:
  - void payloadUpload(String payload):
    Uploads data to Google Sheets using a POST request to a Google Apps Script endpoint.
    - payload (String): The data to be uploaded, formatted as a string.
    - The function tries to connect to Wi-Fi, then sends a POST request with the data as the payload.
    - The request is sent to a Google Apps Script (GSID) using SSL on port 443.
    - The response from the server is printed to the Serial Monitor.

  - void initializeClient():
    Establishes a secure connection to the Google Apps Script server using SSL.
    - The client connects to the server (`script.google.com`) and listens for a response.

  Notes:
  - `ssidg` and `passcodeg` are Wi-Fi credentials (SSID and password).
  - `gsidg` is the Google Apps Script ID used in the URL to access your script.
  - The script is expected to accept HTTP POST requests, and `value=Hello` is just an example.
  - The code tries to connect to Wi-Fi three times. If successful, it uploads the payload and prints the server response.
  - The client uses SSL (port 443) for secure communication.

*/

void payloadUpload(String payload) {
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
      Serial.print("payload: ");
      payload = payload + String("\"}");
      Serial.println(payload);

      // Make a HTTP request:
      client.println(String("POST /macros/s/") + String(gsidg) + String("/exec HTTP/1.1"));
      client.println("Host: script.google.com");
      client.println("Content-Type: application/x-www-form-urlencoded");
      //client.println("Connection: close");
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
        Serial.println("disconnected from server");
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

void initializeClient() {
  Serial.println("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {      // if you get a connection, report back via serial:
    Serial.print("Connected to ");
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
  Serial.println("end intializeClient");
}
