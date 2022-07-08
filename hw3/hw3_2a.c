#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

int main(void) {
	
	pid_t child_pid;
	struct timeval start, end;
	double run;

	printf("[parent] before fork(), pid = %d, ppid = %d\n", getpid(), getppid());
	gettimeofday(&start, NULL);

	child_pid = fork();

	if(child_pid < 0) {
		fprintf(stderr, "Forked Failed");
		return 1;
	}

	else if(child_pid > 0) {
		wait(NULL);
		gettimeofday(&end, NULL);
		run = (end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) / 1000000.0);
		printf("[parent] pid = %d, ppid = %d, child_pid = %d, running time = %f\n",getpid(), getppid(), child_pid, run);
		exit(0);
	}

	else{
		printf("[child] pid = %d, ppid = %d, child_pid = %d\n",getpid(), getppid(), child_pid);
		execl("./hw3_2b","./hw3_2b",NULL);
	}

	return 0;

}
