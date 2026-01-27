#include <stdio.h>
#include <stdlib.h>

int main(void) {

char * fileName;
FILE* file = fopen(fileName, "r");

char line[10];
while (fgets(line, sizeof(line), file))  {
    interpretLine(&memory, line);
}

fclose(file);
}
