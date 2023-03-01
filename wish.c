#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>

//handles the execution of the command given to the wish 
void execution(char *args[],int *i, char *path, int *size) {

	int ret;
	char temp[50];
	
	for(int j = 0; j < *size; j++) {
		strcpy(&temp[j], &path[j]); //handles the path combination
		strcat(&temp[j], args[0]);
		
		if (access(&temp[j], X_OK) == 0) {
				
			args[0] = &temp[j];
			
			args[*i] = NULL;
			
			if((ret = execv(args[0], args) == -1)){
				write(STDERR_FILENO,"wish: execv failed\n", strlen("wish: execv failed\n"));
				exit(1);
			}
			return;
		}
	}
	write(STDERR_FILENO,"wish: path not found\n", strlen("wish: path not found\n")); // returns error if no path was valid
	return;
}

void batchMode(FILE * file, char *path, int *pathNumb) {
	
	char *line = NULL;
	size_t len = 0; 
	pid_t pid;
	char *args[100] = {};
	char temp[50];
	
	while(getline(&line, &len, file) != -1) {
		int i = 0;
	
		if (line[0] == 35 || strlen(line) == 1){ 
			//continues to next if line is empty or first one is # as in (#! /bin/bash)
			continue;
		}
		
		int len = strlen(line);
		if (line[len-1] == '\n') {
			line[len-1] = 0;
		}
		
		char *cut = strtok(line, " ");
		
		while (cut != NULL) { // saves arguments to a list that can be used later
	
			args[i] = cut;
			i++;
			cut = strtok(NULL, " ");
		}
		 
		if(strcmp(args[0], "cd") == 0) { //changing directory 
			if(i == 2) {
				chdir(args[1]);
				continue;
			}
			write(STDERR_FILENO,"wish: cd problem\n", strlen("wish: cd problem\n"));
			
			continue;
		}
		if(strcmp(args[0], "path") == 0) { //changes the path
			for (int k = 0; k <= *pathNumb; k++) {
				path[k] = '\0';
			}
			pathNumb = 0;
			
			for (int k = 0; k < i--; k++) {
			
				strcpy(&temp[k], args[k+1]);
				strcat(&temp[k], "/");
							
				strcpy(&path[k], &temp[k]);

				pathNumb++;
			}
			continue;
		}
		
		pid = fork();
		
		if (pid == 0) {
			execution(args, &i, path, pathNumb);
			exit(0);
		}
		else if (pid < 0) {
			write(STDERR_FILENO,"wish: fork failed\n", strlen("wish: fork failed\n"));
			exit(1);
		}
		wait(NULL);
			
		
	}
	free(line);
		
	return;
}

int main(int argc, char *argv[]) {
	
	char *input = NULL;
	size_t len = 0;
	pid_t pid;
	char path[] = {"/bin/"};
	int pathNumb = 1;
	char *args[100] = {};
	char temp[50];

	if(argc > 2) {
		write(STDERR_FILENO,"wish: ./wish [file]\n", strlen("wish: ./wish [file]\n"));
		
		exit(1);
	}
	if (argc == 2) { //if a file is given it redirects to batchMode
		FILE * file = fopen(argv[1], "r");
		
		if (file == NULL) {
			write(STDERR_FILENO,"wish: cannot open file\n", strlen("wish: cannot open file\n"));
			
			exit(1);
		}
		
		batchMode(file, path, &pathNumb);
		fclose(file);
		return 0;
	}
	
	while (1) {
		int i = 0;
	
		printf("wish> ");
		getline(&input, &len, stdin);
		if (strcmp(input, "exit\n") == 0) { //handles the exit and empty line
			free(input);
			exit(0);
		}
		else if(feof(stdin)) {
			printf("\n");
			free(input);
			exit(0);
		}
		else if (input[0] == '\n') { 
			continue;
		}
		
		int len = strlen(input);
		if (input[len-1] == '\n') {
			input[len-1] = 0;
		}
		
		char *cut = strtok(input, " ");
		
		while (cut != NULL) { // handles the cutting of the lines
	
			args[i] = cut;
			i++;
			cut = strtok(NULL, " ");
			
		}
		 
		if(strcmp(args[0], "cd") == 0) { //directory change
			if(i == 2) {
				chdir(args[1]);
				continue;
			}
			write(STDERR_FILENO,"wish: cd problem\n", strlen("wish: cd problem\n"));
			continue;
		}
		
		if(strcmp(args[0], "path") == 0) { //changing path
			for (int k = 0; k <= pathNumb; k++) {
				path[k] = '\0';
			}
			pathNumb = 0;
			
			for (int k = 0; k < i--; k++) {
			
				strcpy(&temp[k], args[k+1]);
				strcat(&temp[k], "/");
							
				strcpy(&path[k], &temp[k]);

				pathNumb++;
			}
			continue;
		
		}
		
		pid = fork();
		
		if ( pid == 0) {
			execution(args, &i, path, &pathNumb);
			exit(0);
		}
		else if (pid < 0) {
			write(STDERR_FILENO,"wish: fork failed\n", strlen("wish: fork failed\n"));
			exit(1);
		}
		wait(NULL);
		
	}
	
	return(0);
}
