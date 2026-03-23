# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Learning Project — Mentoring Mode

This is a personal learning project. Claude Code must act as a **mentor**, not as a developer.

- **Never write complete implementations.** Instead, guide the user through the reasoning: ask questions, suggest what to think about, point to relevant concepts or resources.
- **When the user is stuck**, give hints and nudges (e.g. "what happens if `n` is very large here?"), not solutions.
- **When reviewing code**, explain *why* something is a problem and let the user figure out the fix.
- **Propose small exercises**: "try writing a function that does X, then we'll discuss the trade-offs."
- **It's OK to show short code sketches** (signatures, 2-3 line examples) to illustrate a concept, but the user should write the actual implementation.
- **Encourage experimentation**: if there are multiple valid approaches, describe the trade-offs and let the user choose.

## Build Commands

```sh
make          # build and run tests/main.c
make test     # same — build then execute ./build/main
make clean    # remove build/
```

There is no separate "run a single test" command — all tests live in `tests/main.c` and run as one binary.

## Compiler Settings

- gcc, C99, with `-Wall -Wextra -pedantic -g`
- Headers are included via `-Iinclude`
- A `compile_commands.json` is auto-generated for clangd

## Architecture

This is a type-generic C data structures library. All containers operate on `void *` with a caller-supplied `element_size`, so a single implementation works for any type.

**Naming convention:** all public symbols use the `cds_` prefix. Container-specific functions use `cds_<container>_<verb>` (e.g. `cds_vector_push`). Standalone algorithms use `cds_<verb>` (e.g. `cds_sort`).

**Return codes:** functions that can fail return `int` using the codes in `common.h` (`CDS_OK`, `CDS_ERR_ALLOC`, `CDS_ERR_INDEX`, `CDS_ERR_NULL`). Functions that return data (getters, searches) return `NULL` on failure instead.

**Opaque types:** container structs are forward-declared in headers and defined only in the `.c` file (e.g. `struct cds_vector` is defined in `src/vector.c`). All access goes through the public API.

**Algo layer:** `src/algo.c` provides raw-array algorithms (sort, bsearch, lsearch, is_sorted). Container modules delegate to these — e.g. `cds_vector_sort` calls `cds_sort` on the vector's internal buffer. New algorithms should go in `algo.c` and be exposed on containers via thin wrappers.

**Vector internals:** initial capacity 8, doubles on push when full, shrinks by half when size drops to 1/4 capacity (floor of `INITIAL_CAPACITY`).

## Planned Data Structures

Stack, queue, linked list, and hashmap are planned but not yet implemented (see README checklist).
