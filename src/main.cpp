/* Library Import */
#include "Arduino.h"
#include "Wire.h"
#include "DHT.h"
#include "DHT_U.h"
#include "Adafruit_SSD1306.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "dev-env.h"
#include "Blynk.h"
#include "BlynkSimpleEsp32.h"
#include "Adafruit_Sensor.h"

/* Variable Deklar */
#define DHT_PIN 0   // D3
#define Relay_pin 2 // D4
#define Soil_Moisture A0
#define ldr_pin 16 //D0
#define led 16 //D0

#define DHT_TYPE DHT11
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

uint32_t delayMS;
DHT_Unified dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BlynkTimer timer;

void kelembapan()
{
    int value = analogRead(Soil_Moisture);
    Serial.println(F("Kelembapan: "));
    Serial.println(value);
    Blynk.virtualWrite(V0, value);

    if (value >= 273 && value <= 380)
    {
        digitalWrite(Relay_pin, HIGH);
        Serial.println("Relay status: OFF");
        display.print("Kelembapan: ");
        display.println(value);
    }

    else if (value > 600)
    {
        digitalWrite(Relay_pin, LOW);
        Serial.println("Relay status: OFF");
        display.print("Kelembapan: ");
        display.println(value);
    }
}

void dht11() {
    delay(delayMS);
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
        Blynk.virtualWrite(V1, event.temperature);
    }
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
        Serial.println(F("Error reading humidity"));
    }
    else
    {
        Serial.print(F("Humidity: "));
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
        Blynk.virtualWrite(V2, event.relative_humidity);
    }
}

void lampu() {
    int ldrStatus = digitalRead(ldr_pin); // Membaca status dari LDR
    if (ldrStatus == HIGH) {
        digitalWrite(led, HIGH); // Menyalakan LED
        Serial.println("LED menyala karena LDR terdeteksi cahaya");
    } else {
        digitalWrite(led, LOW); // Mematikan LED
        Serial.println("LED mati karena LDR tidak terdeteksi cahaya");
    }
}

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, password);
    while (Blynk.connected() == false)
    {
        Blynk.connect();
        delay(1000);
    }
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
    { // Address 0x3D untuk 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    pinMode(Relay_pin, OUTPUT);
    pinMode(Soil_Moisture, INPUT);
    pinMode(DHT_PIN, INPUT);
    timer.setInterval(1000L, kelembapan);
    timer.setInterval(1000L, dht11);

    /* DHT22 Sensor */
    dht.begin();

    Serial.println("DHT22 Sensor");
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);

    Serial.println("Temp Sensor");
    Serial.print(F("Sensor Temp")); Serial.print(sensor.resolution); Serial.println(F("°C"));

    Serial.println("Humidity Sensor");
    Serial.print(F("Sensor Humidity")); Serial.print(sensor.resolution); Serial.println(F("%"));
}

void loop()
{
    lampu();
    kelembapan();
    dht11();
    Blynk.run();
    delay(2000);
}