void getTemperatureSet() {
  
  WiFiClient node;
  if (node.connect(host, 80))
  {
    Serial.println("Connected");
    Serial.print("Requesting URL: ");
    Serial.println(String(host) + url);

    node.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    delay(10);
    Serial.println("Respond:");
  }
  String line = "";
  bool begin = false;
  while (node.available() || !begin) {
    char in = node.read();

    if (in == '{') {
      begin = true;
    }

    if (begin) line += (in);

    if (in == '}') {
      Serial.println(line);
      break;
    }
  }
  int tempStart = line.indexOf("temperature\":") + 14;
  int tempEnd = tempStart + 6;
  String tempS = line.substring(tempStart, tempEnd);
  int humStart = line.indexOf("humidity\":") + 11;
  int humEnd = humStart + 6;
  String humS = line.substring(humStart, humEnd);
  currTemp = tempS.toFloat();
  currHum = humS.toFloat();
  tempTimer = millis();
  delay(10);
}
