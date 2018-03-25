// Function to set up the Heating system

void heatFunction() {
  
  if ((state & B01000000) == B01000000) {
    analogWrite(outCooler, 0);
    state = (state & B10101111);
  }

  if (currTemp < tarTemp && !flag && ((state & B00100000) == B00000000)) {
    flag = true;
    analogWrite(outHeating, 1024);
    state = (state & B10001111) | B00100000;
  }

  else if (currTemp <= thrTemp && flag && ((state & B00100000) == B00000000)) {
    analogWrite(outHeating, 1024);
    state = (state & B10001111) | B00100000;
  }

  else if (currTemp >= tarTemp && ((state & B00100000) == B00100000)) {
    analogWrite(outHeating, 0);
    state = (state & B10001111) | B00010000;
  }

}

