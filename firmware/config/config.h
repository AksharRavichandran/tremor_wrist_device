#pragma once

// =============================================================
// SYSTEM CONFIGURATION — tune all thresholds here
// =============================================================

// --- Sampling ---
#define SAMPLE_RATE_HZ          100     // IMU polling frequency
#define SAMPLE_INTERVAL_MS      (1000 / SAMPLE_RATE_HZ)

// --- Bandpass Filter ---
// 2-12 Hz FIR bandpass (covers all clinical tremor bands)
// Separates tremor from slow voluntary motion and high-freq noise
#define FILTER_ENABLED          true

// --- Tremor Detection (Short Window) ---
#define TREMOR_WINDOW_SIZE      50      // Samples (~0.5s @ 100Hz)
                                        // Shorter = faster, noisier
                                        // Longer  = smoother, slower

// --- Tremor Thresholds (units: m/s^2 RMS of filtered signal) ---
// !! CALIBRATE THESE with your specific user !!
// Start by observing serial output during rest vs. tremor episodes
#define THRESH_MILD_TREMOR      0.5f    // Above this = MILD_TREMOR
#define THRESH_SEVERE_TREMOR    1.5f    // Above this = SEVERE_TREMOR

// --- Fatigue Tracking (Long Window) ---
#define FATIGUE_WINDOW_SIZE     3000    // Samples (~30s @ 100Hz rolling avg)
                                        // Increase for longer trend detection
#define FATIGUE_ALERT_THRESH    0.8f    // Long-term avg above this = FATIGUE_ELEVATED
                                        // Set between MILD and SEVERE thresholds

// --- Haptic Feedback ---
#define VIB_PWM_MILD            80      // 0–255 duty cycle for mild state
#define VIB_PWM_SEVERE          220     // 0–255 duty cycle for severe state
#define VIB_PWM_FATIGUE         50      // 0–255 duty cycle for fatigue pulse
#define VIB_PULSE_ON_MS         250     // Pulse on-time
#define VIB_PULSE_OFF_MS        500     // Pulse off-time (mild)
#define VIB_DOUBLE_PULSE_GAP    150     // Gap between double pulses (fatigue)

// --- SD Card Logging (Optional) ---
#define ENABLE_SD_LOGGING       false
#define LOG_INTERVAL_MS         500     // Log entry every 500ms
#define LOG_FILENAME            "/tremor_log.csv"

// --- Serial Debug ---
#define SERIAL_DEBUG            true
#define SERIAL_BAUD             115200
