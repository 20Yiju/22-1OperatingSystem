//My previous solution was perfect, so I didn't modify it!
/*

	This program reads text lines and converts them into uppercase using a child process.
	Parent) Repeat until the user types "quit":
		- Read a message from the user.
		- Send the message to the child through a pipe.
		- Receive the converted message from the child through another pipe.
		- Display the converted message.

	Child) Repeat until the child cannot read from the pipe any more:
		- Receive a message from the parent through a pipe.
		- Convert all lowercase characters in the message into uppercase characters.
		- Send the converted message to the parent through another pipe.

	Complete the programming following the instructions.


	$ ./a.out
	Input messages and child will convert it to uppercase. Type "quit" to quit.
	hello							// user's input
	[parent] in_mesg = [HELLO]
	nice to see you					// user's input
	[parent] in_mesg = [NICE TO SEE YOU]
	Welcome to HGU!					// user's input
	[parent] in_mesg = [WELCOME TO HGU!]
									// the user typed just Enter
	[parent] in_mesg = []
	quit							// user's input
	Terminating parent.
	Terminating child.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <errno.h>

#define MAX_LEN 256

#define READ_END 0
#define WRITE_END 1


// DO NOT declare global variables

void Parent(int out, int in);
void Child(int in, int out);

int main()
// TO DO: The main function contains bugs. Find and fix ALL bugs.
//      (Changing correct statements will be penalized.)
{
	printf("Input messages and child will convert it to uppercase. Type \"quit\" to quit.\n");


	// the following six lines should be moved before fork()
	int p2c[2] = { 0 };		// pipe from parent to child
	int c2p[2] = { 0 };		// pipe from child to parent
	if(pipe(p2c) < 0 || pipe(c2p) < 0){
		fprintf(stderr, "Failed to create pipe.\n");
		return -1;
	}

	pid_t child_pid = fork();

	if(child_pid < 0){
		fprintf(stderr, "Failed to create process.\n");
		return -1;

	} else if(child_pid > 0){
		close(p2c[READ_END]);
		close(c2p[WRITE_END]);

		Parent(p2c[WRITE_END], c2p[READ_END]);	// DO NOT modify this line.
		printf("Terminating parent.\n");		// This line MUST be displayed.

	} else {	
		close(p2c[WRITE_END]);
		close(c2p[READ_END]);
		
		Child(p2c[READ_END], c2p[WRITE_END]);	// DO NOT modify this line.
		printf("Terminating child.\n");			// This line MUST be displayed. (IMPORTANT!)
	}

	return 0;
}

void Parent(int out, int in)
{
	char out_mesg[MAX_LEN] = "";
	char in_mesg[MAX_LEN] = "";
	while(1){
		// TO DO:
		//	Read a text line (out_mesg) until the enter key using fgets().
		//	If the input text is "quit", break the loop.
		//		DO NOT send "quit" to the child.
		//	Send the input text to child through pipe.
		//	Receive the converted text (in_mesg) from the child through pipe.
		//	Display the converted text.
		//		Put a prefix "[parent]" before the message to indicate it was printed from the parent.
		fgets(out_mesg, MAX_LEN, stdin);
		out_mesg[strlen(out_mesg) -1] = '\0';

		
		if(strcmp(out_mesg, "quit") == 0){
			break;
		}

		write(out, out_mesg, MAX_LEN);
		read(in, in_mesg, MAX_LEN);
		printf("[parent] in_mesg = [%s]\n", in_mesg);

	}
	close(in);
	close(out);
	// TO DO: close the pipes
}

void Child(int in, int out)
{
	char mesg[MAX_LEN] = "";
	while(1){
		// TO DO:
		//	Read a message from the parent through a pipe.
		//		On failure, break the loop.
		//		Hint) If all ends of a pipe were closed, read() from the pipe returns zero.
		//	Convert all lowercase characters in the message into the uppercase characters.
		//		Use the 'int toupper(int ch);' function declared in ctype.h.
		//	Send the converted message to the parent through another pipe.
		//	DO NOT display any message in the child.
		int end = read(in, mesg, MAX_LEN);
		if(end == 0){
			break;
		}
		for(int i = 0; i < strlen(mesg); i++){
			mesg[i] = toupper(mesg[i]);
		}

		write(out, mesg, MAX_LEN);
	}

	// TO DO: Close the pipes.	
	close(in);
	close(out);
}
