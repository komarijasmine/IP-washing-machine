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
 *  - This module owns an internal static Memory instance
 *  - memInit() allocates and initializes it
 *  - memFree() releases internal allocator data and Memory instance
 */


/* @brief Initialize the module
 *
 * @pre: memory is not previously initialised
 * @post: all MEM_CELLS are set to 0. After this call, 
 *        allocations and accesses are valid.
 *
 * @return If called when already initialized, this implementation
 * 	   returns MEM_OK and leaves existing memory unchanged.
 * 	   If allocation fails, module prints error and exits.
 */
int memInit(void);

/*
    * @brief Free all memory associated with the module-owned Memory instance.
 *
 * @pre memory is initialised
 * @post All module-owned memory is released and the module becomes uninitialized.
 */
void memFree(void);

/*
 * @brief Allocates n contiguous cells.
 *
 * Corresponds to the mini-language command: Mal x n
 *
 * @pre: outStart is not NULL, n > 0
 *
 * @post: 
 *        On sucess:
 *          - Writes start index of allocated block into *outStart
 *          - Allocated cells are initialised to 0
 *          - Internal free-space tracking is updated
 *        On failure:
 *          - Prints error message and exits
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
 * @return MEM_OK on success
 * @note Prints error message on double free
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
 *
 * @note If memory is uninitialized, outValue is NULL, the address is
 *       out of bounds, or the address is not allocated, prints
 *       error message and exits.
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
 *
 * @note If memory is uninitialized, the address is out of bounds, or the
 *       address is not allocated, the module prints an error message to stderr
 *       and exits.
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
 * 
 * @note If memory is uninitialized, the address is out of bounds, or the
 *       address is not allocated, the module prints an error message to stderr
 *       and exits.
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
 * 
 * @note If memory is uninitialized, the address is out of bounds, or the
 *       address is not allocated, the module prints an error message to stderr
 *       and exits.
 */
int memDec(int i);

#endif
