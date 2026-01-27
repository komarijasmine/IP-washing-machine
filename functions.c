#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "functions.h"
#include "memory.h"

<<<<<<< HEAD
// Custom list data type that stores the identifier of an array (_arrayName_), the length of the array (_length_), and it's address in memory (_address_)
typedef struct
{
    char arrayName;
    int length;
    int address;
    Array *next;
} Array;

// Creates static head to list with array identifiers
static Array *arrays = NULL;

Array *checkArray(char arrayName);
int fetchAdress(char arrayName, int index);
int freeArrayName(char arrayName);
int *getVals(char arrayName1, char arrayName2, int *vals);

Array *checkArray(char arrayName)
{
    /* Local function 
    EFFECT: Checks whether an array with identifier _arrayName_ exists
    OUTPUT: The array of which the member .arrayName is equal to _arrayName_; NULL if no array with identifier _arrayName_ exists */

    if (arrays)
    {
        Array *array = arrays->next;
        while (array)
        {
            if (array->arrayName == arrayName)
            {
                return array;
            }

            array = array->next;
        }
    }

    return NULL;
}


int fetchAdress(char arrayName, int index)
{
    /* Local function 
    EFFECT: Check whether the array with identifier _arrayName_ exists and whether index is within its range
    OUTPUT: The adress in memory of the element with index _index_ of the array with the identifier _arrayName_; 
    -1 if no array with the identifier _arrayName_ exists; 
    -2 if index is outside of the range of the array with the identifier _arrayName_ */

    Array *array = checkArray(arrayName); 
    if (array)
    {
        if (array->length > index)
        {
            return array->address + index;
        }

        return -2;
    }
    
    return -1;
}


int freeArrayName(char arrayName)
{
    /* Local function
    EFFECT: Removes the element of the arrays list with the member .arrayName that is equal to _arrayName_
    OUTPUT: The memory address of the array with the identiifer _arrayName_; 
    -1 if no element with member .arrayName equal to _arrayName_ exists */

    if (arrays)
    {
        Array *array = arrays->next;
        Array *previous = NULL;

        while (array)
        {
            if (array->arrayName == arrayName)
            {
                int address = array->address;

                if (previous)
                {
                    previous->next = array->next;
                }
                else
                {
                    arrays = array->next;
                }

                free(array);

                return address;
            }

            previous = array;
            array = array->next;
        }
    }

    return -1;
}


int *getVals(char arrayName1, char arrayName2, int *vals)
{
    /* Local function 
    EFFECT: Fetches the address in memory and the value of the first element of 2 arrays with the identifier _arrayName1_ and _arrayName2_. 
    This is stored in _vals_, where vals[0] is the memory adress of the array with the identifier _arrayName1_, 
    vals[1] is the memory adress of the array with the identifier _arrayName2_,
    vals[2] is the value of the first element of the array with the identifier _arrayName1_,
    and vals[3] is the value of the first element of the array with the identifier _arrayName2_
    OUTPUT: 0 upon success; 1 if fetching the memory address of the array with the identifier _arrayName1_ failed; 
    2 if fetching the memory address of the array with the identifier _arrayName2_ failed;
    3 if reading the memory cell of the first element of the array with the identifier _arrayName1_ failed,
    4 if reading the memory cell of the first element of the array with the identifier _arrayName1_ failed  */

    vals[0] = fetchAdress(arrayName1, 0);
    if (vals[0] < 0)
    {
        return 1;
    }

    vals[1] = fetchAdress(arrayName2, 0);
    if (vals[1] < 0)
    {
        return 2;
    }

    if (memRead(vals[0], &vals[2]))
    {
        return 3;
    }

    if (memRead(vals[1], &vals[3]))
    {
        return 4;
    }

=======
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
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    return 0;
}


<<<<<<< HEAD
int init(void)
{
    // Initialize the memory
    if (memInit())
    {
        // Initialization failed
        return 1;
    }

=======
int decrease(Memory *m, char arrayName, int index)
{
    int array = checkArray(m, arrayName, index);
    if (array < 0)
    {
        return 1;
    }
    
    m->cells[array + index]--;
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    return 0;
}


<<<<<<< HEAD
int freeAll(void)
{
    while (arrays)
    {
        // Frees the first element of _arrays_. _arrays_ points to the next element after removal
        if (freeArray(arrays->arrayName))
        {
            // Freeing failed
            return 1;
        }
    }

=======
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

>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    return 0;
}


<<<<<<< HEAD
int assign(char arrayName, int value)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
=======
int printCell(Memory *m, char arrayName, int index)
{
    int array = checkArray(m, arrayName, index);
    if (array < 0)
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    {
        return 1;
    }

<<<<<<< HEAD
    if (!memWrite(address, value))
=======
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
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    {
        return 2;
    }

    return 0;
}


<<<<<<< HEAD
int increase(char arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
=======
int printArray(Memory *m, char arrayName)
{
    int array = checkArrayName(m, arrayName);
    if (array < 0)
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    {
        return 1;
    }

<<<<<<< HEAD
    if (memInc(address))
    {
        return 2;
    }

    return 0;
}


int decrease(char arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        return 1;
    }

    if (memDec(address))
    {
        return 2;
    }

    return 0;
}


int allocate(char arrayName, int length)
{
    Array *newElement = malloc(sizeof(Array));
    if (!newElement)
    {
        return 1;
    }

    newElement->arrayName = arrayName;
    newElement->length = length;
    newElement->next = NULL;

    if (memAlloc(length, &(newElement->length)))
    {
        free(newElement);
        return 2;
    }

    if (!arrays)
    {
        arrays = newElement;
        return 0;
    }

    Array *element = arrays;
    while (element->next)
    {
        element = element->next;
    }
    element->next = newElement;

    return 0;
}


int printCell(char arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        return 1;
    }

    int *val;
    if (!memRead(address, val))
    {
        return 1;
    }

    printf("%d\n", *val);

    return 0;
}


int add(char arrayName1, char arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] + vals[3]))
    {
        return 2;
    }

    return 0;
}


int subtract(char arrayName1, char arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] - vals[3]))
    {
        return 2;
    }

    return 0;
}


int multiply(char arrayName1, char arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] * vals[3]))
    {
        return 2;
    }

    return 0;
}


int andCells(char arrayName1, char arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], (vals[2] * vals[3]) % 2))
    {
        return 2;
    }

    return 0;
}


int xorCells(char arrayName1, char arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], (vals[2] + vals[3]) % 2))
    {
        return 2;
    }

    return 0;
}


int freeArray(char arrayName)
{
    int address = freeArrayName(arrayName);
    if (address < 0)
    {
        return 1;
    }

    if (memFreeBlock(address))
    {
        return 2;
    }

    return 0;
}


int printArray(char arrayName)
{
    Array *array = checkArray(arrayName);
    if (!array)
    {
        return 1;
    }

    printf("[ ");
    for (int i = 0, n = array->length; i < n; i++)
    {
        int *val;
        if (memRead(array->address + i, val))
        {
            return 2;
        }

        printf("%d ", *val);
    }
    printf("]\n");
    
=======
    int i = 0;
    int cell_value = m->cells[array];

    printf("[ ");
    while (cell_value != INT_MAX)
    {
        printf("%d ", cell_value);
        cell_value = m->cells[array + ++i];
    }
    printf("]\n");

>>>>>>> 9679122515220888bf85d4da35d0e118fb874400
    return 0;
}

