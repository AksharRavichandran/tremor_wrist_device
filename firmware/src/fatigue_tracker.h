#pragma once
#include <Arduino.h>
#include "../../firmware/config/config.h"

// Maintains a long rolling-window average of tremor RMS
// to detect fatigue-driven intensity creep over minutes
class FatigueTracker {
public:
    FatigueTracker();
    void  update(float tremorRMS);
    float getLongTermAvg() const;
    bool  isFatigueElevated() const;
    void  reset();

private:
    float _buf[FATIGUE_WINDOW_SIZE];
    int   _idx, _count;
    float _sum;
};
