#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
	
	DIR *di = opendir(".");
	struct dirent *dp;
	struct stat s_buf;
	struct tm *date;

	while((dp = readdir(di)) != NULL) {
		
		stat(dp -> d_name, &s_buf);
		date = localtime(&s_buf.st_mtime);

		if(S_ISREG(s_buf.st_mode)) {
			printf("[reg] %s uid=%d, gid=%d, %ld bytes, mtime=%04d:%02d:%02d %02d:%02d:%02d\n", dp -> d_name, s_buf.st_uid, s_buf.st_gid, s_buf.st_size, date->tm_year + 1900, date->tm_mon + 1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);

			}

		else if(S_ISDIR(s_buf.st_mode)) {
			printf("[dir] %s\n", dp -> d_name );
		}
	}
	
	closedir(di);

	return 0;
}	
