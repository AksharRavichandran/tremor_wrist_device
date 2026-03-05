#include "bandpass_filter.h"
#include <string.h>

// FIR coefficients — 2-12 Hz bandpass, 100 Hz Fs, 32nd order, Hamming window
// To regenerate: use scipy.signal.firwin2 or GNU Octave fir1()
// scipy: firwin(33, [2/50, 12/50], pass_zero=False, window='hamming')
const float BandpassFilter::COEFFS[ORDER + 1] = {
    -0.00047f, -0.00082f, -0.00059f,  0.00083f,  0.00394f,  0.00754f,
     0.00922f,  0.00659f, -0.00168f, -0.01504f, -0.02913f, -0.03833f,
    -0.03610f, -0.01551f,  0.02368f,  0.07354f,  0.12291f,  0.15867f,
     0.17126f,  0.15867f,  0.12291f,  0.07354f,  0.02368f, -0.01551f,
    -0.03610f, -0.03833f, -0.02913f, -0.01504f, -0.00168f,  0.00659f,
     0.00922f,  0.00754f,  0.00394f
};

BandpassFilter::BandpassFilter() : _idx(0) {
    memset(_buf, 0, sizeof(_buf));
}

float BandpassFilter::process(float sample) {
    _buf[_idx] = sample;
    float acc = 0.0f;
    int k = _idx;
    for (int i = 0; i <= ORDER; i++) {
        acc += COEFFS[i] * _buf[k];
        k = (k == 0) ? ORDER : k - 1;
    }
    _idx = (_idx >= ORDER) ? 0 : _idx + 1;
    return acc;
}

void BandpassFilter::reset() {
    memset(_buf, 0, sizeof(_buf));
    _idx = 0;
}
