# Wiring Guide

## MPU6050 GY-521 → ESP32

| GY-521 Pin | ESP32 Pin | Notes |
|------------|-----------|-------|
| VCC        | 3V3       | 3.3V only — GY-521 has onboard regulator |
| GND        | GND       | |
| SDA        | GPIO 21   | Default I2C SDA |
| SCL        | GPIO 22   | Default I2C SCL |
| AD0        | GND       | Sets I2C address 0x68 (pull HIGH for 0x69) |
| INT        | (unused)  | Can use for interrupt-driven sampling later |

## Vibration Motor Circuit (per motor)

```
ESP32 GPIO25 ──[1kΩ]──► 2N2222 Base
                         2N2222 Collector ──► Motor (–)
                         2N2222 Emitter  ──► GND
                         Motor (+)       ──► 3.3V
                         1N4001 anode    ──► Collector
                         1N4001 cathode  ──► Motor (+) / 3.3V
```
Repeat with GPIO26 for motor 2.

## RGB LED (common cathode)

| LED Pin      | ESP32 Pin | In Series |
|--------------|-----------|-----------|
| Red anode    | GPIO 18   | 220Ω      |
| Green anode  | GPIO 19   | 220Ω      |
| Blue anode   | GPIO 4    | 220Ω      |
| Cathode      | GND       | —         |

> Use GPIO 4 for blue — NOT GPIO 21 (that's I2C SDA).

## Mode Button

```
ESP32 GPIO15 ── [Button] ── GND
(Firmware uses INPUT_PULLUP; press = LOW)
```

## Power

**Development**: USB-C directly to ESP32 dev board.

**Wearable prototype**:
```
LiPo (+) ──► TP4056 BAT+
LiPo (–) ──► TP4056 BAT–
TP4056 OUT+ ──► ESP32 VIN (or 5V pin via boost if needed)
TP4056 OUT– ──► GND
```
The ESP32 dev board has an onboard 3.3V LDO, so feed it 4.2V–5V on VIN.

## Optional: SD Card (SPI)

| SD Module | ESP32 Pin | Notes |
|-----------|-----------|-------|
| CS        | GPIO 5    | |
| MOSI      | GPIO 23   | |
| MISO      | GPIO 19   | Conflicts with LED green — remap LED if using SD |
| SCK       | GPIO 18   | Conflicts with LED red — remap LED if using SD |
| VCC       | 3V3       | |
| GND       | GND       | |

If using SD, move LED pins to GPIO 32 (R), 33 (G), 27 (B) and update pins.h.
