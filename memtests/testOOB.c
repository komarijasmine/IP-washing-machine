#include "memory.h"

int main(void) {
    int v = 0;
    memInit();
    memRead(-1, &v);          // should print error + exit(0)
    return 1;                 // should never reach
}

