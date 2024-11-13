//FUNCTION FOR HANDLING SHIFTS
// fon means fuly on, foff fully off, TKO takeoff clutch, kpoint kisspoint
/*PARK     P   A   C
           NEUTRAL  N   A   C
                    1   A B C
                    2   A B E
                    3   B C E
                    4   B D E
                    5   B C D
                    6   C D E
                    7   A C D
                    8   A D E
           REVERSE  R   A B D      */
void park() {
  // GEARP A C
  
  if (vehicleSpeed <1 && currentGear == 0){
  ledcWrite(PARK, 0);// APPLY THE PARK PAWL
  }

  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(BSOL, 255);    // Clutch B OFF (static) ********
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);    // Clutch D OFF (static)
  ledcWrite(ESOL, 255);    // Clutch E OFF (static)
}


void reverse() {
  ledcWrite(PARK, 255);  // DISABLE THE PARK PAWL

  if (vehicleSpeed <1 && currentGear == 0)
  kpointB();  // Kiss point for clutch B

  // GEAR_REVERSE A B* CR A B D
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcFade(BSOL, 0, 255, 150); // Clutch B ON with fade in 150ms
  ledcWrite(CSOL, 0);    // Clutch C OFF (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(ESOL, 0);    // Clutch E OFF (static)

}


void neutral() {
  ledcWrite(PARK, 255);// DISABLE THE PARK PAWL
  
  kpointB();  // Kiss point for clutch B
  
  // GEAR_NEUTRAL A B* C
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(BSOL, 0);    // Clutch B OFF (static), at kisspoint
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);    // Clutch D OFF (static)
  ledcWrite(ESOL, 255);    // Clutch E OFF (static)
}

///////////////SHIFTING TO DRIVE/////////////////////////////
void shiftN1() {
  // GEAR1 A B C
  int shiftTimeB[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };
  int durationB = shiftTimeB[tpsIndex][rpmIndex];

  kpointB();

  // FROM GEARN A B* C TO GEAR1 A B C
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcFade(BSOL, 0, 255, durationB);  // Clutch B ON with fade using duration from array
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 0);    // Clutch D OFF (static)
  ledcWrite(ESOL, 0);    // Clutch E OFF (static)
}




///////////////////////////////////UPSHIFTS////////////////////////////////
void shift12() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationC = shiftTimeC[tpsIndex][rpmIndex];
  int durationE = shiftTimeE[tpsIndex][rpmIndex];

  // Kiss point for clutch E
  kpointE();

  // Configure static clutches directly
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(DSOL, 0);    // Clutch D OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(CSOL, ESOL, 0, 255, durationC, durationE, releaseDelayMs);
}

void shift23() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeA[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationA = shiftTimeA[tpsIndex][rpmIndex];
  int durationC = shiftTimeC[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(ESOL, 255);  // Clutch E ON (static)
  ledcWrite(DSOL, 0);    // Clutch D OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ASOL, CSOL, 0, 255, durationA, durationC, releaseDelayMs);
}

void shift34() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeD[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationC = shiftTimeC[tpsIndex][rpmIndex];
  int durationD = shiftTimeD[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(ESOL, 255);  // Clutch E ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(CSOL, DSOL, 0, 255, durationC, durationD, releaseDelayMs);
}

void shift45() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationE = shiftTimeE[tpsIndex][rpmIndex];
  int durationC = shiftTimeC[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ESOL, CSOL, 0, 255, durationE, durationC, releaseDelayMs);
}

void shift56() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeB[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationB = shiftTimeB[tpsIndex][rpmIndex];
  int durationE = shiftTimeE[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(BSOL, ESOL, 0, 255, durationB, durationE, releaseDelayMs);
}

void shift67() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeA[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationE = shiftTimeE[tpsIndex][rpmIndex];
  int durationA = shiftTimeA[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(BSOL, 0);    // Clutch B OFF (static)
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ESOL, ASOL, 0, 255, durationE, durationA, releaseDelayMs);
}

void shift78() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationC = shiftTimeC[tpsIndex][rpmIndex];
  int durationE = shiftTimeE[tpsIndex][rpmIndex];

  // Configure static clutches directly
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(BSOL, 0);    // Clutch B OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(CSOL, ESOL, 0, 255, durationC, durationE, releaseDelayMs);
}

//////////////////////////////////DOWNSHIFTS///////////////////////////////

void shift87() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationE = shiftTimeE[tpsIndex][rpmIndex];
  int durationC = shiftTimeC[tpsIndex][rpmIndex];

  // Kiss point for clutch C
  kpointC();

  // Configure static clutches directly
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(BSOL, 0);    // Clutch B OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ESOL, CSOL, 0, 255, durationE, durationC, releaseDelayMs);
}

