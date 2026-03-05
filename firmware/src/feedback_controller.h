#pragma once
#include <Arduino.h>
#include "state_machine.h"

class FeedbackController {
public:
    void begin();
    void update(DeviceState state, unsigned long nowMs);

private:
    void setLED(uint8_t r, uint8_t g, uint8_t b);
    void setMotor(uint8_t duty);

    unsigned long _lastToggle  = 0;
    unsigned long _lastToggle2 = 0;
    bool _pulse1 = false;
    bool _pulse2 = false;
};
