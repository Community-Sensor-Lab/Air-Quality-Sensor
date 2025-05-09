



void initializeRTC(){
  Serial.print("starting RTC... ");
  Wire.begin();  // connect to RTC
  if (!rtc.begin()) {
    Serial.println("RTC failed");
//    display.clearDisplay();
//    display.setCursor(0, 0);
    display.println("RTC Not ]");
    display.display();
  }
  else
    Serial.println("RTC Connected");
    display.println("RTC Connected");
    display.display();
  // TO SET TIME at compile: run once to syncro then run again with line commented out
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}