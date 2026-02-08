# Issue: `free_with_pool` never releases memory to the OS

## Summary

The memory pool system in `kernel/mem.c` allocates memory in ~32KB blocks via
`add_block_to_memory_pool()` but **never returns those blocks to the OS**. The
`free_with_pool()` macro only returns items to the pool's internal free list —
it does not deallocate blocks even when every item in a block is free. This
means the process RSS can only grow, never shrink, for the lifetime of the
agent.

## Affected Code

- **`kernel/mem.c`**: `add_block_to_memory_pool()` (allocates blocks),
  `init_memory_pool()` (initializes pool)
- **`kernel/soarkernel.h`**: `free_with_pool()` macro (lines 495–500) and
  `allocate_with_pool()` macro (lines 487–493)

## Current Behavior

### Allocation (`allocate_with_pool`)

When the free list is empty, `add_block_to_memory_pool()` calls
`allocate_memory(size, POOL_MEM_USAGE)` to get a new ~32KB block. Items in the
block are linked into the free list. The block pointer is stored in
`p->first_block` as a singly-linked list (each block's first `sizeof(char*)`
bytes point to the previous block).

### Deallocation (`free_with_pool`)

```c
#define free_with_pool(p,item) { \
  fill_with_garbage ((item), (p)->item_size); \
  *(void * *)(item) = (p)->free_list; \
  (p)->free_list = (void *)(item); \
  decrement_used_count(p); \
  increment_free_list_length(p); }
```

The item is prepended to the pool's free list. **No check is made for whether
the block the item belongs to is now fully free.** The block is never returned
to `free_memory()` / the OS.

## Impact

This is a **contributing factor** to the linear memory growth observed in
long-running agents such as the counter demo. While the primary leak is that
top-level instantiations are never deallocated (see issue #10 and
`NO_TOP_LEVEL_REFS`), the pool system makes it impossible for even properly
freed items to reduce the process RSS.

### Measured Impact (counter demo, 100K decision cycles)

```
Time(s)  RSS(MB)
0        3.5
1        23.5
2        43.6
3        63.5
4        83.8
5        103.7
```

Pool statistics after 10K decision cycles show that pools only grow:

| Pool            | Used  | Free | Total Bytes |
|-----------------|-------|------|-------------|
| preference      | 10008 |   36 |   1,767,744 |
| instantiation   | 10002 |   46 |   1,044,992 |
| condition       | 60005 |   11 |   5,761,536 |
| int constant    | 10000 |  230 |     491,040 |
| identifier      | 10004 |  102 |   2,021,200 |
| wme             |    14 |  180 |      32,592 |

The `wme` pool demonstrates that when items are properly freed, the pool
stabilizes (used stays at 14, plenty of free items). But the pool blocks
themselves (~32KB each) are never returned to the OS, so even correctly
functioning pools contribute to RSS high-water marks.

## Expected Behavior

When all items in a pool block are free, the block should be returned to the OS
via `free_memory()`. At minimum, there should be a mechanism (perhaps a
periodic sweep or a threshold-based policy) to release fully-free blocks.

## Design Considerations

1. **Detecting fully-free blocks**: The current free list mixes items from all
   blocks. To detect when a block is fully free, we would need either:
   - A per-block used count, or
   - Segregated free lists (one per block), or
   - A sweep that walks the free list and determines which blocks are
     fully free

2. **Performance trade-off**: Pool allocators exist for performance. Any
   block-release mechanism should not significantly impact the fast path
   (`allocate_with_pool` / `free_with_pool`). A periodic sweep (e.g., after
   each decision cycle, or after `init-soar`) would avoid per-operation
   overhead.

3. **Block size**: Each block is `DEFAULT_BLOCK_SIZE` (~32KB, defined in
   `mem.c` line 196). This is large enough that releasing even one block
   per pool can meaningfully reduce RSS.

4. **Interaction with `NO_TOP_LEVEL_REFS`**: Even if the top-level
   instantiation leak (issue #10) is fully fixed, the pool system will still
   prevent RSS from decreasing. Both issues should be addressed for
   long-running agents.

## Suggested Approach

The simplest approach that preserves pool performance:

1. Add a `used_count` field to each block header (or track it externally).
2. On `free_with_pool`, decrement the owning block's count.
3. When a block's count reaches zero, unlink it from `first_block` chain,
   remove its items from the free list, and call `free_memory()` on it.
4. Optionally retain one empty block per pool as a hysteresis buffer to avoid
   thrashing.

A less invasive alternative:

1. Add a `release_empty_blocks(memory_pool *p)` function that walks the block
   list and free list, identifies fully-free blocks, and releases them.
2. Call it from `init-soar` and/or periodically (e.g., every N decision
   cycles).

## Reproduction

```bash
# Build
./make-SiOO

# Run counter demo for 100K decision cycles
cd cli
echo -e "counter-demo\nwatch 0\nrun 100000 d\nstats -memory\nquit" | ./SiOO
```

Observe that RSS grows to ~100MB+ while WM size stays at 14. The `stats
-memory` output shows pools with thousands of used items that are never freed.

## Related Issues

- Issue #10: Top-level instantiation memory lifecycle leak (the reason pools
  accumulate used items in the first place)
- PR #26: Memory leak fixes (symbol ref leaks in `io.c` and `rhsfun.c`)
- PR #27: Additional ref leak fix (`remove_rhs_function`)
- `soarBuildOptions.h` `NO_TOP_LEVEL_REFS`: Partial workaround for issue #10

## Labels

`bug`, `memory`, `kernel`, `modernization`
