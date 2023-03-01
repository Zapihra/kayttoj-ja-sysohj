#include <stdio.h>
#include <stdlib.h>

void rleRead(FILE *file) { //handles the reading and printing of the file
	int count[20];
	char cha[2];	
	int elem;
	
	while(!feof(file)) { //help with condition of file loop from https://github.com/Abdullah296/ostep-projects/tree/master/initial-utilities/wunzip
		elem = fread(&count, 4, 4, file);
		if (elem != 4) {
			break;
		}
		elem = fread(&cha, 1, 1, file);
		if (elem != 1) {
			break;
		}
		if(*cha == '\n') {
			printf("\n");
			continue;
		}
		for (int i = 0; i < *count; i++) {
			fprintf(stdout, "%c", *cha);
		}
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	FILE *file;
	
	if (argc == 1) {
		printf("my-unzip: file1 [file2 ...]\n");
		exit(1);
	}
	for(int i = 1; i < argc; i++) { // handles the multiple files
		file = fopen(argv[i], "r");
		
		if (file == NULL) {
			printf("my-unzip: cannot open file\n");
			exit(1);
		}
		
		rleRead(file);
		
		fclose(file);
	}	

}
