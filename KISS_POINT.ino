void kpointA() {
  int kissPointValuesA[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64}
  };
  int kissPointValueA = kissPointValuesA[modIndex][rpmIndex]; // Retrieve value from array
  ledcWrite(ASOL, kissPointValueA);
}

void kpointB() {
  int kissPointValuesB[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64}
  };
  int kissPointValueB = kissPointValuesB[modIndex][rpmIndex]; // Retrieve value from array
  ledcWrite(BSOL, kissPointValueB);
}

void kpointC() {
  int kissPointValuesC[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64}
  };
  int kissPointValueC = kissPointValuesC[modIndex][rpmIndex]; // Retrieve value from array
  ledcWrite(CSOL, kissPointValueC);
}

void kpointD() {
  int kissPointValuesD[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64}
  };
  int kissPointValueD = kissPointValuesD[modIndex][rpmIndex]; // Retrieve value from array
  ledcWrite(DSOL, kissPointValueD);
}

void kpointE() {
  int kissPointValuesE[8][8] = {
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64},
    {64, 64, 64, 64, 64, 64, 64, 64}
  };
  int kissPointValueE = kissPointValuesE[modIndex][rpmIndex]; // Retrieve value from array
  ledcWrite(ESOL, kissPointValueE);
}
