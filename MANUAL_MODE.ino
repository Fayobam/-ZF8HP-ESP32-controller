
void manual() {
    
   
    unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
    unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
    unsigned long currentTime = millis();
    
    int upButtonState = digitalRead(upButtonPin);
    int downButtonState = digitalRead(downButtonPin);
    

    if (upButtonState == LOW && (currentTime - lastDebounceTime) > debounceDelay) {
      shiftUp();
      lastDebounceTime = currentTime;
    }

    if (downButtonState == LOW && (currentTime - lastDebounceTime) > debounceDelay) {
      shiftDown();
      lastDebounceTime = currentTime;
    }
  }



void shiftUp() {
  switch (targetGear) {
    case 0:
      shiftN1();
      targetGear = 1;
      break;
    case 1:
      shift12();
      targetGear = 2;
      break;
    case 2:
      shift23();
      targetGear = 3;
      break;
    case 3:
      shift34();
      targetGear = 4;
      break;
    case 4:
      shift45();
      targetGear = 5;
      break;
    case 5:
      shift56();
      targetGear = 6;
      break;
    case 6:
      shift67();
      targetGear = 7;
      break;
    case 7:
      shift78();
      targetGear = 8;
      break;
    default:
      Serial.println("Already in highest gear");
      break;
  }
  Serial.print("Shifted up to gear: ");
  Serial.println(targetGear);
}

void shiftDown() {
  switch (targetGear) {
    case 8:
      shift87();
      targetGear = 7;
      break;
    case 7:
      shift76();
      targetGear = 6;
      break;
    case 6:
      shift65();
      targetGear = 5;
      break;
    case 5:
      shift54();
      targetGear = 4;
      break;
    case 4:
      shift43();
      targetGear = 3;
      break;
    case 3:
      shift32();
      targetGear = 2;
      break;
    case 2:
      shift21();
      targetGear = 1;
      break;
    default:
      Serial.println("Already in lowest gear");
      break;
  }
  Serial.print("Shifted down to gear: ");
  Serial.println(targetGear);
}

