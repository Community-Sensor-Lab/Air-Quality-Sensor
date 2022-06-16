/*
  Write to Google Sheets through a Wifi POST HTTP1.1 request to a Google script.
*/
void payloadUpload(String payload) {
//  for (int i = 1; i < 4; i++) { // allways try to connect to wifi
    //    if (password != "") // if password is not empty
    //      wStatus = WiFi.begin(ssid, password);
    //    else
    //      wStatus = WiFi.begin(ssid);
    //    delay(500);
    Provisioning_Wifi();
//    while (WiFi.status() != WL_CONNECTED) {   //delete when this is no longer needed
//      digitalWrite(LEDPIN, HIGH);     delay(200);
//      digitalWrite(LEDPIN, LOW);      delay(200);
//    }


    if (WiFi.status() == WL_CONNECTED) {
      if (!client.connected()) {
        initializeClient();
      }
      Serial.print("payload: ");
      payload = payload_base + String("\"") + payload + String("\"}");
      Serial.println(payload);
      // Make a HTTP request:
      client.println(POSTCommand);
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
      // example url to GET from: See notes above on how to make a google spreadsheet to communicate with

      char url[] = "https://script.google.com/macros/s/AKfycbxG4kbBaP2-emw1eFy_rL06vR5Uc7oD4lBBwp7q5QKpF3KU_Q09/exec";
      char server[] = "www.google.com";

      char payload[] = "{\"command\":\"appendRow\",\"sheet_name\":\"Sheet1\",\"values\":\"1,2,3,4,5,6,7,8,9,10,11,12,13,14,15\"}";
      char HTTPreply[5000];
      GETcommand(HTTPreply, server, url);
      Serial.println("Reply:  ");
      Serial.println(HTTPreply);

      POSTcommand(HTTPreply, payload, server, url);
      Serial.println("Reply:  ");
      Serial.println(HTTPreply);

      client.stop();
      if (!client.connected()) {
        Serial.println("disconnected from server");
      };
      WiFi.end();
      //break;
    }
    //    else {
    //      Serial.print("Trying to connect to Wifi : "); Serial.println(i);
    //      //      WiFi.beginProvision();
    //      //      delay(200);
//  }
}
//  if (wStatus != WL_CONNECTED)
//    Serial.println("Continuing without WiFi");
//  //    WiFi.beginProvision();
//  //    delay(200);
//}

void initializeClient() {
  Serial.print("\nStarting connection to server... ");
  if (client.connectSSL(server, 443)) {      // if you get a connection, report back via serial:
    Serial.print("Connected to "); Serial.println(server);
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
