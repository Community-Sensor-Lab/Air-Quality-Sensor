
void initializeRTC(){
  Serial.println("starting RTC... ");
  Wire.begin();  
  if (!rtc.begin()) {
    stat |= 0x03;
    Serial.println("RTC Failed");
    display.println("RTC Not Started..");
    display.display();
  }
  else
    Serial.println("RTC Connected");
    display.println("RTC Connected");
    display.display();

  /***
  TO SET TIME at compile: run once to syncro then run again with line commented out.

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  **/
}