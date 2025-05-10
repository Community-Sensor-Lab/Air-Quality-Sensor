
void initializeRTC(){
  Serial.print("starting RTC... ");
  Wire.begin();  
  if (!rtc.begin()) {
    Serial.println("RTC Failed");
    stat = stat | 0x04; // 3rd bit set 'rtc not started'
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