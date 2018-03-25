// Process to set target up.

void setTarget(int st) {

  switch (st) {
    case 0:
      state = (state & B11110000) | B00000001; // First of all we delete all the target states then we set up the new one, in this case "State Off".
      break;

    case 1:
      state = (state & B11110000) | B00000010; // First of all we delete all the target states then we set up the new one, in this case "State Heat".
      break;

    case 2:
      state = (state & B11110000) | B00000100; // First of all we delete all the target states then we set up the new one, in this case "State Cool".
      break;

    case 3:
      state = (state & B11110000) | B00001000; // First of all we delete all the target states then we set up the new one, in this case "State Auto".
      break;

    default:
      Serial.println("Error getting the data");
      break;
  }

}
