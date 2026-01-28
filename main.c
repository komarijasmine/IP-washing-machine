#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

int main(void) 
{
	FILE* file = fopen("text.txt", "r");
	
	if (file == NULL) 
	{
        fprintf(stderr, "Error opening file.\n");
		exit(0);
    }
	
	int max_length = 10;
	char line[max_length];
	
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
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) 
			{
				if (c != ' ')
				{
					return 1;
				}	
            }
        }

        // Remove newline for the interpreter
        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') continue;

        // Interpret the valid line
        if (interpretLine(line))
		{
			return 2;
		}
	}

	fclose(file);

    return 0;
}





