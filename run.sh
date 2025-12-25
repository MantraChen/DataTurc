#!/usr/bin/env bash
set -e

# ====== ≈‰÷√«¯ ======
CXX=g++
STD=gnu++20
CXXFLAGS="-O2 -Wall -Wextra -Iinclude"
OUT=main
# ===================
if [[ -n "$1" ]]; then
    SRC="$1"
else
    echo "”√∑®: ./run.sh problems/xxx.cpp"
    exit 1
fi

echo "[BUILD] $SRC"

$CXX -std=$STD $CXXFLAGS "$SRC" -o "$OUT"

echo "[RUN]"
./"$OUT"
