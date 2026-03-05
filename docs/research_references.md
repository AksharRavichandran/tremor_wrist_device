# Research References

Annotated bibliography organized by topic. Start with the bolded entries per section.

---

## 1. Tremor Quantification with Wearable IMUs

**Patel S. et al. (2010)**
"Monitoring motor fluctuations in patients with Parkinson's disease using wearable sensors."
*IEEE Transactions on Information Technology in Biomedicine*, 14(6), 1413–1421.
https://doi.org/10.1109/TITB.2010.2049573
> Direct relevance: validates RMS of accelerometer signal against clinical tremor scales.
> Shows that wrist-mounted IMU RMS correlates well with UPDRS tremor scores.
> START HERE for understanding the signal processing approach.

**Grimaldi G. & Manto M. (2008)**
"Neurological tremor: sensors, signal processing and emerging applications."
*Sensors*, 8(3), 1399–1422.
https://doi.org/10.3390/s8031399
> Covers all clinical tremor frequency bands (4–12 Hz), sampling rate requirements,
> and why bandpass filtering is essential before RMS estimation.
> Essential reading for filter design choices.

Keijsers N.L. et al. (2006)
"Ambulatory assessment of tremor intensity and patient activity."
*Movement Disorders*, 21(2), 244–248.
> Validates variance-based tremor quantification against clinical ratings in ambulatory setting.

Tsipouras M.G. et al. (2012)
"Automated methodology for levodopa-induced dyskinesia assessment based on wrist accelerometer data."
*Computer Methods and Programs in Biomedicine*, 107, 97–106.
> Demonstrates automated motion classification using variance features from wrist IMU —
> methodologically similar to this project.

Pulliam C.L. et al. (2012)
"Continuous inertial measurement unit monitoring can accurately quantify freezing of gait and tremor."
*Gait & Posture*, 35(1), 153–155.
> Real-time monitoring with consumer IMUs; practical implementation notes.

---

## 2. MS-Specific Tremor

**Alusi S.H. et al. (2001)**
"A study of tremor in multiple sclerosis."
*Brain*, 124(4), 720–730.
https://doi.org/10.1093/brain/124.4.720
> Definitive clinical characterization of MS tremor: cerebellar tremor typically 2.5–6 Hz,
> present in ~25–58% of MS patients. Sets the frequency target for your bandpass filter.

Feys P. et al. (2015)
"Technology-supported upper limb training in MS."
*Multiple Sclerosis Journal*, 21(9), 1154–1169.
> Review of wearable-assisted rehab interventions for MS upper limb dysfunction.
> Useful for framing the clinical motivation.

Teepker M. et al. (2008)
"Tremor in MS — frequency, severity, and patient impact."
*Multiple Sclerosis*, 14(7), 981–987.
> Patient-reported impact of tremor; relevant for justifying the assistive feedback approach.

---

## 3. Haptic Feedback for Tremor Assistance

**Salminen A-L. et al. (2018)**
"Vibrotactile feedback for tremor suppression in wearable devices: a review."
*Journal of NeuroEngineering and Rehabilitation*, 15, 79.
https://doi.org/10.1186/s12984-018-0418-4
> Direct match: reviews vibrotactile feedback paradigms for tremor conditions,
> compares pulsed vs continuous patterns, reports effectiveness data.
> Essential reading for your feedback_controller design decisions.

Prochazka A. et al. (1997)
"Sensory prompting in tremor suppression."
*Journal of Neurophysiology*.
> Foundational work showing vibrotactile cues can modulate voluntary motor control
> via muscle spindle afferent pathways.

Rocon E. et al. (2007)
"Design and validation of a rehabilitation robotic exoskeleton for tremor assessment and suppression."
*IEEE Transactions on Neural Systems and Rehabilitation Engineering*, 15(3), 367–378.
> Covers the upper-limb tremor sensing and feedback pipeline; more complex than your
> project but useful reference for threshold and feedback design.

---

## 4. Fatigue in MS

Freal J.E. et al. (1984)
"Symptomatic fatigue in MS."
*Archives of Physical Medicine and Rehabilitation*, 65, 135–138.
> Original characterization of MS fatigue as a distinct symptom, affecting ~78% of patients.

Induruwa I. et al. (2012)
"Fatigue in MS — a brief review."
*Journal of the Neurological Sciences*, 323(1–2), 9–15.
> Concise review of MS fatigue mechanisms; supports the clinical rationale for
> your long-term trend monitoring feature.

---

## 5. Wearable Embedded Systems Design

**Majumder S. et al. (2017)**
"Wearable sensors for remote health monitoring."
*Sensors*, 17(1), 130.
https://doi.org/10.3390/s17010130
> Excellent overview of IMU-based wearable sensor architecture (power, latency, form factor).
> Validates the ESP32 + IMU + battery architecture for health monitoring applications.

Yang G-Z. (2006)
*Body Sensor Networks*. Springer, London.
> Textbook covering wearable sensor network design; relevant chapters on power budgeting
> and sensor placement.

---

## 6. Key Datasheets & Technical Docs

| Resource | URL |
|----------|-----|
| MPU6050 Datasheet | https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf |
| MPU6050 Register Map | https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf |
| ElectronicCats MPU6050 Arduino Library | https://github.com/ElectronicCats/mpu6050 |
| ESP32 LEDC PWM Docs | https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html |
| PlatformIO ESP32 Board Config | https://docs.platformio.org/en/latest/boards/espressif32/esp32dev.html |
| FIR Filter Design (scipy) | https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.firwin.html |

---

## 7. Tremor Frequency Reference Table

| Tremor Type             | Frequency Band | Notes |
|-------------------------|----------------|-------|
| MS cerebellar tremor    | 2.5–6 Hz       | Primary target for this device |
| Essential tremor        | 4–8 Hz         | Overlap; good test population |
| Parkinsonian rest tremor| 4–6 Hz         | Similar band, different mechanism |
| Physiological tremor    | 8–12 Hz        | Passes filter; contributes to baseline |
| Voluntary motion        | 0–2 Hz         | Rejected by highpass component |
| Electrical noise        | >50 Hz         | Rejected by MPU6050 DLPF + FIR |

> At 100 Hz sampling, Nyquist = 50 Hz — well above all relevant tremor bands.
