#include "fatigue_tracker.h"
#include <string.h>

FatigueTracker::FatigueTracker() { reset(); }

void FatigueTracker::reset() {
    memset(_buf, 0, sizeof(_buf));
    _idx = _count = 0;
    _sum = 0;
}

void FatigueTracker::update(float rms) {
    float old = _buf[_idx];
    _buf[_idx] = rms;
    _idx = (_idx + 1) % FATIGUE_WINDOW_SIZE;
    if (_count < FATIGUE_WINDOW_SIZE) _count++;
    _sum += rms - old;
}

float FatigueTracker::getLongTermAvg() const {
    return _count > 0 ? _sum / _count : 0.0f;
}

bool FatigueTracker::isFatigueElevated() const {
    // Only flag after window is full (avoids false early alert)
    return _count >= FATIGUE_WINDOW_SIZE &&
           getLongTermAvg() >= FATIGUE_ALERT_THRESH;
}
