#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	FILE* file = fopen("text.txt", "r");
	
	if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
		exit(0);
    }
	
	Memory *memory = NULL;
	int st = memInit(&memory);
	if (!st) {
		fprintf(stderr, "Not enough memory.\n");
		fclose(file);
		exit(0);
	}

	int flush = 1;
	int max_length = 10;
	char line[max_length];
	
	while (fgets(line, sizeof(line), file))  {
		for (int i = 0; i < max_length; i++) {
			if (line[i] == '\n') {
				flush = 0;
				break;
			}
		}

		if (flush) {
			char c;
			while (1)
			{
				c = fgets(c, sizeof(c), file);
				if (c == '\n') {
					break;
				}
				else if (c != ' ') {
					// cleanup function
					return 2;
				}
			}
		}
		
		// skips an empty line
		if (line[0] == '\n' || line[0] == '\0') {
			continue;
		}
		
		int code = interpretLine(&memory, line);
		
		// skips invalid line (missing parameter, too many parameters, or unknown commands)
		if (code != 0) {
			continue; 
		}
	}
	
	fclose(file);
	memFree(memory); // cleanup function

// the errors

}