void shift76() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeA[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationA = shiftTimeA[tpsIndex][rpmIndex];
  int durationE = shiftTimeE[tpsIndex][rpmIndex];

  // Kiss point for clutch E
  kpointE();

  // Configure static clutches directly
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(BSOL, 0);    // Clutch B OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ASOL, ESOL, 0, 255, durationA, durationE, releaseDelayMs);
}

void shift65() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeB[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationE = shiftTimeE[tpsIndex][rpmIndex];
  int durationB = shiftTimeB[tpsIndex][rpmIndex];

  // Kiss point for clutch B
  kpointB();

  // Configure static clutches directly
  ledcWrite(CSOL, 255);  // Clutch C ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ESOL, BSOL, 0, 255, durationE, durationB, releaseDelayMs);
}

void shift54() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationC = shiftTimeC[tpsIndex][rpmIndex];
  int durationE = shiftTimeE[tpsIndex][rpmIndex];

  // Kiss point for clutch E
  kpointE();

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(DSOL, 255);  // Clutch D ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(CSOL, ESOL, 0, 255, durationC, durationE, releaseDelayMs);
}

void shift43() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeD[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationD = shiftTimeD[tpsIndex][rpmIndex];
  int durationC = shiftTimeC[tpsIndex][rpmIndex];

  // Kiss point for clutch C
  kpointC();

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(ESOL, 255);  // Clutch E ON (static)
  ledcWrite(ASOL, 0);    // Clutch A OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(DSOL, CSOL, 0, 255, durationD, durationC, releaseDelayMs);
}

void shift32() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeA[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationC = shiftTimeC[tpsIndex][rpmIndex];
  int durationA = shiftTimeA[tpsIndex][rpmIndex];

  // Kiss point for clutch A
  kpointA();

  // Configure static clutches directly
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(ESOL, 255);  // Clutch E ON (static)
  ledcWrite(DSOL, 0);    // Clutch D OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(CSOL, ASOL, 0, 255, durationC, durationA, releaseDelayMs);
}

void shift21() {
  // Populate shift time arrays based on load (TPS) and RPM
  int shiftTimeE[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  int shiftTimeC[8][8] = {
    {400, 400, 400, 400, 400, 400, 300, 200},
    {400, 400, 400, 400, 400, 300, 300, 200},
    {400, 400, 400, 400, 300, 300, 200, 200},
    {400, 400, 400, 300, 300, 200, 200, 200},
    {400, 400, 300, 300, 200, 200, 200, 200},
    {400, 300, 300, 200, 200, 200, 200, 200},
    {300, 300, 200, 200, 200, 200, 200, 200},
    {200, 200, 200, 200, 200, 200, 200, 200}
  };

  // Retrieve durations
  int durationE = shiftTimeE[tpsIndex][rpmIndex];
  int durationC = shiftTimeC[tpsIndex][rpmIndex];

  // Kiss point for clutch C
  kpointC();

  // Configure static clutches directly
  ledcWrite(ASOL, 255);  // Clutch A ON (static)
  ledcWrite(BSOL, 255);  // Clutch B ON (static)
  ledcWrite(DSOL, 0);    // Clutch D OFF (static)
  
  // Apply active PWM clutches simultaneously with a small delay
  int releaseDelayMs = 50; // Delay in milliseconds for staging
  
  // Perform the shift
  shiftSolenoids(ESOL, CSOL, 0, 255, durationE, durationC, releaseDelayMs);
}

