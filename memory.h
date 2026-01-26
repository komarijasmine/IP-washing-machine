#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define MEM_CELLS 100

/* Opaque definition of the memory struct */
typedef struct Memory Memory;

/* Initialize memory (cells = 0, free block of size MEM_CELLS) */
void memInit(Memory *m);

/* Free internal allocator (free list nodes). Call at end of program */
void memFree(Memory *m);

/*
 * Allocate n contiguous cells
 * Returns 1 on success (and writes start index into *outStart),
 * returns 0 on failuer (not enough memory or invalid args).
 * Allocated cells are initialized to 0.
 */
int memAlloc(Memory *m, int n, int *outStart);

/*
 * Free a previously allocated block [start, start+len).
 * Assumes caller passer a valid allocated block (no double-free)
 */
void memFreeBlock(Memory *m, int start, int len);

/* Safe access operations: return 1 on success, 0 on wrong access/invalid args. */
int memRead(const Memory *m, int start, int len, int i, int *outValue);
int memWrite(Memory *m, int start, int len, int i, int value);
int memInc(Memory *m, int start, int len, int i);
int memDec(Memory*m, int start, int len, int i);

#endif
