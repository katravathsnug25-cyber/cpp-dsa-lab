/**
 * @file raft_election.cpp
 * @brief Raft leader election term & quorum state machine simulation in C++17.
 * Time Complexity : O(1) per election event
 * Space Complexity: O(1)
 */

#include <cassert>
#include <iostream>
#include <string>
using namespace std;

enum class RaftRole { Follower, Candidate, Leader };

class RaftNode {
    int id;
    int clusterSize;
    int currentTerm = 0;
    int votedFor = -1;
    int votesReceived = 0;
    RaftRole role = RaftRole::Follower;

public:
    RaftNode(int nodeId, int totalNodes) : id(nodeId), clusterSize(totalNodes) {
        assert(id >= 0 && clusterSize > 0);
    }

    void startElection() {
        ++currentTerm;
        role = RaftRole::Candidate;
        votedFor = id;
        votesReceived = 1;
    }

    bool grantVote(int candidateId, int candidateTerm) {
        if (candidateTerm < currentTerm) return false;
        if (candidateTerm > currentTerm) {
            currentTerm = candidateTerm;
            role = RaftRole::Follower;
            votedFor = -1;
            votesReceived = 0;
        }
        if (votedFor == -1 || votedFor == candidateId) {
            votedFor = candidateId;
            return true;
        }
        return false;
    }

    void recordVoteGrant(int grantTerm) {
        if (role == RaftRole::Candidate && grantTerm == currentTerm) {
            ++votesReceived;
            if (votesReceived * 2 > clusterSize) {
                role = RaftRole::Leader;
            }
        }
    }

    void observeHigherTerm(int higherTerm) {
        if (higherTerm > currentTerm) {
            currentTerm = higherTerm;
            role = RaftRole::Follower;
            votedFor = -1;
            votesReceived = 0;
        }
    }

    int term() const { return currentTerm; }
    RaftRole nodeRole() const { return role; }
    int votes() const { return votesReceived; }
};

int main() {
    RaftNode n0(0, 5);
    n0.startElection();
    assert(n0.term() == 1);
    assert(n0.nodeRole() == RaftRole::Candidate);
    assert(n0.votes() == 1);

    assert(n0.grantVote(2, 1) == false); // already voted for self in term 1
    assert(n0.grantVote(2, 2) == true);  // steps down to term 2 and votes for node 2
    assert(n0.nodeRole() == RaftRole::Follower);
    assert(n0.term() == 2);

    RaftNode leaderCand(1, 5);
    leaderCand.startElection();
    leaderCand.recordVoteGrant(1); // 2 votes
    leaderCand.recordVoteGrant(1); // 3 votes -> majority of 5
    assert(leaderCand.nodeRole() == RaftRole::Leader);

    cout << "[PASS] Raft Leader Election Sim OK\n";
    return 0;
}
