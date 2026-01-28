#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#define MEM_CELLS 100

#define MEM_OK 1

/**
 * @file memory.h
 * @brief Embedded memory with an allocator and bounds-checked access 
 * with a fixed size of 100 integer cells for the mini-language interpreter. 
 *
 * The module manages the integer array and supports:
 *  - Allocation of blocks
 *  - Freeing of previously allocated blocks
 *  - Safe read, write, increase, and decrease
 *
 * Ownership:
 *  - Memory *m is owned and allocated by the caller
 *  - Internal allocator data must be released by memFree
 */


/* @brief Initialize the module
 *
 * @pre: memory is not previously initialised
 * @post: all MEM_CELLS are set to 0. After this call, 
 *        allocations and accesses are valid.
 *
 * @return MEM_OK on success
 * 	   MEM_ERR_NULL if memory is previously initialised
 * 	   MEM_ERR_NOSPACE if allocation fails
 */
int memInit(void);

/*
 * @brief Free all memory associated with Memory object
 *
 * @pre: memory is initialised
 * @post All memory owned is released
 */
void memFree(void);

/*
 * @brief Allocates n contiguous cells.
 *
 * Corresponds to the mini-language command: Mal x n
 *
 * @pre: outStart is not NULL, n => 0
 * @post: 
 *        On sucess:
 *          - Writes start index of allocated block into *outStart
 *          - Allocated cells are initialised to 0
 *          - Internal free-space tracking is updated
 *        On failure:
 *          - MEM_ERR_NULL  outstart is NULL
 *          - MEM_ERR_OOB   if n is invalid
 *          - MEM_ERR_NOSPACE  if no suitable block exists
 */
int memAlloc(int n, int *outStart);

/*
 * @brief Free a previously allocated block
 *
 * Corresponds to the mini-language command: Fre x
 *
 * @param:     start:  index in which to start freeing
 * 	       len:    size of the block to free
 *
 * @pre:
 *  - No double or partial frees
 *
 * @post:
 *  - Region becomes available for future allocations
 *  - Internal free-space tracking is updated
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if memory is not initlaised
 *  - MEM_ERR_OOB if start is invalid
 */
int memFreeBlock(int start, int len);

/*
 * @brief Safe read from an allocated block
 *
 * @param:  i: 	        index in which to read
 *          outValue:   address to store read value
 * 
 * @pre: 
 *  - memory is initialised
 *  - outvalue is not NULL
 *   
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if outValue is NULL
 *  - MEM_ERR_OOB if access is out of bounds
 *  - MEM_ERR_FREE if index is not inside allocated block
 */
int memRead(int i, int *outValue);

/*
 * @brief Safe write into an allocated block
 *
 * @param  i:	     index in which to write
 *         value:    value to write
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if memory is not innitialised
 *  - MEM_ERR_OOB if access is out of bounds
 *  - MEM_ERR_FREE if index is not inside allocated block
 */
int memWrite(int i, int value);

/*
 * @brief Safe increment of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Inc x n
 *
 * @param  i:	index in which to increase
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is not initialised
 *  - MEM_ERR_OOB if access is out of bounds
 *  - MEM_ERR_FREE if index is not inside allocated block
 */
int memInc(int i);

/*
 * @brief Safe decrement of a cell within an allocated block
 * 
 * Corresponds to the mini-language command Dec x n
 *
 * @param  i:	index in which to decrease
 *
 * @return:
 *  - MEM_OK on success
 *  - MEM_ERR_NULL if m is not initialised
 *  - MEM_ERR_OOB if access is out of bounds
 *  - MEM_ERR_FREE if index is not inside allocated block
 */
int memDec(int i);

#endif
