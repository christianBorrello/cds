# cds — C Data Structures

A personal library of data structures implemented in C.

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

## Data Structures

- [x] vector (dynamic array)
- [ ] stack
- [ ] queue
- [ ] list (linked list)
- [ ] hashmap

## Architectural Improvements

- [ ] **Custom Allocator** — Allow callers to inject their own `malloc`/`realloc`/`free` via a `cds_allocator` struct, enabling arena allocators, pool allocators, or debug-tracking allocators. Affects all containers.
- [ ] **Overflow protection** — Add a `safe_mul` helper to detect arithmetic overflow in expressions like `n * element_size` before passing them to `realloc`/`malloc`. Prevents silent wrap-around on large inputs.
- [ ] **Minimal test framework** — Replace printf-based tests with `ASSERT_EQ`/`ASSERT_TRUE` macros, automatic pass/fail counting, and non-zero exit code on failure (needed for CI).
- [ ] **Build system: sanitizers & dependency tracking** — Add `-MMD -MP` for header dependency tracking, ASan/UBSan targets (`make sanitize`), and separate `make build` from `make test`.
- [ ] **Iterator pattern** — Define a unified iteration interface (`cds_iter`) that works across all containers (vector, list, hashmap), so traversal code doesn't depend on container internals.
- [ ] **Umbrella header** — Create a single `cds/cds.h` that includes all sub-headers, so callers can `#include <cds/cds.h>` instead of including each one separately.
- [ ] **Error signaling consistency** — Document or unify the current dual pattern: `CDS_ERR_*` return codes for mutations vs `NULL` returns for getters/searches.
