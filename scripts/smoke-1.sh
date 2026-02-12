#!/usr/bin/env bash
set -euo pipefail

PS="./push_swap"
CHECKER="./checker_OS"

have() { command -v "$1" >/dev/null 2>&1; }
die() { echo "Error: $*" >&2; exit 1; }

gen_unique() {
  local n="$1" lo="$2" hi="$3"
  if have jot; then
    jot -r $((n * 6)) "$lo" "$hi" | awk '!seen[$0]++' | head -n "$n"
  elif have shuf; then
    shuf -i "${lo}-${hi}" -n "$n"
  else
    die "Need either 'jot' (macOS) or 'shuf' (Linux)."
  fi
}

gen_arg() {
  local n="$1"
  local lo hi
  lo=$(( -n * 20 ))
  hi=$((  n * 20 ))
  gen_unique "$n" "$lo" "$hi" | tr '\n' ' '
}

count_ops_ok() {
  local arg="$1"
  local tmp
  tmp="$(mktemp)"
  local res
  res="$("$PS" $arg | tee "$tmp" | "$CHECKER" $arg || true)"
  local cnt
  cnt="$(wc -l < "$tmp" | tr -d ' ')"
  rm -f "$tmp"
  printf "%s %s\n" "$res" "$cnt"
}

expect_error() {
  # expects "Error" on stderr, and empty stdout
  local desc="$1"
  shift
  local out err
  out="$(mktemp)"
  err="$(mktemp)"
  set +e
  "$PS" "$@" >"$out" 2>"$err"
  local code=$?
  set -e

  if ! grep -q "^Error$" "$err"; then
    echo "❌ [$desc] expected Error on stderr"
    echo "stderr:"
    cat "$err"
    echo "stdout:"
    cat "$out"
    rm -f "$out" "$err"
    exit 1
  fi

  # stdout should be empty
  if [[ -s "$out" ]]; then
    echo "❌ [$desc] expected empty stdout, got:"
    cat "$out"
    rm -f "$out" "$err"
    exit 1
  fi

  rm -f "$out" "$err"
  echo "✅ [$desc] Error ok (exit=$code)"
}

expect_ok() {
  local desc="$1"
  local arg="$2"
  local res cnt
  read -r res cnt < <(count_ops_ok "$arg")
  if [[ "$res" != "OK" ]]; then
    echo "❌ [$desc] checker result: $res"
    echo "ARG: $arg"
    exit 1
  fi
  echo "✅ [$desc] OK | ops=$cnt"
}

[[ -x "$PS" ]] || die "push_swap not found/executable at $PS"
[[ -x "$CHECKER" ]] || die "checker not found/executable at $CHECKER"

echo "== Error handling tests =="
expect_error "invalid token" 1 2 a 3
expect_error "duplicate" 1 2 3 2
expect_error "overflow +" 2147483648
expect_error "overflow -" -2147483649
expect_error "empty string arg" ""

echo
echo "== Correctness smoke =="
expect_ok "sorted small (0 ops expected)" "1 2 3 4 5"
expect_ok "reverse 5" "5 4 3 2 1"
expect_ok "mixed small" "2 1 3 5 4"

echo
echo "== Random correctness + op counts =="
# A few runs each (increase loops if you want)
for i in {1..10}; do
  ARG="$(gen_arg 20)"
  expect_ok "rand20 #$i" "$ARG"
done

echo
echo "== 100 / 500 score-style samples =="
ARG100="$(gen_arg 100)"
expect_ok "rand100 sample" "$ARG100"

ARG500="$(gen_arg 500)"
expect_ok "rand500 sample" "$ARG500"

echo
echo "Smoke tests passed ✅"
