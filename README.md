# cpp-dsa-lab

Structured, clean C++17 Data Structures, Algorithms, Number Theory, Dynamic Programming, Backtracking, Matrix Exponentiation, Game Theory, Concurrency, Distributed Systems, AI/Agent Primitives, Network Flow, Geometry, Probabilistic Structures, Lock-Free Buffers, WAL Storage, Order Statistic Trees, Suffix Arrays, MapReduce, Raft Consensus, and RAG Pipelines with built-in edge-case assertions.

## Compile & Run (C++17)

g++ -std=c++17 -Wall -Wextra -Werror <folder/file.cpp> -o app && ./app

# Or run modules 21-30 test suite:
./run_all.sh

## Modules & Complexity

| Folder | File | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| 01-arrays-strings | binary_search.cpp | O(log n) | O(1) |
| 02-linked-list | singly_linked_list.cpp | O(n) | O(1) |
| 03-stack-queue | valid_parentheses.cpp | O(n) | O(n) |
| 04-trees-graphs | bfs_traversal.cpp | O(V + E) | O(V) |
| 05-sorting-searching | merge_sort.cpp | O(n log n) | O(n) |
| 06-cpp17-core | modern_cpp_essentials.cpp | O(n) | O(n) |
| 07-maths-number-theory | legendary_maths.cpp | O(n log log n) | O(n) |
| 08-logical-mystery | logic_and_mystery.cpp | O(n) | O(1) |
| 09-dynamic-programming | dp_classics.cpp | O(n * W) | O(W) |
| 10-advanced-graphs-dsu | dijkstra_and_dsu.cpp | O(E log V) | O(V + E) |
| 11-tries-advanced-strings | trie_and_kmp.cpp | O(N + M) | O(M) |
| 12-segment-fenwick-trees | range_queries.cpp | O(log N) | O(N) |
| 13-backtracking-branch-bound | n_queens.cpp | O(N!) pruned | O(N) |
| 14-matrix-expo-combinatorics | matrix_expo.cpp | O(k^3 log N) | O(k^2) |
| 15-game-theory-bitmask | nim_and_bitmask_dp.cpp | O(N^2 * 2^N) | O(N * 2^N) |
| 16-ai-cloud-systems | agent_cloud_primitives.cpp | O(1) / O(N log K) | O(C + K) |
| 17-concurrency-thread-pool | thread_pool.cpp | O(1) enqueue | O(T + W) |
| 18-distributed-consensus-hashing | consistent_hashing.cpp | O(V log(N * V)) | O(N * V) |
| 19-ai-memory-vector-search | vector_memory.cpp | O(M * D + K log M) | O(M * D) |
| 20-agent-orchestrator-fsm | agent_fsm.cpp | O(1) | O(1) |
| 21-network-flow-matching | max_flow.cpp | O(V * E^2) | O(V + E) |
| 22-computational-geometry | convex_hull.cpp | O(N log N) | O(N) |
| 23-probabilistic-data-structures | bloom_filter.cpp | O(K) | O(M) bits |
| 24-lock-free-ring-buffer | spsc_ring_buffer.cpp | O(1) | O(N) |
| 25-kv-storage-wal | wal_kv_store.cpp | O(1) / O(L) | O(K + L) |
| 26-red-black-order-statistic-tree | order_statistic_tree.cpp | O(log N) avg | O(N) |
| 27-string-hashing-suffix-array | rolling_hash_and_sa.cpp | O(N log^2 N) | O(N) |
| 28-parallel-mapreduce-work-stealing | parallel_mapreduce.cpp | O(N / T + M log M) | O(M + T) |
| 29-raft-leader-election-sim | raft_election.cpp | O(1) | O(1) |
| 30-rag-hybrid-ranker-pipeline | rag_pipeline.cpp | O(M * D + K log M) | O(M * D + C) |
