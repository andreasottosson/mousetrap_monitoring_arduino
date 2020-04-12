# TODO:
# - Better logic in trapEventHandler
# - Add WiFi
# - Add MQTT support

struct TRAP
{
    int pin;
    String name;
    bool triggered;
};

struct TRAP trapPins[] = {
    {5, "trap1", false},
    {4, "trap2", false}
};

void trapEventHandler(struct TRAP *trap, int trapValue) {
    if (trapValue == 1 && trap->triggered == false) {
        Serial.println("Trap triggered: "+trap->name);
        trap->triggered = true;
    } else if (trapValue == 0 && trap->triggered == true) {
        Serial.println("Trap reset: "+trap->name);
        trap->triggered = false;
    }
}

void setup() {
    Serial.begin(9600);
    for (byte i = 0; i < (sizeof(trapPins) / sizeof(trapPins[0])); i++ ) {
        pinMode(trapPins[i].pin, INPUT);
    }
}

void loop() {
    int trapValue;
    for (byte i = 0; i < (sizeof(trapPins) / sizeof(trapPins[0])); i++ ) {
        trapValue = digitalRead(trapPins[i].pin);
        trapEventHandler(&trapPins[i], trapValue);
        delay(1000);
    }
}