# push_swap Defense Guide

## 1) Project workflow (end-to-end)

1. Input is received from `argv` in `main`.
2. `parse_args` validates every token:
- accepts either `./push_swap "1 2 3"` or `./push_swap 1 2 3`
- validates integer format and 32-bit range
- rejects duplicates
- builds stack `a`
3. Early exit:
- if no values or already sorted, program exits with no operations
4. `assign_indexes` converts each value into a rank (`index`) from `0..n-1`.
- Example: values `[42, -5, 9]` become indexes `[2, 0, 1]`
5. Sorting path selection:
- `size <= 5`: `sort_small`
- `size > 5`: `chunk_sort`
6. Operations are printed to `stdout` (`sa`, `pb`, `rr`, etc.) as the solution.
7. All allocated memory is freed.

## 2) Core algorithm strategy

## 2.1 Why indexing is important
The algorithm sorts by `index` instead of raw `value`. This normalizes comparisons and makes chunk boundaries simple and robust.

## 2.2 Small inputs (2..5)
`sort_small` uses deterministic minimal logic:
- `2`: one possible `sa`
- `3`: handled by `sort_three_cases` (all orderings)
- `4` and `5`:
1. rotate minimum to top
2. push minimum(s) to `b`
3. sort remaining 3 in `a`
4. `pa` values back from `b`

## 2.3 Large inputs (>5): chunk + cheapest reinsertion
`chunk_sort` runs in 3 phases:

1. **Push phase (`push_all_but_three`)**
- Keep only 3 elements in `a`.
- Push elements from `a` to `b` by chunk windows (`chunk_limit`).
- Prefer rotating toward nearest candidate in the current chunk.
- After `pb`, sometimes `rb` to keep lower indexes deeper in `b`.

2. **Sort base**
- Sort remaining 3 elements in `a` with `sort_small`.

3. **Push-back phase (`push_back_all`)**
- For each candidate in `b`, compute:
- rotation needed in `a` to reach insertion position
- rotation needed in `b` to bring candidate to top
- total move cost via `calc_move_cost`
- choose cheapest candidate
- apply combined rotations (`rr`/`rrr` when possible), then `pa`

Final step: `rotate_to_min(a)` to bring index `0` to top and finish fully sorted.

## 3) Data model and operation model

## 3.1 Node structure
`typedef struct s_node` in `include/push_swap.h`:
- `value`: original integer
- `index`: rank in sorted order
- `next`: linked-list pointer

## 3.2 Primitive allowed operations
Implemented exactly as push_swap ops:
- swap: `sa`, `sb`, `ss`
- push: `pa`, `pb`
- rotate: `ra`, `rb`, `rr`
- reverse rotate: `rra`, `rrb`, `rrr`

Every higher-level sorting decision reduces to these primitives.

## 4) Function-by-function explanation

## 4.1 Entry point

### `src/main.c`
- `main(int argc, char **argv)`
- Creates stacks `a` and `b`
- Parses input
- Skips work if empty/already sorted
- Assigns indexes
- Chooses `sort_small` or `chunk_sort`
- Frees memory and exits

## 4.2 Parsing and validation

### `src/parse.c`
- `parse_int(char *s, int *out)`
- Parses optional sign + digits, checks overflow, writes parsed int

- `static void add_token(t_node **a, t_node **tail, char *token, char **split)`
- Parses one token, checks duplicates, allocates node, appends to stack

- `static void parse_split(t_node **a, char *arg)`
- Handles single-string form (`argc == 2`) using `ft_split`

- `static void parse_argv(t_node **a, int argc, char **argv)`
- Handles multi-argument form (`argc > 2`)

- `void parse_args(t_node **a, int argc, char **argv)`
- Dispatcher between split mode and argv mode

### `src/parse_utils.c`
- `is_overflow(long res, int sign)`
- Checks 32-bit signed bounds while parsing

- `parse_sign(char *s, int *i)`
- Reads leading `+`/`-`, updates parse index

- `has_duplicate(t_node *a, int value)`
- Linear duplicate detection in stack

- `free_split_local(char **split)`
- Frees result of `ft_split`

## 4.3 Stack construction and utilities

