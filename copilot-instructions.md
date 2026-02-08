# SiOO (Soar in One Object) — Copilot Instructions

## Project Overview

SiOO is an embeddable cognitive architecture based on Soar 8.5.2, written in C.
The codebase descends from a late-1990s/early-2000s research system and is being
incrementally modernized toward modern C while preserving behavioral stability.

- **License:** GPLv3 (kernel has BSD-licensed portions from the original Soar project)
- **Default branch:** `SiOO-9.9.1-DEVO`
- **Language:** C (compiled with `gcc -ansi` in the kernel, `gcc -std=gnu99` in the CLI)

## Repository Structure

```
sioo/
├── kernel/          # Core Soar engine (static library: libsoarkernel.a)
│   ├── soarkernel.h           # Master header — included by ALL kernel .c files
│   ├── soarBuildOptions.h     # Compile-time feature flags
│   ├── Makefile.in / configure / configure.in  # Autotools build
│   └── *.c                    # ~35 source files
├── cli/             # Command-line interface (SiOO binary)
│   ├── Makefile               # Builds SiOO and libSiOO.a
│   ├── main.c                 # Entry point
│   └── linenoise/             # Line-editing library
├── tools/           # Utility programs (siooc, gsioo, soar2sioo)
├── out/             # Build output directory (binaries, libraries, headers)
├── make-SiOO        # Top-level shell build script
└── copilot-instructions.md    # This file
```

## Build System

### Kernel Build
```bash
cd kernel && chmod u+x ./configure && ./configure && make
```
- Uses autotools (configure.in → configure → Makefile.in → Makefile)
- **Compiler flags:** `gcc -g -w -Wall -ansi -fpcc-struct-return -DUNIX`
- `-ansi` flag means C89/C90 standard — **no `inline` keyword** (use `static` for header functions)
- `-w` suppresses all warnings (masks `-Wall`)
- Produces `libsoarkernel.a`

### CLI Build
```bash
cd cli && make
```
- Uses `-std=gnu99` (C99 with GNU extensions)
- Links against `libsoarkernel.a`
- Produces `SiOO` binary and `libSiOO.a`
- **Known issue:** Link step may fail with `-lieee` if `libieee` is not installed

### Full Build
```bash
sh make-SiOO   # from repo root
```

## Architecture Notes

### Global Agent State
- All agent state lives in the `soar_agent` global struct pointer
- Accessed via macro: `current_agent(field)` → `(soar_agent->field)`
- This macro is intentionally kept as a macro — it's structural, not behavioral
- Do NOT try to convert `current_agent` to a function without major refactoring

### Memory Management
- Custom memory pools: `allocate_with_pool()`, `free_with_pool()` (macros in soarkernel.h)
  - These are complex multi-statement macros with conditional compilation
  - **Leave as macros for now** — converting them requires careful handling of
    `fill_with_garbage`, `increment_used_count`, `decrement_used_count` side macros
- Reference counting on 4 types: Symbol, WME, Preference, Production
  - As of this session: `symbol_add_ref`/`symbol_remove_ref`,
    `wme_add_ref`/`wme_remove_ref`, `preference_add_ref`/`preference_remove_ref`,
    `production_add_ref`/`production_remove_ref` are now **static functions**
    in soarkernel.h
