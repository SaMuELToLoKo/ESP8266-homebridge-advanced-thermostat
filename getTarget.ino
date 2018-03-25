// Function to get Target

byte getTarget(byte value) {

  byte target = 0;
  byte check = value;
  check &= B00001111; // First of all we delete all the target states
  
  switch (check) {
    case B00000001:
      target = 0; // "State Off".
      break;

    case B00000010:
      target = 1; // "State Heat".
      break;

    case B00000100:
      target = 2; // "State Cool".
      break;

    case B00001000:
      target = 3; // "State Auto".
      break;

    default:
      Serial.println("Error getting Target data");
      break;
  }
return target;
}

