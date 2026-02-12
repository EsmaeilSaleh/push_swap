#!/usr/bin/env bash
set -euo pipefail

PS_BIN="${PS_BIN:-./push_swap}"
CHECKER_BIN=""

find_checker() {
	if [ -x ./checker ]; then
		CHECKER_BIN=./checker
	elif [ -x ./checker_linux ]; then
		CHECKER_BIN=./checker_linux
	elif command -v checker >/dev/null 2>&1; then
		CHECKER_BIN=checker
	fi
}

gen_sequence() {
	local n="$1"
	local seed="$2"

	awk -v n="$n" -v seed="$seed" 'BEGIN {
		srand(seed);
		for (i = 0; i < n; i++)
			a[i] = i;
		for (i = n - 1; i > 0; i--) {
			j = int(rand() * (i + 1));
			tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
		for (i = 0; i < n; i++)
			printf "%d%s", a[i], (i + 1 < n ? " " : "\n");
	}'
}

run_bench() {
	local size="$1"
	local runs="$2"
	local seed="$3"
	local i=0
	local total=0
	local min=2147483647
	local max=0
	local args
	local ops
	local count
	local verdict

	while [ "$i" -lt "$runs" ]; do
		args="$(gen_sequence "$size" "$seed")"
		ops="$($PS_BIN $args)"
		count="$(printf "%s\n" "$ops" | awk 'NF { c++ } END { print c + 0 }')"
		if [ "$count" -lt "$min" ]; then
			min="$count"
		fi
		if [ "$count" -gt "$max" ]; then
			max="$count"
		fi
		total=$((total + count))
		if [ -n "$CHECKER_BIN" ]; then
			verdict="$(printf "%s\n" "$ops" | $CHECKER_BIN $args | awk 'NF { print $NF }')"
			if [ "$verdict" != "OK" ]; then
				echo "[FAIL] checker returned '$verdict' for size $size" >&2
				exit 1
			fi
		fi
		seed=$((seed + 97))
		i=$((i + 1))
	done
	echo "size=$size runs=$runs min=$min avg=$(awk -v t="$total" -v r="$runs" 'BEGIN { printf "%.2f", t / r }') max=$max"
}

if [ ! -x "$PS_BIN" ]; then
	echo "push_swap binary not found/executable: $PS_BIN" >&2
	echo "Build first: make" >&2
	exit 1
fi

find_checker
if [ -n "$CHECKER_BIN" ]; then
	echo "Checker: $CHECKER_BIN"
else
	echo "Checker: not found (skipping validity check)"
fi

run_bench 3 "${RUNS_3:-100}" 103
run_bench 5 "${RUNS_5:-100}" 105
run_bench 10 "${RUNS_10:-100}" 110
run_bench 50 "${RUNS_50:-100}" 150
run_bench 100 "${RUNS_100:-50}" 200
run_bench 500 "${RUNS_500:-20}" 600
