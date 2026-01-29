#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interpreter.h"

#define MAX_LENGTH 20

int main(void) 
{
	FILE* file = fopen("test.txt", "r");
	
	if (file == NULL) 
	{
        fprintf(stderr, "Error opening file\n");
		return 1;
    }
	
	if (initializeProgram())
	{
		return 2;
	}

	int line_number = 1;
	char line[MAX_LENGTH];
	
	while (fgets(line, sizeof(line), file)) 
	{
        int flush = 0;

		// Check whether more character exist on line
        if (strchr(line, '\n') == NULL && !feof(file)) 
		{
            flush = 1;
        }

        if (flush) 
		{		
			// Remove all whitespace characters left in the line. If not a whitespace, terminate program
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) 
			{
				if (!isspace(c))
				{
					fprintf(stderr, "Line %d: Line too long\n", line_number);
					terminateProgram();
					return 3;
				}	
            }
        }

        // Remove newline for the interpreter
        char *nl = strchr(line, '\n');
        if (nl) 
		{
			*nl = '\0';
		}
		
        // Skip empty lines
        if (line[0] == '\0')
		{
            line_number++;
			continue;
		}

        // Interpret the valid line
        if (interpretLine(line))
		{
			terminateProgram();
			return 4;
		}

		line_number++;
	}

	// Close file and free all memory
	fclose(file);
	if (terminateProgram())
	{
		return 5;
	}

    return 0;
}





