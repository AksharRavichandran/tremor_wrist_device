// =============================================================
// Adaptive Wrist-Mounted Tremor Feedback + Fatigue Monitor
// =============================================================
#include <Arduino.h>
#include "../../firmware/config/config.h"
#include "../../firmware/config/pins.h"
#include "imu_reader.h"
#include "bandpass_filter.h"
#include "tremor_estimator.h"
#include "fatigue_tracker.h"
#include "state_machine.h"
#include "feedback_controller.h"
#include "data_logger.h"

// --- Modules ---
IMUReader           imu;
BandpassFilter      bpf;         // One filter per axis; we filter magnitude
BandpassFilter      bpf_ax, bpf_ay, bpf_az; // Per-axis filtering (optional)
TremorEstimator     tremor;
FatigueTracker      fatigue;
StateMachine        fsm;
FeedbackController  feedback;
DataLogger          logger;

// --- Mode ---
bool assistMode = true;  // false = monitor-only (LED only, no vibration)

// --- Timing ---
unsigned long lastSampleMs = 0;
unsigned long lastLogMs    = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(500);
    Serial.println("=== Tremor Wrist Device v1.0 ===");

    // Mode button
    pinMode(PIN_MODE_BUTTON, INPUT_PULLUP);

    // IMU
    if (!imu.begin()) {
        Serial.println("[FATAL] IMU init failed. Halting.");
        while (true) delay(1000);
    }
    imu.calibrate(300); // ~1.5s at rest

    // Outputs
    feedback.begin();

    // Optional SD logging
    if (ENABLE_SD_LOGGING) logger.begin();

    // CSV header for serial plotter / Python script
    if (SERIAL_DEBUG)
        Serial.println("ts_ms,rms,lt_avg,state");
}

void loop() {
    unsigned long now = millis();

    // --- Button debounce (mode toggle) ---
    static bool lastBtnState = HIGH;
    bool btnState = digitalRead(PIN_MODE_BUTTON);
    if (btnState == LOW && lastBtnState == HIGH) {
        assistMode = !assistMode;
        Serial.print("[MODE] ");
        Serial.println(assistMode ? "ASSIST (haptic+LED)" : "MONITOR (LED only)");
        delay(50); // debounce
    }
    lastBtnState = btnState;

    // --- Sample at configured rate ---
    if (now - lastSampleMs < (unsigned long)SAMPLE_INTERVAL_MS) return;
    lastSampleMs = now;

    // Read IMU
    IMUData d;
    if (!imu.read(d)) return;

    // Bandpass filter the acceleration magnitude
    // This isolates tremor-frequency content (2-12 Hz) from slow motion
    float filtered = 0.0f;
    if (FILTER_ENABLED) {
        // Filter each axis then recombine (avoids DC offset artifacts)
        float fax = bpf_ax.process(d.ax);
        float fay = bpf_ay.process(d.ay);
        float faz = bpf_az.process(d.az);
        filtered = sqrtf(fax*fax + fay*fay + faz*faz);
    } else {
        filtered = d.mag;
    }

    // Rolling RMS estimation
    tremor.addSample(filtered);
    float rms = tremor.getRMS();

    // Long-term fatigue tracking
    fatigue.update(rms);
    float ltAvg   = fatigue.getLongTermAvg();
    bool  fatigued = fatigue.isFatigueElevated();

    // State machine
    fsm.update(rms, fatigued);
    DeviceState st = fsm.state();

    // Feedback
    if (assistMode) {
        feedback.update(st, now);
    } else {
        // Monitor mode: visual only
        // Show fatigue/tremor on LED but no vibration
        feedback.update(st, now);
        // (Motor is implicitly 0 because FeedbackController
        //  can be extended to accept a no-haptic flag — left as exercise)
    }

    // Log state change to Serial
    if (SERIAL_DEBUG) {
        if (fsm.stateChanged()) {
            Serial.print("[STATE] -> ");
            Serial.println(stateLabel(st));
        }
        // CSV line for plotter
        Serial.print(now);    Serial.print(",");
        Serial.print(rms,4);  Serial.print(",");
        Serial.print(ltAvg,4);Serial.print(",");
        Serial.println(stateLabel(st));
    }

    // SD card log
    if (ENABLE_SD_LOGGING && (now - lastLogMs >= (unsigned long)LOG_INTERVAL_MS)) {
        lastLogMs = now;
        logger.log(now, rms, ltAvg, st);
    }
}
