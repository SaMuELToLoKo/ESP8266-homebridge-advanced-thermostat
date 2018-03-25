// Function to set the system off

void offFunction() {

 if ((state & B00010000) == B00000000) {
  analogWrite(outHeating, 0);
  analogWrite(outCooler, 0);
  state = (state & B10001111) | B00010000;
 }
}

