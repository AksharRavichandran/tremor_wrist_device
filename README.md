# Adaptive Wrist-Mounted Tremor Feedback + Fatigue Monitoring Device

A wearable embedded systems project for real-time tremor detection, haptic feedback,
and fatigue monitoring — designed to assist individuals with neurological conditions
such as Multiple Sclerosis (MS).

---

## System Architecture

```
[MPU6050 IMU] --I2C--> [ESP32 Microcontroller]
                              |
                    +---------+-----------+
                    |   Signal Processing  |
                    |  Bandpass 2-12 Hz   |
                    |  Rolling RMS Window  |
                    |  Fatigue Trend Avg   |
                    +---------+-----------+
                              |
              +---------------+---------------+
              v               v               v
      [Vibration Motor]   [RGB LED]    [SD Card Logger]
       Haptic Feedback    Status Ind.    (Optional)
```

## Device States

| State            | LED Color | Vibration Pattern  |
|------------------|-----------|--------------------|
| STABLE           | Green     | Off                |
| MILD_TREMOR      | Yellow    | Slow pulse         |
| SEVERE_TREMOR    | Red       | Continuous strong  |
| FATIGUE_ELEVATED | Purple    | Slow double-pulse  |

---

## Repository Structure

```
tremor-wrist-device/
├── firmware/
│   ├── src/
│   │   ├── main.cpp                    # Entry point
│   │   ├── imu_reader.{cpp,h}          # MPU6050 I2C + calibration
│   │   ├── bandpass_filter.{cpp,h}     # 2-12 Hz FIR filter
│   │   ├── tremor_estimator.{cpp,h}    # Rolling RMS/variance
│   │   ├── fatigue_tracker.{cpp,h}     # Long-term trend monitoring
│   │   ├── state_machine.{cpp,h}       # State transition logic
│   │   ├── feedback_controller.{cpp,h} # Haptic + LED output
│   │   └── data_logger.{cpp,h}         # Optional SD card logging
│   ├── config/
│   │   ├── config.h                    # All tunable thresholds
│   │   └── pins.h                      # GPIO assignments
│   └── lib/                            # Vendored or custom libs
├── hardware/
│   ├── bom.md                          # Bill of materials + prices
│   ├── wiring_guide.md                 # Pin-by-pin wiring diagram
│   └── enclosure/
│       └── enclosure_notes.md          # 3D print / fabrication notes
├── scripts/
│   ├── serial_plotter.py               # Live serial data visualizer
│   └── tremor_log_analyzer.py          # Post-session CSV analysis
├── docs/
│   ├── project_overview.md             # Design decisions + power budget
│   └── research_references.md         # Annotated paper list
├── platformio.ini                      # Build config
└── .gitignore
```

---

## Quickstart

### Prerequisites
- [VS Code](https://code.visualstudio.com/) + [PlatformIO extension](https://platformio.org/)
- ESP32 dev board, MPU6050 breakout, vibration motor(s), RGB LED
- See `hardware/bom.md` for complete parts list (~$30–55)

### Flash the Firmware
```bash
git clone https://github.com/yourname/tremor-wrist-device.git
cd tremor-wrist-device
pio run --target upload
pio device monitor --baud 115200
```

### Tune Parameters
All thresholds, window sizes, and timing live in `firmware/config/config.h`.
Start with defaults and adjust based on serial output during bench testing.

---

## Development Milestones

- [ ] IMU reads and prints calibrated data over Serial
- [ ] Bandpass filter reduces motion noise (verify in serial plotter)
- [ ] Rolling RMS correctly computed and thresholded
- [ ] State machine transitions verified on bench
- [ ] Haptic + LED feedback working for all 4 states
- [ ] Fatigue trend detection working over 2+ min session
- [ ] Full wearable assembly with wrist strap
- [ ] (Optional) SD card logging producing readable CSV

---

## License
MIT
