//NodeMCU Central Heating Home-Kit

#include <ESP8266WiFi.h>

#define outHeating 14 //D20 Pin activation for Central Heating
#define outCooler 12 //D6 Set pin to the Cooler option output
#define outWater 13 //D7 Pin activation for Heat Water

WiFiServer server(80); //Set server port
WiFiClient node; // Client to Get Temperature from a Node ESP8266

String readString;
const char* host = ""; // Ip of the target Node *This is used to ask to another node about the current Temperature*
const String url = "/dht"; // Url to get the tempature from another Node
float tarTemp = 22.0;
float thrTemp = 20.0;
float thrCool = 25.0;
float currTemp;
float currHum;
boolean flag = false; // Flag to set when the Thermostat has to activate it. This flag tell if has to use the Target or the Treshold to turn it on.
long tempTimer =0;
long tempWait = 50000;
byte state = B00000000;
byte unit = 0;

///// WiFi SETTINGS - Replace with your values /////////////////
const char* ssid = "Your SSID";
const char* password = "Your Password";
////////////////////////////////////////////////////////////////////



void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiStart();
  getTemperatureSet();
}

void loop() {

  //Reconnect on lost WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    WiFiStart();
  }

  WiFiClient client = server.available();

  if (!client) {
    if (millis() - tempTimer > tempWait) {
      getTemperatureSet();
    }
    centralHeatingStatus();
    delay(10);
    return;
  }

  while (client.connected() && !client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();


  // Match the request
  if (req.indexOf("") != -10) {  //checks if you're on the main page

    Serial.println(req);

  // Status, send the information the Homebridge require in JSON Format
    if (req.indexOf("/status") != -1) {
      byte tarState = getTarget(state);
      byte currState = getCurrState(state);
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("{\n" "\"currentState\": " + String(currState) + ",\n " "\"getTargetState\": " + String(tarState) + "" "\n}");
      delay(10);
      return;
    }
    
  // Info, send the information the Homebridge require in JSON Format
       if (req.indexOf("/info") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      client.println("{\n" "\"temperature\": " + String(currTemp) + ",\n " "\"tarTemperature\": " + String(tarTemp) + ",\n " "\"heatThreshold\": " + String(thrTemp) + ",\n " "\"coolThreshold\": " + String(thrCool) + ",\n " "\"humidity\": " + String(currHum) + ",\n " "\"units\": " + String(unit) + "" "\n}");
      delay(10);
      return;
    }

  // To set Target State
    if (req.indexOf("/setTargetState") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      int set = req.substring(21, req.indexOf("HTTP")).toInt();
      Serial.println(set);
      setTarget(set);
      flag = false;
      client.println(byte(state));
      delay(10);
      return;
    }
  // To set Target Temperature
    if (req.indexOf("/targetTemperature") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      float set = req.substring(24, req.indexOf("HTTP")).toFloat();
      Serial.println(set);
      tarTemp = set;
      flag = false;
      client.println(String(tarTemp));
      delay(10);
      return;
    }

    // To set Heat Threshold
    if (req.indexOf("/heatThreshold") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      float set = req.substring(20, req.indexOf("HTTP")).toFloat();
      Serial.println(set);
      thrTemp = set;
      client.println(String(thrTemp));
      delay(10);
      return;
    }

  // To set Cool Threshold
    if (req.indexOf("/coolThreshold") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      float set = req.substring(20, req.indexOf("HTTP")).toFloat();
      Serial.println(set);
      thrCool = set;
      client.println(String(thrCool));
      delay(10);
      return;
    }

  // Set Display Units
    if (req.indexOf("/units") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      byte set = req.substring(12, req.indexOf("HTTP")).toInt();
      Serial.println(set);
      unit = set;
      if (unit == 0) {
        float thrTemp = 20.0;
        float thrCool = 25.0;
      }
      else if (unit == 1) {
        float thrTemp = 68.0;
        float thrCool = 77.0;
      }
      flag = false;
      client.println(String(unit));
      delay(10);
      return;
    }

  //Set Target State via Web
    if (req.indexOf("/off") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state = (state & B11110000) | B00000001; // First of all we delete all the target states then we set up the new one, in this case "State Off".
      delay(10);
      return;
    }

  //Set Target State via Web
    if (req.indexOf("/heat") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state = (state & B11110000) | B00000010; // First of all we delete all the target states then we set up the new one, in this case "State Heat".
      delay(10);
      return;
    }
    
  //Set Target State via Web
    if (req.indexOf("/cool") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state = (state & B11110000) | B00000100; // First of all we delete all the target states then we set up the new one, in this case "State Cool".
      delay(10);
      return;
    }
    
  //Set Target State via Web
    if (req.indexOf("/auto") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state = (state & B11110000) | B00001000; // First of all we delete all the target states then we set up the new one, in this case "State Auto".
      delay(10);
      return;
    }
    
  //This Function is implemented but is not working on the Plugin yet.
    if (req.indexOf("/targetRelativeHumidity") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      delay(10);
      return;
    }

 // This is used to Control A Water system. Is a simply Switch.
    if (req.indexOf("/waterOn") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state |= B10000000; // Set up the Water State to On.
      delay(10);
      return;
    }

    if (req.indexOf("/waterOff") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      state &= B01111111; // Set up the Water State to off.
      delay(10);
      return;
    }

    if (req.indexOf("/waterStatus") != -1) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println();
      byte water = getWaterState(state);
      client.println(water);
      delay(10);
      return;
    }

  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;

  }

  //Web Platform**** I'm Still working in it.

  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<h1>Welcome to the Main page of the Central Heating Control Center.</h1><br>";
  s += "<br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Turn Central Heating Off \" onclick=\"location.href='/off'\">";
  s += "<input type=\"button\" name=\"bl\" value=\"Set Central Heating in Heat Mode\" \" onclick=\"location.href='/heat'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Set Central Heating in Cool Mode\" onclick=\"location.href='/cool'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Set Central Heating in Auto Mode\" onclick=\"location.href='/auto'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Turn Water Heating On\" onclick=\"location.href='/waterOn'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Turn Water Heating Off\" onclick=\"location.href='/waterOff'\">";
  s += "</html>\n";

  client.flush();
  
  // Send the response to the client
  client.print(s);

  delay(1);

}

