#include "Arduino.h"
#include "DHT.h"
#include "DHT_U.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "dev-env.h"
#include "Blynk.h"
#include "BlynkSimpleEsp32.h"
#include "Adafruit_Sensor.h"
#include "Wire.h"
#include "ESP8266WiFiMulti.h"

#define DHTPIN D3
#define DHTTYPE DHT22
#define Ldr D0
#define Relay_Pump D4
#define Relay_Lamp D5

#define SERVER_TIMEOUT 5000
#define WIFI_TIMEOUT 5000

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

#define detik1 (2 * 1000)
unsigned long detik1_terakhir = 0L;

#define detik2 (7 * 1000)
unsigned long detik2_terakhir = 0L;

static bool displayTempHumidity = true;

ESP8266WiFiMulti wifiMulti;

void LDR()
{
    int ldrValue = digitalRead(Ldr);
    if (ldrValue == 1)
    {
        digitalWrite(Relay_Lamp, LOW);
    }
    else
    {
        digitalWrite(Relay_Lamp, HIGH);
    }
    Serial.print("LDR: ");
    Serial.print(ldrValue);
    Serial.println("");

    Blynk.virtualWrite(V6, ldrValue);
}

void SoilMoisture()
{
    int soilMoisture = analogRead(A0);
    Serial.print("Soil Moisture: Kelembapan= ");
    Serial.print(soilMoisture);
    Serial.println();
    if (soilMoisture >= 380)
    {
        digitalWrite(Relay_Pump, LOW);
        Serial.println("Relay Menyala >= 380");
    }
    else if (soilMoisture < 240)
    {
        digitalWrite(Relay_Pump, LOW);
        Serial.println("Relay Mati < 240");
    }
    else
    {
        digitalWrite(Relay_Pump, HIGH);
        Serial.println("Relay Mati");
    }
    Blynk.virtualWrite(V0, soilMoisture);
}

void dht22()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("DHT22: Humidity = ");
    Serial.print(h);
    Serial.print("%  Temperature = ");
    Serial.print(t);
    Serial.println(" °C");
    Blynk.virtualWrite(V5, h);
    Blynk.virtualWrite(V4, t);
}

void wifiSelect()
{
    wifiMulti.addAP(ssid1, password1);
    wifiMulti.addAP(ssid2, password2);
    wifiMulti.addAP(ssid3, password3); 
    wifiMulti.addAP(ssid4, password4); 
    wifiMulti.addAP(ssid5, password5); 

    unsigned long startWiFi = millis();

    while (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        lcd.setCursor(0, 0);
        lcd.print("IP address: ");
        lcd.setCursor(0, 1);
        lcd.print(WiFi.localIP());

        if (millis() > startWiFi + WIFI_TIMEOUT)
        {
            Serial.println("\tNo WiFi. ");
            break;
        }
    }
}

void checkBlynk()
{
    if (wifiMulti.run() == WL_CONNECTED)
    {
        unsigned long startConnecting = millis();

        while (!Blynk.connected())
        {
            Blynk.connect();

            if (millis() > startConnecting + SERVER_TIMEOUT)
            {
                // Serial.print("\tUnable to connect to server. ");
                break;
            }
        }
    }
}
void setup()
{
    /* Serial Monitor Baud */
    Serial.begin(115200);

    /* Lcd Setup */
    lcd.init();
    lcd.backlight();

    /* Blynk Setup */
    wifiSelect();
    Blynk.config(BLYNK_AUTH);
    checkBlynk();

    timer.setInterval(1000L, SoilMoisture);
    timer.setInterval(1000L, dht22);
    timer.setInterval(1000L, LDR);

    /* Dht22 Setup */
    dht.begin();

    /* Pin Setup */
    pinMode(Relay_Lamp, OUTPUT);
    pinMode(Relay_Pump, OUTPUT);
    digitalWrite(Relay_Lamp, HIGH);
    digitalWrite(Relay_Pump, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("SMKN 2 TANGSEL");
    lcd.setCursor(0, 1);
    lcd.print("Smart Garden");
    delay(2000);
    lcd.clear();
}

void loop()
{
    static float lastHumidity = 0.0;
    static float lastTemperature = 0.0;
    static unsigned long lastUpdate = 0;
    const long updateInterval = 5000;

    float currentHumidity = dht.readHumidity();
    float currentTemperature = dht.readTemperature();

    Blynk.run();
    timer.run();
    if (millis() - detik1_terakhir > detik1)
    {
        detik1_terakhir = millis();
        dht22();
        SoilMoisture();
        LDR();
        Serial.println(" ");
    }

    if (millis() - lastUpdate > updateInterval || lastHumidity != currentHumidity || lastTemperature != currentTemperature)
    {
        lastUpdate = millis();
        lastHumidity = currentHumidity;
        lastTemperature = currentTemperature;

        lcd.clear();
        if (displayTempHumidity)
        {
            lcd.setCursor(0, 0);
            lcd.print("H:");
            lcd.print(currentHumidity);
            lcd.print("%");
            lcd.setCursor(8, 0);
            lcd.print("T:");
            lcd.print(currentTemperature);
            lcd.print("C");
            lcd.setCursor(0, 1);
            lcd.print("Kelembapan: ");
            lcd.setCursor(12, 1);
            lcd.print(analogRead(A0));
        }
    }
}
