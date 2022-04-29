
void POSTcommand(char* reply, char* payload, char* server, char* url)  {
  // url e.g. https://script.google.com/macros/s/AKfycbxG4kbBaP2-emw1eFy_rL06vR5Uc7oD4lBBwp7q5QKpF3KU_Q09/exec

  // PARSE THE URL for Host and Path to make HTTP POST Request
  // get Host from url. script.google.com from example above
  char* p1 = strstr(url, "https://") + 8; // find where hostname starts
  char* p2 = strchr(p1, '/'); // look for next '/' after start of hostname for end of hostname
  char host[128] = "Host: ";
  strncpy(host + 6, p1, (long)(p2 - p1)); // copy hostname into hostString after "Host: "
  host[(long)(p2 - p1) + 6] = 0; // terminate string with 0 very important
  char POSTstring[256] = "POST ";
  strncpy(POSTstring + 5, p2, strlen(p2)); // copy path from url after "GET "
  strncpy(POSTstring + 5 + strlen(p2), " HTTP/1.1\0", 10); // terminate with http etc and a 0
  Serial.println(POSTstring);
  Serial.println(host);
  Serial.println(payload);

  // PLACE POST Request
  Serial.print("Starting connection to server... ");
  WiFiSSLClient client;
  if (client.connect(server, 443)) {
    Serial.print("connected to server...");

    client.println(POSTstring);
    client.println(host);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(strlen(payload));
    client.println();
    client.print(payload);
    client.println();

    int indx = 0;
    while (client.connected()) {
      if (client.available())
        reply[indx++] = client.read();
    }
    reply[indx] = 0; // terminate string
  }
  else {
    Serial.println("No connection to server. stopping.");
    while (true);
  }
  Serial.println("disconnecting from server");
  client.stop();
  Serial.println("-----------------------------");

  // PARSE REPLY FOR FURTHER ACTION
  int httpStatus = atoi(reply + 8); // get http status from beginning of reply
  Serial.print("httpStatus: ");
  Serial.print(httpStatus);
  switch (httpStatus) {
    case 302:  // redirect
      Serial.print(" redirecting to...\n");
      p1 = strstr(reply, "Location: ") + 10; // parsing location
      p2 = strchr(p1, 10);
      char urll[512]; // can be long
      strncpy(urll, p1, (long)(p2 - p1));
      urll[(long)(p2 - p1)] = 0;
      Serial.println(urll);
      GETcommand(reply, server, urll); // recursion with new urll, same reply buffer
      break;
    case 200:
      Serial.print(" OK\n");
      break;
    default:
      Serial.print(" something else\n");
  }
}
