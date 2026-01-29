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

// Local functions
static Array *checkArray(const char *arrayName);
int fetchAddress(const char *arrayName, int index);
int freeArrayName(const char *arrayName, int *addressAndLength);
int singleElementOperation(int address, int value1, int value2, char operator);
int executeDualArrayOperator(Array *array1, Array *array2, char operator, int onlyFirstElement);
int dualArrayOperator(const char *arrayName1, const char *arrayName2, char operator, int onlyFirstElement);

static Array *checkArray(const char *arrayName)
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


int fetchAddress(const char *arrayName, int index)
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

        fprintf(stderr, "Wrong Memory Access.\n");
        // fprintf(stderr, "Error: index %d is outside of the range of the array with identifier %s\n", index, arrayName);
        return -2;
    }

    fprintf(stderr, "Try to use variable that does not exist.\n");
    // fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName);
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

    fprintf(stderr, "Try to use variable that does not exist.\n");
    // fprintf(stderr, "Error: no array with identifier %s exists\n", arrayName); 
    return 1;
}


int singleElementOperation(int address, int value1, int value2, char operator)
{
    /* Local function
    EFFECT: Computes result of _operator_ _value1_ _value2_ and writes result to _address_
    OUTPUT: 0 upon successful execution of the function; 
    1 if no or an invalid operator was given */

    int result = 0;
    if (operator == '+')
    {
        result = value1 + value2;
    }
    else if (operator == '-')
    {
        result = value1 - value2;
    }
    else if (operator == '*')
    {
        result = value1 * value2;
    }
    else if (operator == '&')
    {
        result = (value1 * value2) % 2;
    }
    else if (operator == '^')
    {
        result = (value1 + value2) % 2;
    }
    else
    {
        return 1;
    }

    if (memWrite(address, result))
    {
        return 2;
    }

    return 0;
}


int executeDualArrayOperator(Array *array1, Array *array2, char operator, int onlyFirstElement)
{
    /* Local function
    EFFECT: Computes result of _operator_ _array1_ _array2_ and writes result to _array1_. If 
    _onlyFirstElement_ is 1, the operation is only done on the first element of _array1_ and
    _array2_. If _onlyFirstElement_ is 0, _array1_ and _array2_ must be the same lenght, and
    the operation will be done on each element of _array1_ and _array2_ (pointwise) 
    OUTPUT: 0 upon successful execution of the function; 
    1 if reading the elements of the array with identifier _arrayName1_ failed;
    2 if reading the elements of the array with identifier_arrayName2_ failed; 
    3 if writing to the array with identifier _arrayName1_ failed 
    4 if no or an invalid operator was supplied;
    5 if _onlyFirstElement_ is 0 and _array1_ and _array2_ are of different length */
    
    int i, n = 1;
    if (!onlyFirstElement)
    {
        // Check whether arrays are of same length
        if (array1->length != array2->length)
        {
            fprintf(stderr, "Logic operation between sequences of different length.\n");
            // fprintf(stderr, "Error: a point-wise AND or XOR operation cannot be performed on the array with identifier %s of length %d and the array with identifier %s of length %d. The length of the arrays must be the same\n", array1->arrayName, array1->length, array2->arrayName, array2->length);
            return 5;
        }

        n = array1->length;
    }

    for (i = 0; i < n; i++)
    {
        int element1;
        if (memRead(array1->address + i, &element1))
        {
            // fprintf(stderr, "Error: reading to address %d (index %d) of the array with identifier %s failed\n", array1->address + i, i, array1->arrayName);
            return 1;
        }

        int element2;
        if (memRead(array2->address + i, &element2))
        {
            // fprintf(stderr, "Error: reading to address %d (index %d) of the array with identifier %s failed\n", array2->address + i, i, array2->arrayName);
            return 2;
        }

        int error = singleElementOperation(array1->address + i, element1, element2, operator);
        if (error)
        {
            if (error == 1)
            {
                fprintf(stderr, "Error: invalid or no operator supplied\n");
                return 4;
            }
            if (error == 2)
            {
                // fprintf(stderr, "Error: writing to address %d (index %d) of the array with identifier %s failed\n", array1->address + i, i, array1->arrayName);
                return 3;
            }
        }
    }

    return 0;
}

int dualArrayOperator(const char *arrayName1, const char *arrayName2, char operator, int onlyFirstElement)
{
    /* Local function
    EFFECT: Computes the result of performing the operator associated with _operator_ on the array
    with identifier _arrayName1_ and the array with identifier _arrayName2_ and writes result to 
    the array with identifier _arrayName1_. If _onlyFirstElement_ is 1, the operation is only done 
    on the first element of the array with identifier _arrayName1_ and the array with identifier 
    _arrayName2_. If _onlyFirstElement_ is 0, the array with identifier _arrayName1_ and the array
    with identifier _arrayName2_ must be the same lenght, and the operation will be done pointwise 
    on each element of the arrays
    OUTPUT: 0 upon successful execution of the function; 
    1 if fetching the array with the identifier _arrayName1_ failed; 
    2 if fetching the array with the identifier _arrayName2_ failed;
    3 if reading, writing, or computing the result failed */

    Array *array1 = checkArray(arrayName1);
    if (!array1 || array1->address < 0)
    {
        fprintf(stderr, "Try to use variable that does not exist.\n");
        // fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName1);
        return 1;
    }

    Array *array2 = checkArray(arrayName2);
    if (!array2 || array2->address < 0)
    {
        fprintf(stderr, "Try to use variable that does not exist.\n");
        // fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName2);
        return 2;
    }

    if (executeDualArrayOperator(array1, array2, operator, onlyFirstElement))
    {
        return 3;
    }

    return 0;
}


