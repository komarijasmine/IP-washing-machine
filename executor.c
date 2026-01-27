#include "parser.h"
#include "variables.h"
#include "memory.h"

extern Memory * mem;

static Variable *getVar(char name) {
    int index = varIndex(name);

    /* invalid name or not in use */
    if (index < 0 || var[index].use == 0) {
        fprintf(stderr, "Try to use a variable that does not exist.\n");
        exit(0);
    }

    return &var[index];
}

int execCommand(Command *cmd) {
    int variable = varCheck(cmd->var1);
    int length = cmd->length;
    int * value;
    int * value2;
    int * start;
    int valid;
    int x, y;
    int index = varIndex(cmd->var1);
    int index2 = varIndex(cmd->var2);


    switch(cmd->type) {

        case MAL:
            if (variable) {
                fprintf(stderr, "Try to use a variable that does not exist.");
            }
            int valid = memAlloc(&mem, length, &start);

            if (!valid) {
                fprintf(stderr, "Not Enough Memory.");
            }
            varCreate(cmd->var, &start, length);
            break;

        case ASS:
            if (variable == 0) {
                valid = memAlloc(&mem, length, &start);
                if (!valid) {
                    fprintf(stderr, "Not Enough Memory.");
                }
            }
            x = getVar(cmd->var);
            valid = memWrite(&mem, x->start, x->.length, x->start, cmd->value);
            if (!valid) {
                fprintf(stderr, "Wrong Memory Access.");
            }

            // get the position of x (if not, allocate x in memory)
            // write the value in the first cell of x (in memory)
            break;
    
        case INC:
           if (variable == 0) {
                valid = memAlloc(&mem, length, *start);
                if (!valid) {
                    fprintf(stderr, "Not Enough Memory.");
                }
            }
            x = getVar(cmd->var);
            valid = memInc(&mem, x->start, x->length, valid);
            if (!valid) {
                fprintf(stderr, "Wrong Memory Access.");
            }
            // check if x is in use (if not, allocate x in memory)
            // then call the memory function
 
            break;

        case DEC:
            if (variable == 0) {
                valid = memAlloc(&mem, length, &start);
                if (!valid) {
                    fprintf(stderr, "Not Enough Memory.");
                }
            }

            x = getVar(cmd->var)

            valid = memDec(&mem, x->start, x->length, valid);
            if (!valid) {
                fprintf(stderr, "Wrong Memory Access.");
            }

            // check if x is in use (if not, allocate x in memory)
            // then call the memory function MemDec
            break;
    

        case PRI:
            if (cmd->length > var[index].length) {
                fprintf(stderr, "Wrong Memory Access.");
            }
            x = getVar(cmd->var);
            int i = 0; 
            valid = memRead(&mem, x->start, x->length, i, &value);
            if (!valid) {
                fprintf(stderr, "Wrong Memory Access.");
            }
            printf("%d\n", value);

            // find variable in variable array. if n > length return error message
            // print the content of the memory cell of (start + n)
            break;

        case ADD:
            x = getVar(cmd->var);
            y = getVar(cmd->var2);

            if (!memRead(&mem, x->start, x->length, 0, &value)) {
                fprintf(stderr,"Wrong Memory Access.");
            }

            if (!memRead(&mem, y->start, y->length, 0, &value2)) {
                fprintf(stderr, "Wrong Memory Access.");
            }

            memWrite(&mem, x->start, x->length, 0, value + value2);

            // find variable x[start]
            // find variable y[start]
            // add content together
            // store in x[start]
            break;

        case SUB:
            x = getVar(cmd->var);
            y = getVar(cmd->var2);

            memRead(&mem, x->start, x->length, 0, &value);
            memRead(&mem, y->start, y->length, 0, &value2);
            memWrite(&mem, x->start, x->length, 0, value - value2);

            // find variable x[start]
            // find variable y[start]
            // subtract y[start] from x[start]
            // store in x[start]
            break;

        case MUL:
            x = getVar(cmd->var);
            y = getVar(cmd->var2);

            memRead(&mem, x->start, x->length, 0, &value);
            memRead(&mem, y->start, y->length, 0, &value2);
            memWrite(&mem, x->start, x->length, 0, value * value2);

            // find variable x[start]
            // find variable y[start]
            // multiply them together
            // store in x[start]
            break;

        case AND:
            y = getVar(cmd->var2);
            if (x->length != y->length) {
                printf(stderr, "Logic operation between sequences of different length.");
            }

            for (int i = 0; i < x->length; i++) {
                int a, b;
                memRead(&mem, x->start, x->length, i, &a);
                memRead(&mem, y->start, y->length, i, &b);
                memWrite(&mem, x->start, x->length, i, (a * b) % 2);
            }

            // check if x.length != y.length
                // return error message
            // perform operation with for loop
                // (x[i] * y[i]) % 2
            // store in x cells
            break;

        case XOR:
            y = getVar(cmd->var2);
            if (x->length != y->length) {
                printf(stderr, "Logic operation between sequences of different length.");
            }

            for (int i = 0; i < x->length; i++) {
                int a, b;
                memRead(&mem, x->start, x->length, i, &a);
                memRead(&mem, y->start, y->length, i, &b);
                memWrite(&mem, x->start, x->length, i, (a + b) % 2);
            }

            // check if x.length != y.length
                // return error message
            // perform operation with for loop
                // (x[i] + y[i]) % 2
            // store in x
            break;

        case FRE:
            x = getVar(cmd->var);

            memFreeBlock(&mem, x->start, x->length);
            varFree(cmd->var);
            
            // check if x is used: if not, return error message
            // free variable
            // free memory
            break;

        case PRA:
            int content;
            printf("[ ");
            for (int i = 0; i < length; i++) {
                content = memRead(&mem, var[index].start, var[index].length, i, &value);
                printf("%d ", &value);
            }
            printf("]");
            break;

    }
    return 1;
}

