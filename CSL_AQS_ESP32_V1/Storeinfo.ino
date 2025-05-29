
#include "EEPROM.h"

typedef struct {
  boolean valid;
  char ssid[64];
  char passcode[64];
  char gsid[128];
} Secrets;

Secrets provisionInfo;

void initializeEEPROM() {

  Serial.print("Starting EEPROM ... ");
  if (!EEPROM.begin(512)) {
    Serial.println("Failed to initialize EEPROM. Restarting...");
    delay(1000);
    ESP.restart();
  }
  uint8_t val = EEPROM.readByte(0);

  if (val != 0xAA) {  // never been initialized
    memset(&provisionInfo, 0, sizeof(provisionInfo));
    EEPROM.writeByte(0, 0xAA);
    EEPROM.commit();
    Serial.printf("EEPROM initialized. value %x\n", val);
    return;
  }
  if (val == 0xAA) {
    Serial.printf("EEPROM has provisioning info that may work. value %x\n", val);
    return;
  }
}

bool check_valid() {
  if (EEPROM.readByte(0) == 0xAA) {
    return true;
  }
  return false;
}

void readProvisionInfo() {
  EEPROM.readBytes(0, &provisionInfo, sizeof(provisionInfo));
  return;
}

void storeProvisionInfo() {
  EEPROM.writeBytes(0, &provisionInfo, sizeof(provsionInfo));
  EEPROM.commit();
  return;
}





{

  //Since the variables in the parameter are passed by reference we are able to modify the global variables value directly
  //As such we can assign the saved values to the global variable that will then be used to connect to the wifi and the google sheet
  ssid = info.saved_ssid;
  passcode = info.saved_passcode;
  gsid = info.saved_gsid;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting to previously saved network...");
  display.print("Saved ssid: ");
  display.println(info.saved_ssid);
  display.print("Saved passcode: ");
  display.println(info.saved_passcode);
  display.print("Saved gsid: ");
  display.println(info.saved_gsid);
  display.display();
  Serial.println(info.saved_ssid);
  Serial.println(info.saved_passcode);
  Serial.println(info.saved_gsid);
  delay(5000);
  return;
}
