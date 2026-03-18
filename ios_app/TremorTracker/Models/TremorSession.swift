import Foundation

struct TremorSession: Identifiable {
    let id = UUID()
    let sourceFilename: String
    let samples: [TremorSample]
    let createdAt: Date

    var durationSeconds: Double {
        guard let last = samples.last else { return 0 }
        return last.timestampMs / 1000.0
    }

    var meanRMS: Double {
        guard !samples.isEmpty else { return 0 }
        return samples.map { $0.rms }.reduce(0, +) / Double(samples.count)
    }

    var peakRMS: Double {
        samples.map { $0.rms }.max() ?? 0
    }

    var finalFatigue: Double {
        samples.last?.longTermAvg ?? 0
    }

    var stateBreakdown: [String: Double] {
        let total = Double(samples.count)
        guard total > 0 else { return [:] }
        let counts = samples.reduce(into: [:]) { partial, sample in
            partial[sample.state.rawValue, default: 0] += 1
        }
        return counts.mapValues { (Double($0) / total) * 100.0 }
    }

    var title: String {
        sourceFilename
    }

    var subtitle: String {
        "Duration: \(durationText)"
    }

    var durationText: String {
        String(format: "%.1f min", durationSeconds / 60.0)
    }

    var meanRMSText: String {
        String(format: "%.3f m/s²", meanRMS)
    }

    var peakRMSText: String {
        String(format: "%.3f m/s²", peakRMS)
    }

    var finalFatigueText: String {
        String(format: "%.3f m/s²", finalFatigue)
    }

    static var sample: TremorSession {
        let samples = (0..<300).map { idx in
            TremorSample(timestampMs: Double(idx * 100), rms: 0.6, longTermAvg: 0.4, state: .mild)
        }
        return TremorSession(sourceFilename: "sample.csv", samples: samples, createdAt: Date())
    }
}

struct TremorSample {
    let timestampMs: Double
    let rms: Double
    let longTermAvg: Double
    let state: DeviceState
}

enum DeviceState: String {
    case stable = "STABLE"
    case mild = "MILD_TREMOR"
    case severe = "SEVERE_TREMOR"
    case fatigue = "FATIGUE_ELEVATED"

    static func from(_ raw: String) -> DeviceState {
        DeviceState(rawValue: raw) ?? .stable
    }
}
