#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* serverUrl = "http://your-django-server.com/api/relay-command/";

const int relayPin1 = D5;
const int relayPin2 = D6;

bool relay1Status = false;
bool relay2Status = false;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);

  WifiConnect();
}

void loop() {
  CheckRelay();

  delay(1000);
}
 
void WifiConnect() {
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  Wifi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serail.println("-----------------------------");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void CheckRelay(){
  if (Wifi.status() == WL_CONNECTED ) {
    HTTPClient http;
    http.begin(serverUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0 ) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      
    } else {
      Serial.println("ERROR on HTTP requset...");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

void ControlRelay(String jsonResponse) {
  StaticJsonDocument<200> jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, jsonResponse);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* relay1Command = jsonDoc["relay1"];
  const char* relay2Command = jsonDoc["relay2"];

  ControlRelay1(relay1Command);
  ControlRelay2(relay2Command);

}

void Relay_1(const char* command) {
  if (strcmp(command, "START") == 0) {
    relay1Status = true;
    digitalWrite(relayPin1, HIGH);
  } else if (strcmp(command, "STOP") == 0) {
    relay1Status = false;
    digitalWrite(relayPin1, LOW);
  }
}

void Relay_2(const char* command) {
  if (strcmp(command, "START") == 0) {
    relay2Status = true;
    digitalWrite(relayPin2, HIGH);
  } else if (strcmp(command, "STOP") == 0) {
    relay2Status = false;
    digitalWrite(relayPin2, LOW);
  }
}






