int init(void)
{
    // Initialize the memory
    if (memInit())
    {
        // fprintf(stderr, "Error: initializing memory failed\n");
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
        if (!arrayName) {
            memFree();
            return 1;
        }

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
    int address = fetchAddress(arrayName, 0);
    if (address < 0)
    {
        // fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);       
        return 1;
    }

    if (memWrite(address, value))
    {
        // fprintf(stderr, "Error: writing to address %d of the array with identifier %s failed\n", address, arrayName);     
        return 2;
    }

    return 0;
}


int increase(const char *arrayName, int index)
{
    int address = fetchAddress(arrayName, index);
    if (address < 0)
    {
        // fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);     
        return 1;
    }

    if (memInc(address))
    {
        // fprintf(stderr, "Error: increasing the value of the address %d of the array with identifier %s failed\n", address, arrayName);    
        return 2;
    }

    return 0;
}


int decrease(const char *arrayName, int index)
{
    int address = fetchAddress(arrayName, index);
    if (address < 0)
    {
        // fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);  
        return 1;
    }

    if (memDec(address))
    {
        // fprintf(stderr, "Error: decreasing the value of the address %d of the array with identifier %s failed\n", address, arrayName);    
        return 2;
    }

    return 0;
}


int allocate(const char *arrayName, int length)
{
    if (length <= 0)
    {
        fprintf(stderr, "Error: invalid length %d of array\n", length);
        return 1;
    }

    // Check that _arrayName_ doesn't already exist
    if (checkArray(arrayName))
    {
        fprintf(stderr, "Error: array with identifier %s already exists\n", arrayName);
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
        fprintf(stderr, "Error: an error occured while allocating memory to store identifier %s\n", arrayName);
        return 4;
    }

    newElement->length = length;
    newElement->next = NULL;

    // Allocate space in memory for array and store its address
    if (memAlloc(length, &(newElement->address)))
    {
        free(newElement->arrayName);
        free(newElement);
        // fprintf(stderr, "Error: allocating memory for the array with identifier %s failed\n", arrayName);
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
    int address = fetchAddress(arrayName, index);
    if (address < 0)
    {
        // fprintf(stderr, "Error: fetching address of the array with identifier %s failed\n", arrayName);    
        return 1;
    }

    int val;
    if (memRead(address, &val))
    {
        // fprintf(stderr, "Error: reading the address %d of the array with identifier %s failed\n", address, arrayName);
        return 2;
    }

    printf("%d\n", val);

    return 0;
}


int add(const char *arrayName1, const char *arrayName2)
{
    if (dualArrayOperator(arrayName1, arrayName2, '+', 1))
    {
        return 1;
    }

    return 0;
}


int subtract(const char *arrayName1, const char *arrayName2)
{
    if (dualArrayOperator(arrayName1, arrayName2, '-', 1))
    {
        return 1;
    }

    return 0;
}


int multiply(const char *arrayName1, const char *arrayName2)
{
    if (dualArrayOperator(arrayName1, arrayName2, '*', 1))
    {
        return 1;
    }

    return 0;
}


int andArrays(const char *arrayName1, const char *arrayName2)
{
    if (dualArrayOperator(arrayName1, arrayName2, '&', 0))
    {
        return 1;
    }

    return 0;
}


int xorArrays(const char *arrayName1, const char *arrayName2)
{
    if (dualArrayOperator(arrayName1, arrayName2, '^', 0))
    {
        return 1;
    }

    return 0;
}


int freeArray(const char *arrayName)
{
    int addressAndLength[2];
    if (freeArrayName(arrayName, addressAndLength))
    {
        // fprintf(stderr, "Error: freeing array with identifier %s failed\n", arrayName);
        return 1;
    }

    if (memFreeBlock(addressAndLength[0], addressAndLength[1]))
    {
        // fprintf(stderr, "Error: freeing memory of array with identifier %s failed\n", arrayName);
        return 2;
    }

    return 0;
}


int printArray(const char *arrayName)
{
    Array *array = checkArray(arrayName);
    if (!array)
    {
        fprintf(stderr, "Try to use variable that does not exist.\n");
        // fprintf(stderr, "Error: no array with identifier %s exist\n", arrayName);
        return 1;
    }

    printf("[ ");
    for (int i = 0, n = array->length; i < n; i++)
    {
        int val;
        if (memRead(array->address + i, &val))
        {
            // fprintf(stderr, "Error: reading the address %d of the array with identifier %s failed\n", array->address + i, arrayName);
            return 2;
        }

        printf("%d ", val);
    }
    printf("]\n");
    
    return 0;
}

