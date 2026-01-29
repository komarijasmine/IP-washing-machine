#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h" 


int makeInt(char* str, int* num);
int callCommand(char* opName, char* paramater1, char* paramater2);
	
int interpretLine(char *line)
{	
	// Split the line into individual parts using ' ' as delimiter
	char* opName = strtok(line, " ");
	char* parameter1 = strtok(NULL, " ");
	char* parameter2  = strtok(NULL, " ");

	if (strtok(NULL, " "))
	{
		fprintf(stderr, "Command contains too many paramater\n");
		return 1;
	}

	if (callCommand(opName, parameter1, parameter2))
	{
		return 2;
	}

	return 0;
}


int makeInt(char* str, int* num)
{
	/* Local function 
    EFFECT: Turns a string containing numbers into an integer number of base-10
    OUTPUT: 0 upon successful execution; 1 if the string contains characters that are not numbers */

	char* endptr;
    *num = strtol(str, &endptr, 10);

	// If _endptr_ is a non-empty string, then _str_ must have contained non-number characters
    if (*endptr != '\0')
    {
        fprintf(stderr, "Invalid paramater %s. Must be a number\n", str);
        return 1;
    }

    return 0;
}


int callCommand(char* opName, char* parameter1, char* parameter2)
{
	/* Local function 
    EFFECT: Calls a function based on _opName_ passing _parameter1_ and _paramater2_. 
	Checks whether correct amount of parameters have been passed.
    OUTPUT: 0 upon successful execution; 
	1 if an incorrect number of paramters for the function identified with _opName_ has been passed;
	2 if _parameter2_ could not be converted to a decimal number when this is necessary for the 
	function identified with _opName_; 
	3 if the function identified with _opName_ failed */

	if (!strcmp(opName, "Ass")){
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		int num;
		if (makeInt(parameter2, &num))
		{
			return 2;
		}

		if (assign(parameter1, num))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Inc"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		int index;
		if (makeInt(parameter2, &index))
		{
			return 2;
		}

		if (increase(parameter1, index))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Dec"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		int index;
		if (makeInt(parameter2, &index))
		{
			return 2;
		}

		if (decrease(parameter1, index))
		{
			return 3;
		}

        return 0;
	}			
	else if (!strcmp(opName, "Mal"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		int length;
		if (makeInt(parameter2, &length))
		{
			return 2;
		}

		if (allocate(parameter1, length))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Pri"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		int index;
		if (makeInt(parameter2, &index))
		{
			return 2;
		}

		if (printCell(parameter1, index))
		{
			return 3;
		}

        return 0;
    }
    else if (!strcmp(opName, "Add"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		if (add(parameter1, parameter2))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Sub"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		if (subtract(parameter1, parameter2))
		{
			return 3;
		}

        return 0;
	}	
	else if (!strcmp(opName, "Mul"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		if (multiply(parameter1, parameter2))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "And"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		if (andCells(parameter1, parameter2))
		{
			return 3;
		}

        return 0;
	}	
	else if (!strcmp(opName, "Xor"))
	{
		if (!parameter2)
		{
            fprintf(stderr, "Command %s requires 2 parameters, but only 1 was supplied\n", opName);
			return 1;
		}

		if (xorCells(parameter1, parameter2))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Fre"))
	{
		if (parameter2)
		{
            fprintf(stderr, "Command %s requires 1 parameters, but 2 were supplied\n", opName);
			return 1;
		}

		if (freeArray(parameter1))
		{
			return 3;
		}

        return 0;
	}
	else if (!strcmp(opName, "Pra"))
	{
		if (parameter2)
		{
            fprintf(stderr, "Command %s requires 1 parameters, but 2 were supplied\n", opName);
			return 1;
		}

		if (printArray(parameter1))
		{
			return 3;
		}

        return 0;
	}

    fprintf(stderr, "Unknown command %s\n", opName);
	return 4;
}


int initializeProgram(void)
{
	if (init())
	{
		fprintf(stderr, "Initializing program failed\n");
		return 1;
	}

	return 0;
}


int terminateProgram(void)
{
	if (freeAll())
	{
		fprintf(stderr, "Terminating program failed\n");
		return 1;
	}

	return 0;
}

