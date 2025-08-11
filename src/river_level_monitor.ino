/*
  river_level_monitor.ino
  -----------------------
  Aim          : Measure river water level using a sonar sensor and transmit the data to ThingSpeak.
  Author       : G. Lakshmi Vara Prasad (GLV13)
  Developed by : IIT Guwahati - Technology Innovation and Development Foundation (TIDF)
  Client       : North Eastern Electric Power Corporation Limited (NEEPCO)
*/

#include "ThingSpeak.h"
#include <WiFi.h>

// Network Credentials. Change according to your network
const char *ssid = "GLV13";
const char *password = "Varaprasad@13";

WiFiClient client;

// Enter ThingSpeak Channel number and API here.
unsigned long channel_number = 1;
const char *APIKey = "XXXXXXXXXXXXXXXX";   // Replace with your ThingSpeak APIKey

// Timer Variables
unsigned long curTime;
unsigned long preTime = 0;
unsigned long interval = 15000;

// Defining Ultrasonic sensor pin
#define sensor_pin 34
long distance = 0;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  Serial.println("ThingSpeak Initialized");
}

// Checking the Wi-Fi connection
void check_WiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Reconnecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(5000);
      Serial.print(".");
    }
    Serial.println("\nReconnected to WiFi.");
  }
}

// Reading sensor data
void read_sensor() {
  distance = analogRead(sensor_pin) * 2.5;  // For MB7388 Sensor; The multiplying factor is 2.5 (12-bit ADC)
  distance = distance / 10;                 // Converting mm to cm
}

// Publishing in the ThingSpeak
void write_things() {
  int x = ThingSpeak.writeField(channel_number, 1, distance, APIKey);
  if (x == 200) {
    Serial.println("Data published successfully");
  } else {
    Serial.println("Problem in publishing data. HTTP error code " + String(x));
  }
}

// Print data to the Serial Monitor while debugging
void print_data() {
  Serial.print("Distance = ");
  Serial.print(distance);  // Dividing by 10 converts mm to cm
  Serial.println(" cm");
}

void loop() {
  curTime = millis();
  if ((curTime - preTime) >= interval) {
    check_WiFi();
    read_sensor();
    write_things();
    print_data();
    preTime = curTime;
  }
}
