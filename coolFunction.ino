// Function to set up the cooler system

void coolFunction() {

  if ((state & B00100000) == B00100000) {
    analogWrite(outHeating, 0);
    state &= B11011111;
  }

  if (currTemp > tarTemp && !flag && ((state & B01000000) == B00000000)) {
    flag = true;
    analogWrite(outCooler, 1024);
    state = (state & B10001111) | B01000000;
  }

  else if (currTemp >= thrCool && flag && ((state & B01000000) == B00000000)) {
    analogWrite(outCooler, 1024);
    state = (state & B10001111) | B01000000;
  }

  else if (currTemp <= tarTemp && ((state & B01000000) == B01000000)) {
    analogWrite(outCooler, 0);
    state = (state & B10001111) | B00010000;
  }
}

