# Project Overview & Design Decisions

## Signal Processing Pipeline

```
Raw IMU (ax,ay,az) @ 100Hz
        │
        ▼
  [MPU6050 DLPF @ 44Hz]   ← Hardware lowpass, removes >44Hz noise
        │
        ▼
  [FIR Bandpass 2-12Hz]   ← Software filter, isolates tremor band
        │
        ▼
  [Vector Magnitude]       ← sqrt(ax²+ay²+az²) of filtered axes
        │
        ▼
  [Rolling RMS Window]     ← 0.5s window, energy metric
        │
        ▼
  [State Machine]          ← Threshold-based classification
        │
        ▼
  [Feedback Controller]    ← Haptic + LED output
```

## Key Design Decisions

### Why FIR Bandpass Filter?
- Raw IMU magnitude conflates intentional wrist motion (~0.5–2 Hz) with tremor (2.5–12 Hz)
- Without filtering, walking or reaching triggers false tremor alerts
- The 2–12 Hz FIR bandpass isolates tremor-band content from both slow motion and electrical noise
- FIR (Finite Impulse Response) has linear phase — no time-domain distortion of the tremor signal
- IIR (e.g. Butterworth) is smaller but has nonlinear phase; acceptable alternative for this application

### Why RMS over Peak Amplitude?
- RMS (Root Mean Square) measures signal *energy*, not instantaneous peaks
- Tremor energy is more clinically meaningful than peak excursion
- Validated against UPDRS tremor scales in Patel et al. (2010) and Keijsers et al. (2006)
- Rolling RMS is robust to single-sample noise spikes

### Why 100 Hz Sampling?
- Nyquist requirement for 12 Hz ceiling: minimum 24 Hz
- 100 Hz provides 8× margin, handles aliasing safely
- MPU6050 internal sampling up to 1 kHz; 100 Hz via polling is straightforward
- Higher rates (200 Hz+) offer no benefit for tremor detection but increase CPU load

### Window Size Trade-off (TREMOR_WINDOW_SIZE = 50 → 0.5s)
- Too short (<20 samples, <0.2s): high variance, frequent false state changes
- Too long (>150 samples, >1.5s): slow to respond, feedback lag feels unresponsive
- 0.5s matches timescale of user-perceivable tremor episodes

### State Priority Design
Severe > Mild > Fatigue > Stable prevents fatigue alert from masking an active severe tremor event.
Fatigue is a background metric; acute tremor always takes priority for immediate feedback.

---

## Power Budget

| Component          | Current (typical) | Notes |
|--------------------|-------------------|-------|
| ESP32 (WiFi off)   | 80 mA             | Use modem_sleep or light_sleep to reduce |
| MPU6050 active     | 3.9 mA            | — |
| Vibration motor ×2 | 60–100 mA each    | When on; duty-cycle reduces average |
| RGB LED            | ~15 mA            | At mid-brightness (PWM ~128) |
| **Total (active)** | ~160–300 mA       | |
| **Total (avg)**    | ~120–180 mA       | Motors pulsed, not always on |

### Runtime Estimates
| Battery   | Avg Draw | Runtime |
|-----------|----------|---------|
| 500 mAh   | 150 mA   | ~3.3 hr |
| 1000 mAh  | 150 mA   | ~6.7 hr |
| 2000 mAh  | 150 mA   | ~13 hr  |

Recommendation: **1000 mAh LiPo** for 4–6 hour clinical session with comfortable margin.

---

## Calibration Procedure

1. Power on device
2. Rest wrist flat on table, completely still
3. Wait for "[IMU] Calibrating..." message (~1.5s collection)
4. Device will print "[IMU] Calibration done." and boot normally
5. Repeat calibration if worn orientation changes significantly

For different users or tremor severities, adjust thresholds in config.h based on serial output during rest and during peak tremor episodes.

---

## Future Enhancements

1. **Interrupt-driven IMU sampling** — Use MPU6050 INT pin for precise 100Hz timing
2. **Bluetooth serial** — Replace USB cable with BLE for wireless monitoring
3. **Spectral analysis** — FFT to distinguish tremor type by dominant frequency
4. **Adaptive thresholds** — Auto-tune THRESH_MILD based on user's baseline RMS
5. **OLED display** — Show numeric RMS and session time on small 0.96" OLED
