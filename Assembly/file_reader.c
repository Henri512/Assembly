#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringData *GetInputList(char *filePath)
{
	FILE *fp;
	const size_t line_size = 100;
	char *line = malloc(line_size);

	fp = fopen(filePath, "r");
	if (fp == NULL)
	{
		exit(-1);
	}
	StringData *tokenList, *current;
	tokenList = current = NULL;

	//read line by line
	while (fgets(line, line_size, fp) != NULL)
	{
		char* token = strtok(line, "#");
		token = strtok(line, "@");
		token = strtok(line, ";");
		token = strtok(line, "\n");

		if (token && strlen(token) > 0 && token[0] != '#' && token[0] != '@'
			&& token[0] != ';' && token[0] != '\n')
		{
			if (!current)
			{
				tokenList = makeStringData(token);
				current = tokenList;
			}
			else
			{
				current->next = makeStringData(token);
				current = current->next;
			}
		}
	}

	fclose(fp);
	if (line)
	{
		free(line);
	}
	current = tokenList;
	while (current)
	{
		printf("%s\n", current->data);
		current = current->next;
	}

	return NULL;
}
