/*  Sensirion SPS20 PM sensor.
      functions:
      - void initializeSCD30()
      - String readSCD30(float Pbme) where Pbme is the ambient pressure from bme sensor
      It returns a string with the CO2 ppm, T C, 
     concentration for 0.5um, 1um, 2.5um, 10um and average particle size in um
*/

// String readSCD30(float Pbme) {
//   int ctr = 0;
//   if (!(stat & 0x08)) { // if bit 3 not set sensor is present so read values
//     stat &= 0xEF; //  clear bit 4 for timeout
//     scd41.setAmbientPressure(Pbme); // update CO2 sensor to current pressure
//     // wait for data avail on CO2 sensor
//     while (!scd41.dataAvailable()) {
//       delay(1000);
//       if (ctr > 61) {  // timeout is 61s
//         stat |= 0x10; // set bit 4 timeout
//         break;
//       }
//       else
//         ctr += 1;
//     }
//     CO2 = scd41.getCO2();
//     float Tco2 = scd41.getTemperature();
//     float RHco2 = scd41.getHumidity();
//     return (String(CO2) + String(", ") + String(Tco2) + String(", ") + String(RHco2) + String(", "));
//   }
//   else  // sensor is not present
//     return (String("no SCD30,,,"));
// }

// void initializeSCD30(int samplingInterval)  {
//   Serial.print("starting SCD30... ");
//   if (scd41.begin() == false) {
//     Serial.println("SCD30 Not Detected");
//     stat |= 0x08; // set bit 3
//     display.println("SCD30 Not Detected");display.display();
//   }
//   else  {
//     Serial.println("SCD30 ok");
//     display.println("SCD30 Connected");display.display();
//     stat &= 0xF7; // clear bit 3
//     scd41.setAmbientPressure(0);      // 0=deactivate press. correction. Default 1013.25 mBar
//     scd41.setAutoSelfCalibration(false);     // de-activate Automatic Self-Calibraton
//     //CO2sensor.setForcedRecalibrationFactor(453);
//     scd41.setMeasurementInterval(samplingInterval); // Change measurement interval seconds 2-1800
//     delay(1000);
//   }
// }
