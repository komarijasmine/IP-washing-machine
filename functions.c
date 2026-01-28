#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "memory.h"

/* Custom list data type that stores the identifier of an array (_arrayName_), 
the length of the array (_length_), and it's address in memory (_address_) */
typedef struct
{
    char *arrayName;
    int length;
    int address;
    Array *next;
} Array;

// Creates static HEAD to list with array identifiers
static Array *arrays = NULL;

Array *checkArray(char *arrayName);
int fetchAdress(char *arrayName, int index);
int freeArrayName(char *arrayName);
int *getVals(char *arrayName1, char *arrayName2, int *vals);

Array *checkArray(char *arrayName)
{
    /* Local function 
    EFFECT: Checks whether an array with identifier _arrayName_ exists
    OUTPUT: The array of which the member .arrayName is equal to _arrayName_; 
    NULL if no array with the identifier _arrayName_ exists */

    if (arrays)
    {
        Array *array = arrays->next;
        while (array)
        {
            if (!strcmp(array->arrayName, arrayName))
            {
                return array;
            }

            array = array->next;
        }
    }

    fprintf(stderr, "No array with identifier %s exist", arrayName);
    return NULL;
}


int fetchAdress(char *arrayName, int index)
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

        fprintf(stderr, "Index %d is outside of the range of the array with the identifier %s", index, arrayName);
        return -2;
    }

    fprintf(stderr, "No array with identifier %s exist", arrayName); 
    return -1;
}


int freeArrayName(char *arrayName)
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
            if (!strcmp(array->arrayName, arrayName))
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

    fprintf(stderr, "No array with identifier %s exists", arrayName); 
    return -1;
}


int *getVals(char *arrayName1, char *arrayName2, int *vals)
{
    /* Local function 
    EFFECT: Fetches the address in memory and the value of the first element of 2 arrays with the 
    identifier _arrayName1_ and _arrayName2_. This is stored in _vals_, where vals[0] is the memory 
    adress of the array with the identifier _arrayName1_, vals[1] is the memory adress of the array 
    with the identifier _arrayName2_, vals[2] is the value of the first element of the array with 
    the identifier _arrayName1_, and vals[3] is the value of the first element of the array with 
    the identifier _arrayName2_
    OUTPUT: 0 upon successful execution of the function; 
    1 if fetching the memory address of the array with the identifier _arrayName1_ failed; 
    2 if fetching the memory address of the array with the identifier _arrayName2_ failed;
    3 if reading the memory cell of the first element of the array with the identifier _arrayName1_ failed,
    4 if reading the memory cell of the first element of the array with the identifier _arrayName1_ failed */

    vals[0] = fetchAdress(arrayName1, 0);
    if (vals[0] < 0)
    {
        fprintf(stderr, "Fetching the memory adress of the array with identifier %s failed", arrayName1); 
        return 1;
    }

    vals[1] = fetchAdress(arrayName2, 0);
    if (vals[1] < 0)
    {
        fprintf(stderr, "Fetching the memory adress of the array with identifier %s failed", arrayName1);
        return 2;
    }

    if (memRead(vals[0], &vals[2]))
    {
        fprintf(stderr, "Reading the memory cell of the array with identifier %s with the adress %d failed", arrayName1, vals[0]);
        return 3;
    }

    if (memRead(vals[1], &vals[3]))
    {
        fprintf(stderr, "Reading the memory cell of the array with identifier %s with the adress %d failed", arrayName2, vals[1]);
        return 4;
    }

    return 0;
}


int init(void)
{
    // Initialize the memory
    if (memInit())
    {
        fprintf(stderr, "Initializing memory failed");
        return 1;
    }

    return 0;
}


int freeAll(void)
{
    while (arrays)
    {
        // Frees the first element of _arrays_. _arrays_ points to the next element after removal
        if (freeArray(arrays->arrayName))
        {
            fprintf(stderr, "Freeing array failed");
            return 1;
        }
    }

    return 0;
}


int assign(char *arrayName, int value)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        fprintf(stderr, "Fetching adress of the array with identifier %s failed", arrayName);       
        return 1;
    }

    if (!memWrite(address, value))
    {
        fprintf(stderr, "Writing to address %d of the array with identifier %s failed", address, arrayName);     
        return 2;
    }

    return 0;
}


int increase(char *arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        fprintf(stderr, "Fetching adress of the array with identifier %s failed", arrayName);     
        return 1;
    }

    if (memInc(address))
    {
        fprintf(stderr, "Increasing the value of the address %d of the array with identifier %s failed", address, arrayName);    
        return 2;
    }

    return 0;
}


int decrease(char *arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        fprintf(stderr, "Fetching adress of the array with identifier %s failed", arrayName);  
        return 1;
    }

    if (memDec(address))
    {
        fprintf(stderr, "Decreasing the value of the address %d of the array with identifier %s failed", address, arrayName);    
        return 2;
    }

    return 0;
}


int allocate(char *arrayName, int length)
{
    // Store _arrayName_ and its length _length_ and its adress in memory in the same element
    Array *newElement = malloc(sizeof(Array));
    if (!newElement)
    {
        fprintf(stderr, "Creating a new element to store array identifier %s failed", arrayName);
        return 1;
    }

    newElement->arrayName = arrayName;
    newElement->length = length;
    newElement->next = NULL;

    // Allocate space in memory for array and store its address
    if (memAlloc(length, &(newElement->address)))
    {
        free(newElement);
        fprintf(stderr, "Allocating memory for the array with identifier %s failed", arrayName);
        return 2;
    }

    // Set HEAD to _newElement_ as currently no other arrays
    if (!arrays)
    {
        arrays = newElement;
        return 0;
    }

    // Add _newElement_ to the end of the list with array identifiers
    Array *element = arrays;
    while (element->next)
    {
        element = element->next;
    }
    element->next = newElement;

    return 0;
}


int printCell(char *arrayName, int index)
{
    int address = fetchAdress(arrayName, 0);
    if (address < 0)
    {
        fprintf(stderr, "Fetching adress of the array with identifier %s failed", arrayName);    
        return 1;
    }

    int *val;
    if (!memRead(address, val))
    {
        fprintf(stderr, "Reading the address %d of the array with identifier %s failed", address, arrayName);
        return 2;
    }

    printf("%d\n", *val);

    return 0;
}


int add(char *arrayName1, char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] + vals[3]))
    {
        fprintf("Writing to adress %d of the array with identifier %s failed", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int subtract(char *arrayName1, char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] - vals[3]))
    {
        fprintf("Writing to adress %d of the array with identifier %s failed", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int multiply(char *arrayName1, char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] * vals[3]))
    {
        fprintf("Writing to adress %d of the array with identifier %s failed", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int andCells(char *arrayName1, char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], (vals[2] * vals[3]) % 2))
    {
        fprintf("Writing to adress %d of the array with identifier %s failed", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int xorCells(char *arrayName1, char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], (vals[2] + vals[3]) % 2))
    {
        fprintf("Writing to adress %d of the array with identifier %s failed", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int freeArray(char *arrayName)
{
    int address = freeArrayName(arrayName);
    if (address < 0)
    {
        fprintf(stderr, "Freeing array with identifier %s failed", arrayName);
        return 1;
    }

    if (memFreeBlock(address))
    {
        fprintf(stderr, "Freeing memory of array with identifier %s failed", arrayName);
        return 2;
    }

    return 0;
}


int printArray(char *arrayName)
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
            fprintf(stderr, "Reading the address %d of the array with identifier %s failed", array->address + i, arrayName);
            return 2;
        }

        printf("%d ", *val);
    }
    printf("]\n");
    
    return 0;
}

