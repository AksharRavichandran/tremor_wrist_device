#include "imu_reader.h"
#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

static MPU6050 mpu;

bool IMUReader::begin() {
    Wire.begin();
    mpu.initialize();
    if (!mpu.testConnection()) {
        Serial.println("[IMU] ERROR: MPU6050 not found. Check SDA/SCL wiring.");
        return false;
    }
    // ±4g accel, ±500 dps gyro — good balance for wrist tremor
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);
    // Set DLPF to ~44Hz to reduce raw sensor noise before our software filter
    mpu.setDLPFMode(MPU6050_DLPF_BW_44);
    Serial.println("[IMU] MPU6050 initialized OK");
    return true;
}

void IMUReader::calibrate(int samples) {
    Serial.println("[IMU] Calibrating — keep wrist COMPLETELY STILL...");
    delay(1000);
    long axS=0,ayS=0,azS=0,gxS=0,gyS=0,gzS=0;
    int16_t ax,ay,az,gx,gy,gz;
    for (int i = 0; i < samples; i++) {
        mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
        axS+=ax; ayS+=ay; azS+=az;
        gxS+=gx; gyS+=gy; gzS+=gz;
        delay(5);
    }
    _ax_off = (axS/samples)*A_SCALE;
    _ay_off = (ayS/samples)*A_SCALE;
    _az_off = (azS/samples)*A_SCALE - 9.81f; // remove gravity from z-axis
    _gx_off = (gxS/samples)*G_SCALE;
    _gy_off = (gyS/samples)*G_SCALE;
    _gz_off = (gzS/samples)*G_SCALE;
    Serial.println("[IMU] Calibration done.");
}

bool IMUReader::read(IMUData& out) {
    int16_t ax,ay,az,gx,gy,gz;
    mpu.getMotion6(&ax,&ay,&az,&gx,&gy,&gz);
    out.ax = ax*A_SCALE - _ax_off;
    out.ay = ay*A_SCALE - _ay_off;
    out.az = az*A_SCALE - _az_off;
    out.gx = gx*G_SCALE - _gx_off;
    out.gy = gy*G_SCALE - _gy_off;
    out.gz = gz*G_SCALE - _gz_off;
    out.mag = sqrtf(out.ax*out.ax + out.ay*out.ay + out.az*out.az);
    return true;
}
