#ifndef FUNCTIONS_H
#define FUNCTIONS_H

<<<<<<< HEAD
typedef struct Array;

/* EFFECT: Initializes the memory. If not called before other functions, an error will be thrown 
OUTPUT: 0 upon success; 1 if memory initilazation failed */
int initializeMemory(void);

/* EFFECT: Frees the memory allocated for all created arrays and all identifiers 
OUTPUT: 0 upon success; 1 if freeing memory failed*/
int freeAll(void);

/* Assigns the value _value_ to the first element of the array identified with _arrayName_ */
int assign(char arrayName, int value);

/* Increases element _index_ of the array identified with _arrayName_ by 1 */
int increase(char arrayName, int index);

/* Decreases element _index_ of the array identified with _arrayName_ by 1 */
int decrease(char arrayName, int index);

/* Allocates memory for an array of lenght _length_ with the identifier _arrayName_ */
int allocate(char arrayName, int length);

/* Prints element _index_ of the array with the identifier _arrayName_ */
int printCell(char arrayName, int index);

/* Adds the value of the first element of the array with the identifier _arrayName2_ to the value of
the first element of the array with the identifier _arrayName1_ */
int add(char arrayName1, char arrayName2);

/* Subtracts the value of the first element of the array with the identifier _arrayName2_ to the 
value of the first element of the array with the identifier _arrayName1_ */
int subtract(char arrayName1, char arrayName2);

/* Multiplies the value of the first element of the array with the identifier _arrayName2_ to the 
value of the first element of the array with the identifier _arrayName1_ */
int multiply(char arrayName1, char arrayName2);

/* Performs the and operation on the value of the first element of the array with the identifier 
_arrayName1_ and the value of the first element of the array with the identifier _arrayName2_ and 
outputs the value to the first element of the array with the identifier _arrayName1_ */
int andCells(char arrayName1, char arrayName2);

/* Performs the xor operation on the value of the first element of the array with the identifier 
_arrayName1_ and the value of the first element of the array with the identifier _arrayName2_ and 
outputs the value to the first element of the array with the identifier _arrayName1_ */
int xorCells(char arrayName1, char arrayName2);

/* Frees the memory allocated for the array with the identifier _arrayName_ and removes the 
identifier _arrayName_ */
int freeArray(char arrayName);

/* Prints the contents of the array with the identifier _arrayName_ in the form "[ x x x ]" */
int printArray(char arrayName);
=======
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
>>>>>>> 9679122515220888bf85d4da35d0e118fb874400

#endif
