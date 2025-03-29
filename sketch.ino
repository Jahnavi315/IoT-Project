#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// Wi-Fi Credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak API Details
const char* thingSpeakAPI = "https://api.thingspeak.com/update?api_key=VPPFQPASEP7EB5Z3";  // Replace with your API Key

// Power Calculation Constants
const float voltage = 230.0;  // Assume classroom voltage is 230V
float current = 0.0;  // Simulated current sensor value

// OLED Display (SSD1306)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
        Serial.println("SSD1306 initialization failed!");
        while (1);
    }
    display.clearDisplay();
    
    // Wi-Fi Connection
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
}

void loop() {
    // Simulate ACS712 Current Sensor Data (Random 0-10A)
    current = random(1, 10);
    float power = voltage * current;  // P = V × I

    // Display Data in Serial Monitor
    Serial.print("Current: "); Serial.print(current); Serial.print(" A, ");
    Serial.print("Power: "); Serial.print(power); Serial.println(" W");

    // Display Data on OLED Screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print("Current: "); display.print(current); display.println(" A");
    display.print("Power: "); display.print(power); display.println(" W");
    display.display();

    // Send Data to ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(thingSpeakAPI) + "&field1=" + String(current) + "&field2=" + String(power);
        
        http.begin(url);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.println("Data Sent to ThingSpeak!");
        } else {
            Serial.println("Error Sending Data.");
        }
        http.end();
    }

    // Alert if Power Exceeds 1500W
    if (power > 1500) {
        Serial.println("⚠ ALERT: High Power Consumption Detected!");
    }

    delay(10000);  // Send data every 10 seconds
}