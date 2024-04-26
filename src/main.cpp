/* Library Import */
#include "Arduino.h"
#include "Wire.h"
#include "DHT.h"
#include "DHT_U.h"
#include "Adafruit_SSD1306.h"
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"
#include "dev-env.h"

/* Variable Deklar */
#define DHT_PIN 2 //D4
#define Relay_pin 0 //D3
#define Soil_Moisture A0

#define DHT_TYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

DHT_Unified dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void kelembapan()
{
    int value = analogRead(Soil_Moisture);
    Serial.println(F("Kelembapan: "));
    Serial.println(value);

    if (value > 380)
    {
        digitalWrite(Relay_pin, HIGH);
        Serial.println("Relay status: ON");
        // display.setCursor(0, 63);
        display.print("Kelembapan: ");
        display.println(value);
    }
    else if (value < 275)
    {
        digitalWrite(Relay_pin, LOW);
        Serial.println("Relay status: OFF");
        // display.setCursor(0, 63);
        display.print("Kelembapan: ");
        display.println(value);
    }
}

void temp()
{
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print(F("Temperature: "));
        display.print(event.temperature);
        display.print(F("°C"));
        display.display();
        Serial.print(F("Value: "));
        Serial.println(event.temperature);
    }
}


void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("WiFi connected successfully!");
    Serial.println(WiFi.localIP());

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D untuk 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }

    dht.begin();

    pinMode(Relay_pin, OUTPUT);
    pinMode(Soil_Moisture, INPUT);
}

void loop() {
    kelembapan();
    // temp();
    delay(2000);
}