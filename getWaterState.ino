// Function to get Current State.

byte getWaterState(byte value) {

  byte water = 0;
  byte check = value;
  check &= B10000000; // First of all we delete all the target states
  
  switch (check) {
    case B00000000:
      water = 0; // "State Off".
      break;

    case B10000000:
      water = 1; // "State Heat".
      break;

    default:
      Serial.println("Error getting Current State data");
      break;
  }
return water;
}

