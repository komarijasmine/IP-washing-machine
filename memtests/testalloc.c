#include "memory.h"

int main(void) {
    int A = -1, B = -1;
    memInit();

    memAlloc(MEM_CELLS, &A);  // consumes all memory
    memAlloc(1, &B);          // should print "Not enough memory." + exit(0)
    return 1;
}
