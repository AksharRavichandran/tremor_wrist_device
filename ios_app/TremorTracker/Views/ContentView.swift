import SwiftUI
import UniformTypeIdentifiers

struct ContentView: View {
    @EnvironmentObject private var store: SessionStore
    @State private var showingImporter = false
    @State private var importError: String?

    var body: some View {
        NavigationView {
            VStack(spacing: 16) {
                header
                sessionList
            }
            .padding()
            .navigationTitle("Tremor Tracker")
            .toolbar {
                ToolbarItem(placement: .navigationBarTrailing) {
                    Button("Import CSV") {
                        showingImporter = true
                    }
                }
            }
            .fileImporter(
                isPresented: $showingImporter,
                allowedContentTypes: [UTType.commaSeparatedText, UTType.plainText],
                allowsMultipleSelection: false
            ) { result in
                handleImport(result)
            }
            .alert(item: $importError) { message in
                Alert(title: Text("Import Failed"), message: Text(message), dismissButton: .default(Text("OK")))
            }
        }
    }

    private var header: some View {
        VStack(alignment: .leading, spacing: 6) {
            Text("Track tremor sessions over time")
                .font(.headline)
            Text("Import CSV logs from the device SD card.")
                .font(.subheadline)
                .foregroundColor(.secondary)
        }
        .frame(maxWidth: .infinity, alignment: .leading)
    }

    private var sessionList: some View {
        List {
            if store.sessions.isEmpty {
                Text("No sessions yet. Tap Import CSV to get started.")
                    .foregroundColor(.secondary)
            } else {
                ForEach(store.sessions) { session in
                    NavigationLink(destination: SessionDetailView(session: session)) {
                        VStack(alignment: .leading, spacing: 4) {
                            Text(session.title)
                                .font(.headline)
                            Text(session.subtitle)
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                        }
                    }
                }
            }
        }
        .listStyle(.plain)
    }

    private func handleImport(_ result: Result<[URL], Error>) {
        do {
            let urls = try result.get()
            guard let url = urls.first else { return }
            let session = try CSVImporter.importSession(from: url)
            store.add(session)
        } catch {
            importError = error.localizedDescription
        }
    }
}

extension String: Identifiable {
    public var id: String { self }
}

#Preview {
    ContentView()
        .environmentObject(SessionStore())
}
