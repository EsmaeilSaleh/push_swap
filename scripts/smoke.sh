#!/usr/bin/env bash
set -euo pipefail

PS_BIN="${PS_BIN:-./push_swap}"
CHECKER_BIN="${CHECKER_BIN:-}"

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

rand_args() {
	local n="$1"
	local seed="$2"

	awk -v n="$n" -v seed="$seed" 'BEGIN {
		srand(seed);
		for (i = 0; i < n; i++) a[i] = i;
		for (i = n - 1; i > 0; i--) {
			j = int(rand() * (i + 1));
			t = a[i]; a[i] = a[j]; a[j] = t;
		}
		for (i = 0; i < n; i++) printf "%d%s", a[i], (i + 1 < n ? " " : "\n");
	}'
}

assert_error() {
	local cmd="$1"
	local err
	set +e
	err=$(eval "$cmd" 2>&1 >/dev/null)
	status=$?
	set -e
	if [ "$status" -eq 0 ] || [ "$err" != "Error" ]; then
		echo "FAIL error-case: $cmd"
		exit 1
	fi
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
	echo "Checker: not found, using internal AWK validator"
fi

sorted="1 2 3 4 5"
ops="$($PS_BIN $sorted)"
count="$(printf "%s\n" "$ops" | awk 'NF{c++} END{print c+0}')"
res="$(validate_case "$sorted" "$ops")"
echo "sorted_5 ops=$count res=$res"
[ "$count" -eq 0 ] && [ "$res" = "OK" ]

rev3="3 2 1"
ops="$($PS_BIN $rev3)"
res="$(validate_case "$rev3" "$ops")"
echo "reverse_3 res=$res"
[ "$res" = "OK" ]

rev5="5 4 3 2 1"
ops="$($PS_BIN $rev5)"
res="$(validate_case "$rev5" "$ops")"
echo "reverse_5 res=$res"
[ "$res" = "OK" ]

assert_error "$PS_BIN 1 2 2"
assert_error "$PS_BIN 2147483648"
assert_error "$PS_BIN -2147483649"
echo "error_cases res=OK"

args100="$(rand_args 100 321)"
ops100="$($PS_BIN $args100)"
res100="$(validate_case "$args100" "$ops100")"
count100="$(printf "%s\n" "$ops100" | awk 'NF{c++} END{print c+0}')"
echo "random_100 ops=$count100 res=$res100"
[ "$res100" = "OK" ]

args500="$(rand_args 500 654)"
ops500="$($PS_BIN $args500)"
res500="$(validate_case "$args500" "$ops500")"
count500="$(printf "%s\n" "$ops500" | awk 'NF{c++} END{print c+0}')"
echo "random_500 ops=$count500 res=$res500"
[ "$res500" = "OK" ]

echo "smoke: OK"
