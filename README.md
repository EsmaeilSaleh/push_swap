push_swap

Benchmark helper:
- Build first: `make`
- Run: `./scripts/bench.sh`
- Override run counts with env vars, e.g. `RUNS_100=100 RUNS_500=40 ./scripts/bench.sh`

Correctness reproducer:
- Run `./scripts/find_ko.sh` to stop on first KO/TIMEOUT and save artifacts in `failures/`.
- If `checker_OS` is not present, the script falls back to an internal AWK validator.
- A timeout-wait bug in the reproducer was fixed (zombie polling); it now uses watchdog + `wait`, so hangs are captured deterministically.

Smoke tests:
- Run `./scripts/smoke.sh`

Debug replay:
- Build with `-DDEBUG` in `CFLAGS` to print per-op stack snapshots and cycle checks to `stderr`.
