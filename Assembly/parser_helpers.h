#pragma once
#include <stdio.h>

typedef struct tokenList
{
	char **tokens;
	int size;
} TokenList;

char *getTokenFromLine(char *line);

char isEndToken(char *token);

char isValidToken(char *token);

FILE *getInputFile(char *filePath);

TokenList *getLabelTokens(char *token);