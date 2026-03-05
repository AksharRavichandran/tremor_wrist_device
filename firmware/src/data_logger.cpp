#include "data_logger.h"
#include "../../firmware/config/config.h"
#include "../../firmware/config/pins.h"

#if ENABLE_SD_LOGGING
#include <SD.h>
#include <SPI.h>
#endif

bool DataLogger::begin() {
#if ENABLE_SD_LOGGING
    if (!SD.begin(PIN_SD_CS)) {
        Serial.println("[SD] Init FAILED — check wiring and PIN_SD_CS");
        return false;
    }
    File f = SD.open(LOG_FILENAME, FILE_WRITE);
    if (f) {
        f.println("timestamp_ms,rms_ms2,long_term_avg,state");
        f.close();
        _ready = true;
        Serial.println("[SD] Logger ready: " LOG_FILENAME);
        return true;
    }
    Serial.println("[SD] Could not open file for writing");
    return false;
#else
    Serial.println("[SD] Logging disabled (ENABLE_SD_LOGGING=false)");
    return false;
#endif
}

void DataLogger::log(unsigned long ms, float rms, float ltAvg, DeviceState state) {
#if ENABLE_SD_LOGGING
    if (!_ready) return;
    File f = SD.open(LOG_FILENAME, FILE_APPEND);
    if (f) {
        f.print(ms);      f.print(",");
        f.print(rms, 4);  f.print(",");
        f.print(ltAvg,4); f.print(",");
        f.println(stateLabel(state));
        f.close();
    }
#endif
}
