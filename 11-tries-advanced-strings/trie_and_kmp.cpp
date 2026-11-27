/**
 * @file trie_and_kmp.cpp
 * @brief Prefix Tree (Trie) and Knuth-Morris-Pratt (KMP) string matching in C++17.
 * Time Complexity : Trie Insert/Search O(L), KMP O(N + M)
 * Space Complexity: Trie O(Total Characters), KMP O(M)
 */

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Trie {
    struct Node {
        array<unique_ptr<Node>, 26> children{};
        bool isEnd = false;
    };
    unique_ptr<Node> root = make_unique<Node>();

public:
    void insert(const string& word) {
        Node* curr = root.get();
        for (char c : word) {
            int idx = c - 'a';
            assert(idx >= 0 && idx < 26);
            if (!curr->children[idx]) {
                curr->children[idx] = make_unique<Node>();
            }
            curr = curr->children[idx].get();
        }
        curr->isEnd = true;
    }

    bool search(const string& word) const {
        const Node* curr = root.get();
        for (char c : word) {
            int idx = c - 'a';
            if (idx < 0 || idx >= 26 || !curr->children[idx]) return false;
            curr = curr->children[idx].get();
        }
        return curr->isEnd;
    }
};

vector<int> kmpSearch(const string& text, const string& pattern) {
    if (pattern.empty() || text.size() < pattern.size()) return {};
    int m = static_cast<int>(pattern.size());
    vector<int> lps(m, 0);
    for (int i = 1, len = 0; i < m;) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len > 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }

    vector<int> matches;
    int n = static_cast<int>(text.size());
    for (int i = 0, j = 0; i < n;) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
            if (j == m) {
                matches.push_back(i - j);
                j = lps[j - 1];
            }
        } else if (j > 0) {
            j = lps[j - 1];
        } else {
            ++i;
        }
    }
    return matches;
}

int main() {
    Trie trie;
    trie.insert("mastermax");
    trie.insert("master");
    assert(trie.search("mastermax"));
    assert(trie.search("master"));
    assert(!trie.search("mast"));

    assert(kmpSearch("ababcabcabababd", "ababd") == vector<int>({10}));
    assert(kmpSearch("aaaaa", "aa") == vector<int>({0, 1, 2, 3}));
    cout << "[PASS] Trie & KMP OK\n";
    return 0;
}
