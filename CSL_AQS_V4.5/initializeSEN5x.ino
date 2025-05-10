/*  Sensirion SPS20 PM sensor.
      functions:
      - void initializeSCD30()
      - String readSCD30(float Pbme) where Pbme is the ambient pressure from bme sensor
      It returns a string with the CO2 ppm, T C, 
     concentration for 0.5um, 1um, 2.5um, 10um and average particle size in um
*/


String readSen5x(){
    uint16_t error;
    char errorMessage[256];

    delay(1000);

    error = sen5x.readMeasuredPmValues(
    massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
    massConcentrationPm10p0, numberConcentrationPm0p5, numberConcentrationPm1p0,
    numberConcentrationPm2p5, numberConcentrationPm4p0, numberConcentrationPm10p0,
    typicalParticleSize);

    if (error) {
        Serial.print("Error trying to execute readMeasuredValues(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        
        return "";
    } 
    else{
        String sen5xString = String(massConcentrationPm1p0) + String(", ") + String(massConcentrationPm2p5) + String(", ") + 
        String(massConcentrationPm4p0) + String(", ") + String(massConcentrationPm10p0) + String(", ") + String(numberConcentrationPm0p5) + 
        String(",") + String(numberConcentrationPm1p0) + String(",") + String(numberConcentrationPm2p5) + String(",") + String(numberConcentrationPm4p0) + 
        String (",") + String(numberConcentrationPm10p0) + String(",") + String(typicalParticleSize) + String(",") +
        String(ambientHumidity) + String(", ") + String(ambientTemperature) + String(", ") + String(vocIndex) + String(", ") + String(noxIndex)+ String(", ");
        return sen5xString;
    }
}

void initializeSen5x(){
    Serial.print("starting Sen5x... ");
    Serial.begin(115200);
    
    // while (!Serial) {
    //     delay(100);
    // }

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

    // Start Measurement
    error = sen5x.startMeasurement();
    if (error) {
        Serial.print("Error trying to execute startMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

}