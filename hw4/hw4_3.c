#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define FILENAME "hw4_3_22000690.shm"
#define BUFFER_SIZE 128
#define TRUE 1
#define FALSE 0

typedef struct{
	char buffer[BUFFER_SIZE];
	int in, out;
} KeyBuffer;

void InitBuffer(KeyBuffer *buf);
int IsEmpty(KeyBuffer *buf);
int IsFull(KeyBuffer *buf);
int GetKeyCount(KeyBuffer *buf);
void InsertKey(KeyBuffer *buf, char key);
char DeleteKey(KeyBuffer *buf);
void Producer(int shm_fd);
void Consumer(int shm_fd);

int main() {
	int shmfd, pid;

	shmfd = shm_open(FILENAME, O_CREAT | O_RDWR, 0666);

	if(shmfd == -1){
		printf("shm_open failed\n");
		return -1;
	}

	ftruncate(shmfd, sizeof(KeyBuffer));
	pid = fork();
	
	if(pid == -1){
		printf("fork failed\n");
		exit(-1);
	}

	if(pid > 0){
		Producer(shmfd);
		wait(NULL);
	}
	else{
		Consumer(shmfd);
	}

}

void Producer(int shm_fd){
	KeyBuffer *buf = mmap(0, sizeof(KeyBuffer), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	InitBuffer(buf);
	char key;
	printf("Type keys. Press ESC to end\n");
	while(1){
		while(IsFull(buf)) ;
		scanf("%s", &key);
		InsertKey(buf, key);
		if(key == 27){
			InsertKey(buf, '\0');
			InsertKey(buf, '\0');
			printf("Terminating producer.\n");
			return;
		}
		else{
			printf("[producer] inserting key = %s (IsEmpty = %d, IsFull = %d, KeyCount = %d, in = %d, out = %d)\n", &key, IsEmpty(buf), IsFull(buf), GetKeyCount(buf), buf->in, buf->out);
		}
	}
}

void Consumer(int shm_fd){
	KeyBuffer *buf = mmap(0, sizeof(KeyBuffer), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	while(1){
		while(IsEmpty(buf) || GetKeyCount(buf) < 3) ;
		char key = DeleteKey(buf);
		if(key == 27){
			printf("Terminating consumer.\n");
			break;
		}
		else{
			printf("==> [consumer] retrieved key = %s (IsEmpty = %d, IsFull = %d, KeyCount = %d, in = %d, out = %d)\n", &key, IsEmpty(buf), IsFull(buf), GetKeyCount(buf), buf->in, buf->out);
		}
	}
	shm_unlink(FILENAME);
	exit(0);
}

void InitBuffer(KeyBuffer *buf){
	buf->in = 0;
	buf->out = 0;
}

int IsEmpty(KeyBuffer *buf){
	if(buf->in == buf->out){
		return 1;
	}
	return 0;
}

int IsFull(KeyBuffer *buf){
	if((buf->in + 1) % BUFFER_SIZE == buf->out){
		return 1;
	}
	return 0;
}

int GetKeyCount(KeyBuffer *buf){
	if(buf->in < buf->out){
		printf("EXCEPTION: out is bigger than in\n");
		exit(-1);
	}
	return (buf->in) - (buf->out);
}

void InsertKey(KeyBuffer *buf, char key){
	strcpy(buf[buf->in].buffer, &key);
	buf->in = (buf->in +  1) % BUFFER_SIZE;
}
char DeleteKey(KeyBuffer *buf){
	char item[BUFFER_SIZE];
	strcpy(item, buf[buf->out].buffer);
	buf -> out = (buf->out + 1) % BUFFER_SIZE;
	return *item;
}

