#include <stdio.h>
#include <limits.h>
#include "memory.h"

/* Small helper to print error codes nicely */
static void expect_ok(const char *what, int code) {
    if (code == MEM_OK) {
        printf("[ OK ] %s\n", what);
    } else {
        printf("[FAIL] %s -> error code %d\n", what, code);
    }
}

static void expect_err(const char *what, int code) {
    if (code != MEM_OK) {
        printf("[ OK ] %s (got expected error %d)\n", what, code);
    } else {
        printf("[FAIL] %s -> expected error but got MEM_OK\n", what);
    }
}


int main(void) {
    Memory *m = NULL;

    /* 1) Init */
    expect_ok("memInit", memInit(&m));
    if (m == NULL) {
        printf("Memory pointer is NULL after memInit, aborting.\n");
        return 1;
    }

    /* 2) Allocate a block of 5 cells */
    int startA = -1;
    expect_ok("memAlloc(A, 5)", memAlloc(m, 5, &startA));
    printf("A start = %d\n", startA);

    /* Your memFreeBlock() scans until it finds INT_MAX, so we must place it.
       This means the block is treated as "INT_MAX-terminated". */
    expect_ok("memWrite(A[4] = INT_MAX sentinel)", memWrite(m, startA, 4, INT_MAX));

    /* 3) Write/read some values */
    expect_ok("memWrite(A[0]=10)", memWrite(m, startA, 0, 10));
    expect_ok("memWrite(A[1]=20)", memWrite(m, startA, 1, 20));

    int v = 0;
    expect_ok("memRead(A[0])", memRead(m, startA, 0, &v));
    printf("A[0] = %d (expected 10)\n", v);

    expect_ok("memRead(A[1])", memRead(m, startA, 1, &v));
    printf("A[1] = %d (expected 20)\n", v);

    /* 4) Inc/Dec */
    expect_ok("memInc(A[0])", memInc(m, startA, 0));
    expect_ok("memRead(A[0])", memRead(m, startA, 0, &v));
    printf("A[0] after inc = %d (expected 11)\n", v);

    expect_ok("memDec(A[1])", memDec(m, startA, 1));
    expect_ok("memRead(A[1])", memRead(m, startA, 1, &v));
    printf("A[1] after dec = %d (expected 19)\n", v);

    /* 5) Allocate another block (3 cells) */
    int startB = -1;
    expect_ok("memAlloc(B, 3)", memAlloc(m, 3, &startB));
    printf("B start = %d\n", startB);

    /* Place sentinel for B too (last index = 2) */
    expect_ok("memWrite(B[2] = INT_MAX sentinel)", memWrite(m, startB, 2, INT_MAX));
    expect_ok("memWrite(B[0]=7)", memWrite(m, startB, 0, 7));

    expect_ok("memRead(B[0])", memRead(m, startB, 0, &v));
    printf("B[0] = %d (expected 7)\n", v);

    /* 6) Free block A, then allocate again to see if space is reused */
    expect_ok("memFreeBlock(A)", memFreeBlock(m, startA));

    int startC = -1;
    expect_ok("memAlloc(C, 4)", memAlloc(m, 4, &startC));
    printf("C start = %d (often should reuse freed space, depending on best-fit)\n", startC);

    /* Sentinel for C */
    expect_ok("memWrite(C[3] = INT_MAX sentinel)", memWrite(m, startC, 3, INT_MAX));

    /* 7) Try some expected errors */
    expect_err("memRead(out of bounds i=-1) should fail", memRead(m, startC, -1, &v));
    expect_err("memWrite(out of bounds addr) should fail", memWrite(m, MEM_CELLS + 5, 0, 123));


    /* 8) Cleanup */
    printf("Freeing memory...\n");
    memFree(m);

    printf("Done.\n");
    return 0;
}

