#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CPU_FILE "/proc/cpuinfo"
#define MEM_FILE "/proc/meminfo"
#define LOADAVG_FILE "/proc/loadavg"

#define BUFFER_SIZE 512

char buffer[BUFFER_SIZE];
int buffer_size = 0;
int buffer_pos = 0;

int ReadTextLine(int fd, char str[], int max_len);

int main() {
	
	int cf = 0;
	int mf = 0;
	int lf = 0;
	int index_m = 0;
	int index_l = 0;
	
	char test_l[BUFFER_SIZE] = {0};
	char test_m[BUFFER_SIZE] = {0};
	char test_c[BUFFER_SIZE] = {0};
	
	cf = open(CPU_FILE, O_RDONLY);
	mf = open(MEM_FILE, O_RDONLY);
	lf = open(LOADAVG_FILE, O_RDONLY);

	if (cf < 0 || mf < 0 || lf < 0) {
		printf("FAILED...");
		return -1;
	}

	char intel[100];
	char numcpu[100];
	for(int i = 0; i < 5; i++) {
		
		ReadTextLine(cf, test_c, BUFFER_SIZE);
		
	}
	sscanf(test_c, "%*[^':'] %*c %[^'\n']", intel);

	for(int j = 0; j < 7; j++) {
		
		ReadTextLine(cf, test_c, BUFFER_SIZE);
	}

	sscanf(test_c, "%*[^':'] %*c %s", numcpu);
	
	printf("# of processor cores = %s\n", numcpu);
	printf("CPU model = %s\n", intel);

	ReadTextLine(mf, test_m, BUFFER_SIZE);

	char a[10], b[10], c[10];
	
	sscanf(test_m, "%s %s %s", a, b, c);
	
	printf("Mem Total = %s\n", b);

	ReadTextLine(lf, test_l, BUFFER_SIZE);
	char *text1[BUFFER_SIZE] = {0};
	float avg1;
	float avg5;
	float avg15;
	
	char d[10], e[10], f[10];
	
	sscanf(test_l, "%s %s %s", d, e, f);

	avg1 = atof(d);
	avg5 = atof(e);
	avg15 = atof(f);

	printf("loadavg1 = %.6f, loadavg5 = %.6f, loadavg15 = %.6f\n", avg1, avg5, avg15);
	
	close(cf);
	close(mf);
	close(lf);

}

int ReadTextLine(int fd, char str[], int max_len) {
	int i = 0;
	int j = 0;
	int ret = 0;
	char test[BUFFER_SIZE] = {0, };

	if(lseek(fd, 0, SEEK_CUR) == 0)
		buffer_pos = buffer_size = 0;

	while(j < max_len - 1) {
		if(buffer_pos == buffer_size) {
			buffer[0] = 0;
			buffer_size = read(fd, buffer, BUFFER_SIZE);
			buffer_pos = 0;
		}

		if(buffer_size == 0) {
			if(j == 0)
				ret = EOF;
			break;
		}

		while(j < max_len - 1 && buffer_pos < buffer_size) {
			str[j++] = buffer[buffer_pos++];
			if(str[j - 1] == '\0' || str[j - 1] == 10) {
				j--;
				max_len = j;
				break;
			}
		}
	}

	str[j] = 0;

	return ret;

}
