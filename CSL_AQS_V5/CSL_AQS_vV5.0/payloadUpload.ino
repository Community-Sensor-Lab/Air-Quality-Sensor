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
  Serial.println("end intializeClient");
}
