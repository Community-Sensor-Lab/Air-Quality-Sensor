/*  Sensirion SPS20 PM sensor.
      functions:
      - void initializeSCD30()
      - String readSCD30(float Pbme) where Pbme is the ambient pressure from bme sensor
      It returns a string with the CO2 ppm, T C, 
     concentration for 0.5um, 1um, 2.5um, 10um and average particle size in um
*/

void initializeOLED()  {
  Serial.println("starting 128x64 OLED... ");
  if (!display.begin(0x3C, true)) // Address 0x3C for 128x32
    Serial.println(F("SH110X  allocation failed"));
  else  {
    Serial.println("OLED Connected");
    display.println("OLED Connected");
    display.display();
    display.setRotation(1);

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.display();
  }
   
}

bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce )  {
  if (digitalRead(button))  {
    buttonState = true;
    return state;
  }
  else if (buttonState && millis() - prevTime > debounce) {
    buttonState = false;
    prevTime = millis();
    return !state;
  }
  else
    return state;
}
