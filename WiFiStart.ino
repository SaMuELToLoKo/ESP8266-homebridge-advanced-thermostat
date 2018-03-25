//Initialitation and connection to the configured Router Station

void WiFiStart() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    server.begin();
  }
}
