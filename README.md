# cds — C Data Structures

A personal learning project: implementing classic data structures and algorithms in C from scratch.

The goal is not to produce a production-ready library, but to deeply understand how these fundamentals work at the lowest level — memory layout, pointer arithmetic, allocation strategies, and algorithmic trade-offs.

## Build & Run

```sh
make        # compiles and runs tests/main.c
make clean  # removes build artifacts
```

## Structure

```
include/cds/   — public headers
src/           — implementations
tests/         — test suite
```

## What's Implemented

### Data Structures
- **vector** — dynamic array with push/pop, get/set, sort, binary & linear search, clear, reserve
- **llist** — singly-linked list with prepend/append, insert, pop/pop_back, remove/purge, find/contains
- **stack** — LIFO container (composition over llist)
- **queue** — FIFO container (composition over llist)
- **hashmap** — hash table with separate chaining (in progress)

### Algorithms
- **quicksort** — median-of-three pivot, insertion sort fallback for small subarrays
- **binary search** — on sorted arrays
- **linear search** — on unsorted arrays
- **is_sorted** — sorted check

### API conventions
- All public symbols use the `cds_` prefix: `cds_<container>_<verb>`
- Functions return `int` error codes (`CDS_OK`, `CDS_ERR_NULL`, `CDS_ERR_INDEX`, etc.)
- Data is returned via `void *out` parameter (copy semantics, no internal pointer exposure)
- All containers are opaque types — internals hidden in `.c` files
- `count` = number of elements, `size` = byte size of data

## What's Next

### In progress
- hashmap — complete implementation with dynamic resizing ([#12](https://github.com/christianBorrello/cds/issues/12))
- merge sort — O(n log n) sorting for linked lists ([#11](https://github.com/christianBorrello/cds/issues/11))

### Algorithms
- heap sort
- reverse
- shuffle

### Future ideas
- **Iterators** — generic traversal pattern across all containers (callback-based or opaque iterator)
- **Binary search tree** — BST with AVL or red-black balancing (rotations, invariant maintenance)
- **Heap + priority queue** — binary heap backed by vector, implicit tree via index arithmetic
- **Memory arena / pool allocator** — custom allocator for bulk allocation and controlled memory layout

Architectural improvements and enhancements are tracked as [GitHub issues](https://github.com/christianBorrello/cds/issues).
