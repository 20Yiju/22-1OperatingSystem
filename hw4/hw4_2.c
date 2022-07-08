#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define FILENAME "shm_hw4_2_22000690.shm"
#define SIZE 1024

typedef struct {
	char name[32];  //name
	char id[32];    //student id
	char major[32]; //major
} Student; 

int main(){

	int shmfd, pid;

	shmfd = shm_open(FILENAME, O_CREAT | O_RDWR, 0666);

	if(shmfd == -1){
		printf("shm_open failed");
		return -1;
	}

	ftruncate(shmfd, SIZE*sizeof(Student));
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		exit(-1);
	}
	
	if(pid > 0){
		Student *s = mmap(0, SIZE*sizeof(Student), PROT_WRITE, MAP_SHARED, shmfd,  0);
		
		
		sprintf(s[0].name,"%s","Peter Parker");
		sprintf(s[0].id,"%s", "2200001");
		sprintf(s[0].major, "%s","Computer Science\n");

		sprintf(s[1].name,"%s","Natasha Romanoff");
		sprintf(s[1].id,"%s" ,"2200002");
		sprintf(s[1].major, "%s","Electric Engineering\n");

		sprintf(s[2].name,"%s","Clark Kent");
		sprintf(s[2].id, "%s","2200003");
		sprintf(s[2].major, "%s","GLS\n");
		
		sprintf(s[3].name,"%s","Diana Prince");
		sprintf(s[3].id, "%s","2200004");
		sprintf(s[3].major, "%s","CSEE\n");

		sprintf(s[4].id, "%s", "");
		wait(NULL);
	}
	else{

		Student *s = mmap(0, SIZE*sizeof(Student), PROT_READ, MAP_SHARED, shmfd,  0);
		

		int i = 0;

		while(strcmp(s[i].id,"") != 0){
			printf("name = %s, ", (char *)s[i].name);
			printf("id = %s, ", (char *)s[i].id);
			printf("major = %s", (char *)s[i].major);

			i++;
		}
		shm_unlink(FILENAME);
		exit(0);
	}

}

