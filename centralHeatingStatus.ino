/*
    Main void of Central Heating Process
    Check the byte state '1' represent the state described.
    XXXX0001 = TARGET OFF
    XXXX0010 = TARGET HEAT
    XXXX0100 = TARGET COOL
    XXXX1000 = TARGET AUTO
    X001XXXX = CURRENT OFF
    X010XXXX = CURRENT HEAT
    X100XXXX = CURRENT COOL
    1XXXXXXX = WATER STATE
*/
void centralHeatingStatus() {

//Target off
  if ((state & B00000001) == B00000001) {
    offFunction();
  }
//Target Heat
  else if ((state & B00000010) == B00000010) {
    heatFunction();
  }
//Target Cool
  else if ((state & B00000100) == B00000100) {
    coolFunction();
  }
//Target Auto
  else if ((state & B00001000) == B00001000) {
    autoFunction();
  }

//Target Water
  if ((state & B10000000) == B10000000) {
    analogWrite(outWater, 1024);
  }
  else {
    analogWrite(outWater, 0);
  }

}
