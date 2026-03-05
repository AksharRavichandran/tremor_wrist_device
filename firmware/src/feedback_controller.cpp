#include "feedback_controller.h"
#include "../../firmware/config/config.h"
#include "../../firmware/config/pins.h"

void FeedbackController::begin() {
    // LED PWM channels
    ledcSetup(PWM_CH_LED_R, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcSetup(PWM_CH_LED_G, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcSetup(PWM_CH_LED_B, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcAttachPin(PIN_LED_RED,   PWM_CH_LED_R);
    ledcAttachPin(PIN_LED_GREEN, PWM_CH_LED_G);
    ledcAttachPin(PIN_LED_BLUE,  PWM_CH_LED_B);

    // Motor PWM channels
    ledcSetup(PWM_CH_MOTOR1, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcSetup(PWM_CH_MOTOR2, PWM_FREQ_HZ, PWM_RESOLUTION);
    ledcAttachPin(PIN_VIBRATION_1, PWM_CH_MOTOR1);
    ledcAttachPin(PIN_VIBRATION_2, PWM_CH_MOTOR2);

    setLED(0, 255, 0); // Boot: green
    setMotor(0);
}

void FeedbackController::update(DeviceState state, unsigned long now) {
    switch (state) {

        case DeviceState::STABLE:
            setLED(0, 200, 0);
            setMotor(0);
            break;

        case DeviceState::MILD_TREMOR:
            setLED(200, 130, 0); // warm yellow
            // Slow pulse: on for VIB_PULSE_ON_MS, off for VIB_PULSE_OFF_MS
            if (!_pulse1 && (now - _lastToggle >= (unsigned long)VIB_PULSE_OFF_MS)) {
                _pulse1 = true; _lastToggle = now;
            } else if (_pulse1 && (now - _lastToggle >= (unsigned long)VIB_PULSE_ON_MS)) {
                _pulse1 = false; _lastToggle = now;
            }
            setMotor(_pulse1 ? VIB_PWM_MILD : 0);
            break;

        case DeviceState::SEVERE_TREMOR:
            setLED(255, 0, 0);
            setMotor(VIB_PWM_SEVERE);
            break;

        case DeviceState::FATIGUE_ELEVATED:
            setLED(128, 0, 160); // purple
            // Double-pulse pattern: pulse-gap-pulse-longoff
            // Simplified: use two independent timers
            if (!_pulse1 && (now - _lastToggle >= 800UL)) {
                _pulse1 = true; _lastToggle = now;
            } else if (_pulse1 && (now - _lastToggle >= (unsigned long)VIB_PULSE_ON_MS)) {
                _pulse1 = false; _lastToggle = now;
            }
            setMotor(_pulse1 ? VIB_PWM_FATIGUE : 0);
            break;
    }
}

void FeedbackController::setLED(uint8_t r, uint8_t g, uint8_t b) {
    ledcWrite(PWM_CH_LED_R, r);
    ledcWrite(PWM_CH_LED_G, g);
    ledcWrite(PWM_CH_LED_B, b);
}

void FeedbackController::setMotor(uint8_t duty) {
    ledcWrite(PWM_CH_MOTOR1, duty);
    ledcWrite(PWM_CH_MOTOR2, duty);
}