### `src/stack.c`
- `node_new(int value)`
- Allocates node, initializes `index=-1`

- `stack_add_back(t_node **stack, t_node *new_node)`
- Appends by traversing to list end

- `stack_add_back_fast(t_node **stack, t_node **tail, t_node *new_node)`
- O(1) append when caller tracks `tail`

### `src/stack_utils.c`
- `stack_size(t_node *stack)`
- Returns list length

- `is_sorted(t_node *stack)`
- Checks ascending `value` order

- `free_stack(t_node **stack)`
- Frees all nodes

## 4.4 Index assignment (value compression)

### `src/assign_indexes.c`
- `static int *stack_to_array(t_node *a, int size)`
- Copies stack values into dynamic array

- `static void set_indexes(t_node *a, int *arr, int size)`
- For each node value, finds its sorted position and stores into `index`

- `void assign_indexes(t_node *a)`
- Orchestrates copy -> sort -> assign -> free

### `src/indexing.c`
- `static void swap_ints(int *a, int *b)`
- Utility swap

- `static int partition(int *arr, int low, int high)`
- Quicksort partition routine

- `static void quicksort(int *arr, int low, int high)`
- Recursive quicksort

- `void sort_array(int *arr, int size)`
- Public array sort wrapper

## 4.5 Primitive operations

### `src/ops_swap.c`
- `static void swap_stack(t_node **s)`
- Swaps first two nodes

- `sa`, `sb`, `ss`
- Apply swap and optionally print op

### `src/ops_push.c`
- `static void push_stack(t_node **from, t_node **to)`
- Moves top element between stacks

- `pa`, `pb`
- Apply push and optionally print op

### `src/ops_rotate.c`
- `rotate_stack(t_node **s)`
- Moves first node to end

- `ra`, `rb`, `rr`
- Apply rotate and optionally print op

### `src/ops_reverse.c`
- `rev_rotate_stack(t_node **s)`
- Moves last node to front

- `rra`, `rrb`, `rrr`
- Apply reverse rotate and optionally print op

## 4.6 Small-sort logic

### `src/sort_small_utils.c`
- `sort_three_cases(t_node **a, int first, int second, int third)`
- Hard-coded decision tree for all 3-element permutations

- `min_pos(t_node *a)`
- Finds position of minimum index in stack

### `src/sort_small.c`
- `static void sort_three(t_node **a)`
- Extracts first 3 indexes and delegates to cases

- `static void rotate_min_to_top(t_node **a)`
- Chooses `ra` vs `rra` shortest path to bring min to top

- `static void sort_four(t_node **a, t_node **b)`
- Push min to `b`, sort 3, push back

- `static void sort_five(t_node **a, t_node **b)`
- Push two mins to `b`, sort 3, push back twice

- `void sort_small(t_node **a, t_node **b)`
- Dispatcher for size 2..5

## 4.7 Chunk push phase and final alignment

### `src/sort_chunk.c`
- `static int get_chunk_size(int size)`
- Heuristic chunk width based on input size

- `static int find_chunk_pos(t_node *a, int chunk_limit, int max_index)`
- Finds nearest element in `a` that belongs to current chunk and should be pushed

- `static void rotate_to_pos(t_node **a, int pos)`
- Rotates `a` shortest direction to target position

- `static void push_all_but_three(t_node **a, t_node **b, int size)`
- Pushes chunked elements from `a` to `b` until only 3 remain in `a`

- `void chunk_sort(t_node **a, t_node **b, int size)`
- Full large-sort controller: push phase -> sort base -> push back -> final align

### `src/sort_chunk_utils.c`
- `find_min_pos(t_node *a)`
- Finds position of minimum index

- `rotate_to_min(t_node **a)`
- Final rotation until index `0` is on top

- `find_max_pos(t_node *b)`
- Finds max index position in `b` (utility retained for strategy/debug evolution)

## 4.8 Cost-based reinsertion phase

### `src/sort_cost.c`
- `static int abs_value(int n)`
- Absolute value helper

- `get_rot_value(int pos, int size)`
- Converts absolute position into signed rotation distance:
- positive => rotate up (`ra/rb`)
- negative => reverse rotate (`rra/rrb`)

