#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "navajo";
const char* password = "nafkaard";
const char* mqtt_server = "usm.revolusi-it.com";
const char* user_mqtt = "usm";
const char* pass_mqtt = "usmjaya24";

const char* topik = "test/test";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Pesan dari MQTT [");
    Serial.print(topic);
    Serial.print("] ");

    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    const char* nim = doc["nim"];
    const char* messages = doc["messages"];

    Serial.print("NIM: ");
    Serial.println(nim);
    Serial.print("Messages: ");
    Serial.println(messages);

    unsigned long start = millis();
    if (String(nim) == "G.231.20.0136") {
        if (String(messages) == "D5=1") { digitalWrite(D5, HIGH); }
        if (String(messages) == "D6=1") { digitalWrite(D6, HIGH); }
        if (String(messages) == "D7=1") { digitalWrite(D7, HIGH); }
        if (String(messages) == "D8=1") { digitalWrite(D8, HIGH); }

        if (String(messages) == "D5=0") { digitalWrite(D5, LOW); }
        if (String(messages) == "D6=0") { digitalWrite(D6, LOW); }
        if (String(messages) == "D7=0") { digitalWrite(D7, LOW); }
        if (String(messages) == "D8=0") { digitalWrite(D8, LOW); }
    }
    unsigned long end = millis();
    Serial.print("Processing time: ");
    Serial.print(end - start);
    Serial.println(" ms");
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Menghubungkan kembali ke MQTT Server: ");
        Serial.println(mqtt_server);
        if (client.connect("G.231.20.0136", user_mqtt, pass_mqtt)) {
            Serial.println("Terhubung ke MQTT server");
            client.subscribe(topik);
        } else {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println(" coba lagi dalam 5 detik...");
            delay(5000);
        }
    }
}

void konek_wifi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Terhubung ke WiFi");
}

void publishData(float temperature, float humidity) {
    StaticJsonDocument<256> doc;
    doc["nim"] = "G.231.20.0136";
    doc["suhu"] = temperature;
    doc["kelembaban"] = humidity;

    char buffer[256];
    size_t n = serializeJson(doc, buffer);

    Serial.print("Publishing data: ");
    Serial.println(buffer);

    client.publish(topik, buffer, n);
}

void setup() {
    Serial.begin(9600);
    Wire.begin(D2, D1);
    lcd.begin(16, 2);
    lcd.backlight();
    dht.begin();

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    konek_wifi();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        konek_wifi();
    }

    if (!client.connected()) {
        reconnect();
    }

    client.loop();

    static unsigned long lastPublishTime = 0;
    if (millis() - lastPublishTime >= 5000) {
        lastPublishTime = millis();
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        lcd.setCursor(0, 0);
        lcd.print("Suhu: " + String(t) + "C");
        lcd.setCursor(0, 1);
        lcd.print("Kelembaban: " + String(h) + "%");

        publishData(t, h);
    }
}
