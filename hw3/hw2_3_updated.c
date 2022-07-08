//My previous solution was perfect, so I didn't modify it!
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define MAX_PATH 256
#define MAX_RESULT 100

void FindLargestFile(char *start_dir, char *largest, long *size);

int main(int argc, char *argv[]) {
		
	if(argc < 2) {
		printf("Usage: %s <start_dir>\n", argv[0]);
		return 0;
	}

	char *start_dir = argv[1];
	char largest[MAX_PATH] = "";
	long size = 0L;  // the initial value of size should be zero
	FindLargestFile(start_dir, largest, &size);
	
	if(size>0) {
		printf("largest file = %s, size = %ld\n", largest, size);
//		printf("largest file = %s (%ld bytes)\n", largest, size);
	}
	else {
		printf("no file with size > 0.\n");
	}

	return 0;
}

void FindLargestFile(char *start_dir, char *largest, long *size) {
	
	DIR *di = opendir(start_dir);
	struct dirent *dp;
	struct stat s_buf;
	char filename[1024];

	while((dp = readdir(di)) != NULL){
		
		if(!strcmp(dp -> d_name, ".") || !strcmp(dp -> d_name, "..")) {
			continue;
		}
		
		sprintf(filename, "%s/%s", start_dir, dp->d_name);
		
		stat(filename, &s_buf);

		if(S_ISDIR(s_buf.st_mode)) {	
			FindLargestFile(filename, largest, size);
		}
		else if(S_ISREG(s_buf.st_mode)) {
			if(s_buf.st_size > *size) {
//				printf("%ld %ld\n", s_buf.st_size,*size);	
				strcpy(largest, filename);
				*size = s_buf.st_size;
			}
		}
	}
	closedir(di);
}
