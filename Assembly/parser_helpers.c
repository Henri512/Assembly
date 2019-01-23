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
	token = strtok(token, "@");
	token = strtok(token, ";");
	token = rightTrim(line, NULL);
	token = leftTrim(line, NULL);
	return token;
}

char isEndToken(char *token)
{
	return !strcmp(token, ENDDIRECTIVE);
}

char isValidToken(char *token)
{
	return token && strlen(token) > 0
		&& token[0] != ';';
}

char isTokenComment(char *token)
{
	return token[0] == '#' || token[0] == '@';
}

char isTokenEmptyLine(char *token)
{
	return token == NULL || strlen(token) == 0;
}

TokenList *getInstructionsTokens(char *token)
{
	TokenList *tokenList = NULL;
	const char *delimiter = " ";
	char *tok, *tempToken;
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

	tempToken = (char*)malloc(sizeof(char) * strlen(token) + 1);
	if (tempToken == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	strcpy(tempToken, token);

	// get the first token 
	tok = strtok(tempToken, delimiter);

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

void freeTokenList(TokenList *tokenList)
{
	int i;
	if (tokenList && tokenList->tokens)
	{
		/*
		for (i = 0; i < tokenList->size; i++)
		{
			free(tokenList->tokens[i]);
		}*/
		free(tokenList);
	}
}

char getAddressingType(TokenList *tokenList)
{
	return 0;
}
