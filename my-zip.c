#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rleWrite (FILE * file) { //handles the compression and writing it to output

	char cha = 0, temp = 0;
	int count = 0;
	
	while ((cha = fgetc(file)) != EOF) {
	
		if (cha == temp) {
			count++;
		}
		
		else {
			if (count != 0) {
				
				fwrite(&count,4,sizeof(count),stdout);
				fwrite(&temp,sizeof(temp),1,stdout);	
			}			
			
			temp = cha;
			count = 1;
			
		}	
	}
	temp = *"\n";
	
	fwrite(&count,4,sizeof(count),stdout);
	fwrite(&temp,sizeof(temp),1,stdout);
}



int main(int argc, char *argv[]) {
	
	FILE * file;
	
	if (argc == 1) {
		printf("my-zip: file1 [file2 ...]\n");
		exit(1);
	}
	for(int i = 1; i < argc; i++) {
		file = fopen(argv[i], "r");
		
		if (file == NULL) {
			printf("my-zip: cannot open file\n");
			exit(1);
		}
		
		rleWrite(file);
		fclose(file);
		
	}
}



