#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "memory.h"

/* Custom list data type that stores the identifier of an array (_arrayName_), 
the length of the array (_length_), and it's address in memory (_address_) */
typedef struct Array
{
    char *arrayName;
    int length;
    int address;
    struct Array *next;
} Array;

// Creates static HEAD to list with array identifiers
static Array *arrays = NULL;

Array *checkArray(const char *arrayName);
int fetchaddress(const char *arrayName, int index);
int freeArrayName(const char *arrayName, int *addressAndLength);
int getVals(const char *arrayName1, const char *arrayName2, int *vals);

Array *checkArray(const char *arrayName)
{
    /* Local function 
    EFFECT: Checks whether an array with identifier _arrayName_ exists
    OUTPUT: The array of which the member .arrayName is equal to _arrayName_; 
    NULL if no array with identifier _arrayName_ exists */

    if (arrays)
    {
        Array *array = arrays;
        while (array)
        {
            if (!strcmp(array->arrayName, arrayName))
            {
                return array;
            }

            array = array->next;
        }
    }

    return NULL;
}


int fetchaddress(const char *arrayName, int index)
{
    /* Local function 
    EFFECT: Check whether the array with identifier _arrayName_ exists and whether index is within its range
    OUTPUT: The address in memory of the element with index _index_ of the array with the identifier _arrayName_; 
    -1 if no array with the identifier _arrayName_ exists; 
    -2 if index is outside of the range of the array with identifier _arrayName_ */

    Array *array = checkArray(arrayName); 
    if (array)
    {
        if (index >= 0 && index < array->length)
        {
            return array->address + index;
        }

        fprintf(stderr, "Error: index %d is outside of the range of the array with identifier %s\n", index, arrayName);
        return -2;
    }

    fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName);
    return -1;
}


int freeArrayName(const char *arrayName, int *addressAndLength)
{
    /* Local function
    EFFECT: Removes the element of the list with array identifiers of which the member .arrayName 
    is equal to _arrayName_. Stores the members .address and .length of the element to remove in 
    _addressAndLength_[0] and _addressAndLength_[1] respectively
    OUTPUT: 0 upon successful execution of the function; 
    1 if no element with member .arrayName equal to _arrayName_ exists */

    if (arrays)
    {
        Array *array = arrays;
        Array *previous = NULL;

        while (array)
        {
            if (!strcmp(array->arrayName, arrayName))
            {
                addressAndLength[0] = array->address;
                addressAndLength[1] = array->length;

                if (previous)
                {
                    previous->next = array->next;
                }
                else
                {
                    arrays = array->next;
                }

                free(array->arrayName);
                free(array);

                return 0;
            }

            previous = array;
            array = array->next;
        }
    }

    fprintf(stderr, "Error: no array with identifier %s exists\n", arrayName); 
    return 1;
}


int getVals(const char *arrayName1, const char *arrayName2, int *vals)
{
    /* Local function 
    EFFECT: Fetches the address in memory and the value of the first element of 2 arrays with the 
    identifier _arrayName1_ and _arrayName2_. This is stored in _vals_, where vals[0] is the memory 
    address of the array with the identifier _arrayName1_, vals[1] is the memory address of the array 
    with the identifier _arrayName2_, vals[2] is the value of the first element of the array with 
    the identifier _arrayName1_, and vals[3] is the value of the first element of the array with 
    the identifier _arrayName2_
    OUTPUT: 0 upon successful execution of the function; 
    1 if fetching the memory address of the array with the identifier _arrayName1_ failed; 
    2 if fetching the memory address of the array with the identifier _arrayName2_ failed;
    3 if reading the memory cell of the first element of the array with the identifier _arrayName1_ failed,
    4 if reading the memory cell of the first element of the array with the identifier _arrayName2_ failed */

    vals[0] = fetchaddress(arrayName1, 0);
    if (vals[0] < 0)
    {
        return 1;
    }

    vals[1] = fetchaddress(arrayName2, 0);
    if (vals[1] < 0)
    {
        return 2;
    }

    if (memRead(vals[0], &vals[2]))
    {
        fprintf(stderr, "Error: reading the memory cell of the array with identifier %s with the address %d failed\n", arrayName1, vals[0]);
        return 3;
    }

    if (memRead(vals[1], &vals[3]))
    {
        fprintf(stderr, "Error: reading the memory cell of the array with identifier %s with the address %d failed\n", arrayName2, vals[1]);
        return 4;
    }

    return 0;
}


int init(void)
{
    // Initialize the memory
    if (memInit())
    {
        fprintf(stderr, "Error: initializing memory failed\n");
        return 1;
    }

    return 0;
}


