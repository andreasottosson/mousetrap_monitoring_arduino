// TODO:
// - Future: Add MQTT support

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

struct TRAP
{
    int pin;
    String name;
    bool triggered;
};

struct TRAP trapPins[] = {
    // false = 0, true = 1
    {5, "trap1", false},
    {4, "trap2", false}
};

void sendNotification(String trap) {

    if ((WiFi.status() == WL_CONNECTED)) {

        WiFiClient client;
        HTTPClient http;

        Serial.println("[HTTP] begin..");

        http.begin(client, "http://maker.ifttt.com/trigger/your_ifttt_event/with/key/your_ifttt_key");
        http.addHeader("Content-Type", "application/json");

        Serial.print("[HTTP] POST...");

        int httpCode = http.POST("{\"value1\":\""+trap+"\"}");

        if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
            const String& payload = http.getString();
            Serial.println("received payload:\n<<");
            Serial.println(payload);
            Serial.println(">>");
        }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

}

void trapEventHandler(struct TRAP *trap) {
    if (trap->triggered == true) {
        Serial.println("Trap triggered: "+trap->name);
        sendNotification(trap->name);
    } else if (trap->triggered == false) {
        Serial.println("Trap reset: "+trap->name);
    }
}

void setup() {
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    for (byte i = 0; i < (sizeof(trapPins) / sizeof(trapPins[0])); i++ ) {
        pinMode(trapPins[i].pin, INPUT);
    }
}

void loop() {
    int trapCurrentValue;

    for (byte i = 0; i < (sizeof(trapPins) / sizeof(trapPins[0])); i++ ) {
        trapCurrentValue = trapPins[i].triggered;
        trapPins[i].triggered = digitalRead(trapPins[i].pin);
        if (trapCurrentValue != trapPins[i].triggered) trapEventHandler(&trapPins[i]);
        delay(1000);
    }
}