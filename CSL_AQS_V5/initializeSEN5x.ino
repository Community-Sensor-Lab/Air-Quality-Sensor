/*!
* @brief Initialize Sen5x
* @param
*/

void initializeSen5x(){
    Serial.println("starting Sen5x... ");
    Serial.begin(115200);
    Wire.begin();
    sen5x.begin(Wire);

    uint16_t error;
    char errorMessage[256];
    error = sen5x.deviceReset();
    if (error) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    float tempOffset = 0.0;

    error = sen5x.setTemperatureOffsetSimple(tempOffset);
    if (error) {
        Serial.print("Error trying to execute setTemperatureOffsetSimple(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        Serial.print("Temperature Offset set to ");
        Serial.print(tempOffset);
        Serial.println(" deg. Celsius (SEN54/SEN55 only");
    }

    error = sen5x.startMeasurement();
    if (error) {
        Serial.print("Error trying to execute startMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println(F("SEN5X Connected"));
    display.println("SEN5X Connected");
    display.display();
}


/*!
* @brief Read meassurement from Sen5X
* @param 
* @details Meassurement reads:
    massConcentrationPm1p0,
    massConcentrationPm4p0,
    massConcentrationPm10p0,
    numberConcentrationPm0p5,
    numberConcentrationPm1p0,
    numberConcentrationPm2p5,
    numberConcentrationPm4p0,
    numberConcentrationPm10p0,
    typicalParticleSize,
    ambientHumidity,
    ambientTemperature
*/
String readSen5x(){
    float massConcentrationPm1p0 = 0;
    float massConcentrationPm4p0 = 0;
    float massConcentrationPm10p0 = 0;
    float numberConcentrationPm0p5 = 0;
    float numberConcentrationPm1p0 = 0;
    float numberConcentrationPm2p5 = 0;
    float numberConcentrationPm4p0 = 0;
    float numberConcentrationPm10p0 = 0;
    float typicalParticleSize = 0;
    float ambientHumidity = 0;
    float ambientTemperature = 0;

    uint16_t error;
    char errorMessage[256];

    delay(1000);

    // TODO: MAKE A PULL REQUEST TO EDIT READMEASSUMENT OPTIONS
    error = sen5x.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
        noxIndex);

    if (error) {
        Serial.print("Error trying to execute readMeasuredValues(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    error = sen5x.readMeasuredPmValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, numberConcentrationPm0p5, numberConcentrationPm1p0,
        numberConcentrationPm2p5, numberConcentrationPm4p0, numberConcentrationPm10p0,
        typicalParticleSize);

    if (error) {
    Serial.print("Error trying to execute readMeasuredValues(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    } 

    // massConcentrationPm1p0, massConcentrationPm2p5,
    // massConcentrationPm4p0,massConcentrationPm10p0, 
    // numberConcentrationPm0p5, numberConcentrationPm1p0,
    // numberConcentrationPm2p5, numberConcentrationPm4p0 
    // numberConcentrationPm10p0, typicalParticleSize,
    // ambientHumidity, ambientTemperature, vocIndex, noxIndex,
    
    String sen5xString = String(massConcentrationPm1p0) + String(", ") + String(massConcentrationPm2p5) + String(", ") + 
                         String(massConcentrationPm4p0) + String(", ") + String(massConcentrationPm10p0) + String(", ") +
                         String(ambientHumidity) + String(", ") + String(ambientTemperature) + String(", ") +
                         String(vocIndex) + String(", ") + String(noxIndex) + String(",") + String(numberConcentrationPm0p5) +  String(",") +
                         String(numberConcentrationPm1p0) + String(",") + String(numberConcentrationPm2p5) + String(",") +
                         String(numberConcentrationPm4p0) + String (",") + String(numberConcentrationPm10p0) + String(",") + 
                         String(typicalParticleSize);
                         
    return sen5xString;
    
}