int freeAll(void)
{
	while (arrays)
	{
		// Frees the first element of _arrays_. _arrays_ points to the next element after removal
        char *arrayName = arrays->arrayName;
		if (freeArray(arrayName))
		{
			memFree();
			return 1;
		}
 	}

	memFree();

	return 0;
}


int assign(const char *arrayName, int value)
{
    int address = fetchaddress(arrayName, 0);
    if (address < 0)
    {
        fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);       
        return 1;
    }

    if (memWrite(address, value))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", address, arrayName);     
        return 2;
    }

    return 0;
}


int increase(const char *arrayName, int index)
{
    int address = fetchaddress(arrayName, index);
    if (address < 0)
    {
        fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);     
        return 1;
    }

    if (memInc(address))
    {
        fprintf(stderr, "Error: increasing the value of the address %d of the array with identifier %s failed\n", address, arrayName);    
        return 2;
    }

    return 0;
}


int decrease(const char *arrayName, int index)
{
    int address = fetchaddress(arrayName, index);
    if (address < 0)
    {
        fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);  
        return 1;
    }

    if (memDec(address))
    {
        fprintf(stderr, "Error: decreasing the value of the address %d of the array with identifier %s failed\n", address, arrayName);    
        return 2;
    }

    return 0;
}


int allocate(const char *arrayName, int length)
{
    if (length <= 0)
    {
        fprintf(stderr, "Error: invalid length %d of array", length);
        return 1;
    }

    // Check that _arrayName_ doesn't already exist
    if (checkArray(arrayName))
    {
        fprintf(stderr, "Error: array with identifier %s already exists", arrayName);
        return 2;
    }

    // Store _arrayName_ and its length _length_ and its address in memory in the same element
    Array *newElement = malloc(sizeof(Array));
    if (!newElement)
    {
        fprintf(stderr, "Error: creating a new element to store array identifier %s failed\n", arrayName);
        return 3;
    }

    // Create new char* _newElement_->arrayName and copy into this, as _arrayName_ will be deleted
    // after the line is executed
    newElement->arrayName = strdup(arrayName);
    if (!newElement->arrayName)
    {
        free(newElement);
        fprintf(stderr, "Error: an error occured while allocating memory to store identifier %s", arrayName);
        return 4;
    }

    newElement->length = length;
    newElement->next = NULL;

    // Allocate space in memory for array and store its address
    if (memAlloc(length, &(newElement->address)))
    {
        free(newElement);
        fprintf(stderr, "Error: allocating memory for the array with identifier %s failed\n", arrayName);
        return 5;
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


int printCell(const char *arrayName, int index)
{
    int address = fetchaddress(arrayName, index);
    if (address < 0)
    {
        fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);    
        return 1;
    }

    int val;
    if (memRead(address, &val))
    {
        fprintf(stderr, "Error: reading the address %d of the array with identifier %s failed\n", address, arrayName);
        return 2;
    }

    printf("%d\n", val);

    return 0;
}


int add(const char *arrayName1, const char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] + vals[3]))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int subtract(const char *arrayName1, const char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] - vals[3]))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int multiply(const char *arrayName1, const char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] * vals[3]))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int andCells(const char *arrayName1, const char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] & vals[3]))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int xorCells(const char *arrayName1, const char *arrayName2)
{
    int vals[4];
    if (getVals(arrayName1, arrayName2, vals))
    {
        return 1;
    }

    if (memWrite(vals[0], vals[2] ^ vals[3]))
    {
        fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", vals[0], arrayName1);
        return 2;
    }

    return 0;
}


int freeArray(const char *arrayName)
{
    int addressAndLength[2];
    if (freeArrayName(arrayName, addressAndLength))
    {
        fprintf(stderr, "Error: freeing array with identifier %s failed\n", arrayName);
        return 1;
    }

    if (memFreeBlock(addressAndLength[0], addressAndLength[1]))
    {
        fprintf(stderr, "Error: freeing memory of array with identifier %s failed\n", arrayName);
        return 2;
    }

    return 0;
}


int printArray(const char *arrayName)
{
    Array *array = checkArray(arrayName);
    if (!array)
    {
        fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName);
        return 1;
    }

    printf("[ ");
    for (int i = 0, n = array->length; i < n; i++)
    {
        int val;
        if (memRead(array->address + i, &val))
        {
            fprintf(stderr, "Error: reading the address %d of the array with identifier %s failed\n", array->address + i, arrayName);
            return 2;
        }

        printf("%d ", val);
    }
    printf("]\n");
    
    return 0;
}

