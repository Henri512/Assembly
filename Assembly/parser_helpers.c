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
	token = leftTrim(token, NULL);
	token = rightTrim(token, NULL);
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

TokenList *getInstructionsTokens(char *token)
{
	TokenList *tokenList;
	const char *delimiter = " ";
	char *tok;
	char **tokens = NULL;
	int i = 0, j = 0;
	tokenList = (TokenList*)malloc(sizeof(TokenList));
	if (tokenList == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	tokens = (char**)malloc(sizeof(char*) * 100);
	if (tokens == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	// get the first token 
	tok = strtok(token, delimiter);

	// walk through other tokens
	while (tok != NULL)
	{
		tokens[i++] = tok;
		tok = strtok(NULL, delimiter);
	}

	tokenList->tokens = (char**)malloc(sizeof(char*)*i);
	if (tokenList->tokens == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	while (j < i)
	{
		tokenList->tokens[j] = tokens[j];
		j++;
	}
	tokenList->size = j;

	free(tokens);
	return tokenList;
}

