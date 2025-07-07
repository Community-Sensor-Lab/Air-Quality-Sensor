/*!
* @brief Initialize OLEd and button A interrupt
*/
void initializeOLED() {
  
  Serial.println("starting 128x64 OLED... ");
  if (!display.begin(0x3C, true))  // Address 0x3C for 128x32
    Serial.println(F("SH110X  allocation failed"));
  else {
    Serial.println("OLED Connected");
    display.println("OLED Connected");
    display.display();
    display.setRotation(1);

    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    //pinMode(BUTTON_C, INPUT_PULLUP);

    //Set Interrupt
    attachInterrupt(digitalPinToInterrupt(BUTTON_A), A, CHANGE);
    //attachInterrupt(digitalPinToInterrupt(BUTTON_B), B, CHANGE);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Hello World");
    display.display();
  }
}

// Utilities 

/*!
* @brief Interrupt Handler To force provisioning
*/
void A() {
  force_pro = true;
 
}

/*!
* @brief Clear screen once
* @details 
*/

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


/*!
* @brief Display Sensor Data for scd40, bme280, sen5x
*/
void displaySensorData(float measuredvbat) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("T: %.2f C\nP: %.2f mBar\nRH: %.2f%%\n", Tbme, Pbme, RHbme);
  display.printf("CO2: %d ppm\nPM2.5: %.2f ug/m^3\nVOCs: %.2f\nNOX: %.2f", CO2scd41, massConcentrationPm2p5 , vocIndex, noxIndex);
  display.printf("\nBat: %.2f V", measuredvbat);
  display.display();
}
