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

/* @brief Array representing the memory.
 * 
 * @returns pointer to the memory array.
 */
int *memCells(Memory *m);

/* @brief Initialize and allocate memory object
 *
 * If *pm is NULL, memory is allocated
 *
 * @param pm Pointer to a Memory pointer
 *
 * @pre: pm is not NULL
 * @post: all MEM_CELLS are set to 0. After this call, 
 *        allocations and accesses are valid.
 *        *pm points to a valid initialized Memory object
 *
 * @return MEM_OK on success
 * 	   MEM_ERR_NULL if pm is NULL
 * 	   MEM_ERR_NOSPACE if allocation fails
 */
void memInit(Memory **pm);

/*
 * @brief Free all memory associated with a Memory object
 * @param m pointer to memory object to destroy
 *
 * @pre: pm is not NULL
 * @post All memory owned by m is released
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
 *          - Writes start index of allocated block into *outStart
 *          - Allocated cells are initialised to 0
 *          - Internal free-space tracking is updated
 *        On failure:
 *          - MEM_ERR_NULL  if m or outstart is NULL
 *          - MEM_ERR_OOB   if n is invalid
 *          - MEM_ERR_NOSPACE  if no suitable block exists
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
 *
 * @post:
 *  - Region becomes available for future allocations
 *  - Internal free-space tracking is updated
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is NULL
 *  - MEM_ERR_OOB if start is invalid
 */
int memFreeBlock(Memory *m, int start);

/*
 * @brief Safe read from an allocated block
 * 
 * @pre: 
 *  - m is not NULL
 *  - outvalue is not NULL
 *  - len > 0
 *  - (start, len) refers to an allocated block
 *   
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m or outValue is NULL
 *  - MEM_ERR_OOB if access is out of bounds
 */
int memRead(const Memory *m, int start, int i, int *outValue);

/*
 * @brief Safe write into an allocated block
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is NULL
 *  - MEM_ERR_OOB if access is out of bounds
 */
int memWrite(Memory *m, int start, int i, int value);

/*
 * @brief Safe increment of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Inc x n
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is NULL
 *  - MEM_ERR_OOB if access is out of bounds
 */
int memInc(Memory *m, int start, int i);

/*
 * @brief Safe decrement of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Dec x n
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is NULL
 *  - MEM_ERR_OOB if access is out of bounds
 */
int memDec(Memory*m, int start, int i);

#endif
