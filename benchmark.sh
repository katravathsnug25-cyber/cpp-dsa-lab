#!/usr/bin/env bash
set -euo pipefail
for s in 24-lock-free-ring-buffer/spsc_ring_buffer.cpp 26-red-black-order-statistic-tree/order_statistic_tree.cpp 31-lru-lfu-cache-engine/lfu_cache.cpp 33-astar-pathfinding-grid/astar_grid.cpp; do
  b=".tmp_bench_$$"
  f=""; grep -q "std::thread" "$s" && f="-pthread"
  g++ -std=c++17 -O2 $f -Wall -Wextra -Werror "$s" -o "$b" && ./"$b" && rm -f "$b"
done
