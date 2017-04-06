/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

//for LED status
#include <Ticker.h>
Ticker ticker;

MDNSResponder mdns;

const char* devicename = "Son_of_a_switch";

ESP8266WebServer server(80);

//String webPage = "";

const int gpio13Led = 13;
const int gpio12Relay = 12;

void tick()
{
  //toggle state
  int state = digitalRead(gpio13Led);  // get the current state of GPIO1 pin
  digitalWrite(gpio13Led, !state);     // set pin to the opposite state
}


//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void resetSettings() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.reset();
  delay(1000);
}

void setup(void) {
  Serial.begin(115200);
  delay(500);

  ticker.attach(0.6, tick);
  WiFiManager wifiManager;
  attachInterrupt(digitalPinToInterrupt(0), resetSettings, FALLING);
  if (!wifiManager.autoConnect(devicename)) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  ticker.detach();

  // preparing GPIOs
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);

  pinMode(gpio12Relay, OUTPUT);
  digitalWrite(gpio12Relay, HIGH);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin(devicename, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/", []() {
    if(digitalRead(gpio12Relay) == HIGH) {
      server.send(200, "application/json", "{\"on\":true}");
    } else {
      server.send(200, "application/json", "{\"on\":false}");
    }
  });
  server.on("/state", []() {
    if(digitalRead(gpio12Relay) == HIGH) {
      server.send(200, "application/json", "{\"on\":true}");
    } else {
      server.send(200, "application/json", "{\"on\":false}");
    }
  });
  server.on("/on", []() {
    turnOn();
    server.send(200, "application/json", "{\"on\":true}");
    delay(500);
  });
  server.on("/off", []() {
    turnOff();
    server.send(200, "application/json", "{\"on\":false}");
    delay(500);
  });
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Start up & set up completed");
}

void turnOff() {
    digitalWrite(gpio13Led, HIGH);
    digitalWrite(gpio12Relay, LOW);
}

void turnOn() {
    digitalWrite(gpio13Led, LOW);
    digitalWrite(gpio12Relay, HIGH);
}

void loop(void) {
  server.handleClient();
}
