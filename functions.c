#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "functions.h"
#include "memory.h"

#define CR sizeof(int) / sizeof(char)

int checkArrayName(Memory *m, char arrayName)
{
    // Local function. Check whether array name already exists. If so, returns adress (index in memory) of array, otherwise returns -1.

    int i = 0;
    unsigned char *cell_values = (unsigned char *) m->cells;
    unsigned char cell_value = cell_values[MEM_CELLS * CR - 1];

    while (cell_value != CHAR_MAX)
    {
        if (cell_value == arrayName)
        {
            return (int) cell_values[MEM_CELLS * CR - 2 - i];
        }

        i += 2;
        cell_value = cell_values[MEM_CELLS * CR - 1 - i];
    }

    return -1;
}


int checkArray(Memory *m, char arrayName, int index)
{
    // Local function. Check whether array and index exists. If so, returns adress (index in memory) of array, otherwise returns negative value.

    int array = checkArrayName(m, arrayName);
    if (array < 0)
    {
        return -1;
    }

    // Check whether list terminates before index
    for (int i = 0; i <= index; i++)
    {
        if (m->cells[array + i] == INT_MAX)
        {
            return -2;
        }
    }

    return array;
}


Memory *initializeMemory(void) {
    Memory *m;
    memInit(m);

    if (!m)
    {
        return 1;
    }

    return 0;
}

int assign(Memory *m, char arrayName, int value)
{
    int array = checkArrayName(m, arrayName);
    if (array < 0)
    {
        return 1;
    }

    m->cells[array] = value;
    return 0;
}


int increase(Memory *m, char arrayName, int index)
{
    int array = checkArray(m, arrayName, index);
    if (array < 0)
    {
        return 1;
    }
    
    m->cells[array + index]++;
    return 0;
}


int decrease(Memory *m, char arrayName, int index)
{
    int array = checkArray(m, arrayName, index);
    if (array < 0)
    {
        return 1;
    }
    
    m->cells[array + index]--;
    return 0;
}


int allocate(Memory *m, char arrayName, int size)
{
    int i = 0;
    unsigned char *cell_values = (unsigned char *) m->cells;
    unsigned char cell_value = cell_values[MEM_CELLS * CR - 1];

    while (cell_value != CHAR_MAX)
    {
        // Check whether array name already exists
        if (cell_value == arrayName)
        {
            return 1;
        }

        i += 2;
        cell_value = cell_values[MEM_CELLS * CR - 1 - i];
    }

    // Allocate array of size + 1 (extra space for array terminator)
    int index = memAlloc(m, size + 1);
    if (index < 0)
    {
        return 2;
    }

    // Create variable-pointer pair at end of memory
    cell_values[MEM_CELLS * CR - 1 - i] = arrayName;
    cell_values[MEM_CELLS * CR - 2 - i] = (unsigned char) index;
    cell_values[MEM_CELLS * CR - 3 - i] = CHAR_MAX;

    // Add array terminator
    m->cells[size] = INT_MAX;

    return 0;
}


int printCell(Memory *m, char arrayName, int index)
{
    int array = checkArray(m, arrayName, index);
    if (array < 0)
    {
        return 1;
    }

    printf("%d\n", m->cells[array + index]);
    return 0;
}


int add(Memory *m, char arrayName1, char arrayName2)
{
    int array1 = checkArrayName(m, arrayName1);
    if (array1 < 0)
    {
        return 1;
    }

    int array2 = checkArrayName(m, arrayName2);
    if (array2 < 0)
    {
        return 1;
    }

    m->cells[array1] += m->cells[array2];
    return 0;
}


int subtract(Memory *m, char arrayName1, char arrayName2)
{
    int array1 = checkArrayName(m, arrayName1);
    if (array1 < 0)
    {
        return 1;
    }

    int array2 = checkArrayName(m, arrayName2);
    if (array2 < 0)
    {
        return 1;
    }

    m->cells[array1] -= m->cells[array2];
    return 0;
}


int multiply(Memory *m, char arrayName1, char arrayName2)
{
    int array1 = checkArrayName(m, arrayName1);
    if (array1 < 0)
    {
        return 1;
    }

    int array2 = checkArrayName(m, arrayName2);
    if (array2 < 0)
    {
        return 1;
    }

    m->cells[array1] *= m->cells[array2];
    return 0;
}


int andCells(Memory *m, char arrayName1, char arrayName2)
{
    int array1 = checkArrayName(m, arrayName1);
    if (array1 < 0)
    {
        return 1;
    }

    int array2 = checkArrayName(m, arrayName2);
    if (array2 < 0)
    {
        return 1;
    }

    m->cells[array1] = (m->cells[array1] * m->cells[array2]) % 2;
    return 0;
}


int xorCells(Memory *m, char arrayName1, char arrayName2)
{
    int array1 = checkArrayName(m, arrayName1);
    if (array1 < 0)
    {
        return 1;
    }

    int array2 = checkArrayName(m, arrayName2);
    if (array2 < 0)
    {
        return 1;
    }

    m->cells[array1] = (m->cells[array1] + m->cells[array2]) % 2;
    return 0;
}


int freeArray(Memory *m, char arrayName)
{
    int array = checkArrayName(m, arrayName);
    if (array < 0)
    {
        return 1;
    }

    if (!memFreeBlock(m, arrayName))
    {
        return 2;
    }

    return 0;
}


int printArray(Memory *m, char arrayName)
{
    int array = checkArrayName(m, arrayName);
    if (array < 0)
    {
        return 1;
    }

    int i = 0;
    int cell_value = m->cells[array];

    printf("[ ");
    while (cell_value != INT_MAX)
    {
        printf("%d ", cell_value);
        cell_value = m->cells[array + ++i];
    }
    printf("]\n");

    return 0;
}