- `calc_move_cost(int rot_a, int rot_b)`
- Computes real cost with shared-rotation optimization:
- same sign => `max(|rot_a|, |rot_b|)` using `rr`/`rrr`
- opposite sign => `|rot_a| + |rot_b|`

### `src/sort_insert.c`
- `static void update_insert_data(int *data, int idx, int pos, int target)`
- Tracks two candidates while scanning `a`:
- global minimum index position
- smallest index greater than target

- `find_insert_pos_a(t_node *a, int index)`
- Returns insertion position in `a` for a node from `b`
- If a greater index exists, insert before nearest greater
- Otherwise insert before global minimum (wrap-around)

### `src/sort_rotate_plan.c`
- `static void rotate_both_up(...)`
- Executes shared forward rotations (`rr`)

- `static void rotate_both_down(...)`
- Executes shared reverse rotations (`rrr`)

- `exec_combined_rotations(...)`
- Applies shared rotations first, then remaining individual rotations

### `src/sort_pushback.c`
- `static void pick_best_move(t_node *a, t_node *b, int *best_ra, int *best_rb)`
- Evaluates every candidate in `b`, computes insertion cost, keeps cheapest

- `push_back_all(t_node **a, t_node **b)`
- Repeats best-move selection, rotates, then `pa` until `b` is empty

## 4.9 Error handling and debug

### `src/utils.c`
- `static void free_split(char **split)`
- Internal free helper

- `error_exit(t_node **a, t_node **b, char **split)`
- Frees all known allocations, prints `Error\n`, exits with status `1`

### `src/debug.c`
- `#ifdef DEBUG` enables runtime operation tracing

- `static int has_cycle(t_node *s)`
- Floyd cycle detection to catch pointer corruption

- `static void print_stack(const char *name, t_node *s)`
- Prints stack content as `(value|index)` pairs to `stderr`

- `debug_log_op(const char *op, t_node *a, t_node *b)`
- Logs operation + stacks; exits on cycle detection

- Non-DEBUG build version of `debug_log_op`
- No-op stub to remove debug overhead

## 5) Complexity and performance discussion

- Parsing duplicates: worst-case `O(n^2)` due to linear duplicate scan per insert.
- Index assignment:
- copy `O(n)`
- quicksort average `O(n log n)`, worst `O(n^2)`
- set indexes `O(n^2)` (scan sorted array for each node)
- Small sort: constant time for `n<=5`.
- Chunk phase + push-back planning:
- practical performance is good for 42 thresholds due to chunking and combined rotations.
- `pick_best_move` scans all of `b` each reinsertion step, giving a quadratic-like behavior overall, acceptable for project constraints.

## 6) Practical defense script (what to say)

1. “I parse robustly (split mode and argv mode), reject invalid/overflow/duplicates, and fail safely with one `Error` path.”
2. “I compress values into indexes so algorithm logic works on rank space, not raw integers.”
3. “For `<=5`, I use deterministic minimal routines.”
4. “For larger sets, I push by chunks to `b`, keep 3 in `a`, sort base, then reinsert from `b` using cheapest rotation cost.”
5. “My reinsertion cost model exploits `rr/rrr` when both stacks rotate in same direction.”
6. “I finalize by rotating minimum to top and free everything.”

## 7) Build, run, and demonstrate quickly

```bash
make
./push_swap 2 1 3 6 5 8
ARG="4 67 3 87 23"; ./push_swap $ARG | ./checker_linux $ARG
./scripts/smoke.sh
./scripts/bench.sh
```

Debug trace build example:

```bash
make fclean
make CFLAGS="-Wall -Wextra -Werror -I include -I libft -DDEBUG"
./push_swap 3 2 1
```

## 8) Potential improvement points (good to mention honestly)

- Optimize duplicate detection during parse (`hash set` or sorted pre-check) to avoid `O(n^2)`.
- Optimize index assignment map (binary search on sorted array or hashmap from value->index).
- Tune chunk size heuristics adaptively from measured distributions.
- Cache insertion positions/costs incrementally during push-back to reduce recomputation.
