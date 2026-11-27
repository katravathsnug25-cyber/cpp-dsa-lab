#!/usr/bin/env bash
set -euo pipefail

count=0
while IFS= read -r src; do
    bin=".tmp_bin_$$"
    if grep -qE '#include <thread>|#include <mutex>|#include <condition_variable>|#include <future>' "$src"; then
        g++ -std=c++17 -pthread -Wall -Wextra -Werror "$src" -o "$bin"
    else
        g++ -std=c++17 -Wall -Wextra -Werror "$src" -o "$bin"
    fi
    ./"$bin"
    rm -f "$bin"
    count=$((count + 1))
done < <(find . -maxdepth 2 -mindepth 2 -name '*.cpp' | sort)

echo "[ALL PASS] All $count modules verified cleanly."
