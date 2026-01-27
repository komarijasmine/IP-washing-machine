#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define MEM_CELLS 100

#define MEM_OK 1
#define MEM_ERR_NULL -1
#define MEM_ERR_OOB -2
#define MEM_ERR_NOSPACE -3

/**
 * @file memory.h
 * @brief Embedded memory with an allocator and bounds-checked access 
 * with a fixed size of 100 integer cells for the mini-language interpreter. 
 *
 * The module manages the integer array and supports:
 *  - allocation of blocks
 *  - freeing of previously allocated blocks
 *  - safe read, write, increase, and decrease within an allocated block
 *
 * Ownership:
 *  - Memory *m is owned and allocated by the caller
 *  - Internal allocator data must be released by memFree
 */

/* Opaque definition of the memory struct. Users only manipulate Memory 
 * objects through the functions bellow */
typedef struct Memory Memory;

/* @brief Initialize memory and allocator
 *
 * @pre: m is not NULL
 * @post: all MEM_CELLS are set to 0. After this call, 
 *        allocations and accesses are valid.
 */
void memInit(Memory *m);

/*
 * @brief Release internal allocator data.
 *
 * @pre: m is not NULL
 * @post: any dynamically allocated internal structures 
 *        used by the allocator are freed
 * Notes:
 *  - Does NOT free m itself
 *  - Call this once at program end to avoid leaks.
 */
void memFree(Memory *m);

/*
 * @brief Allocates n contiguous cells.
 *
 * Corresponds to the mini-language command: Mal x n
 *
 * @pre: m and outStart are not NULL, n > 0
 * @post: 
 *        On sucess:
 *          - Returns 1
 *          - Writes start index of allocated block into *outStart
 *          - Allocated cells are initialised to 0
 *          - Internal free-space tracking is updated
 *        On failure:
 *          - Returns 0
 *          - Interpreter should call an exception ("Not enough memory") 
 */
int memAlloc(Memory *m, int n, int *outStart);

/*
 * @brief Free a previously allocated block
 *
 * Corresponds to the mini-language command: Fre x
 *
 * @pre:
 *  - m is not NULL
 *  - len > 0
 *  - 0 <= start < MEM_CELLS
 *  - start + len <= MEM_CELLS
 *  - (start, len) is a currently allocated block
 *  - No double or partial frees
 * @post:
 *  - Region becomes available for future allocations
 *  - Internal free-space tracking is updated
 */
void memFreeBlock(Memory *m, int start, int len);

/*
 * @brief Safe read from an allocated block
 * 
 * @pre: 
 *  - m is not NULL
 *  - outvalue is not NULL
 *  - len > 0
 *  - (start, len) refers to an allocated block
 * 
 * @post:
 *  - returns 1 on success and writes the value to *outValue
 *  - returns 0 on failure
 */
int memRead(const Memory *m, int start, int len, int i, int *outValue);

/*
 * @brief Safe write into an allocated block
 *
 * @post:
 *  - Returns 1 on success
 *  - Returns 0 on failure
 */
int memWrite(Memory *m, int start, int len, int i, int value);

/*
 * @brief Safe increment of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Inc x n
 *
 * @post:
 *   - Returns 1 on success
 *   - Returns 0 on failure
 */
int memInc(Memory *m, int start, int len, int i);

/*
 * @brief Safe decrement of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Dec x n
 *
 * @post:
 *   - Returns 1 on success
 *   - Returns 0 on failure
 */
int memDec(Memory*m, int start, int len, int i);

#endif
