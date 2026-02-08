# Code Review Findings: CLI Security Fixes (PR #29)

> **Date:** 2026-02-07  
> **PR:** [#29 - Fix buffer overflows, command injection, and NULL dereference in CLI](https://github.com/Sonophoto/sioo/pull/29)  
> **Files Changed:** `cli/main.c`, `cli/parsing.c`  
> **Security Scan:** CodeQL — 0 alerts found

---

## Summary of Changes Made

### `cli/main.c`
- **Buffer overflow fix:** Replaced `strcat()` path construction with `snprintf()` into `PATH_MAX`-sized buffers (was 256-byte fixed buffers)
- **NULL dereference fix:** Added NULL guard for `env_TERM` before `strncmp()` calls — `getenv("TERM")` can return NULL
- Added `#include <limits.h>` for `PATH_MAX`

### `cli/parsing.c`
- **Command injection fix:** Replaced `strcpy()`/`strcat()`/`system()` chain in `edit` handler with `snprintf()` + shell metacharacter validation
- **NULL dereference fix:** Added `ntokens` bounds check before accessing `tokens[1]`
- **Code cleanup:** Removed leftover debug `printf()` statements from the `edit` command handler

---

## Code Review Comments

### 1. `cli/parsing.c` line 142 — `print()` vs `printf()` usage

**Finding:** The new code uses `print()` instead of `printf()` for error messages.

**Resolution:** Not a bug. `print()` is a custom function defined in `soarkernel.h` (declared as `extern void print(char *format, ...)`). It is the standard output function used throughout the SiOO codebase for kernel-aware printing. The usage is consistent with existing code conventions.

### 2. `cli/parsing.c` line 148 — `bool` type and `TRUE` constant

**Finding:** Uses `bool` type and `TRUE` constant instead of standard C boolean conventions (`stdbool.h` with `true`, or `int` with `1`).

**Resolution:** Not a bug. `bool` and `TRUE`/`FALSE` are defined in `soarkernel.h` as part of the Soar kernel type system. These types are used extensively throughout the entire codebase. Using them here is consistent with established project conventions.

### 3. `cli/parsing.c` line 162 — Filenames with spaces can still break the `system()` call

**Finding:** While shell metacharacters are now filtered (`;`, `|`, `&`, `$`, `` ` ``, `'`, `"`, `(`, `)`, `<`, `>`), the filename could still contain spaces or other characters that would break the shell command constructed via `snprintf()` and passed to `system()`. Consider using `execvp()` or similar functions that don't invoke a shell, or properly quote the filename argument.

**Resolution:** Acknowledged as a valid concern for future improvement. However:
- The current fix is a **significant security improvement** over the previous code which had zero input validation
- Changing to `execvp()` would require a larger refactor (forking, exec, wait pattern)
- The existing `system(command)` fallback on line 175 of `parsing.c` passes *all* unrecognized user input directly to the shell — this is a much larger attack surface that is by-design behavior (shell passthrough)
- **Recommendation:** A follow-up issue should address replacing all `system()` calls with safer alternatives across the entire CLI

---

## Remaining Issues Identified (Not Fixed in PR #29)

These are pre-existing issues found during analysis that were outside the scope of the security fix:

### 4. `cli/parsing.c` line 175 — Unrestricted shell passthrough

```c
/* So our command is not a soarapi command... */
/* Lets feed it to /bin/sh and see what happens */
system(command);
```

Any command not recognized by SiOO is passed directly to `system()`. This is intentional design (acts as a shell) but represents a significant security surface if SiOO is ever used in contexts where user input should be restricted.

### 5. `cli/parsing.c` line 125 — `cd` command missing bounds check

```c
if ( !strncmp(command, "cd", 2) )
  {
    sys_chdir(tokens[1]);
    return SOAR_OK;
  }
```

The `cd` command accesses `tokens[1]` without first checking that `ntokens >= 2`. If a user types just `cd` with no argument, this could dereference uninitialized memory.

### 6. `demos/toh_demo.c` line 268 — Transposed `memset` arguments

```c
memset( g, sizeof( glb_info ), 0 );  // Arguments are swapped!
```

The GCC compiler warns about this: `-Wmemset-transposed-args`. The correct call should be:
```c
memset( g, 0, sizeof( glb_info ) );
```

### 7. Kernel memory leak (Issue #10)

The kernel has a known memory leak in the reference counting/justification system that causes the agent to exhaust memory after extended runs (~4-5 million decision cycles). This is documented in Issue #10 and requires architectural-level changes to resolve. Build options `NO_TOP_JUSTS` and `NO_TOP_LEVEL_REFS` serve as partial workarounds.

---

## Security Summary

| Finding | Severity | Status |
|---------|----------|--------|
| Buffer overflow in `main.c` path construction | **Critical** | ✅ Fixed in PR #29 |
| NULL dereference on `env_TERM` in `main.c` | **High** | ✅ Fixed in PR #29 |
| Command injection in `edit` handler (`parsing.c`) | **Critical** | ✅ Fixed in PR #29 |
| NULL dereference on `tokens[1]` in `edit` handler | **High** | ✅ Fixed in PR #29 |
| Debug `printf()` left in production code | **Low** | ✅ Fixed in PR #29 |
| Filenames with spaces break `edit` command | **Low** | ⚠️ Noted for future fix |
| Unrestricted `system()` shell passthrough | **Medium** | ⚠️ By design, noted |
| `cd` command missing argument check | **Medium** | ⚠️ Noted for future fix |
| `memset` transposed arguments in `toh_demo.c` | **Medium** | ⚠️ Pre-existing, noted |
| Kernel memory leak (Issue #10) | **High** | ⚠️ Architectural issue |
