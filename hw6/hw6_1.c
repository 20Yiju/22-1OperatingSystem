#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MesgQueueParent2Child 1024	// replace this number with an arbitrary number
#define MesgQueueChild2Parent 1025	// replace this number with an arbitrary number


// TO DO: declare a structure for messages
typedef struct msf_buf{
	long type;
	int num;
}mes;

void Parent(int msgq_snd, int msgq_rcv);
int Child(int msgq_rcv, int msgq_snd);

int main()
{
	// TO DO: Create two messsage queues using the keys MesgQueueParent2Child and MesgQueueChild2Parent
	// 		  On failure, display an error message.

	int msgq_p2c = msgget(MesgQueueParent2Child, IPC_CREAT|0666);
	if(msgq_p2c == -1){
		printf("msgget failed\n");
		exit(0);
	}

	int msgq_c2p = msgget(MesgQueueChild2Parent, IPC_CREAT|0666);
	if(msgq_c2p == -1){
		printf("msgget failed\n");
		exit(0);
	}

	pid_t child_pid = fork();
	if(child_pid < 0){
		printf("Failed to create process.\n");
		return -1;
	} else if(child_pid > 0){
		Parent(msgq_p2c, msgq_c2p);						// DO NOT modify this line

		// TO DO: Destroy the two message queues.

		printf("Parent terminating.\n");				// This smessage should be displayed.

		if(msgctl(msgq_p2c, IPC_RMID,NULL) == -1){
			printf("msgctl failed\n");
			exit(0);
		}

		if(msgctl(msgq_c2p, IPC_RMID,NULL) == -1){
			printf("msgctl failed\n");
			exit(0);
		}

	} else {
		int sum = Child(msgq_p2c, msgq_c2p);			// DO NOT modify this line
		printf("Child terminating. sum = %d\n", sum);	// This smessage should be displayed.
	
	}

	return 0;
}

void Parent(int msgq_snd, int msgq_rcv)
// TO DO: implement this function
{
	/* TO DO:
        - Repeat
            . Read an integer from the user.
            . Send the number to the child using msgsnd().
			  On failure, display an error message.
            . If the input number is negative, break the loop.
            . Receive the sum of the input numbers from the child using msgrcv().
			  On failure, display an error message.
            . Display the sum. Put a prefix "[parent]" to indicate it was printed by the parent.
	*/
	printf("Input number and child will accumulate ( -1 to finish).\n");
	int msg = 0;
	mes msg_bufs, msg_bufr;
	while(1){
		scanf("%d", &msg);
		msg_bufs.type = 1;
		msg_bufs.num = msg;
		if(msgsnd(msgq_snd, &msg_bufs, sizeof(msg_bufs.num),0) == -1){
			printf("msgsnd failed\n");
			exit(0);
		}
		if(msg < 0){
			break;
		}
		msg_bufr.type = 1;
		if(msgrcv(msgq_rcv, &msg_bufr, sizeof(msg_bufr.num), 0, 0) != sizeof(msg_bufr.num)){
			printf("msgrcv failed\n");
			exit(0);
		}
		printf("[parent] sum = %d\n", msg_bufr.num);
	}


}

int Child(int msgq_rcv, int msgq_snd)
// TO DO: implement this function
{
	/* TO DO:
		- Initialize sum by zero.
		- Repeat
			. Receive a number from the parent using msgrcv().
			  On failure, display an error message.
			. If the number is negative, break the loop.
			. Display the input number. Put a prefix "[child]" to indicate it was printed by the child.
			. Add the number to sum.
			. Send the sum to the parent using msgsnd(). 
			  On failure, display an error message.
		- Return sum.
	*/
	mes msg_bufr, msg_bufs;
	int sum = 0;
	while(1){
		msg_bufr.type = 1;
		if(msgrcv(msgq_rcv, &msg_bufr, sizeof(msg_bufr.num), 0, 0) != sizeof(msg_bufr.num)){
			printf("msgrcv failed\n");
			exit(0);
		}
		if(msg_bufr.num < 0){
			break;
		}
		
		printf("[child] input number = %d\n", msg_bufr.num);
		sum += msg_bufr.num;
		msg_bufs.type = 1;
		msg_bufs.num = sum;
		if(msgsnd(msgq_snd, &msg_bufs, sizeof(msg_bufs.num),0) == -1){
			printf("msgsnd failed\n");
			exit(0);
		}

	}
	return sum;

}

