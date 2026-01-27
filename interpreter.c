#include <stdio.h>
#include <string.h>
#include "functions.h"
#include "memory.h"

int interpretLine(Memory *m, char *tmp)
{
    char instruction[] = tmp;

    char *command = strtok(instruction, " ");  
    char *parameter1 = strtok(NULL, " ");
    char *parameter2 = strtok(NULL, " ");

    if (strcmp(command, "Ass") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        assign(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Inc") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        increase(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Dec") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        decrease(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Mal") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        allocate(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Pri") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        printCell(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Add") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        add(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Sub") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        subtract(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Mul") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        multiply(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "And") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        andCells(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Xor") == 0)
    {
        if (parameter2 == NULL)
        {
            return 1;
        }
        xorCells(m, *parameter1, *parameter2);
        return 0;
    }
    else if (strcmp(command, "Fre") == 0)
    {
        if (parameter2 != NULL)
        {
            return 2;
        }
        freeArray(m, *parameter1);
        return 0;
    }
    else if (strcmp(command, "Pra") == 0)
    {
        if (parameter2 != NULL)
        {
            return 2;
        }
        printArray(m, *parameter1);
        return 0;
    }
    else
    {
        return 3;
    }
}