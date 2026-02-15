_This project has been created as part of the 42 curriculum by esaleh._

# push_swap

## Description

`push_swap` is a 42 sorting project where the goal is to sort a stack of integers using a second auxiliary stack and a restricted set of operations.

The program takes integers as arguments, validates the input, and prints the sequence of operations needed to sort the stack in ascending order with as few moves as possible.

This repository also includes debugging and benchmarking helpers to validate correctness and measure move counts.

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./push_swap 2 1 3 6 5 8
```

### Validate with checker (if available)

```bash
ARG="2 1 3 6 5 8"; ./push_swap $ARG | ./checker_linux $ARG
```

### Project scripts

```bash
./scripts/smoke.sh
./scripts/find_ko.sh
./scripts/bench.sh
```

Optional benchmark tuning:

```bash
RUNS_100=100 RUNS_500=40 ./scripts/bench.sh
```

## Resources

Classic references used for this project:

- 42 subject and evaluation criteria for `push_swap`
- Stack data structure fundamentals (LIFO operations and complexity)
- Sorting algorithm concepts relevant to constrained operations (partitioning/chunking, insertion strategy, cost-based move selection)
- 42 `libft` documentation and source code patterns

Helpful reading:

- https://en.wikipedia.org/wiki/Stack_(abstract_data_type)
- https://en.wikipedia.org/wiki/Sorting_algorithm
- https://en.wikipedia.org/wiki/Quicksort
- https://en.wikipedia.org/wiki/Insertion_sort
