struct TRAP
{
    int pin;
    bool triggered;
};

struct TRAP trapPins[] = {
    {5, false},
    {4, false}
};

void handleTrigger(struct TRAP *trap) {
    if (trap->triggered == false) Serial.println("Trap triggered on pin: "+String(trap->pin));
    trap->triggered = true;
}

void handleReset(struct TRAP *trap) {
    if (trap->triggered == true) Serial.println("Trap reset on pin: "+String(trap->pin));
    trap->triggered = false;
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
        if (trapValue == 1) handleTrigger(&trapPins[i]);
        if (trapValue == 0) handleReset(&trapPins[i]);
        delay(1000);
    }
}
