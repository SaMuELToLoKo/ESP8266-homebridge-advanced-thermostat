// Funtion to set up auto mode

void autoFunction() {

  if (!flag) {
    flag = true;
  }

  if (currTemp > tarTemp) {
    coolFunction();
  }
  else if (currTemp < tarTemp) {
    heatFunction();
  }

  else {
    offFunction();
  }
}

