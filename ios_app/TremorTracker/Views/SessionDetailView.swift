import SwiftUI

struct SessionDetailView: View {
    let session: TremorSession

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 16) {
                summaryCard
                metricsCard
                stateBreakdownCard
            }
            .padding()
        }
        .navigationTitle("Session")
    }

    private var summaryCard: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text(session.title)
                .font(.headline)
            Text(session.subtitle)
                .foregroundColor(.secondary)
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding()
        .background(Color(.secondarySystemBackground))
        .cornerRadius(12)
    }

    private var metricsCard: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text("Key Metrics")
                .font(.headline)
            metricRow(label: "Duration", value: session.durationText)
            metricRow(label: "Mean RMS", value: session.meanRMSText)
            metricRow(label: "Peak RMS", value: session.peakRMSText)
            metricRow(label: "Final Fatigue", value: session.finalFatigueText)
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding()
        .background(Color(.secondarySystemBackground))
        .cornerRadius(12)
    }

    private var stateBreakdownCard: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text("State Breakdown")
                .font(.headline)
            ForEach(session.stateBreakdown.sorted(by: { $0.key < $1.key }), id: \.key) { state, percent in
                metricRow(label: state, value: String(format: "%.1f%%", percent))
            }
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding()
        .background(Color(.secondarySystemBackground))
        .cornerRadius(12)
    }

    private func metricRow(label: String, value: String) -> some View {
        HStack {
            Text(label)
            Spacer()
            Text(value)
                .foregroundColor(.secondary)
        }
    }
}

#Preview {
    SessionDetailView(session: TremorSession.sample)
}
