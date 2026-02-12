#!/usr/bin/env bash
set -euo pipefail

PS="./push_swap"
CHECKER="./checker_OS"

# ---------- helpers ----------
have() { command -v "$1" >/dev/null 2>&1; }

die() { echo "Error: $*" >&2; exit 1; }

mk_dirs() {
  mkdir -p failures
}

# Generate N unique random integers in a range.
# macOS: jot -r
# Linux: shuf
gen_unique() {
  local n="$1" lo="$2" hi="$3"
  if have jot; then
    # jot can repeat; enforce uniqueness via awk
    jot -r $((n * 6)) "$lo" "$hi" \
      | awk '!seen[$0]++' \
      | head -n "$n"
  elif have shuf; then
    shuf -i "${lo}-${hi}" -n "$n"
  else
    die "Need either 'jot' (macOS) or 'shuf' (Linux)."
  fi
}

gen_arg() {
  local n="$1"
  # widen range to avoid accidental duplicates after uniq filtering
  local lo hi
  lo=$(( -n * 20 ))
  hi=$((  n * 20 ))
  gen_unique "$n" "$lo" "$hi" | tr '\n' ' '
}

run_one() {
  local n="$1"
  local arg
  arg="$(gen_arg "$n")"

  # Run push_swap -> checker
  # Save ops to temp first so we can store it if KO happens
  local ops tmpops out
  tmpops="$(mktemp)"
  out="$("$PS" $arg | tee "$tmpops" | "$CHECKER" $arg || true)"

  if [[ "$out" != "OK" ]]; then
    mk_dirs
    local ts
    ts="$(date +"%Y%m%d_%H%M%S")"
    printf "%s\n" "$arg" > "failures/ARG_${n}_${ts}.txt"
    cp "$tmpops" "failures/OPS_${n}_${ts}.txt"
    printf "%s\n" "$out" > "failures/RES_${n}_${ts}.txt"

    echo "KO FOUND (n=$n)"
    echo "Saved:"
    echo "  failures/ARG_${n}_${ts}.txt"
    echo "  failures/OPS_${n}_${ts}.txt"
    echo "  failures/RES_${n}_${ts}.txt"
    echo
    echo "Reproduce with:"
    echo "  ARG=\"$(cat "failures/ARG_${n}_${ts}.txt")\""
    echo "  $PS \$ARG | $CHECKER \$ARG"
    rm -f "$tmpops"
    exit 1
  fi

  rm -f "$tmpops"
}

# ---------- main ----------
[[ -x "$PS" ]] || die "push_swap not found/executable at $PS"
[[ -x "$CHECKER" ]] || die "checker not found/executable at $CHECKER"

sizes=(3 5 20 100 500)

# defaults
MAX_ITERS=1000

# CLI: ./scripts/find_ko.sh [iters]
if [[ "${1:-}" =~ ^[0-9]+$ ]]; then
  MAX_ITERS="$1"
fi

echo "Searching for KO (up to ${MAX_ITERS} iterations per size)..."
for n in "${sizes[@]}"; do
  echo "== size $n =="
  for ((i=1; i<=MAX_ITERS; i++)); do
    run_one "$n"
    if (( i % 50 == 0 )); then
      echo "  passed $i"
    fi
  done
done

echo "No KO found in this run ✅"
