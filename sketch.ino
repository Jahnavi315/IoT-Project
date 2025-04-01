#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* thingSpeakAPI = "https://api.thingspeak.com/update?api_key=VPPFQPASEP7EB5Z3";  

const float voltage = 230.0;  // Assume classroom voltage is 230V
float current = 0.0;  // Simulated current sensor value

// OLED Display (SSD1306)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  
        Serial.println("SSD1306 initialization failed!");
        while (1);
    }
    display.clearDisplay();
    
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
}

void loop() {
    // Simulating ACS712 Current Sensor Data (Random 0-10A)
    current = random(1, 10);
    float power = voltage * current;  

    Serial.print("Current: "); Serial.print(current); Serial.print(" A, ");
    Serial.print("Power: "); Serial.print(power); Serial.println(" W");

    // Displaying Data on OLED Screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.print("Current: "); display.print(current); display.println(" A");
    display.print("Power: "); display.print(power); display.println(" W");
    display.display();

    // Sending Data to ThingSpeak
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

    // Alert if Power Exceeds 1500W - threshold
    if (power > 1500) {
        Serial.println("⚠ ALERT: High Power Consumption Detected!");
    }

    delay(10000); 
}