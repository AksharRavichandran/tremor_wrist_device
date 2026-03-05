#pragma once
#include <Arduino.h>
#include "../../firmware/config/config.h"

// Computes rolling RMS and variance of filtered acceleration magnitude
class TremorEstimator {
public:
    TremorEstimator();
    void  addSample(float filteredMag);
    float getRMS()      const;
    float getVariance() const;
    bool  isFull()      const { return _count >= TREMOR_WINDOW_SIZE; }
    void  reset();

private:
    float _buf[TREMOR_WINDOW_SIZE];
    int   _idx, _count;
    float _sum, _sumSq;
};