- Known architectural leak: top-level instantiations never deallocated due to
  persistent WME references (see `NO_TOP_LEVEL_REFS` in soarBuildOptions.h and PR #27)

### Linked List Operations (DLL macros)
- `insert_at_head_of_dll()`, `remove_from_dll()`, `fast_remove_from_dll()`
- These take **field names** as macro arguments (e.g., `next`, `prev`)
- **Cannot be converted to functions** without major refactoring since C has no
  way to parameterize struct field access (would need offsetof-based approach)
- Same applies to `push()` macro which uses `allocate_cons()`

### The `common` Macro
- `#define common var.common_symbol_info` in soarkernel.h
- Used everywhere as `sym->common.reference_count`, `sym->common.symbol_type`, etc.
- **Do NOT remove** — would require touching every file in the project

### Test/Type Encoding Macros
- `test_is_blank_test`, `test_is_complex_test`, `rhs_value_is_symbol`, etc.
- These use pointer bit-twiddling for type tagging (low bits of pointers)
- **Leave as macros** — they rely on pointer arithmetic that is intentionally
  type-unsafe for performance reasons

## Modernization Work Completed

### PR #26 — Memory Leak Fixes
- Fixed symbol ref leaks in `io.c` (`do_input_cycle`)
- Fixed `malloc` buffer leak in `rhsfun.c` (`capitalize_symbol_rhs_function_code`)
- Fixed 29 call-site leaks via `symbol_remove_ref(name)` in `remove_rhs_function()`

### PR #27 — Additional Ref Leak + Docs
- Fixed stored symbol ref leak in `remove_rhs_function()` (`rf->name`)
- Updated `NO_TOP_LEVEL_REFS` documentation in `soarBuildOptions.h`

### PR #29 — CLI Security Fixes
- Buffer overflow fixes: `strcat()` → `snprintf()` with `PATH_MAX`
- Command injection prevention: shell metacharacter validation
- NULL pointer dereference fix: `getenv("TERM")` guard

### This Session — Macro-to-C Conversions (kernel only)
Converted preprocessor macros to proper C constructs without changing behavior:

**Value constants → enums:**
- `decide.c`: decider flags → `enum decider_flag`
- `consistency.c`: decision types → `enum consistency_decision_type`
- `io.c`: output link status → `enum output_link_status`
- `rete.c`: rete test types → `enum rete_test_type`, `enum relational_rete_test_type`
- `rete.c`: beta node types → `enum bnode_type`
- `rete.c`: operand list → `enum operand_assert_list`

**Function-like macros → static functions (in .c files):**
- `decide.c`: `promote_if_needed`, `mark_unknown_level_if_needed`, `update_levels_if_needed`
- `osupport.c`: `add_to_os_tc_if_needed`, `add_to_os_tc_if_id`
- `chunk.c`: `add_results_if_needed`
- `backtrace.c`: `add_to_grounds`, `add_to_potentials`, `add_to_locals`
- `lexer.c`: `record_position_of_start_of_lexeme`, `store_and_advance`, `finish`

**Reference-counting macros → static functions (in soarkernel.h):**
- `symbol_add_ref`, `symbol_remove_ref`
- `wme_add_ref`, `wme_remove_ref`
- `preference_add_ref`, `preference_remove_ref`
- `production_add_ref`, `production_remove_ref`

## Remaining Macros NOT Yet Converted

### Safe to convert in future sessions:
- `rete.c` bitwise test macros: `bnode_is_hashed`, `bnode_is_memory`, etc.
  → static functions (simple single-expression)
- `rete.c` utility macros: `var_locations_equal`, `field_from_wme`
  → static functions (but `field_from_wme` relies on struct field ordering)
- `soarkernel.h`: `reading_from_top_level()` → static function
- `soarkernel.h`: `savestring()` → static function (simple malloc+strcpy)
- `soarkernel.h` value constants: `VARIABLE_SYMBOL_TYPE` etc. → enum
- `soarkernel.h` value constants: `TOP_GOAL_LEVEL` etc. → enum or const

### Intentionally left as macros:
- `current_agent(x)` — structural accessor, not behavioral
- `common` — field alias, would break everything
- `insert_at_head_of_dll`, `remove_from_dll`, `fast_remove_from_dll`
  — use field-name parameters
- `push`, `allocate_cons`, `free_cons` — depend on DLL macros
- `allocate_with_pool`, `free_with_pool` — complex multi-statement with
  conditional sub-macros
- `increment_used_count`, `decrement_used_count` — conditional compilation stubs
- Test/RHS encoding macros (pointer bit-twiddling)
- `make_equality_test`, `make_test_from_complex_test`, etc. — pointer arithmetic

## Spelling and Documentation Quality

- **Always** correct spelling errors in comments and documentation files
  (`.md`, `.html`, `.txt`, `.text`) whenever they are encountered.
- This applies to all prose: code comments, README files, build scripts,
  and any other human-readable text in the repository.

## Coding Conventions

- `bool` type is `typedef char bool` (not `_Bool` or `stdbool.h`)
- `TRUE`/`FALSE`/`NIL` are `#define`'d as `(1)`/`(0)`/`(0)`
- `byte` is `typedef unsigned char byte`
- Use `print()` not `printf()` for kernel output (Soar's own print function)
- Use `allocate_memory()`/`free_memory()` not `malloc()`/`free()` for tracked allocations
- No test infrastructure exists — validation is via successful compilation and
  manual testing with Soar agent programs

## Pre-existing Issues

- `system()` passthrough in CLI `parsing.c` (unrestricted shell commands)
- `memset()` transposed arguments in `cli/demos/toh_demo.c`
- Top-level instantiation memory lifecycle leak (architectural, needs major refactoring)
- Missing `-lieee` library on modern Linux (link failure in CLI)
