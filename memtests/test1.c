// memtests/test1.c
#include <stdio.h>
#include <limits.h>
#include "memory.h"

static void ok(const char *msg) {
    printf("[ OK ] %s\n", msg);
}

static void fail(const char *msg, int rc) {
    printf("[FAIL] %s (rc=%d)\n", msg, rc);
}

static void expect_ok(const char *msg, int rc) {
    if (rc == MEM_OK) ok(msg);
    else fail(msg, rc);
}

int main(void) {
    int A = -1, B = -1, C = -1;
    int v = 0;

    printf("=== test_valid: memory module (exit-on-error) ===\n");

    expect_ok("memInit()", memInit());

    // Allocate A (5 cells)
    expect_ok("memAlloc(A=5)", memAlloc(5, &A));
    printf("A start = %d\n", A);

    // Write/read within A
    expect_ok("memWrite(A+0, 10)", memWrite(A + 0, 10));
    expect_ok("memWrite(A+1, 20)", memWrite(A + 1, 20));

    expect_ok("memRead(A+0)", memRead(A + 0, &v));
    printf("A[0] = %d (expected 10)\n", v);

    expect_ok("memRead(A+1)", memRead(A + 1, &v));
    printf("A[1] = %d (expected 20)\n", v);

    // Inc/Dec inside A
    expect_ok("memInc(A+0)", memInc(A + 0));
    expect_ok("memRead(A+0)", memRead(A + 0, &v));
    printf("A[0] after inc = %d (expected 11)\n", v);

    expect_ok("memDec(A+1)", memDec(A + 1));
    expect_ok("memRead(A+1)", memRead(A + 1, &v));
    printf("A[1] after dec = %d (expected 19)\n", v);

    // Allocate B (3 cells)
    expect_ok("memAlloc(B=3)", memAlloc(3, &B));
    printf("B start = %d\n", B);

    expect_ok("memWrite(B+0, 7)", memWrite(B + 0, 7));
    expect_ok("memRead(B+0)", memRead(B + 0, &v));
    printf("B[0] = %d (expected 7)\n", v);

    // Free A using explicit length
    expect_ok("memFreeBlock(A,5)", memFreeBlock(A, 5));

    // Allocate C (4 cells). With best-fit, this often reuses A's region (start==A).
    expect_ok("memAlloc(C=4)", memAlloc(4, &C));
    printf("C start = %d (often expected %d)\n", C, A);

    expect_ok("memWrite(C+0, 99)", memWrite(C + 0, 99));
    expect_ok("memRead(C+0)", memRead(C + 0, &v));
    printf("C[0] = %d (expected 99)\n", v);

    // Clean up
    printf("Calling memFree()...\n");
    memFree();
    printf("Done.\n");

    return 0;
}

