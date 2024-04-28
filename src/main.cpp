#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "DHT_U.h"
#include "ESP8266HTTPClient.h"
#include "dev-env.h"
// #include "Blynk.h"
// #include "BlynkSimpleEsp8266.h"

/* Variable Deklar */
#define DHT_PIN 0   // D3
#define Soil_Moisture A0
#define ldr_pin 16 // D0
#define led 16     // D0

#define DHT_TYPE DHT22
const int Relay_pin[] = {2, 14};

uint32_t delayMS;
DHT_Unified dht(DHT_PIN, DHT_TYPE);
// BlynkTimer timer;

void kelembapan()
{
    int value = analogRead(Soil_Moisture);
    Serial.println(F("Kelembapan: "));
    Serial.println(value);
    // Blynk.virtualWrite(V0, value);

    if (value >= 273 && value <= 380)
    {
        digitalWrite(Relay_pin[0], HIGH);
        Serial.println("Relay status: OFF");
    }

    else if (value > 600)
    {
        digitalWrite(Relay_pin[0], LOW);
        Serial.println("Relay status: OFF");
    }
}

void dht22()
{
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
        // Blynk.virtualWrite(V1, event.temperature);
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
        // Blynk.virtualWrite(V2, event.relative_humidity);
    }
}

void setup()
{
    Serial.begin(115200);
    // Blynk.begin(auth, ssid, password);
    // while (Blynk.connected() == false)
    // {
    //     Blynk.connect();
    //     delay(1000);
    // }

    pinMode(Relay_pin[0], OUTPUT);
    pinMode(Relay_pin[1], OUTPUT);
    pinMode(DHT_PIN, INPUT);
    // timer.setInterval(1000L, kelembapan);
    // timer.setInterval(1000L, dht22);

    /* DHT22 Sensor */
    dht.begin();

    Serial.println("DHT22 Sensor");
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);

    Serial.println("Temp Sensor");
    Serial.print(F("Sensor Temp"));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));

    Serial.println("Humidity Sensor");
    Serial.print(F("Sensor Humidity"));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
}

void loop() {
    dht22();
    kelembapan();
    digitalWrite(Relay_pin[0], HIGH);
    digitalWrite(Relay_pin[1], LOW);
    Serial.println("relay HIGH");
    delay(2000);
    digitalWrite(Relay_pin[0], LOW);
    digitalWrite(Relay_pin[1], HIGH);
    Serial.println("relay low");
    // Blynk.run();
    delay(2000);
}