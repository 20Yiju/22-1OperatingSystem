#define MAX_CMD 2048
#define MAX_ARG 256
#include <stdio.h>
#include <string.h>

void ParseCommand(char *command, int *argc, char *argv[]);

int main() {
	char command[MAX_CMD];
	command[0] = command[MAX_CMD - 1] = 0;

	int argc = 0;
	char *argv[MAX_ARG] = { NULL };

	while(1) {
		printf("$ ");
		fgets(command, MAX_CMD - 1, stdin);
		command[strlen(command) - 1] = 0;

		if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0)
			break;

		ParseCommand(command, &argc, argv);

		printf("argc = %d\n", argc);
		for(int i = 0; i < argc;i++)
			printf("argv[%d] = %s\n", i, argv[i]);
		printf("argv[%d] = %p\n", argc, argv[argc]);
	}

	printf("Bye!\n");

	return 0;
}

void ParseCommand(char *command, int *argc, char *argv[]) {
	

	//previous code
	/*char *ptr = strtok(command, " ");
	int i = 0;

	while (ptr != NULL) {
		argv[i] = ptr;
		ptr = strtok(NULL, " ");
		i++;
	}
	*argc = i;*/

	//update code
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
