#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

int main() {
	printf("[new child] pid = %d, ppid = %d\n",getpid(), getppid());
	for(int i = 0; i < 3; i++){
		printf("[new child] %d\n", i+1);
		sleep(1);
	}

}
