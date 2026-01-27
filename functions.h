#ifndef FUNCTIONS_H
#define FUNCTIONS_H

Memory *initializeMemory(void);
int assign(Memory *m, char arrayName, int value);
int increase(Memory *m, char arrayName, int index);
int decrease(Memory *m, char arrayName, int index);
int allocate(Memory *m, char arrayName, int size);
int printCell(Memory *m, char arrayName, int index);
int add(Memory *m, char arrayName1, char arrayName2);
int subtract(Memory *m, char arrayName1, char arrayName2);
int multiply(Memory *m, char arrayName1, char arrayName2);
int andCells(Memory *m, char arrayName1, char arrayName2);
int xorCells(Memory *m, char arrayName1, char arrayName2);
int freeArray(Memory *m, char arrayName);
int printArray(Memory *m, char arrayName);

#endif
