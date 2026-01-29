#ifndef INTERPRETER_H
#define INTERPRETER_H

/* EFFECT: Interprets line with format "{Operator} {paramater1} {parameter2}" (note the whitespace 
as delimiter), where parameter2 is optional based on the chosen operator. Executes the chosen 
operater with supplied parameters.

The possible operators are:
Ass {string arrayName} {int number} - assigns _number_ to the first element of the array with 
identifier _arrayName_
Inc {string arrayName} {int index} - increase by 1 the element with index _index_ of the array 
with identifier _arrayName_
Dec {string arrayName} {int index} - decrease by 1 the element with index _index_ of the array 
with identifier _arrayName_
Mal {string arrayName} {int length} - allocates memory for an array of length _length_ with 
identifier _arrayName_
Pri {string arrayName} {int index} - print the value of the element with index _index_ of the 
array with identifier _arrayName_
Add {string arrayName1} {string arrayName2} - add the value of the first element of the array with 
identifier _arrayName1_ to the value of the first element of the array with identifier _arrayName2_ 
and store the result in the first element of _arrayName1_
Sub {string arrayName1} {string arrayName2} - subtract the value of the first element of the array 
with identifier _arrayName1_ from the value of the first element of the array with identifier 
_arrayName2_ and store the result in the first element of _arrayName1_
Mul {string arrayName1} {string arrayName2} - multiply the value of the first element of the array 
with identifier _arrayName1_ with the value of the first element of the array with identifier 
_arrayName2_ and store the result in the first element of _arrayName1_
And {string arrayName1} {string arrayName2} - apply the point-wise AND operation on the value of 
the first element of the array with identifier _arrayName1_ and the value of the first element of 
the array with identifier _arrayName2_ and store the result in the first element of _arrayName1_
Xor {string arrayName1} {string arrayName2} - apply the point-wise XOR operation on the value of 
the first element of the array with identifier _arrayName1_ and the value of the first element of
 the array with identifier _arrayName2_ and store the result in the first element of _arrayName1_
Fre {string arrayName} - free the space allocated for the array with identifier _arrayName_
Pra {string arrayName} - print the content of the array with identifier _arrayName_ in the form 
"[ x x x ]" (for an array of length 3, where x is a number)

OUTPUT: 0 upon successful execution of the function; 1 if more than 2 parameters were supplied; 
2 if executing the operator failed */
int interpretLine(char* line);

/* EFFECT: Initializes the program. Needs to be called before any other function
OUTPUT: 0 upon successful execution of the function; 1 if initialization failed */
int initializeProgram(void);

/* EFFECT: Frees all memory allocated for the arrays and storing array identifiers
OUTPUT: 0 upon successful execution of the function; 1 if freeing memory failed */
int terminateProgram(void);

#endif
