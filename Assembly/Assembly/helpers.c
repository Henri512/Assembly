#include <sys/stat.h>

/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int fileExists(const char* fileName) {
	struct stat buffer;
	int exist = stat(fileName, &buffer);
	if (exist == 0)
		return 1;
	else
		return 0;
}