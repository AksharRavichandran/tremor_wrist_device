# Bill of Materials

| # | Component | Specific Part | Est. Cost | Supplier |
|---|-----------|--------------|-----------|----------|
| 1 | Microcontroller | ESP32 Dev Board (38-pin) | $8–12 | Amazon, Adafruit, DigiKey |
| 2 | IMU | MPU6050 GY-521 breakout | $3–5 | Amazon, AliExpress |
| 3 | Vibration motor | 10mm coin ERM motor, 3V (×2) | $4–6 | Adafruit #1201, Amazon |
| 4 | Motor driver | 2N2222 NPN transistor (×2) | <$1 | Any electronics supplier |
| 5 | Flyback diode | 1N4001 (×2) | <$0.50 | Any |
| 6 | RGB LED | 5mm common-cathode RGB LED | $0.50 | Any |
| 7 | Resistors | 220Ω ×3 (LED), 1kΩ ×2 (transistor base) | <$0.50 | Any |
| 8 | Button | 6mm tactile push button | $0.25 | Any |
| 9 | Battery | 3.7V 1000mAh LiPo + JST connector | $8–12 | Adafruit, Amazon |
| 10 | Charger module | TP4056 with protection circuit | $2–3 | Amazon |
| 11 | Wrist strap | 25mm Velcro or elastic band, 25cm | $3–5 | Hardware store |
| 12 | Enclosure | 3D printed or ABS project box ~65×45×20mm | $3–8 | Self-print or Amazon |
| 13 | Prototyping | Half-size breadboard + jumper wires + perfboard | $8–12 | Amazon |
| (opt) 14 | SD card module | SPI micro-SD breakout | $3–5 | Adafruit, Amazon |
| (opt) 15 | micro-SD card | 8–32 GB micro-SD | $5–8 | Amazon |

**Estimated Total (without SD): ~$41–63**
**With SD logging: ~$49–76**

## Important Notes

- **Voltage**: MPU6050 runs at 3.3V. The GY-521 breakout includes a 3.3V regulator and level shifters, so it's safe to use with 5V Arduino or 3.3V ESP32.
- **Motor current**: Each ERM motor draws ~60–100mA. Do NOT drive directly from GPIO (max ~40mA on ESP32). Always use a transistor driver.
- **LiPo safety**: Use a TP4056 module *with* DW01 protection chip (overcharge/overdischarge/short protection). Do not use bare TP4056 without protection.
- **SD pin conflict**: On ESP32, SPI pins (18/19/23) overlap with LED pin suggestions. If enabling SD logging, remap LED pins to GPIO 32/33/27.
