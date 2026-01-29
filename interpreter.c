#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h" 


int makeInt(const char* str, int* num);
int callCommand(const char* opName, const char* parameter1, const char* parameter2);
	
int interpretLine(char *line)
{	
	// Split the line into individual parts using ' ' as delimiter
	char* opName = strtok(line, " ");
	if (!opName)
	{
		// Empty line, thus skip
		return 0;
	}

	char* parameter1 = strtok(NULL, " ");
	if (!parameter1)
	{
		fprintf(stderr, "Error: no parameter supplied for operator %s\n", opName);
		return 1;
	}

	char* parameter2  = strtok(NULL, " ");

	if (strtok(NULL, " "))
	{
		fprintf(stderr, "Error: too many parameters supplied\n");
		return 2;
	}

	if (callCommand(opName, parameter1, parameter2))
	{
		return 3;
	}

	return 0;
}


int makeInt(const char* str, int* num)
{
	/* Local function 
    EFFECT: Turns a string containing numbers into an integer number of base-10
    OUTPUT: 0 upon successful execution; 1 if the string contains characters that are not numbers */

	char* endptr;
    *num = strtol(str, &endptr, 10);

	// If _endptr_ is a non-empty string, then _str_ must have contained non-number characters
    if (str == endptr || *endptr != '\0')
    {
        fprintf(stderr, "Error: invalid parameter %s. Must be a number\n", str);
        return 1;
    }

    return 0;
}


int callCommand(const char* opName, const char* parameter1, const char* parameter2)
{
	/* Local function 
    EFFECT: Calls a function based on _opName_ passing _parameter1_ and _parameter2_. 
	Checks whether correct amount of parameters have been passed.
    OUTPUT: 0 upon successful execution; 
	1 if an incorrect number of parameters for the function identified with _opName_ have been passed;
	2 if _parameter2_ could not be converted to a decimal number when this is necessary for the 
	function identified with _opName_; 
	3 if the function identified with _opName_ failed
	4 if no operator or an unknown operatoridentified with _opName_ has been supplied */

	if (!opName)
	{
	    fprintf(stderr, "Error: missing operator\n");
    	return 4;	
	}

	if (!strcmp(opName, "Ass")){
		if (!parameter2)
		{
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 2 parameters, but only 1 was supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 1 parameter, but 2 were supplied\n", opName);
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
            fprintf(stderr, "Error: operator %s requires 1 parameter, but 2 were supplied\n", opName);
			return 1;
		}

		if (printArray(parameter1))
		{
			return 3;
		}

        return 0;
	}

    fprintf(stderr, "Error: unknown operator %s\n", opName);
	return 4;
}


int initializeProgram(void)
{
	if (init())
	{
		fprintf(stderr, "Error: initializing program failed\n");
		return 1;
	}

	return 0;
}


int terminateProgram(void)
{
	if (freeAll())
	{
		fprintf(stderr, "Error: terminating program failed\n");
		return 1;
	}

	return 0;
}

