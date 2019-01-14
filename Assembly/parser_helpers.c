#include "parser_helpers.h"
#include "string_helpers.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

FILE *getInputFile(char *filePath)
{
	FILE *fp = NULL;
	const size_t line_size = 100;
	char *line = (char*)malloc(line_size);

	fp = fopen(filePath, "r");
	if (fp == NULL)
	{
		exit(-1);
	}

	return fp;
}

char *getTokenFromLine(char *line)
{
	char *token = strtok(line, "#");
	token = strtok(line, "@");
	token = strtok(line, ";");
	token = strtok(line, "\n");
	token = leftTrim(token, " ");
	token = rightTrim(token, " ");
	return token;
}

char isEndToken(char *token)
{
	return !strcmp(token, ENDDIRECTIVE);
}

char isValidToken(char *token)
{
	return token && strlen(token) > 0 && token[0] != '#' && token[0] != '@'
		&& token[0] != ';' && token[0] != '\n';
}

