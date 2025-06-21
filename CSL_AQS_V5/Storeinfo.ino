FlashStorage(flash_storage, Secrets);

/*!
* @brief check if there are credentials in flash memory
*/
bool check_valid(){
  Secrets info;
  info = flash_storage.read();
  return info.valid;
}

/*!
* @brief store the wifi and gsid credentials 
*/
void storeinfo(String &ssid, String &passcode, String &gsid) {
 
  Secrets info;
  info = flash_storage.read();

 
  if (info.valid == false) {

    String saved_ssid = ssid;
    String saved_passcode = passcode;
    String saved_gsid = gsid;
    saved_ssid.toCharArray(info.saved_ssid, 64);
    saved_passcode.toCharArray(info.saved_passcode, 64);
    saved_gsid.toCharArray(info.saved_gsid, 128);
    info.valid = true;
    flash_storage.write(info);
    display.clearDisplay();
    display.setCursor(0, 0);
    SERIAL_PORT_MONITOR.println();
    SERIAL_PORT_MONITOR.print("Your saved_ssid: ");
    display.print("Your saved ssid: ");
    SERIAL_PORT_MONITOR.println(info.saved_ssid);
    display.println(info.saved_ssid);
    //Uncomment when debugging
    SERIAL_PORT_MONITOR.print("and your saved_passcode: ");
    display.print("Your saved passcode: ");
    SERIAL_PORT_MONITOR.println(info.saved_passcode);
    display.println(info.saved_passcode);
    SERIAL_PORT_MONITOR.print("along with your saved_gsid: ");
    display.print("Your saved gsid: ");
    SERIAL_PORT_MONITOR.println(info.saved_gsid);
    display.println(info.saved_gsid);
    SERIAL_PORT_MONITOR.println("have been saved.");
    display.println("have been saved.");
    display.display();
    return;

  } else {

    //Allow user to force provisioning if they want to update the wifi information or if they have moved to a different location
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("If you would like to force provisioning, press button a");
    display.display();
    delay(2000);
    int i = 0;
    while (i < 80000){
      Serial.print(" ");
      if (force_pro){
        info.valid = false;
        flash_storage.write(info);
        AP_getInfo(ssid, passcode, gsid);
        return;
      }
      i++;
    }

    ssid = info.saved_ssid;
    passcode = info.saved_passcode;
    gsid = info.saved_gsid;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting to previously saved network...");
    display.print("Saved ssid: ");
    display.println(info.saved_ssid);
    //Uncommend during debug
    display.print("Saved passcode: ");
    display.println(info.saved_passcode);
    display.print("Saved gsid: ");
    display.println(info.saved_gsid);
    display.display();
    SERIAL_PORT_MONITOR.print("Your saved_ssid: ");
    SERIAL_PORT_MONITOR.println(info.saved_ssid);
    // Uncommend during debug
    SERIAL_PORT_MONITOR.println(info.saved_passcode);
    SERIAL_PORT_MONITOR.println(info.saved_gsid);
    delay(5000);
    return;
  }
}
