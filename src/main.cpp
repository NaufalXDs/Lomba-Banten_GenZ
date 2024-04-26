#include "Arduino.h"

/* Include Library */
#include "NtpClient.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"
#include "ESP8266HTTPClient.h"
#include "dev-env.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_I2CDevice.h"
#include "SPI.h"
#include "Wire.h"

#if defined(ESP32)
    #include "Wifi.h"
#elif defined(ESP8266)
    #include "ESP8266WiFi.h"
#endif

/* Pin Definition */
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define Relay 3
#define Kelembapan A0

/* Varible Declaration */
uint32_t delayMS;
DHT_Unified dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);


void temperature() {
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
    }
}

void kelembapan() {
    int value = analogRead(Kelembapan);
    Serial.println(F("Kelembapan: ")); Serial.println(value);

    if (value > 380) {
        digitalWrite(Relay, HIGH);
        display.setCursor(0, 63);
        display.print("Kelembapan: ");
        display.println(value);
    } else if (value < 275) {
        digitalWrite(Relay, LOW);
        display.setCursor(0, 63);
        display.print("Kelembapan: ");
        display.println(value);
    }
}

void setup()
{
    Serial.begin(115200);
    /* Pin Setup */
    pinMode(Relay, OUTPUT);
    pinMode(Kelembapan, INPUT);

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

    /* Wifi Connection */
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(600);
        Serial.print(".");
    }
    Serial.print(F("WiFi connected")); Serial.println(WiFi.localIP());

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(46, 10);
    display.println("Wifi Connected");
    display.setCursor(46, 26);
    display.println(WiFi.localIP());
    display.display();

    /* OLED Display */

    if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C) != 0) {
        Serial.println(F("SSD1306 allocation failed"));
    }

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println(F("Hello, World!"));
    display.display();
}

void loop() 
{
    // temperature();
    // kelembapan();
    // delay(1000);
}


