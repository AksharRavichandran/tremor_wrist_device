import Foundation

final class SessionStore: ObservableObject {
    @Published private(set) var sessions: [TremorSession] = []

    func add(_ session: TremorSession) {
        sessions.insert(session, at: 0)
    }
}
