// TODO:
// - Add WiFi
// - Add IFTTT notifications
// - (Add MQTT support)

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

void trapEventHandler(struct TRAP *trap) {
    if (trap->triggered == true) {
        Serial.println("Trap triggered: "+trap->name);
    } else if (trap->triggered == false) {
        Serial.println("Trap reset: "+trap->name);
    }
}

void setup() {
    Serial.begin(9600);
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