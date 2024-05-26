#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "WifiSomying";
const char* password = "SomsakSuksom";
const char* serverUrl = "http://172.20.10.3:8000//api/relay-command/";

const int relayPin1 = D5;
const int relayPin2 = D6;

bool relay1Status = false;
bool relay2Status = false;

void setup() {
  Serial.begin(9600);

  WifiConnect();

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);

  
}

void loop() {
  CheckRelay();

  delay(10000);
}
 
void WifiConnect() {
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("-----------------------------");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void CheckRelay(){
  if (WiFi.status() == WL_CONNECTED ) {
    HTTPClient http;
    WiFiClient client;

    http.begin(client ,serverUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0 ) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);

      ControlRelay(response);

    } else {
      Serial.println("ERROR on HTTP requset...");
      Serial.println(httpResponseCode);
      Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
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

  const char* relay1Command = jsonDoc["relay1_status"];
  const char* relay2Command = jsonDoc["relay2_status"];

  Serial.print("Relay 1 Command: ");
  Serial.println(relay1Command);
  Serial.print("Relay 2 Command: ");
  Serial.println(relay2Command);

  Relay_1(relay1Command);
  Relay_2(relay2Command);
  
}

void Relay_1(const char* command) {
  if (strcmp(command, "START") == 0) {
    relay1Status = true;
    digitalWrite(relayPin1, HIGH);
    Serial.println("Relay 1 Status: ON");
    } else if (strcmp(command, "STOP") == 0) {
    relay1Status = false;
    digitalWrite(relayPin1, LOW);
    Serial.println("Relay 1 Status: OFF");
  }
}

void Relay_2(const char* command) {
  if (strcmp(command, "START") == 0) {
    relay2Status = true;
    digitalWrite(relayPin2, HIGH);
    Serial.println("Relay 2 Status: ON");
  } else if (strcmp(command, "STOP") == 0) {
    relay2Status = false;
    digitalWrite(relayPin2, LOW);
    Serial.println("Relay 2 Status: OFF");
  }
}
