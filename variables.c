#include <stdlib.h>
#include "variables.h"
#include "memory.h"
#include "parser.h"

void varInit(void) {
    for (int i = 0; i < ALPHABET; i++) {
        var[i].name = 'a' + i;
        var[i].start = -1
        var[i].length = 0;
        var[i].use = 0;
    }
}

void varCreate(char name, int start, int length) {
    int i = varIndex(name);
    if (i < 0 || i >= ALPHABET) {
        fprintf(stderr, "Error: Invalid variable name.\n", name);
        exit(0);
    }

    var[i].name = name;
    var[i].start = start;
    var[i].length = length;
    var[i].use = 1;
}

int varIndex(char name) {
    return name - 'a';
}

int varCheck(char name) {
    int i = varIndex(name);
    if (i < 0 || i >= ALPHABET) {
        fprintf(stderr, "Error: Invalid variable name.\n", name);
        exit(0);
    }
    return var[i].use;
}

int varFree(char name) {
    int i = varIndex(name);

    if (i < 0 || i >= ALPHABET) {
        fprintf(stderr, "Error: Invalid variable name.\n", name);
        exit(0);
    }

    else if (var[i].use == 0) {
        fprintf(stderr, "Already free.\n", name);
        return 0;
    }

    var[i].use = 0;
    var[i].start = -1;
    var[i].length = 0;
    return 1;
}
