#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;
WebServer server(80);

// WiFi hotspot
const char* ssid = "SmartLight";
const char* password = "12345678";

// Pins
const int ledPin = 23;
const int irPin = 19;
const int ldrPin = 34;

// Logic
int ldrValue;
bool detected;

// Manual IR simulation
bool manualDetected = false;
bool useManual = true;   // set true for demo

int quietDim = 51;    // 20%
int mlDim = 128;      // 50%
int fullBright = 255;

int currentBrightness = 0;
String currentMode = "";

// ML predicted busy hours
bool predictedBusyHour[24] = {
  false,false,false,false,false,false,
  false,true,true,false,false,false,
  false,false,false,false,false,true,
  true,true,true,false,false,false
};

// Smooth brightness transition
void smoothSetBrightness(int target) {
  if (currentBrightness < target) {
    for (int i = currentBrightness; i <= target; i++) {
      analogWrite(ledPin, i);
      delay(3);
    }
  } else if (currentBrightness > target) {
    for (int i = currentBrightness; i >= target; i--) {
      analogWrite(ledPin, i);
      delay(3);
    }
  }
  currentBrightness = target;
}

// Webpage
String webpage() {
  DateTime now = rtc.now();

  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<title>Smart Street Light</title>";
  html += "<style>";
  html += "body{font-family:Arial;padding:20px;background:#f5f5f5;}";
  html += ".card{background:white;padding:16px;border-radius:10px;margin-bottom:15px;}";
  html += "</style></head><body>";

  html += "<div class='card'><h2>Smart Street Light</h2>";
  html += "Time: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "<br>";
  html += "LDR: " + String(ldrValue) + "<br>";
  html += "Detected: " + String(detected ? "YES" : "NO") + "<br>";
  html += "Brightness: " + String((currentBrightness * 100) / 255) + "%<br>";
  html += "Mode: " + currentMode + "<br></div>";

  html += "<div class='card'><h3>ML Busy Hours</h3>";
  for (int i = 0; i < 24; i++) {
    html += String(i) + ":00 → ";
    html += predictedBusyHour[i] ? "Busy<br>" : "Quiet<br>";
  }
  html += "</div>";

  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

void setup() {
  Serial.begin(115200);

  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("RTC fail");
    while (1);
  }

  // Uncomment ONCE if needed
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  WiFi.softAP(ssid, password);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();

  // SERIAL CONTROL (manual IR)
  if (Serial.available()) {
    char input = Serial.read();

    if (input == '1') {
      manualDetected = true;
      Serial.println("Manual Detection ON");
    } 
    else if (input == '0') {
      manualDetected = false;
      Serial.println("Manual Detection OFF");
    }
  }

  DateTime now = rtc.now();
  int hourNow = now.hour();

  ldrValue = analogRead(ldrPin);

  // Use manual or real IR
  if (useManual) {
    detected = manualDetected;
  } else {
    detected = (digitalRead(irPin) == LOW);
  }

  bool isDay = (ldrValue < 2000);

  if (isDay) {
    smoothSetBrightness(0);
    currentMode = "DAY - OFF";
  } else {
    if (detected) {
      smoothSetBrightness(fullBright);
      currentMode = "LIVE DETECTION - 100%";
    } 
    else if (predictedBusyHour[hourNow]) {
      smoothSetBrightness(mlDim);
      currentMode = "ML PREDICTED - 50%";
    } 
    else {
      smoothSetBrightness(quietDim);
      currentMode = "QUIET - 20%";
    }
  }

  Serial.print("Time: ");
  Serial.print(now.hour()); Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.print(now.second());

  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Detected: "); Serial.print(detected);
  Serial.print(" | Brightness: "); Serial.print((currentBrightness * 100) / 255);
  Serial.print("% | Mode: "); Serial.println(currentMode);

  delay(150);
}