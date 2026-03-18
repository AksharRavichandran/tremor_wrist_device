# TremorTracker iOS (Scaffold)

This folder contains a minimal SwiftUI app scaffold that can import the device CSV logs
and show a session summary aligned to the firmware log format.

## Expected CSV columns
The importer accepts these headers (case-insensitive):
- `timestamp_ms` or `ts_ms`
- `rms_ms2` or `rms`
- `long_term_avg` or `lt_avg`
- `state`

This matches the firmware logger output in `firmware/src/data_logger.cpp`.

## How to run
1. Open Xcode and create a new **iOS App** project named `TremorTracker`.
2. Replace the default Swift files with the contents of `ios_app/TremorTracker/`.
3. Build and run on simulator or device.

## Current features
- Import CSV from Files using the system file picker.
- Session list with summary metrics.
- Session detail view (duration, RMS stats, fatigue, state breakdown).

## Next steps (optional)
- Add charts using Swift Charts for RMS/long-term trend.
- Persist sessions locally (Core Data or SQLite).
- Add per-session notes (meds, sleep, stress) and filters.
