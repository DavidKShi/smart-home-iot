#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ===== WIFI SETTINGS =====
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverName = "http://YOUR_IP:5000"; 

// ===== DHT11 SETUP =====
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== OTHER COMPONENTS =====
#define LEDPIN 12
#define MOTIONPIN 27

// ===== STATE VARIABLES =====
bool ledMotion = false;   // LED triggered by motion
bool ledRemote = false;   // LED triggered by dashboard
bool motionPreviouslyDetected = false;

float lastTemp = -1000;
float lastHum = -1000;

// ===== TIMERS =====
unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 5000;   // send sensor data every 5s
unsigned long lastRemoteTime = 0;
const unsigned long remoteInterval = 300;    // poll remote LED every 0.3s

// ===== PIR DEBOUNCE =====
const unsigned long pirDebounce = 150;       // 150ms minimum pulse
unsigned long lastPirChange = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(MOTIONPIN, INPUT);

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("System ready...");
}

void loop() {
  unsigned long currentMillis = millis();

  // ===== PIR MOTION CHECK =====
  int motionRaw = digitalRead(MOTIONPIN);
  if (motionRaw == HIGH && (currentMillis - lastPirChange > pirDebounce)) {
    if (!motionPreviouslyDetected) {
      Serial.println("=== Motion Detected ===");
      motionPreviouslyDetected = true;
      sendSensorData();  // immediate send on motion
    }
    lastPirChange = currentMillis;
    ledMotion = true;
  } else if (motionRaw == LOW && motionPreviouslyDetected && (currentMillis - lastPirChange > pirDebounce)) {
    Serial.println("--- No Motion ---");
    motionPreviouslyDetected = false;
    lastPirChange = currentMillis;
    ledMotion = false;
    sendSensorData();  // send on motion stop
  }

  // ===== COMBINE LED STATES =====
  digitalWrite(LEDPIN, (ledMotion || ledRemote) ? HIGH : LOW);

  // ===== SEND SENSOR DATA PERIODICALLY =====
  if (currentMillis - lastSensorTime > sensorInterval) {
    sendSensorData();
    lastSensorTime = currentMillis;
  }

  // ===== POLL REMOTE LED STATE =====
  if (currentMillis - lastRemoteTime > remoteInterval) {
    pollRemoteLED();
    lastRemoteTime = currentMillis;
  }
}

// ===== FUNCTION TO SEND SENSOR DATA =====
void sendSensorData() {
  float tempC = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(tempC) || isnan(hum)) return;

  // Only send if temperature/humidity changed significantly
  if (abs(tempC - lastTemp) < 0.1 && abs(hum - lastHum) < 0.1 && !motionPreviouslyDetected) return;

  lastTemp = tempC;
  lastHum = hum;

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String(serverName) + "/data");
    http.addHeader("Content-Type", "application/json");

    String json = "{";
    json += "\"temperature\":" + String(tempC) + ",";
    json += "\"humidity\":" + String(hum) + ",";
    json += "\"motion\":" + String(motionPreviouslyDetected ? 1 : 0);
    json += "}";

    int httpResponseCode = http.POST(json);
    if (httpResponseCode > 0) {
      Serial.print("POST /data response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("POST /data error: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

// ===== FUNCTION TO POLL REMOTE LED =====
void pollRemoteLED() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(String(serverName) + "/get_data");
    int httpCode = http.GET();
    if (httpCode == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(200);
      deserializeJson(doc, payload);
      int newLedRemote = doc["led"];
      if (newLedRemote != ledRemote) {
        ledRemote = newLedRemote;
        Serial.print("Remote LED changed to: ");
        Serial.println(ledRemote ? "ON" : "OFF");
      }
    }
    http.end();
  }
}