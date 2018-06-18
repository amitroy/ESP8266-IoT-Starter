#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "configuration.h"

const char *ssid = SSID;
const char *password = PASSWORD;
const char *mqttServer = MQTTSERVER;
const int mqttPort = MQTTPORT;
const char *mqttUser = MQTTUSER;
const char *mqttPassword = MQTTPASSWORD;
const int LED_PIN = 2;
char json[255];
StaticJsonBuffer<255> jsonBuffer;

WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
    // Setting the IO PIN
    pinMode(LED_PIN, OUTPUT);
    // Serial configuration
    Serial.begin(115200);
    // Connect Wifi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        // WiFi connection blinker
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to the WiFi network");

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected())
    {
        // MQTT connection blinker
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
        Serial.print(".");
        if (client.connect("ESP8266Client", mqttUser, mqttPassword))
        {
            Serial.println();
            Serial.println("connected to MQTT broker");
        }
        else
        {
            Serial.print("Failed with state ");
            Serial.print(client.state());
        }
    }
    // MQTT PubSub configuration
    // client.publish("royesp01/01/dev", "{\"message\": \"hello from royesp01/01/dev\"}");
    client.subscribe("royesp01/02/dev");
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("MQTT topic: ");
    Serial.println(topic);

    // Clear json array
    memset(json, 0, sizeof(json));
    for (int i = 0; i < length; i++)
    {
        json[i] = (char)payload[i];
    }

    Serial.print("Payload:");
    Serial.print(json);

    JsonObject &root = jsonBuffer.parseObject(json);
    const char *message = root["message"];
    const char *status = root["status"];
    const bool isHigh = root["enableLED"];
    const int intensity = root["brightness"];

    Serial.println();
    Serial.println("---");
    Serial.print("Message: ");
    Serial.print(message);
    Serial.println();
    Serial.print("Status: ");
    Serial.print(status);
    Serial.println();
    Serial.print("enableLED: ");
    Serial.print(isHigh);
    Serial.println();
    Serial.print("brightness: ");
    Serial.print(intensity);
    Serial.println();
    Serial.println("---");
    Serial.println();

    ledController(LED_PIN, isHigh, intensity);
    // Clear the jsonBuffer buffer
    jsonBuffer.clear();
}

// Function to control the LED_PIN
void ledController(int ledPin, bool isHigh, int intensity)
{
    if (isHigh)
    {
        //digitalWrite(ledPin, HIGH);
        if (intensity == 0)
        {
            intensity = 100;
        }
        intensity = (intensity * 1023) / 100;
        analogWrite(LED_PIN, intensity);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
}

// The loop
void loop()
{
    client.loop();
}