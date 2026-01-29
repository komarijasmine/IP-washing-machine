#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "interpreter.h"

#define MAX_LENGTH 20

int formatLine(FILE *file, char *line);
int readFile(FILE *file);
int runProgram(FILE *file);

int formatLine(FILE *file, char *line)
{
    /* EFFECT: Formats _line_ into a usable string by trimming the end and checking whether it is 
    not longer than _MAX_LENGTH_ characters
    OUTPUT: 0 upon successful execution; 1 if the string is longer than _MAX_LENGTH_ characters */

    int flush = 0;

    // Check whether more character exist on line
    if (!strchr(line, '\n')) 
    {
        flush = 1;
    }

    if (flush) 
    {		
        // Remove all whitespace characters left in the line. If not a whitespace, terminate program
        int c;
        while ((c = fgetc(file)) != '\n' && c != EOF) 
        {
            if (!isspace((unsigned char) c))
            {
                return 1;
            }	
        }
    }

    // Remove newline for the interpreter
    char *nl = strchr(line, '\n');
    if (nl) 
    {
        *nl = '\0';
    }

    return 0;
}


int readFile(FILE *file)
{
    /* EFFECT: Reads, interprets, and executes lines from _file_
    OUTPUT: 0 upon successful execution; 1 if the string is longer than _MAX_LENGTH_ characters
    2 if interpreting and executing a line failed */

	int line_number = 1;
	char line[MAX_LENGTH + 1];
	
	while (fgets(line, sizeof(line), file)) 
	{
        if (formatLine(file, line))
        {
                fprintf(stderr, "Error: line %d: invalid line. Too many characters\n", line_number);
                return 1;
        }
		
        if (interpretLine(line))
        {
            fprintf(stderr, "Error: line %d: a fatal error occurred\n", line_number);
            return 2;
        }

		line_number++;
	}

    return 0;
}


int runProgram(FILE *file)
{
    /* EFFECT: Initializes program; executes program line-by-line as described in _file_, with each
    line formatted as described in interpreter.h; terminates program by closing file and freeing 
    all allocated memory
    OUTPUT: 0 upon successful execution; 1 if opening _file_ failed;
    2 if initializing the program failed; 
    3 if reading, interpreting, or executing the lines of _file_ failed;
    4 if closing _file_ failed; 5 if freeing memory failed */

    if (file == NULL) 
	{
        fprintf(stderr, "Error: opening file failed\n");
		return 1;
    }
	if (initializeProgram())
	{
		return 2;
	}

    // Read, interpret, and execute each line of _file_
    if (readFile(file))
    {
        terminateProgram();
        return 3;
    }

	// Close file and free all memory
	if (fclose(file))
    {
        terminateProgram();
        return 4;
    }
	if (terminateProgram())
	{
		return 5;
	}

    return 0;
}


int main(void) 
{
    /* EFFECT: Reads, interprets, and executes lines in the format as described in interpreter.h
    OUTPUT: 0 upon successful execution; 1 if an error occurred while executing */

	FILE* file = fopen("test.txt", "r");
	if (runProgram(file))
    {
        return 1;
    }

    return 0;
}





