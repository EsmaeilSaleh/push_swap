#!/usr/bin/env bash
set -euo pipefail

PS_BIN="${PS_BIN:-./push_swap}"
CHECKER_BIN="${CHECKER_BIN:-}"
TIMEOUT_SEC="${TIMEOUT_SEC:-8}"

pick_checker() {
	if [ -n "$CHECKER_BIN" ] && [ -x "$CHECKER_BIN" ]; then
		return
	fi
	if [ -x ./checker_OS ]; then
		CHECKER_BIN=./checker_OS
	elif [ -x ./checker ]; then
		CHECKER_BIN=./checker
	elif command -v checker_OS >/dev/null 2>&1; then
		CHECKER_BIN=checker_OS
	elif command -v checker >/dev/null 2>&1; then
		CHECKER_BIN=checker
	else
		CHECKER_BIN=""
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

validate_with_awk() {
	local args="$1"
	local ops="$2"

	printf "%s\n" "$ops" | awk -v args="$args" '
	function sa_op(   t){ if (a_len < 2) return; t = a[1]; a[1] = a[2]; a[2] = t }
	function sb_op(   t){ if (b_len < 2) return; t = b[1]; b[1] = b[2]; b[2] = t }
	function pa_op(   i){ if (b_len < 1) return; for (i = a_len; i >= 1; i--) a[i + 1] = a[i]; a[1] = b[1]; a_len++; for (i = 1; i < b_len; i++) b[i] = b[i + 1]; delete b[b_len]; b_len-- }
	function pb_op(   i){ if (a_len < 1) return; for (i = b_len; i >= 1; i--) b[i + 1] = b[i]; b[1] = a[1]; b_len++; for (i = 1; i < a_len; i++) a[i] = a[i + 1]; delete a[a_len]; a_len-- }
	function ra_op(   i,t){ if (a_len < 2) return; t = a[1]; for (i = 1; i < a_len; i++) a[i] = a[i + 1]; a[a_len] = t }
	function rb_op(   i,t){ if (b_len < 2) return; t = b[1]; for (i = 1; i < b_len; i++) b[i] = b[i + 1]; b[b_len] = t }
	function rra_op(   i,t){ if (a_len < 2) return; t = a[a_len]; for (i = a_len; i > 1; i--) a[i] = a[i - 1]; a[1] = t }
	function rrb_op(   i,t){ if (b_len < 2) return; t = b[b_len]; for (i = b_len; i > 1; i--) b[i] = b[i - 1]; b[1] = t }
	BEGIN {
		n = split(args, tok, " ");
		a_len = n;
		b_len = 0;
		for (i = 1; i <= n; i++) a[i] = tok[i] + 0;
		valid = 1;
	}
	{
		op = $0;
		if (op == "") next;
		if (op == "sa") sa_op();
		else if (op == "sb") sb_op();
		else if (op == "ss") { sa_op(); sb_op(); }
		else if (op == "pa") pa_op();
		else if (op == "pb") pb_op();
		else if (op == "ra") ra_op();
		else if (op == "rb") rb_op();
		else if (op == "rr") { ra_op(); rb_op(); }
		else if (op == "rra") rra_op();
		else if (op == "rrb") rrb_op();
		else if (op == "rrr") { rra_op(); rrb_op(); }
		else valid = 0;
	}
	END {
		if (!valid || b_len != 0) { print "KO"; exit; }
		for (i = 1; i < a_len; i++) {
			if (a[i] > a[i + 1]) { print "KO"; exit; }
		}
		print "OK";
	}
	'
}

validate_case() {
	local args="$1"
	local ops="$2"

	if [ -n "$CHECKER_BIN" ]; then
		printf "%s\n" "$ops" | "$CHECKER_BIN" $args | awk 'NF{print $NF}'
	else
		validate_with_awk "$args" "$ops"
	fi
}

run_push_swap() {
	local args="$1"
	local outfile="$2"
	local pid
	local timer
	local status

	"$PS_BIN" $args > "$outfile" &
	pid=$!
	(
		sleep "$TIMEOUT_SEC"
		kill -TERM "$pid" 2>/dev/null || true
	) &
	timer=$!
	set +e
	wait "$pid"
	status=$?
	set -e
	kill -TERM "$timer" 2>/dev/null || true
	wait "$timer" 2>/dev/null || true
	if [ "$status" -eq 143 ]; then
		return 124
	fi
	return "$status"
}

save_failure() {
	local args="$1"
	local ops="$2"
	local res="$3"
	local stamp="$4"

	mkdir -p failures
	printf "%s\n" "$args" > "failures/ARG_${stamp}.txt"
	printf "%s\n" "$ops" > "failures/OPS_${stamp}.txt"
	printf "%s\n" "$res" > "failures/RES_${stamp}.txt"
	echo "Failing ARG: $args"
	echo "Saved: failures/ARG_${stamp}.txt"
	echo "Saved: failures/OPS_${stamp}.txt"
	echo "Saved: failures/RES_${stamp}.txt"
}

run_search() {
	local size="$1"
	local runs="$2"
	local seed="$3"
	local i=0
	local args
	local ops
	local res
	local stamp
	local tmp_ops

	tmp_ops="$(mktemp)"
	while [ "$i" -lt "$runs" ]; do
		args="$(gen_sequence "$size" "$seed")"
		if ! run_push_swap "$args" "$tmp_ops"; then
			stamp="$(date +%Y%m%d_%H%M%S)_${size}_${i}"
			save_failure "$args" "" "TIMEOUT" "$stamp"
			rm -f "$tmp_ops"
			exit 1
		fi
		ops="$(cat "$tmp_ops")"
		res="$(validate_case "$args" "$ops")"
		if [ "$res" != "OK" ]; then
			stamp="$(date +%Y%m%d_%H%M%S)_${size}_${i}"
			save_failure "$args" "$ops" "$res" "$stamp"
			rm -f "$tmp_ops"
			exit 1
		fi
		seed=$((seed + 97))
		i=$((i + 1))
	done
	rm -f "$tmp_ops"
}

if [ ! -x "$PS_BIN" ]; then
	echo "push_swap binary not found: $PS_BIN" >&2
	echo "Build first: make" >&2
	exit 1
fi

pick_checker
if [ -n "$CHECKER_BIN" ]; then
	echo "Checker: $CHECKER_BIN"
else
	echo "checker_OS not found; using internal AWK validator."
	echo "To use official checker: place checker_OS in repo root or set CHECKER_BIN=/path/to/checker_OS"
fi

run_search 3 "${RUNS_3:-400}" 103
run_search 5 "${RUNS_5:-400}" 105
run_search 20 "${RUNS_20:-400}" 120
run_search 100 "${RUNS_100:-200}" 200
run_search 500 "${RUNS_500:-50}" 600

echo "No KO found in configured runs."
