//My previous solution was perfect, so I didn't modify it!
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAX_COMMAND 512

void ParseCommand(char *command, int *argc, char *argv[]);

int main() {
	char cwd[1024];
	char command[MAX_COMMAND];
	int argc = 0;
	char *argv[256] = {NULL };
	pid_t child_pid;
	int check = 0;
	int status;
	int num = 1;

	printf("Welcome to my_shell!!:D\n");

	while(1){
		check = 0;
		getcwd(cwd, sizeof(cwd));
		printf("my_shell %s > ", cwd);
		fgets(command, MAX_COMMAND, stdin);
		command[strlen(command) - 1] = 0;

		ParseCommand(command, &argc, argv);

		if(argc == 0){
			continue;
		}
		
		else if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0){
			break;
		}
		else if(strcmp(command, "cd") == 0) {
			int result = chdir(argv[1]);

			if( result == -1){
				printf("change directory fail...\n");
			}
		}
		else {
			if(strcmp(argv[argc-1], "&") == 0){
				argc = argc - 1;
				argv[argc] = NULL;
				check = 1;
			}
			
			child_pid = fork();
			
			if(child_pid < 0){
				fprintf(stderr, "fork filed\n");
				exit(1);
			}

			else if(child_pid > 0){
				if(check == 1){
					waitpid(WNOHANG, &status, 0);
				}
				else{
					waitpid(child_pid, &status, 0);
				}
			}
			else{

				int res = execvp(argv[0], argv);
				if(res == -1) {
					printf("Cannot Execute Command...\n");
					exit(1);
				}
				else{
					exit(1);
				}

			}

	
		}

	}
	printf("GOOD BYE~\n");
	return 0;
}

void ParseCommand(char *command, int *argc, char *argv[]) {

	char *ptr = strtok(command, " ");
	int i = 0;

	while (ptr != NULL) {
		argv[i] = ptr;
		ptr = strtok(NULL, " ");
		i++;
	}
	argv[i] = NULL;
	*argc = i;
	
}
