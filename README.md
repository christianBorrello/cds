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
tests/         — manual test programs
```

## What's Implemented

- **vector** — dynamic array with push/pop, sort, binary & linear search
- **algo** — standalone algorithms on raw arrays (quicksort with median-of-three, binary search, linear search, is_sorted)

## What's Next

### Data Structures
- stack
- queue
- linked list
- hashmap

### Algorithms
- merge sort
- heap sort
- reverse
- shuffle

Architectural improvements and enhancements are tracked as [GitHub issues](https://github.com/christianBorrello/cds/issues).
