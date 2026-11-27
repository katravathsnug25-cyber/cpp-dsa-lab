/**
 * @file agent_fsm.cpp
 * @brief Deterministic Finite State Machine (FSM) and exponential backoff retry policy in C++17.
 * Time Complexity : State transition O(1), Backoff calculation O(1)
 * Space Complexity: O(1)
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
using namespace std;

enum class AgentState {
    Idle,
    Planning,
    ToolExecuting,
    Retrying,
    Completed,
    Failed
};

enum class AgentEvent {
    Start,
    PlanReady,
    ToolSuccess,
    ToolError,
    RetryAllowed,
    RetryExhausted
};

class AgentOrchestrator {
    AgentState state = AgentState::Idle;
    int retriesUsed = 0;
    int maxRetries;

public:
    explicit AgentOrchestrator(int maxRetryCount) : maxRetries(maxRetryCount) {
        assert(maxRetries >= 0);
    }

    AgentState currentState() const { return state; }
    int retryCount() const { return retriesUsed; }

    static long long backoffMs(int attemptIndex, long long baseMs = 100, long long maxMs = 4000) {
        assert(attemptIndex >= 0 && baseMs > 0 && maxMs >= baseMs);
        long long shift = min<long long>(attemptIndex, 30);
        long long delay = baseMs * (1LL << shift);
        return min(delay, maxMs);
    }

    bool step(AgentEvent ev) {
        switch (state) {
            case AgentState::Idle:
                if (ev == AgentEvent::Start) {
                    state = AgentState::Planning;
                    return true;
                }
                return false;

            case AgentState::Planning:
                if (ev == AgentEvent::PlanReady) {
                    state = AgentState::ToolExecuting;
                    return true;
                }
                return false;

            case AgentState::ToolExecuting:
                if (ev == AgentEvent::ToolSuccess) {
                    state = AgentState::Completed;
                    return true;
                }
                if (ev == AgentEvent::ToolError) {
                    if (retriesUsed < maxRetries) {
                        ++retriesUsed;
                        state = AgentState::Retrying;
                    } else {
                        state = AgentState::Failed;
                    }
                    return true;
                }
                return false;

            case AgentState::Retrying:
                if (ev == AgentEvent::RetryAllowed) {
                    state = AgentState::ToolExecuting;
                    return true;
                }
                if (ev == AgentEvent::RetryExhausted) {
                    state = AgentState::Failed;
                    return true;
                }
                return false;

            case AgentState::Completed:
            case AgentState::Failed:
                return false;
        }
        return false;
    }

    static string stateName(AgentState s) {
        switch (s) {
            case AgentState::Idle: return "Idle";
            case AgentState::Planning: return "Planning";
            case AgentState::ToolExecuting: return "ToolExecuting";
            case AgentState::Retrying: return "Retrying";
            case AgentState::Completed: return "Completed";
            case AgentState::Failed: return "Failed";
        }
        return "Unknown";
    }
};

int main() {
    assert(AgentOrchestrator::backoffMs(0, 100, 4000) == 100);
    assert(AgentOrchestrator::backoffMs(2, 100, 4000) == 400);
    assert(AgentOrchestrator::backoffMs(10, 100, 4000) == 4000);

    AgentOrchestrator orch(2);
    assert(orch.step(AgentEvent::Start));
    assert(orch.step(AgentEvent::PlanReady));
    assert(orch.step(AgentEvent::ToolError));
    assert(orch.currentState() == AgentState::Retrying);
    assert(orch.retryCount() == 1);
    assert(orch.step(AgentEvent::RetryAllowed));
    assert(orch.step(AgentEvent::ToolSuccess));
    assert(orch.currentState() == AgentState::Completed);

    cout << "[PASS] Agent Orchestrator FSM OK\n";
    return 0;
}
