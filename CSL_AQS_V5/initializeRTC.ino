/*!
* @brief Starts Real time clock
* @details The real time clock works on battery and it helps keep track of time even after the micro controller is disonnected from a power source.
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