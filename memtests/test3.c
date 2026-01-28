#include <stdio.h>
#include <limits.h>
#include "memory.h"

static void expect_ok(const char *msg, int rc) {
    if (rc == MEM_OK) printf("[ OK ] %s\n", msg);
    else printf("[FAIL] %s -> rc=%d\n", msg, rc);
}

static void expect_err(const char *msg, int rc, int expected) {
    if (rc == expected) printf("[ OK ] %s (got %d)\n", msg, rc);
    else printf("[FAIL] %s -> got %d, expected %d\n", msg, rc, expected);
}

int main(void) {
    int rc;

    printf("=== test_alloccheck ===\n");

    /* 1) init */
    expect_ok("memInit()", memInit());

    /* 2) Before allocation, cells should be FREE (not accessible) */
    int v = 0;
    rc = memRead(0, &v);
    expect_err("memRead(0) before alloc should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    rc = memWrite(0, 123);
    expect_err("memWrite(0) before alloc should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    rc = memInc(0);
    expect_err("memInc(0) before alloc should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    rc = memDec(0);
    expect_err("memDec(0) before alloc should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    /* 3) Allocate A=5 */
    int A = -1;
    expect_ok("memAlloc(A=5)", memAlloc(5, &A));
    printf("A start = %d\n", A);

    /* Now A's cells should be accessible */
    expect_ok("memWrite(A+0, 10)", memWrite(A + 0, 10));
    expect_ok("memRead(A+0)", memRead(A + 0, &v));
    printf("A[0] = %d (expected 10)\n", v);

    expect_ok("memInc(A+0)", memInc(A + 0));
    expect_ok("memRead(A+0)", memRead(A + 0, &v));
    printf("A[0] after inc = %d (expected 11)\n", v);

    /* Put INT_MAX terminator at last cell so memFreeBlock can find end */
    expect_ok("memWrite(A+4, INT_MAX terminator)", memWrite(A + 4, INT_MAX));

    /* 4) Allocate B=3 */
    int B = -1;
    expect_ok("memAlloc(B=3)", memAlloc(3, &B));
    printf("B start = %d\n", B);

    expect_ok("memWrite(B+2, INT_MAX terminator)", memWrite(B + 2, INT_MAX));

    /* 5) Free A and verify A cells become inaccessible again */
    expect_ok("memFreeBlock(A)", memFreeBlock(A));

    rc = memRead(A + 0, &v);
    expect_err("memRead(A+0) after free should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    rc = memWrite(A + 0, 77);
    expect_err("memWrite(A+0) after free should be MEM_ERR_FREE", rc, MEM_ERR_FREE);

    /* 6) Allocate C=4: should reuse freed A space (best-fit) */
    int C = -1;
    expect_ok("memAlloc(C=4)", memAlloc(4, &C));
    printf("C start = %d (often expected %d)\n", C, A);

    expect_ok("memWrite(C+3, INT_MAX terminator)", memWrite(C + 3, INT_MAX));

    /* C should be accessible */
    expect_ok("memWrite(C+0, 5)", memWrite(C + 0, 5));
    expect_ok("memRead(C+0)", memRead(C + 0, &v));
    printf("C[0] = %d (expected 5)\n", v);

    /* 7) Out-of-bounds should still be MEM_ERR_OOB */
    rc = memRead(-1, &v);
    expect_err("memRead(-1) should be MEM_ERR_OOB", rc, MEM_ERR_OOB);

    rc = memWrite(MEM_CELLS, 1);
    expect_err("memWrite(MEM_CELLS) should be MEM_ERR_OOB", rc, MEM_ERR_OOB);

    /* 8) cleanup */
    printf("Calling memFree()...\n");
    memFree();
    printf("Done.\n");
    return 0;
}

