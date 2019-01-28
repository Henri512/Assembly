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

char isTokenComment(char *token);

char isTokenEmptyLine(char *token);

FILE *getInputFile(char *filePath);

FILE *getOutputFile(char *filePath);

TokenList *getInstructionsTokens(char *token);

void freeTokenList(TokenList *tokenList);

char getAddressingType(TokenList *tokenList);
