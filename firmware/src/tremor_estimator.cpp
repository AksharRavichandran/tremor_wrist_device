#include "tremor_estimator.h"
#include <math.h>
#include <string.h>

TremorEstimator::TremorEstimator() { reset(); }

void TremorEstimator::reset() {
    memset(_buf, 0, sizeof(_buf));
    _idx = _count = 0;
    _sum = _sumSq = 0;
}

void TremorEstimator::addSample(float v) {
    float old = _buf[_idx];
    _buf[_idx] = v;
    _idx = (_idx + 1) % TREMOR_WINDOW_SIZE;
    if (_count < TREMOR_WINDOW_SIZE) _count++;
    _sum   += v - old;
    _sumSq += v*v - old*old;
}

float TremorEstimator::getRMS() const {
    if (_count == 0) return 0.0f;
    float msq = _sumSq / _count;
    return sqrtf(msq > 0 ? msq : 0);
}

float TremorEstimator::getVariance() const {
    if (_count < 2) return 0.0f;
    float mean = _sum / _count;
    float msq  = _sumSq / _count;
    float v = msq - mean*mean;
    return v > 0 ? v : 0;
}
