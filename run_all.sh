#!/usr/bin/env bash
set -euo pipefail

build_and_run() {
    local src="$1"
    local use_pthread="${2:-0}"
    local bin=".tmp_bin_$$"
    if [[ "$use_pthread" -eq 1 ]]; then
        g++ -std=c++17 -pthread -Wall -Wextra -Werror "$src" -o "$bin"
    else
        g++ -std=c++17 -Wall -Wextra -Werror "$src" -o "$bin"
    fi
    ./"$bin"
    rm -f "$bin"
}

build_and_run "21-network-flow-matching/max_flow.cpp" 0
build_and_run "22-computational-geometry/convex_hull.cpp" 0
build_and_run "23-probabilistic-data-structures/bloom_filter.cpp" 0
build_and_run "24-lock-free-ring-buffer/spsc_ring_buffer.cpp" 1
build_and_run "25-kv-storage-wal/wal_kv_store.cpp" 0
build_and_run "26-red-black-order-statistic-tree/order_statistic_tree.cpp" 0
build_and_run "27-string-hashing-suffix-array/rolling_hash_and_sa.cpp" 0
build_and_run "28-parallel-mapreduce-work-stealing/parallel_mapreduce.cpp" 1
build_and_run "29-raft-leader-election-sim/raft_election.cpp" 0
build_and_run "30-rag-hybrid-ranker-pipeline/rag_pipeline.cpp" 0
build_and_run "31-lru-lfu-cache-engine/lfu_cache.cpp" 0
build_and_run "32-skip-list-probabilistic-map/skip_list.cpp" 0
build_and_run "33-astar-pathfinding-grid/astar_grid.cpp" 0
build_and_run "34-circuit-breaker-resilience/circuit_breaker.cpp" 0
build_and_run "35-sliding-window-rate-limiter/sliding_window_limiter.cpp" 0

echo "[ALL PASS] Modules 21-35 verified cleanly."
