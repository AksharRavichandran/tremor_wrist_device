#pragma once
#include <Arduino.h>

enum class DeviceState : uint8_t {
    STABLE           = 0,
    MILD_TREMOR      = 1,
    SEVERE_TREMOR    = 2,
    FATIGUE_ELEVATED = 3
};

const char* stateLabel(DeviceState s);

class StateMachine {
public:
    StateMachine();
    void        update(float rms, bool fatigued);
    DeviceState state()        const { return _state; }
    bool        stateChanged() const { return _state != _prev; }

private:
    DeviceState _state, _prev;
};
