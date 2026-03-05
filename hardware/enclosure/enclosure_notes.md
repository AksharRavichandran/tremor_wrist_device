# Enclosure & Physical Build Notes

## Target Dimensions
- ~65mm (L) × 45mm (W) × 22mm (H)
- Worn on dorsal (top) side of wrist
- Strap threads through slots on left/right ends

## Component Layout (top-down, from inside)
1. ESP32 dev board — center, lengthwise
2. MPU6050 — rigidly fixed near center of board; IMU must NOT flex relative to wrist
3. LiPo — below ESP32 (flat cell fits well)
4. Vibration motor(s) — inner face of top shell; vibration transmits through housing to wrist
5. RGB LED — top face, small hole or frosted diffuser window
6. Button — side wall, accessible with thumb

## Fabrication Options

### Option A: 3D Print (Recommended)
- Material: PLA (lightweight) or PETG (more durable)
- Two-part design: bottom tray + snap-fit lid
- Add M2 heat-set brass inserts for screw-together assembly
- Leave 3mm relief channels for wires with zip-tie strain relief points
- Strap slots: 27mm wide × 4mm tall, one each end

### Option B: Commercial ABS Project Box
- Size: Hammond 1551KBK or similar (~65×56×25mm)
- Modify with Dremel: cutouts for LED window, button, USB port, strap slots
- Mount components on M2 standoffs or with hot glue + foam padding

## IMU Mounting — Critical
- The IMU must be **hard-mounted** to the enclosure, which is **hard-mounted** to the wrist
- Any play or flex in the mounting introduces noise that looks like tremor
- Recommended: M2 screws through GY-521 mounting holes into standoffs
- Do NOT use hot glue alone for IMU — too flexible

## Wire Management
- Use 28 AWG silicone-jacketed wire (flexible, won't fatigue and break)
- Add a zip-tie or hot glue bead where wires exit PCB as strain relief
- Use JST-SH 1.0mm connectors between ESP32 and sensors for disassembly

## Weight Target
- Keep under 60g total (including battery and enclosure)
- A 500mAh LiPo weighs ~11g; 1000mAh ~20g
- ESP32 dev board ~8g; full assembly realistic at 40–55g
