/*!
* @brief
*/
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
}