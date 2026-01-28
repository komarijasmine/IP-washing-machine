#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpret.h"
#include "memory.h"

int main(void) {
	FILE* file = fopen("text.txt", "r");
	
	if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
		exit(0);
    }
	
	memInit();
	
	int max_length = 10;
	char line[max_length];
	
	while (fgets(line, sizeof(line), file)) {
        int flush = 0; 
        
        if (strchr(line, '\n') == NULL && !feof(file)) {
            flush = 1;
        }

        if (flush) {
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF) {
            }
            // line is too long, discards and moves on to the next line.
            continue;
        }

        // Remove newline for the interpreter
        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        // Skip empty lines
        if (line[0] == '\n' || line[0] == '\0') continue;

        // Interpret the valid line
        int code = interpretLine(line);
		
		// skips invalid line (missing parameter, too many parameters, or unknown commands)
		if (code != 0) {
			continue; 
		}
	}
	
	fclose(file);

	freeAll();
	memFree(); // cleanup function
    return 0;

// the errors

}





