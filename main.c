#include <stdio.h>
#include <stdlib.h>

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

	char line[256];
	
	while (fgets(line, sizeof(line), file))  {
		
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
	memFree(memory);

// the errors

}




