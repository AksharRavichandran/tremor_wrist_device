#pragma once
#include <Arduino.h>
#include "state_machine.h"

class DataLogger {
public:
    bool begin();
    void log(unsigned long ms, float rms, float ltAvg, DeviceState state);

private:
    bool _ready = false;
};
