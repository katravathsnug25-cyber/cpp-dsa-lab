/**
 * @file wal_kv_store.cpp
 * @brief In-memory Key-Value store with Write-Ahead Log (WAL) replay in C++17.
 * Time Complexity : Put/Get O(1) avg, Replay O(L) for L log records
 * Space Complexity: O(K + L)
 */

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class WalKvStore {
    string walPath;
    unordered_map<string, string> kv;

public:
    explicit WalKvStore(string path) : walPath(std::move(path)) {}

    bool put(const string& key, const string& val) {
        ofstream out(walPath, ios::app);
        if (!out.is_open()) return false;
        out << key << '\t' << val << '\n';
        out.flush();
        if (!out.good()) return false;
        kv[key] = val;
        return true;
    }

    bool get(const string& key, string& outVal) const {
        auto it = kv.find(key);
        if (it == kv.end()) return false;
        outVal = it->second;
        return true;
    }

    bool recover() {
        kv.clear();
        ifstream in(walPath);
        if (!in.is_open()) return false;
        string key, val;
        while (getline(in, key, '\t') && getline(in, val)) {
            kv[key] = val;
        }
        return true;
    }
};

int main() {
    const string tmpWal = "test_wal_tmp.log";
    remove(tmpWal.c_str());

    {
        WalKvStore store(tmpWal);
        assert(store.put("env", "prod"));
        assert(store.put("region", "ap-south-1"));
        assert(store.put("env", "staging"));
    }

    WalKvStore recovered(tmpWal);
    assert(recovered.recover());
    string v;
    assert(recovered.get("env", v) && v == "staging");
    assert(recovered.get("region", v) && v == "ap-south-1");

    remove(tmpWal.c_str());
    cout << "[PASS] WAL KV Store OK\n";
    return 0;
}
