#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int main (int argc, char *argv[]) {

	char *line = NULL;
	size_t len = 0;
	char *pointFound;
	char buffer[200];
	
		
	if (argc == 1) {
		printf("my-grep: searchterm [file ...]\n");
		exit(1);
	}
	else if (argc == 2) {//handling stdin if no file is given and it's mached lines 
		while(fgets(buffer, 200, stdin) != NULL) {
			pointFound = strstr(buffer, argv[1]);
			if (pointFound != NULL) {
				printf("%s", buffer);
			}
		}
	}
	else {
		for (int i = 2; i < argc; i++) { //handles multiple files
			FILE * file = fopen(argv[i], "r");
			
			if (file == NULL) {
				printf("my-grep: cannot open file\n");
				exit(1);
			}
			//prints the files that mached with the wanted element
			while (getline(&line, &len, file) != -1) {
				pointFound = strstr(line, argv[1]);
				if (pointFound != NULL) {
					printf("%s", line);
				}
				
			}
			
			fclose(file);
		
		}
	}
	return 0;
}


