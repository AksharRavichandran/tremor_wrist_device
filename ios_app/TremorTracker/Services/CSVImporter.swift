import Foundation

enum CSVImportError: LocalizedError {
    case unreadable
    case missingColumns
    case empty

    var errorDescription: String? {
        switch self {
        case .unreadable:
            return "Could not read the CSV file."
        case .missingColumns:
            return "CSV is missing required columns."
        case .empty:
            return "CSV file is empty."
        }
    }
}

struct CSVImporter {
    static func importSession(from url: URL) throws -> TremorSession {
        let fileName = url.lastPathComponent
        let data = try Data(contentsOf: url)
        guard let content = String(data: data, encoding: .utf8) else {
            throw CSVImportError.unreadable
        }

        var lines = content.split(whereSeparator: \.isNewline).map { String($0) }
        guard !lines.isEmpty else { throw CSVImportError.empty }

        let header = lines.removeFirst().split(separator: ",").map { $0.trimmingCharacters(in: .whitespaces) }
        let columnIndex = columns(from: header)
        guard columnIndex.isValid else { throw CSVImportError.missingColumns }

        let samples = lines.compactMap { line -> TremorSample? in
            let cols = line.split(separator: ",", omittingEmptySubsequences: false).map { String($0) }
            guard let ts = columnIndex.value(cols, for: "ts"),
                  let rms = columnIndex.value(cols, for: "rms"),
                  let lt = columnIndex.value(cols, for: "ltavg"),
                  let stateRaw = columnIndex.stringValue(cols, for: "state") else {
                return nil
            }

            return TremorSample(
                timestampMs: ts,
                rms: rms,
                longTermAvg: lt,
                state: DeviceState.from(stateRaw)
            )
        }

        return TremorSession(sourceFilename: fileName, samples: samples, createdAt: Date())
    }

    private static func columns(from header: [String]) -> ColumnIndex {
        var map: [String: Int] = [:]
        for (idx, name) in header.enumerated() {
            map[name.lowercased()] = idx
        }

        let aliases: [String: [String]] = [
            "ts": ["timestamp_ms", "ts_ms", "ts"],
            "rms": ["rms_ms2", "rms"],
            "ltavg": ["long_term_avg", "lt_avg", "ltavg"],
            "state": ["state"]
        ]

        var resolved: [String: Int] = [:]
        for (canonical, names) in aliases {
            if let index = names.compactMap({ map[$0] }).first {
                resolved[canonical] = index
            }
        }

        return ColumnIndex(resolved: resolved)
    }
}

private struct ColumnIndex {
    let resolved: [String: Int]

    var isValid: Bool {
        ["ts", "rms", "ltavg", "state"].allSatisfy { resolved[$0] != nil }
    }

    func value(_ cols: [String], for key: String) -> Double? {
        guard let idx = resolved[key], idx < cols.count else { return nil }
        return Double(cols[idx].trimmingCharacters(in: .whitespaces))
    }

    func stringValue(_ cols: [String], for key: String) -> String? {
        guard let idx = resolved[key], idx < cols.count else { return nil }
        return cols[idx].trimmingCharacters(in: .whitespaces)
    }
}
