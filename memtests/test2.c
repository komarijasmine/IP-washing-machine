#include <stdio.h>
#include <limits.h>
#include "memory.h"

static void expect_ok(const char *what, int code) {
    if (code == MEM_OK) printf("[ OK ] %s\n", what);
    else printf("[FAIL] %s -> got %d\n", what, code);
}

static void expect_err(const char *what, int code) {
    if (code != MEM_OK) printf("[ OK ] %s (expected err %d)\n", what, code);
    else printf("[FAIL] %s -> expected error but got MEM_OK\n", what);
}

/* Place INT_MAX terminator at last index of an allocated block length n */
static void put_sentinel(Memory *m, int start, int n) {
    char msg[128];
    snprintf(msg, sizeof(msg), "sentinel at [%d + %d - 1]", start, n);
    expect_ok(msg, memWrite(m, start, n - 1, INT_MAX));
}

int main(void) {
    Memory *m = NULL;
    int rc;

    printf("=== EXTREME TESTS ===\n");

    /* 1) Init + argument validation for memInit */
    expect_err("memInit(NULL) should fail", memInit(NULL));
    expect_ok("memInit(&m)", memInit(&m));

    /* 2) memAlloc parameter edge cases */
    int s = -1;
    expect_err("memAlloc(n=0) should fail", memAlloc(m, 0, &s));
    expect_err("memAlloc(n<0) should fail", memAlloc(m, -3, &s));
    expect_err("memAlloc(n>MEM_CELLS) should fail", memAlloc(m, MEM_CELLS + 1, &s));
    expect_err("memAlloc(outStart=NULL) should fail", memAlloc(m, 3, NULL));

    /* 3) Allocate entire memory in one shot */
    int whole = -1;
    rc = memAlloc(m, MEM_CELLS, &whole);
    expect_ok("memAlloc(whole memory)", rc);
    printf("whole start = %d (expected 0)\n", whole);
    put_sentinel(m, whole, MEM_CELLS);

    /* Now there should be no space left */
    int extra = -1;
    expect_err("memAlloc after full allocation should fail", memAlloc(m, 1, &extra));

    /* Free the whole thing */
    expect_ok("memFreeBlock(whole)", memFreeBlock(m, whole));

    /* 4) Fragmentation + best-fit checks */
    int a=-1,b=-1,c=-1,d=-1,e=-1;

    expect_ok("memAlloc A=10", memAlloc(m, 10, &a));
    put_sentinel(m, a, 10);

    expect_ok("memAlloc B=20", memAlloc(m, 20, &b));
    put_sentinel(m, b, 20);

    expect_ok("memAlloc C=15", memAlloc(m, 15, &c));
    put_sentinel(m, c, 15);

    expect_ok("memAlloc D=5", memAlloc(m, 5, &d));
    put_sentinel(m, d, 5);

    printf("Starts: A=%d B=%d C=%d D=%d\n", a,b,c,d);

    /* Free B and D to create 2 holes: sizes 20 and 5 */
    expect_ok("memFreeBlock(B)", memFreeBlock(m, b));
    expect_ok("memFreeBlock(D)", memFreeBlock(m, d));

    /* Allocate E=5 should pick best-fit hole of size 5 (the old D hole) */
    expect_ok("memAlloc E=5 (best-fit should reuse D hole)", memAlloc(m, 5, &e));
    put_sentinel(m, e, 5);
    printf("E start = %d (expected %d)\n", e, d);

    /* Allocate F=18 should go into B hole (size 20) */
    int f=-1;
    expect_ok("memAlloc F=18 (should go into B hole)", memAlloc(m, 18, &f));
    put_sentinel(m, f, 18);
    printf("F start = %d (expected %d)\n", f, b);

    /* 5) Merge adjacency test:
       Free A (10) and then free the remaining tail of B hole (should be 2 cells left if F used 18 of 20)
       But we don't have an explicit 'free by length'; memFreeBlock depends on INT_MAX sentinel.
       So we can force a small block allocation in that remainder and then free both adjacent blocks.
    */

    /* Free A first */
    expect_ok("memFreeBlock(A)", memFreeBlock(m, a));

    /* Allocate G=2 (should fit in the remainder of B hole if your allocator left 2 cells free) */
    int g=-1;
    rc = memAlloc(m, 2, &g);
    if (rc == MEM_OK) {
        printf("[ OK ] memAlloc G=2 succeeded at %d (expected %d if remainder exists)\n", g, b + 18);
        put_sentinel(m, g, 2);
        /* Free G to make adjacency with A if contiguous layout permits (not always adjacent in this pattern) */
        expect_ok("memFreeBlock(G)", memFreeBlock(m, g));
    } else {
        printf("[INFO] memAlloc G=2 failed with %d; remainder may not exist depending on layout.\n", rc);
    }

    /* 6) memRead/memWrite edge cases */
    int val=0;
    expect_err("memRead with NULL outValue should fail", memRead(m, c, 0, NULL));
    expect_err("memRead with negative i should fail", memRead(m, c, -1, &val));
    expect_err("memWrite with negative i should fail", memWrite(m, c, -2, 123));

    /* 7) memFreeBlock edge cases */
    expect_err("memFreeBlock(start=-1) should fail", memFreeBlock(m, -1));
    expect_err("memFreeBlock(start=MEM_CELLS) should fail", memFreeBlock(m, MEM_CELLS));

    /* 8) Demonstrate sentinel limitation (this is EXPECTED to be unsafe/err-ish):
          allocate H=3, do NOT write INT_MAX sentinel, then memFreeBlock(H) will scan until it finds INT_MAX somewhere.
          Depending on memory contents, this could return OOB or free too much.
          So we DO NOT actually run it automatically, but we show how you would test it.
    */
    printf("\n[NOTE] Sentinel limitation test:\n");
    printf("  If you allocate a block and never write INT_MAX as a terminator,\n");
    printf("  memFreeBlock() may scan too far. That's a design limitation.\n");

    /* Cleanup */
    printf("\nFreeing Memory...\n");
    memFree(m);
    printf("Done.\n");
    return 0;
}

