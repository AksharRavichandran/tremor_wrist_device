#pragma once
#include <Arduino.h>

struct IMUData {
    float ax, ay, az;   // Calibrated acceleration (m/s^2)
    float gx, gy, gz;   // Calibrated gyroscope (deg/s)
    float mag;          // Accel vector magnitude (m/s^2)
};

class IMUReader {
public:
    bool begin();
    bool read(IMUData& out);
    void calibrate(int samples = 300);  // Collect bias at rest

private:
    float _ax_off = 0, _ay_off = 0, _az_off = 0;
    float _gx_off = 0, _gy_off = 0, _gz_off = 0;
    static constexpr float A_SCALE = 9.81f / 8192.0f;  // ±4g range
    static constexpr float G_SCALE = 500.0f / 32768.0f; // ±500 dps range
};
