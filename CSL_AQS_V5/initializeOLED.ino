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
    pinMode(BUTTON_C, INPUT_PULLUP);

    //Set Interrupt
    attachInterrupt(digitalPinToInterrupt(BUTTON_A), A, CHANGE);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.display();
  }
}

/*!
* @brief
* @details 
*/
bool toggleButton(uint8_t button, bool state, bool& buttonState, int& prevTime, int debounce) {
  if (digitalRead(button)) {
    buttonState = true;
    return state;
  } else if (buttonState && millis() - prevTime > debounce) {
    buttonState = false;
    prevTime = millis();
    return !state;
  } else
    return state;
}


// Utilities 

/*!
* @brief Interrupt Handler
*/
void A() {
  force_pro = true;
}

/*!
* @brief Display Sensor Data for scd40, bme280, sen5x
*/
void displaySensorData(float measuredvbat) {
  display.setCursor(0, 0);
  display.print("CO2");
  display.setCursor(40, 0);
  display.print(CO2scd41);
  display.print(" ppm");
  display.print("  ");
  display.print(measuredvbat, 2);
  display.print("V");


  display.setCursor(0, 8);
  display.print("T");
  display.setCursor(40, 8);
  display.print(Tbme, 2);
  display.print("C");

  display.setCursor(0, 16);
  display.print("P");
  display.setCursor(40, 16);
  display.print(Pbme, 2);
  display.print(" mBar");

  display.setCursor(0, 24);
  display.print("RH");
  display.setCursor(40, 24);
  display.print(RHbme, 0);
  display.print("%");

  display.setCursor(0, 32);
  display.print("VOC");
  display.setCursor(40, 32);
  display.print(vocIndex, 2);

  display.setCursor(0, 40);
  display.print("NOX");
  display.setCursor(40, 40);
  display.print(noxIndex, 2);

  display.setCursor(0, 48);
  display.print("Pm 2.5");
  display.setCursor(40, 48);
  display.print(massConcentrationPm2p5, 2);

  display.display();
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("CO2");
  display.setCursor(40, 0);
  display.print(CO2scd41);
  display.print(" ppm");
  display.print("  ");
  display.print(measuredvbat, 2);
  display.print("V");


  display.setCursor(0, 8);
  display.print("T");
  display.setCursor(40, 8);
  display.print(Tbme, 2);
  display.print("C");

  display.setCursor(0, 16);
  display.print("P");
  display.setCursor(40, 16);
  display.print(Pbme, 2);
  display.print(" mBar");

  display.setCursor(0, 24);
  display.print("RH");
  display.setCursor(40, 24);
  display.print(RHbme, 0);
  display.print("%");

  display.setCursor(0, 32);
  display.print("VOC");
  display.setCursor(40, 32);
  display.print(vocIndex, 2);

  display.setCursor(0, 40);
  display.print("NOX");
  display.setCursor(40, 40);
  display.print(noxIndex, 2);

  display.setCursor(0, 48);
  display.print("Pm 2.5");
  display.setCursor(40, 48);
  display.print(massConcentrationPm2p5, 2);

  display.display();
}