#ifndef VARIABLES_H
#define VARIABLES_H
#define ALPHABET 26

typedef struct variables {
    char name; // one-character name of the variable
    int start; // start position in memory array
    int length; // size in memory
    int use; // 1 if name is in use, 0 if free
} Variable;

extern Variable var[ALPHABET];

/* Initializes the members of the Variable array var */
void varInit(void);

/* Creates a variable named name, with memory start position start and with size of length */
void varCreate(char name, int start, int length);

/* Returns the index of the variable in the variable array */
int varIndex(char name);

/* Returns 1 if the variable name is in use, 0 if unused. Returns error if invalid name */
int varCheck(char name);

/* Frees the variable from the variable array (NOT from memory) */
int varFree(char name);

#endif
