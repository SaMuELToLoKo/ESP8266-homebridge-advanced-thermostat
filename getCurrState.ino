// Function to get Current State.

byte getCurrState(byte value) {

  byte current = 0;
  byte check = value;
  check &= B01110000; // First of all we delete all the target states
  
  switch (check) {
    case B00010000:
      current = 0; // "State Off".
      break;

    case B00100000:
      current = 1; // "State Heat".
      break;

    case B01000000:
      current = 2; // "State Cool".
      break;

    default:
      Serial.println("Error getting Current State data");
      break;
  }
return current;
}

