#include <stdio.h>
#include <stdlib.h>

int main(void) {

Memory * memory;
FILE* file = fopen("text.txt", "r");

char line[10];
while (fgets(line, sizeof(line), file))  {
    interpretLine(&memory, line);
}

fclose(file);
}
