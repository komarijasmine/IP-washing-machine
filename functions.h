#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/* EFFECT: Initializes the memory. If not called before other functions, an error will be thrown 
OUTPUT: 0 upon successful execution of the function; 1 if memory initilazation failed */
int init(void);

/* EFFECT: Frees the memory allocated for all created arrays and all identifiers 
OUTPUT: 0 upon successful execution of the function; 1 if freeing memory failed*/
int freeAll(void);

/* EFFECT: Assigns the value _value_ to the first element of the array with identifier  _arrayName_ 
OUTPUT: 0 upon successful execution of the function; 1 if fetching memory address of the array with 
identifier _arrayName_ failed; 2 if writing to the address of the first element of the array with 
identifier _arrayName_ failed */
int assign(char *arrayName, int value);

/* EFFECT: Increases element with index _index_ of the array with identifier _arrayName_ by 1 
OUTPUT: 0 upon successful execution of the function; 1 if fetching memory address of the array with 
identifier _arrayName_ failed; 2 if increasing the value of the address of the first element of the 
array with identifier _arrayName_ failed */
int increase(char *arrayName, int index);

/* EFFECT: Decreases element with index _index_ of the array with identifier _arrayName_ by 1
OUTPUT: 0 upon successful execution of the function; 1 if fetching memory address of the array with 
identifier _arrayName_ failed; 2 if decreasing the value of the address of the first element of the 
array with identifier _arrayName_ failed */
int decrease(char *arrayName, int index);

/* EFFECT: Allocates memory for an array of lenght _length_ with identifier _arrayName_ 
OUTPUT: 0 upon successful execution of the function; 1 if creating a new element to store identifier 
_arrayName_ failed; 2 if allocating memory for the array failed */
int allocate(char *arrayName, int length);

/* EFFECT: Prints element with index _index_ of the array with identifier _arrayName_
OUTPUT: 0 upon successful execution of the function; 1 if fetching memory address of the array with 
identifier _arrayName_ failed; 2 if reading the value of the address of the first element of the 
array with identifier _arrayName_ failed  */
int printCell(char *arrayName, int index);

/* EFFECT: Adds the value of the first element of the array with identifier _arrayName1_ to the value
of the first element of the array with the identifier _arrayName2_ and writes the result to the first 
element of the array with identifier _arrayName1_ 
OUTPUT: 0 upon successful execution of the function; 1 if getting the values of the first elements of 
the arrays with identifiers _arrayName1_ and _arrayName2_ failed; 2 if writing to the first element 
of the array with identifier _arrayName1_ failed */
int add(char *arrayName1, char *arrayName2);

/* EFFECT: Subtracts the value of the first element of the array with identifier _arrayName2_ from the 
value of the first element of the array with the identifier _arrayName1_ and writes the result to the 
first element of the array with identifier _arrayName1_ 
OUTPUT: 0 upon successful execution of the function; 1 if getting the values of the first elements of 
the arrays with identifiers _arrayName1_ and _arrayName2_ failed; 2 if writing to the first element 
of the array with identifier _arrayName1_ failed */
int subtract(char *arrayName1, char *arrayName2);

/* EFFECT: Multiplies the value of the first element of the array with identifier _arrayName1_ with 
the value of the first element of the array with the identifier _arrayName2_ and writes the result 
to the first element of the array with identifier _arrayName1_ 
OUTPUT: 0 upon successful execution of the function; 1 if getting the values of the first elements of 
the arrays with identifiers _arrayName1_ and _arrayName2_ failed; 2 if writing to the first element 
of the array with identifier _arrayName1_ failed */
int multiply(char *arrayName1, char *arrayName2);

/* EFFECT: Performs the AND operation on the value of the first element of the array with identifier 
_arrayName1_ and the value of the first element of the array with identifier _arrayName2_ and writes
the result to the first element of the array with identifier _arrayName1_
OUTPUT: 0 upon successful execution of the function; 1 if getting the values of the first elements 
of the arrays with identifiers _arrayName1_ and _arrayName2_ failed; 2 if writing to the first 
element of the array with identifier _arrayName1_ failed */
int andCells(char *arrayName1, char *arrayName2);

/* EFFECT: Performs the XOR operation on the value of the first element of the array with identifier 
_arrayName1_ and the value of the first element of the array with identifier _arrayName2_ and writes
the result to the first element of the array with identifier _arrayName1_
OUTPUT: 0 upon successful execution of the function; 1 if getting the values of the first elements 
of the arrays with identifiers _arrayName1_ and _arrayName2_ failed; 2 if writing to the first 
element of the array with identifier _arrayName1_ failed */
int xorCells(char *arrayName1, char *arrayName2);

/* EFFCT: Frees the memory allocated for the array with identifier _arrayName_ and removes and frees 
the memory allocated for storing the identifier _arrayName_ 
OUTPUT: 0 upon successful execution of the function; 1 if removing and freeing the memory allocated 
for storing the identifier _arrayName_ failed; 2 if freeing the memory allocated for the array with 
identifier _arrayName_ failed */
int freeArray(char *arrayName);

/* EFFECT: Prints the contents of the array with identifier _arrayName_ in the form "[ x x x ]" 
(for this example, the array contains 3 values x) 
OUTPUT: 0 upon successful execution of the function; 1 if no array with identifier _arrayName_ exists;
2 if reading the contents of the array with identifier _arrayName_ failed */
int printArray(char *arrayName);

#endif
