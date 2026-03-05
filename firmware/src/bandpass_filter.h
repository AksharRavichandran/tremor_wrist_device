#pragma once
#include <Arduino.h>

// =============================================================
// 2-12 Hz FIR Bandpass Filter
// Designed for 100 Hz sampling rate
// Passes tremor-frequency content, rejects slow voluntary motion
// and high-frequency electrical noise.
//
// Clinical tremor bands (Grimaldi & Manto, 2008):
//   MS cerebellar tremor:  2.5–6 Hz
//   Essential tremor:      4–8 Hz
//   Physiological tremor:  8–12 Hz
// =============================================================

class BandpassFilter {
public:
    BandpassFilter();
    float process(float sample);
    void reset();

private:
    static const int ORDER = 32;
    // FIR coefficients for 2-12 Hz bandpass at 100 Hz sample rate
    // Generated via windowed-sinc method (Hamming window)
    static const float COEFFS[ORDER + 1];
    float _buf[ORDER + 1];
    int   _idx;
};
