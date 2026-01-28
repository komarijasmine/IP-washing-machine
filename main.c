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
		int code = interpretLine(&memory, line);
		if (code != 0) continue; 
	}
	
	fclose(file);
	memFree(memory);

// the errors

}



