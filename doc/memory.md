# Memory block header

uint8_t: flags

- bit 0: last block
- bit 1: free block
- bit 2: reserved (0)
- bit 3: reserved (0)
- bit 4: reserved (0)
- bit 5: reserved (0)
- bit 6: reserved (0)
- bit 7: reserved (0)

uint64_t: blocksize

# `malloc(size_t size)`

1. Go to the master block (block 0)
2. Is the block free?
3. If yes...
   1. Temporarily save the `blocksize` (as `tempsize`)
   2. Set the block as used (not free)
   3. Clear out the block with zeros
   4. Set the `blocksize` to `size`
   5. Create a new free block with `tempsize`-`size` as the `blocksize`
   6. Return the pointer to the new block+9 (header info is 9 bytes)
4. If no...
   1. If the block is the last block then return zero
   2. If not then attempt to go to the next block
   3. Check if that block is free, if so then go to step 3.1 else repeat to 4.1

# `free(void* ptr)`
1. Go to `ptr`-9
2. Set the block as free
3. Temporarily save the `blocksize` (as `tempsize`)
4. Go to the next block
5. If block is free...
   1. Add `tempsize` with that blocks `blocksize`+9
   2. Repeat to 4
6. If not then..
   1. Go to `ptr`-9
   2. Set the `blocksize` to `tempsize`