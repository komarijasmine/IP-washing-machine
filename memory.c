#include <stdlib.h>
#include "memory.h"

typedef struct FreeSeg {
    int start;               
    int len;           
    struct FreeSeg *next;
} FreeSeg;

struct Memory {
	int cells[MEM_CELLS];
	FreeSeg *free_list;
};

static FreeSeg *newSeg(int start, int len) {
	FreeSeg *s = (FreeSeg *)malloc(sizeof(FreeSeg));
	if (!s) return NULL;
	s->start = start;
	s->len = len;
	s->next = NULL;
	return s;
}

static int segEnd(const FreeSeg *s) {
	return s->start + s->len;
}

// insert new free segment where it belongs in the list
static void insertSorted(FreeSeg **head, FreeSeg *seg) {
	if (seg == NULL) {
		return;
	}

	if (*head == NULL || seg->start < (*head)->start) {
		seg->next = *head;
		*head = seg;
		return;
	}

	FreeSeg *cur = *head;
	while (cur->next != NULL && cur->next->start < seg->start) {
		cur = cur->next;
	}
	seg->next = cur->next;
	cur->next = seg;
}

// combine segments that overlap
static void combine(FreeSeg *head) {
	FreeSeg *cur = head;
	while (cur != NULL && cur->next != NULL) {
		FreeSeg *next = cur->next;
		int curEnd = segEnd(cur);
		if (curEnd >= next->start) {
			int newEnd = curEnd;
			int nextEnd = segEnd(next);
			if (nextEnd > newEnd) {
				newEnd = nextEnd;
			}
			cur->len = newEnd - cur->start;
			cur->next = next->next;
			free(next);
		} else {
			cur = cur->next;
		}
	}
}

static int indexCheck(int start, int len, int i) {
	if (start < 0 || start >= MEM_CELLS) {
		return 0;
	}
	if (len <= 0) {
		return 0;
	}
	if (start + len > MEM_CELLS) {
		return 0;
	}
	if (i < 0 || i>= len) {
		return 0;
	}
	return 1;
}

void memInit(Memory *m) {
	if (m == NULL) {
		return;
	}
	for (int i = 0; i < MEM_CELLS; i++) {
		m->cells[i] = 0;
	}
	m->free_list = newSeg(0, MEM_CELLS);
}

void memFree(Memory *m) {
	if (m == NULL) {
		return;
	}

	FreeSeg *cur = m->free_list;
	while (cur != NULL) {
		FreeSeg *next = cur->next;
		free(cur);
		cur = next;
	}
	m->free_list = NULL;
}

// returns 0/1 on failure/success and writes start index into int
int memAlloc(Memory *m, int n, int *outStart) {
	if (m == NULL || outStart == NULL) {
		return 0;
	}
	if (n <= 0 || n > MEM_CELLS) {
		return 0;
	}

	FreeSeg *prev = NULL;
	FreeSeg *bestPrev = NULL;

	FreeSeg *prev = NULL;
	FreeSeg *cur = m->free_list;

	// traverse through the whole list to find best fit free block
	while (cur != NULL) {
		if (cur->len >= n) {
			if (best == NULL || cur->len < best->len) {
				best = cur;
				bestPrev = prev;

				// break if we find free block of exact size
				if (cur->len == n) {
					break;
				}
			}
		}
		prev = cur;
		cur = cur->next;
	}

	// not enough space in memory
	if (best == NULL) {
		return 0;
	}

	int start = best->start;

	// free the free block if we found exact size
	if (best->len == n) {
		if (bestPrev != NULL) {
			bestPrev->next = best->next;
		}
		else {
			m->free_list = best->next;
		}
		free(best);
	} 
	
	// reduce free block's size by n
	else {
		best->start += n;
		best->len -= n;
	}

	// initialise space as 0
	for (int i = 0; i <n; i++) {
		m->cells[start + i] = 0;
	}

	// write memory address of the start of the variable in memory
	*outStart = start;
	return 1;
}

// frees blocks in memory
void memFreeBlock(Memory *m, int start, int len) {
	if (m == NULL) {
		return;
	}
	if (len <= 0) {
		return;
	}
	if (start < 0 || start >= MEM_CELLS) {
		return;
	}
	if (start + len > MEM_CELLS) {
		return;
	}

	FreeSeg *seg = newSeg(start, len);
	if (seg == NULL) {
		return;
	}

	insertSorted(&m->free_list, seg);
	combine(m->free_list);
}

// boolean result, but writes it into an int
int memRead(const Memory *m, int start, int len, int i, int *outValue) {
	if (m == NULL || outValue == NULL) {
		return 0;
	}
	if (!indexCheck(start, len, i)) {
		return 0;
	}
	*outValue = m->cells[start + i];
	return 1;
}

int memWrite(Memory *m, int start, int len, int i, int value) {
	if (m == NULL) {
		return 0;
	}
	if (!indexCheck(start, len, i)) {
		return 0;
	}

	m->cells[start + i] = value;
	return 1;
}

//increases value in position by one
int memInc(Memory *m, int start, int len, int i) {
	if (m == NULL) {
		return 0;
	}
	if (!indexCheck(start, len, i)) {
		return 0;
	}

	m->cells[start + i] += 1;
	return 1;
}

int memDec(Memory *m, int start, int len, int i) {
	if (m == NULL) {
		return 0;
	}
	if (!indexCheck(start, len, i)) {
		return 0;
	}
	m->cells[start + i] -= 1;
	return 1;
}
