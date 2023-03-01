#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char buffer[500];
	
	if (argc == 1) {
		exit(0);	
	}
	
	for (int i = 1; i < argc; i++) { //handles all the files one by one
		FILE * file = fopen(argv[i], "r");
		if (file == NULL) {
			printf("my-cat: cannot open file\n");
			exit(1);
		}
		
		while(fgets(buffer, 500, file) != NULL) { //reads and prints the lines in the file
			printf("%s", buffer);
		}
		
		fclose(file);
	}
	return 0;
}
