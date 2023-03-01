#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct list {
	char line[2000];
	struct list *next;
	struct list *prev;
}list;

list * readLine(char *input, list * start);
int writeFile(char *output, list *start);
void * freeMemory(list *start);

int main(int argc, char *argv[]) {
	char input[50];
	char output[50];
	list *start = NULL;

	if (argc == 2) { //handles the input and outputfiles
		strcpy(input, argv[1]);
	}
	else if (argc == 3) {
		strcpy(input, argv[1]);
		strcpy(output, argv[2]);
	}
	else if (argc >= 4) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		exit(1);
	}

	if (strcmp(input, output) == 0 && input[0] != '\0') {
		fprintf(stderr, "Input and output file must differ\n");
		exit(1);
	}

	start = readLine(input, start);
	writeFile(output, start);

	freeMemory(start);
	return 0;
}

list * readLine(char *input, list * start){ //handles reading the given input and saving it to the linked list

	char *liner = NULL;
	size_t len = 0;
	list *new, *point;

	if (input[0] != '\0' ) {
		FILE * file;
		file = fopen(input, "r");

		if(file == NULL) {
			fprintf(stderr, "error: cannot open file '%s'\n", input);
			exit(1);
		}
		printf("File name: %s\n", input);
		while(getline(&liner, &len, file) != -1) { //file input
			
			if((new = (list*)malloc(sizeof(list))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				exit(1);
			}
			
			strcpy(new->line, liner); 
			new->next = NULL;
			new->prev = NULL;
			
			if (start == NULL) {
				start = new;
				point = start;	
			}
			
			else {
				point-> next = new;
				new->prev = point;
						
				point = point->next;	
			}
			
		}
		
		fclose(file);	
	}
	else{//stdin input
		while(getline(&liner, &len, stdin) != -1) {
		
			if((new = (list*)malloc(sizeof(list))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				exit(1);
			}
			
			strcpy(new->line, liner); 
			new->next = NULL;
			new->prev = NULL;
			
			if (start == NULL) {
				start = new;
				point = start;	
			}
			
			else {
				point-> next = new;
				new->prev = point;	
				point = point->next;	
			}	
		}
	
	}
	
	return start;
}


int writeFile(char *output, list *start){ //handles the reverse output
	list *point;
	point = start;

	while (point->next != NULL) {
		point = point->next;
	}

	if (output[0] != '\0') { //output file
		FILE * file;
                file = fopen(output, "w");

                if(file == NULL) {
                        fprintf(stderr, "error: cannot open file '%s'\n", output);
                        exit(1);
                }
		while (point != NULL) {
			fprintf(file, "%s", point->line);
			point = point->prev;
		}
		fclose(file);
	}
	else { //output in stdout
		while (point != NULL) {
			fprintf(stdout, "%s", point->line);
			point = point->prev;
		}
	}
	return 0;
}


void *freeMemory(list *start) {
	list *point = start;

	while (point != NULL) {
		start = point->next;
		free(point);
		point = start;
	}
	return 0;
}
