#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ThingSpeak.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// ThingSpeak channel details
const unsigned long channelId = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

// Temperature sensor details
const int sensorPin = A0;

// Alarm threshold
const float alarmThreshold = 40.0;

// Initialize Wi-Fi client and ThingSpeak client
WiFiClient wifiClient;
ThingSpeakClient thingSpeakClient(wifiClient);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Connect to Wi-Fi network
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  
  // Initialize ThingSpeak channel
  ThingSpeak.begin(thingSpeakClient);
}

void loop() {
  // Read temperature sensor value
  float sensorValue = analogRead(sensorPin);
  float temperature = (sensorValue * 3.3 / 1023.0 - 0.5) * 100.0;
  
  // Print temperature value to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  
  // Update ThingSpeak channel with temperature value
  ThingSpeak.setField(1, temperature);
  
  // Set alarm flag if temperature is above threshold
  int alarm = temperature > alarmThreshold ? 1 : 0;
  
  // Update ThingSpeak channel with alarm flag
  ThingSpeak.setField(2, alarm);
  
  // Send data to ThingSpeak
  int status = ThingSpeak.writeFields(channelId, writeAPIKey);
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.println("Error sending data to ThingSpeak.");
  }
  
  // Wait for 15 seconds before sending next data
  delay(15000);
}
