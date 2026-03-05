#include "state_machine.h"
#include "../../firmware/config/config.h"

const char* stateLabel(DeviceState s) {
    switch(s) {
        case DeviceState::STABLE:           return "STABLE";
        case DeviceState::MILD_TREMOR:      return "MILD_TREMOR";
        case DeviceState::SEVERE_TREMOR:    return "SEVERE_TREMOR";
        case DeviceState::FATIGUE_ELEVATED: return "FATIGUE_ELEVATED";
        default:                            return "UNKNOWN";
    }
}

StateMachine::StateMachine()
    : _state(DeviceState::STABLE), _prev(DeviceState::STABLE) {}

void StateMachine::update(float rms, bool fatigued) {
    _prev = _state;
    // Priority: SEVERE > MILD > FATIGUE > STABLE
    // Active tremor always takes precedence over background fatigue alert
    if      (rms >= THRESH_SEVERE_TREMOR) _state = DeviceState::SEVERE_TREMOR;
    else if (rms >= THRESH_MILD_TREMOR)   _state = DeviceState::MILD_TREMOR;
    else if (fatigued)                    _state = DeviceState::FATIGUE_ELEVATED;
    else                                  _state = DeviceState::STABLE;
}
