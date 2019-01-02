#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Niste uneli naziv fajla!");
		return -1;
	}
	if (!fileExists(argv[1]))
	{
		printf("Fajl sa zadatim imenom ne postoji!");
		return -1;
	}

	processInputFile(argv[1]);

	int a = 0;
	printf("Hello assembly!\n");
	scanf("%d", &a);
	return 0;
}

int processInputFile(char* fileName)
{
	FILE* fp;
	const size_t line_size = 100;
	char* line = malloc(line_size);

	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		return -1;
	}

	//read line by line
	while (fgets(line, line_size, fp) != NULL)
	{
		if (!strcmp(&line, ".end"))
		{
			// process file ending
			printf("Doslo se do kraja fajla, kraj asembliranja!");
		}
		else
		{
			char *token = strtok(line, "#");
			printf("%s\n", token);
			// process each command
		}
	}

	fclose(fp);
	if (line)
	{
		free(line);
	}
	return 0;
}

