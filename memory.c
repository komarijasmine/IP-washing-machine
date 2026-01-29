#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "memory.h"


/* Node for the free-list */
typedef struct FreeSeg {
    int start;              // first free cell index 
    int len;                // length of free segment 
    struct FreeSeg *next;   // next segment in ascending start order
} FreeSeg;

typedef struct Memory Memory;
static Memory *m = NULL;

/* Memory representation */
struct Memory {
	int cells[MEM_CELLS];  // simulated memory cells
	FreeSeg *free_list;    // free segments linked list, sorted by start
};

/* Prints error messages */
static void error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

/* Allocate a new free segment node. Returns NULL on failure */
static FreeSeg *newSeg(int start, int len) {
	FreeSeg *s = (FreeSeg *)malloc(sizeof(FreeSeg));

	if (!s) {
		return NULL;
	}

	s->start = start;
	s->len = len;
	s->next = NULL;
	return s;
}

/* End index of a free segment */
static int segEnd(const FreeSeg *s) {
	return s->start + s->len;
}

/* Insert segment into the list in ascending order. 
 * Does not handle segment overlap */
static void insertSorted(FreeSeg **head, FreeSeg *seg) {
	if (seg == NULL) {
		return;
	}

	// Insert at head if the list is empty or if the segment
	// belongs before current head
	if (*head == NULL || seg->start < (*head)->start) {
		seg->next = *head;
		*head = seg;
		return;
	}
        
	// Find insertion point
	FreeSeg *cur = *head;
	while (cur->next != NULL && cur->next->start < seg->start) {
		cur = cur->next;
	}

	// Place segment between cur and cur->next
	seg->next = cur->next;
	cur->next = seg;
}

/* Merge overlapping/adjacency segments in sorted free-list */
static void combine(FreeSeg *head) {
	FreeSeg *cur = head;

	// Because list is sorted, only cur and cur->next can overlap
	while (cur != NULL && cur->next != NULL) {
		FreeSeg *next = cur->next;
		int curEnd = segEnd(cur);

		// Overlap OR adjacency (>=)
		if (curEnd >= next->start) {
			// Extend cur to cover next
			int newEnd = curEnd;
			int nextEnd = segEnd(next);
			if (nextEnd > newEnd) {
				newEnd = nextEnd;
			}

			cur->len = newEnd - cur->start;

			// Remove next node from list and free
			cur->next = next->next;
			free(next);
		} else {
			// No merge possible, advance
			cur = cur->next;
		}
	}
}

/* Validate index i within memory */
static int addrOK(int addr) {
	return (addr >= 0 && addr < MEM_CELLS);
}

/* Validate index addr within allocated block */
static int isAllocated(int addr) {
	if (m == NULL) {
		return MEM_ERROR;
	}

	if(!addrOK(addr)) {
		return MEM_ERROR;
	}

	FreeSeg *cur = m->free_list;
	while (cur != NULL) {
		if (addr >= cur->start && addr < cur->start + cur->len) {
			return MEM_ERROR;
		}
		cur = cur->next;
	}
	return MEM_OK;  // not found in the free list so its allocated
}



/* Initialize all cells to 0 and allocator to one big free block */
int memInit(void) {
	if (m != NULL) {
		return MEM_OK;
	}

	m = malloc(sizeof(Memory));
	if (m == NULL) {
		error("Not enough memory.");
		return MEM_ERROR;
	}

	for (int i = 0; i< MEM_CELLS; i++) {
		m->cells[i] = 0;
	}

	// One free segment covering the whole memory
	m->free_list = newSeg(0, MEM_CELLS);
	if (m->free_list == NULL) {
		error("Not enough memory.");
		return MEM_ERROR;
	}
	return MEM_OK;
}

/* Free all free-list nodes */
void memFree(void) {
	if (m == NULL) {
		return;
	}

	// Walk through the list and free each node
	FreeSeg *cur = m->free_list;
	while (cur != NULL) {
		FreeSeg *next = cur->next;
		free(cur);
		cur = next;
	}
	free(m);
	m = NULL;
}

/* Allocate n cells using a best-fit policy (1 on success, 0 on failure) */
int memAlloc(int n, int *outStart) {
	if (m == NULL || outStart == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	if (n <= 0 || n > MEM_CELLS) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	// Find best free segment (smallest with len >= n)
	FreeSeg *prev = NULL;
	FreeSeg *bestPrev = NULL;
	FreeSeg *best = NULL;
	FreeSeg *cur = m->free_list;

	// Traverse through the whole list to find best fit free block
	while (cur != NULL) {
		if (cur->len >= n) {
			// Update best choice
			if (best == NULL || cur->len < best->len) {
				best = cur;
				bestPrev = prev;

				// Perfect fit, stop searching
				if (cur->len == n) {
					break;
				}
			}
		}
		prev = cur;
		cur = cur->next;
	}

	// No segment large enough
	if (best == NULL) {
		error("Not enough memory.");
		return MEM_ERROR;
	}

	int start = best->start;

	if (best->len == n) {
		// Perfect fit so remove best from list
		if (bestPrev != NULL) {
			bestPrev->next = best->next;
		}
		else {
			m->free_list = best->next;
		}
		free(best);
	} 
	
	else {
		// shrink free segment by consuming from its start
		best->start += n;
		best->len -= n;
	}

	// Initialise allocated cells to 0
	for (int i = 0; i <n; i++) {
		m->cells[start + i] = 0;
	}

	*outStart = start;
	return MEM_OK;
}

/* Add a block to the free list and merge adjacent/overlapping segments */
int memFreeBlock(int start, int len) {
	if (m == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	if (len <= 0) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
		
	}
	if (!addrOK(start) || !addrOK(start + len - 1)) {
	       	error("Wrong Memory Access.");
			return MEM_ERROR;
	}

	if (isAllocated(start)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	// Ensure entire block is allocated
	for (int i = 0; i< len; i++) {
		if (isAllocated(start + i)) {
			error("Wrong Memory Access.");
			return MEM_ERROR;
		}
	}

	// Create a new free segment node for the block
	FreeSeg *seg = newSeg(start, len);
	if (seg == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	// Insert in sorted order and handle overlap/adjacency
	insertSorted(&m->free_list, seg);
	combine(m->free_list);
	return MEM_OK;
}

/* Safe read of block[i] into *outValue */
int memRead(int i, int *outValue) {
	if (m == NULL || outValue == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	// Ensure access within memory
	if (!addrOK(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	// Ensure access within block
	if (isAllocated(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	*outValue = m->cells[i];
	return MEM_OK;
}

/* Safe write into block[i] */
int memWrite(int i, int value) {
	if (m == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	if (!addrOK(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	if (isAllocated(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	m->cells[i] = value;
	return MEM_OK;
}

/* Safe increment block[i]++ */
int memInc(int i) {
	if (m == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	
	if (!addrOK(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	if (isAllocated(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	m->cells[i] += 1;
	return MEM_OK;
}

/* Safe decrement block[i]-- */
int memDec(int i) {
	if (m == NULL) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	
	if (!addrOK(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}

	if (isAllocated(i)) {
		error("Wrong Memory Access.");
		return MEM_ERROR;
	}
	m->cells[i] -= 1;
	return MEM_OK;
}
