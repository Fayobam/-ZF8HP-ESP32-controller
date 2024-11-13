
void shiftSolenoids(int pwmPinOut, int pwmPinIn, int targetDutyOut, int targetDutyIn, int durationOut, int durationIn, int delayBetween) {
  unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  // Read the current duty cycles
  int startDutyOut = ledcRead(pwmPinOut);
  int startDutyIn = ledcRead(pwmPinIn);
  
  switch (shiftStage) {
    case 0:
      ledcFade(pwmPinOut, startDutyOut, targetDutyOut, durationOut);
      previousMillis = currentMillis;
      shiftStage++;
      break;
    case 1:
      if (currentMillis - previousMillis >= delayBetween) {
        ledcFade(pwmPinIn, startDutyIn, targetDutyIn, durationIn);
        shiftStage++;
      }
      break;
    case 2:
      // Shifting complete
      shiftStage = 0;
      break;
  }

  // Print debug information
  Serial.print("Shifting solenoids: ");
  Serial.print("Out pin: "); Serial.print(pwmPinOut);
  Serial.print(" In pin: "); Serial.print(pwmPinIn);
  Serial.print(" StartDutyOut: "); Serial.print(startDutyOut);
  Serial.print(" TargetDutyOut: "); Serial.print(targetDutyOut);
  Serial.print(" StartDutyIn: "); Serial.print(startDutyIn);
  Serial.print(" TargetDutyIn: "); Serial.print(targetDutyIn);
  Serial.print(" DurationOut: "); Serial.print(durationOut);
  Serial.print(" DurationIn: "); Serial.print(durationIn);
  Serial.print(" DelayBetween: "); Serial.println(delayBetween);
}


// Example shift with ASOL and BSOL
 // shiftSolenoids(ASOL, BSOL, 0, 255, 2000, 2000, 500);  // Adjust target duty cycle and duration values as needed


