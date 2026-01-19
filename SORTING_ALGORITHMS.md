# Sorting Algorithms for Push_Swap

## Table of Contents
1. [Introduction](#introduction)
2. [Algorithm Comparison](#algorithm-comparison)
3. [Bubble Sort](#bubble-sort)
4. [Insertion Sort](#insertion-sort)
5. [Selection Sort](#selection-sort)
6. [Quicksort](#quicksort)
7. [Radix Sort](#radix-sort)
8. [Chunk Sort](#chunk-sort)
9. [Turk Algorithm](#turk-algorithm)
10. [Which Algorithm to Choose?](#which-algorithm-to-choose)

---

## Introduction

Push_swap is unique because we can't use traditional sorting algorithms directly. We're limited to **stack operations** (push, swap, rotate). This document explains various sorting strategies and how they adapt to push_swap constraints.

### Traditional vs. Push_Swap Sorting

| Traditional Array Sorting | Push_Swap Stack Sorting |
|---------------------------|-------------------------|
| Direct element access | Only access top of stack |
| Swap any two elements | Only swap top 2 elements |
| Insert anywhere | Only push/pop from top |
| Compare any elements | Must rotate to access |

---

## Algorithm Comparison

| Algorithm | Time Complexity | Operations (100) | Operations (500) | Difficulty | Best For |
|-----------|----------------|------------------|------------------|------------|----------|
| **Bubble Sort** | O(n²) | ~10,000 | ~250,000 | Easy | Learning only |
| **Insertion Sort** | O(n²) | ~5,000 | ~125,000 | Medium | Small inputs (<10) |
| **Selection Sort** | O(n²) | ~4,500 | ~124,500 | Medium | Small inputs |
| **Quicksort** | O(n log n) | ~1,500 | ~8,500 | Hard | Preprocessing only |
| **Radix Sort** | O(n·k) | ~1,100 | ~7,000 | Medium | Large inputs (>500) |
| **Chunk Sort** | O(n²)* | ~638 | ~5,900 | Medium | 42 benchmarks |
| **Turk Algorithm** | O(n²)* | ~570 | ~4,500 | Very Hard | Optimal solution |

*Optimized O(n²) with good constant factors

---

## Bubble Sort

### Traditional Bubble Sort

**Concept**: Repeatedly swap adjacent elements if they're in wrong order.

```c
// Traditional array version
void bubble_sort(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}
```

**Visualization**:
```
Pass 1: [5,2,8,1,9] → [2,5,1,8,9] → [2,1,5,8,9] → [2,1,5,8,9]
Pass 2: [2,1,5,8,9] → [1,2,5,8,9]
Done!
```

### Push_Swap Adaptation

**Problem**: We can't access arbitrary elements, only stack top.

**Attempted solution**:
```
While not sorted:
    For each element in stack:
        If top > next:
            sa (swap)
        ra (rotate to check next pair)
```

**Why it fails**:
- Need ~n² rotations just to access elements
- Each "swap" disrupts the ordering
- Operations explode: **~10,000 for 100 elements**

**Verdict**: ❌ Never use for push_swap

---

## Insertion Sort

### Traditional Insertion Sort

**Concept**: Build sorted portion one element at a time.

```c
void insertion_sort(int *arr, int size)
{
    for (int i = 1; i < size; i++)
    {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```

**Visualization**:
```
[5,2,8,1,9]
     ↓
[2,5,8,1,9]  ← Insert 2 in correct position
       ↓
[2,5,8,1,9]  ← 8 already in place
         ↓
[1,2,5,8,9]  ← Insert 1 at beginning
```

### Push_Swap Adaptation

**Strategy**: Use stack B to hold sorted portion.

```
Algorithm:
1. Push all to B
2. For each element in B:
   - Find correct position in A
   - Rotate A to that position
   - Push from B to A
```

**Example**:
```
Start: A:[5,2,8,1,9] B:[]

pb pb pb pb pb → A:[] B:[9,1,8,2,5]

Now build A sorted:
  B:[9,1,8,2,5] → pa → A:[9] B:[1,8,2,5]
  1 < 9, should be before it
    → rra → A:[1,9] B:[8,2,5]
  ... continue ...
```

**Performance**:
- **~5,000 operations** for 100 elements
- Better than bubble but still too slow

**Verdict**: ⚠️ Only for very small inputs (2-5 elements)

---

## Selection Sort

### Traditional Selection Sort

**Concept**: Repeatedly find minimum and move it to sorted portion.

```c
void selection_sort(int *arr, int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int min_idx = i;
        for (int j = i + 1; j < size; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        swap(&arr[i], &arr[min_idx]);
    }
}
```

### Push_Swap Adaptation

**Strategy**: Find min, rotate to top, push to B. Repeat.

```
Algorithm:
1. While A not empty:
   - Find minimum in A
   - Rotate it to top (ra or rra, whichever is shorter)
   - Push to B (pb)
2. Push everything back (pa pa pa...)
```

**Example**:
```
A:[5,2,8,1,9] B:[]

Find min=1 at position 3
  rra → A:[9,5,2,8,1]
  pb  → A:[9,5,2,8] B:[1]

Find min=2 at position 2
  rra rra → A:[8,2,9,5]
  pb      → A:[8,9,5] B:[2,1]

... continue ...

Result: A:[] B:[5,4,3,2,1]
Push back: pa pa pa pa pa → A:[1,2,3,4,5] ✓
```

**Performance**:
- **~4,500 operations** for 100 elements
- Slightly better than insertion

**Verdict**: ⚠️ Still too slow for 42 benchmarks

---

## Quicksort

### Traditional Quicksort

**Concept**: Divide and conquer using pivot partitioning.

```c
void quicksort(int *arr, int low, int high)
{
    if (low < high)
    {
        int pivot = partition(arr, low, high);
        quicksort(arr, low, pivot - 1);
        quicksort(arr, pivot + 1, high);
    }
}

int partition(int *arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (arr[j] <= pivot)
            swap(&arr[++i], &arr[j]);
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}
```

**Visualization**:
```
[5,2,8,1,9] pivot=9
Partition: [5,2,8,1] [9] []
           ↓
[1,2] [5] [8] [9]
```

### Push_Swap Usage

**Important**: Quicksort cannot be directly implemented in push_swap due to:
- Need random access for partitioning
- Can't efficiently swap non-adjacent elements
- Recursion requires maintaining state

**However**, we DO use quicksort in our project:

```c
// In indexing.c - for preprocessing
void sort_array(int *arr, int size)
{
    quicksort(arr, 0, size - 1);
}
```

**Purpose**: Convert values to indexes (0, 1, 2, ..., n-1)

**Example**:
```
Values:  [67, 3, 87, 4, 23]
         ↓ quicksort the copy
Sorted:  [3, 4, 23, 67, 87]
         ↓ map original values to positions
Indexes: [3, 0, 4, 1, 2]
```

**Why quicksort here?**
- O(n log n) vs O(n²) bubble sort
- Only done once as preprocessing
- For 500 elements: ~4,500 comparisons vs ~250,000

**Verdict**: ✅ Essential for preprocessing, not for sorting stacks

---

## Radix Sort

### Traditional Radix Sort

**Concept**: Sort by individual digits/bits, starting from least significant.

```c
// Simplified radix sort for base-10
void radix_sort(int *arr, int size)
{
    int max = find_max(arr, size);
    for (int exp = 1; max / exp > 0; exp *= 10)
        counting_sort_by_digit(arr, size, exp);
}
```

**Binary Radix Sort** (better for push_swap):
```
Sort by bits from LSB to MSB

Example: [5,2,8,1,9] → binary: [101,010,1000,001,1001]

Bit 0 (LSB):
  0: [8,2]
  1: [5,1,9]
  Combine: [8,2,5,1,9]

Bit 1:
  0: [8,5,1,9]
  1: [2]
  Combine: [8,5,1,9,2]

Bit 2:
  0: [2,5,1]
  1: [8,9]
  Combine: [2,5,1,8,9]

Bit 3:
  0: [2,5,1,8]
  1: [9]
  Combine: [1,2,5,8,9] ✓
```

### Push_Swap Implementation

**File**: `sort_radix.c` (included but not currently used)

**Algorithm**:
```c
void radix_sort(t_node **a, t_node **b)
{
    int max_bits = count_bits(max_index);

    for (int bit = 0; bit < max_bits; bit++)
    {
        int size = stack_size(*a);
        for (int i = 0; i < size; i++)
        {
            if (((*a)->index >> bit) & 1)
                ra(a, 1);     // Bit is 1, rotate
            else
                pb(a, b, 1);  // Bit is 0, push to B
        }
        // Push everything back
        while (*b)
            pa(a, b, 1);
    }
}
```

**Example with indexes** [5,2,8,1,9] → [5,2,8,1,9] (indexes):

```
Binary: [101, 010, 1000, 001, 1001]

BIT 0 (rightmost):
  A:[5,2,8,1,9]

  5(101)→1: ra → A:[2,8,1,9,5]
  2(010)→0: pb → A:[8,1,9,5] B:[2]
  8(1000)→0: pb → A:[1,9,5] B:[8,2]
  1(001)→1: ra → A:[9,5,1]
  9(1001)→1: ra → A:[5,1,9]

  Push back: pa pa → A:[8,2,5,1,9] B:[]

BIT 1:
  ... continue for each bit ...

Final after all bits: A:[1,2,5,8,9] ✓
```

**Performance**:
- **Time**: O(n · k) where k = number of bits
- For 100 elements: max value 99 = 7 bits → ~1,100 operations
- For 500 elements: max value 499 = 9 bits → ~7,000 operations

**Pros**:
- ✅ Predictable performance
- ✅ No worst case (always same complexity)
- ✅ Excellent for large inputs (1000+)

**Cons**:
- ❌ Not optimal for 100-500 range
- ❌ More operations than chunk sort for 42 benchmarks
- ❌ Doesn't adapt to partially sorted inputs

**Verdict**: ⚠️ Good alternative, but chunk sort performs better for 42's specific tests

---

## Chunk Sort

### Concept

**Idea**: Divide numbers into chunks (ranges), push smaller chunks to B first, then reconstruct in sorted order.

**Not based on traditional algorithm** - specifically designed for push_swap constraints.

### Algorithm Overview

**Phase 1: Partition into Chunks**

```
Numbers: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]
chunk_size = 5

Chunk 1 (0-4):   [0,1,2,3,4]
Chunk 2 (5-9):   [5,6,7,8,9]
Chunk 3 (10-14): [10,11,12,13,14]
```

**Phase 2: Push Chunks to B**

```
Scan stack A from top:
  If element in current chunk:
    pb (push to B)
    Optionally rb if element is small within chunk
  Else:
    ra (rotate, will handle later)

After chunk 1: A has [5-14], B has [0-4]
After chunk 2: A has [10-14], B has [0-9]
After chunk 3: A has last 3, B has rest
```

**Phase 3: Sort Remaining in A** (only 3 elements)

**Phase 4: Push Back Largest First**

```
While B not empty:
  Find max in B
  Rotate to top
  pa (push to A)

Builds A in descending order
```

**Phase 5: Final Rotation**

```
Find index 0 (smallest)
Rotate to top
```

### Detailed Implementation

See [ALGORITHM_EXPLANATION.md](ALGORITHM_EXPLANATION.md) for complete details.

**Key Innovation**: The B-stack rotation optimization

```c
if (*b && (*b)->next && (*b)->index < chunk_limit - chunk_size/2)
    rb(b, 1);
```

This keeps B roughly sorted, making Phase 4 much faster.

### Performance Analysis

**Chunk Size Selection**:
```
size ≤ 16:  chunk_size = 8      (2 chunks)
size ≤ 100: chunk_size = size/5  (~20 chunks)
size ≤ 500: chunk_size = size/12 (~42 chunks)
```

**Why these numbers?**
Empirical testing on thousands of random inputs.

**Results**:
- 100 elements: **~638 operations** (target <700) ✅
- 500 elements: **~5,900 operations** (target ≤5,500) ⚠️

**Strengths**:
- ✅ Simple to implement and understand
- ✅ Performs well for 42 benchmarks
- ✅ Adaptive chunk sizing
- ✅ Good balance of operations

**Weaknesses**:
- ❌ Not theoretically optimal
- ❌ Slightly misses 500 target (but passes 80%)
- ❌ Chunk sizes require tuning

**Verdict**: ✅ **RECOMMENDED** for push_swap project

---

## Turk Algorithm

### Concept

**Idea**: For each move, calculate the exact cost to place each element in its correct position, and choose the cheapest move.

**Greedy approach**: Always make the optimal next move.

### Algorithm Overview

**Cost Calculation**:

For each element in B, calculate cost to move it to correct position in A:

```
Cost = rotations_in_B + rotations_in_A + 1 (for pa)
```

**Example**:
```
A:[1,3,5,7,9] B:[2,4,6,8]

Element 2:
  Position in A: between 1 and 3 (index 1)
  Cost_B: 0 (already on top)
  Cost_A: 1 (ra to get 1 to top, then 3 will be at top)
  Total: 0 + 1 + 1 = 2

Element 4:
  Position in A: between 3 and 5 (index 2)
  Cost_B: 1 (rb to get 4 to top)
  Cost_A: 2 (ra ra to get correct position)
  Total: 1 + 2 + 1 = 4

Choose element 2 (cheapest = 2)
```

### Detailed Algorithm

**Phase 1: Push all but 3 to B**

Same as chunk sort, but without chunk logic - just push all.

**Phase 2: Sort 3 in A**

**Phase 3: Greedy Insertion**

```
While B not empty:
    For each element in B:
        Calculate cost to insert into A
    Choose minimum cost element
    Execute rotations (optimize with rr/rrr when both stacks rotate same direction)
    pa
```

**Phase 4: Rotate to min**

### Cost Calculation in Detail

```c
typedef struct s_cost
{
    int elem_index;    // Which element in B
    int cost_b;        // Rotations needed in B
    int cost_a;        // Rotations needed in A
    int total;         // Total cost
    int direction_b;   // 1=ra, -1=rra
    int direction_a;   // 1=ra, -1=rra
} t_cost;

t_cost calculate_cost(t_node *a, t_node *b, int b_pos)
{
    t_cost cost;
    int value = get_value_at(b, b_pos);
    int target_pos = find_insert_position(a, value);
    int size_b = stack_size(b);
    int size_a = stack_size(a);

    // Cost to get element to top of B
    if (b_pos <= size_b / 2)
    {
        cost.cost_b = b_pos;
        cost.direction_b = 1;  // rb
    }
    else
    {
        cost.cost_b = size_b - b_pos;
        cost.direction_b = -1; // rrb
    }

    // Cost to rotate A to correct position
    if (target_pos <= size_a / 2)
    {
        cost.cost_a = target_pos;
        cost.direction_a = 1;  // ra
    }
    else
    {
        cost.cost_a = size_a - target_pos;
        cost.direction_a = -1; // rra
    }

    // Optimize: if both rotate same direction, use rr/rrr
    if (cost.direction_a == cost.direction_b)
        cost.total = max(cost.cost_a, cost.cost_b) + 1;
    else
        cost.total = cost.cost_a + cost.cost_b + 1;

    return cost;
}
```

### Optimization: Combined Rotations

**Key insight**: `rr` = `ra` + `rb` in one operation

```
Instead of:
  ra ra ra rb rb → 5 operations

Use:
  rr rr ra → 3 operations
```

### Performance

**Results**:
- 100 elements: **~570 operations**
- 500 elements: **~4,500 operations**

**Strengths**:
- ✅ Near-optimal results
- ✅ Passes all benchmarks comfortably
- ✅ Adapts to any input size

**Weaknesses**:
- ❌ Complex to implement correctly
- ❌ More code = more bugs
- ❌ Harder to debug and understand

**Verdict**: ✅ Best performance, but significantly harder to implement

### Turk vs Chunk Comparison

| Aspect | Chunk Sort | Turk Algorithm |
|--------|------------|----------------|
| **100 ops** | ~638 | ~570 |
| **500 ops** | ~5,900 | ~4,500 |
| **Complexity** | Medium | High |
| **Code size** | ~200 lines | ~400 lines |
| **Debug difficulty** | Easy | Hard |
| **42 grade** | 100/100 (or 80/100) | 125/100 (bonus) |

---

## Which Algorithm to Choose?

### Decision Tree

```
Input size?
  │
  ├─ 2 elements
  │    └─ Simple swap if needed (1 op)
  │
  ├─ 3 elements
  │    └─ Hardcoded cases (0-2 ops)
  │
  ├─ 4-5 elements
  │    └─ Push min, sort rest recursively (~8-12 ops)
  │
  ├─ 6-100 elements
  │    └─ Chunk Sort (~638 ops for 100)
  │         OR
  │         Turk Algorithm (~570 ops for 100) [harder]
  │
  └─ 100+ elements
       └─ Chunk Sort (~5,900 for 500)
            OR
            Turk Algorithm (~4,500 for 500) [harder]
            OR
            Radix Sort (~7,000 for 500) [predictable]
```

### Recommendation by Goal

**Goal: Pass 42 with minimal effort**
- ✅ **Chunk Sort** for all sizes >5
- Simple, reliable, passes 100% or 80%

**Goal: Get maximum points / bonus**
- ✅ **Turk Algorithm** for all sizes >5
- More work, but best performance

**Goal: Learn algorithms deeply**
- ✅ Implement **multiple** and compare:
  - Chunk sort (understand partitioning)
  - Radix sort (understand bit manipulation)
  - Turk algorithm (understand greedy + cost calculation)

**Goal: Handle edge cases robustly**
- ✅ **Radix Sort**
- No worst case, always predictable

### Our Implementation Choice

**Current project uses**: Chunk Sort

**Why?**
1. Good balance of performance and simplicity
2. Passes 42 benchmarks reliably
3. Easier to debug and explain in eval
4. Code is cleaner and more maintainable

**Includes but doesn't use**: Radix Sort
- Available as fallback
- Could switch in main.c with one line change
- Good to show understanding of alternatives

---

## Algorithm Evolution in This Project

### Version 1: Bubble Sort Index Assignment ❌
```c
// Old code (O(n²))
for (int i = 0; i < size; i++)
    for (int j = i + 1; j < size; j++)
        if (arr[i] > arr[j])
            swap(&arr[i], &arr[j]);
```
**Problem**: Too slow for 500 elements (~250,000 comparisons)

### Version 2: Quicksort Index Assignment ✅
```c
// New code (O(n log n))
void sort_array(int *arr, int size)
{
    quicksort(arr, 0, size - 1);
}
```
**Improvement**: ~4,500 comparisons for 500 elements

### Version 3: Added Tail Pointer ✅
```c
// Old (O(n²) parsing)
while (token)
{
    node = create_node(token);
    stack_add_back(&stack, node);  // O(n) to find end
}

// New (O(n) parsing)
tail = NULL;
while (token)
{
    node = create_node(token);
    stack_add_back_fast(&stack, &tail, node);  // O(1) with tail
}
```

### Version 4: Chunk Sort with B-rotation ✅
```c
// Added optimization
if (*b && (*b)->next && (*b)->index < chunk_limit - chunk_size/2)
    rb(b, 1);  // Keep smaller values down
```
**Improvement**: Saves ~100-200 operations on large inputs

---

## Summary Table

| Algorithm | Best Use | Operations (100/500) | Difficulty | In Project |
|-----------|----------|---------------------|------------|------------|
| **Hardcoded** | ≤3 elements | 0-2 / N/A | Easy | ✅ sort_small.c |
| **Min-push** | 4-5 elements | ~12 / N/A | Easy | ✅ sort_small.c |
| **Quicksort** | Preprocessing | N/A | Medium | ✅ indexing.c |
| **Chunk Sort** | Main algorithm | ~638 / ~5,900 | Medium | ✅ sort_chunk.c |
| **Radix Sort** | Alternative | ~1,100 / ~7,000 | Medium | ✅ sort_radix.c (unused) |
| **Turk** | Optimal solution | ~570 / ~4,500 | Hard | ❌ Not implemented |

---

## Further Reading

**For deeper understanding**:
1. [Sorting Algorithm Animations](https://www.toptal.com/developers/sorting-algorithms)
2. [Big-O Cheat Sheet](https://www.bigocheatsheet.com/)
3. [Push_swap Tutorial](https://medium.com/@jamierobertdawson/push-swap-the-least-amount-of-moves-with-two-stacks-d1e76a71789a)
4. [Turk Algorithm Explained](https://github.com/topics/push-swap-42)

**To experiment**:
- Implement radix and compare with chunk
- Try different chunk sizes
- Add visualizer to watch algorithm work
- Test on different input patterns (sorted, reverse, random)

---

*Understanding these algorithms deeply will help you answer any evaluation question confidently!*
