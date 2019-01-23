#include "directives.h"
#include "sections.h"
#include "parser_helpers.h"
#include "string_helpers.h"
#include <math.h>

void parseAsciiDirectives(char *token, char hasZeroByte, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0, byteCount = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		valueLength = strlen(directiveValue);
		if (directiveValue[0] != '"' || directiveValue[valueLength - 1] != '"')
		{
			printf("Greska u parsiranju direktive %s, %s nije validna vrednost!", token, directiveValue);
			exit(-1);
		}
		byteCount = byteCount + valueLength - 2;
	}

	addToCurrentCollectionsCount(sectionsCollection, byteCount);
	freeTokenList(tokenList);
}

void parseCharWordLongDirectives(char *token, SectionsCollection *sectionsCollection, char size)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0, byteCount = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		valueLength = strlen(directiveValue);		
		byteCount = byteCount + size;
	}
	addToCurrentCollectionsCount(sectionsCollection, byteCount);
	freeTokenList(tokenList);
}

void parseCharDirective(char *token, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0, byteCount = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		valueLength = strlen(directiveValue);
		char isValueValidHex = directiveValue[0] == '0' && directiveValue[1] == 'x' && valueLength > 2 && valueLength < 5;
		char isValueValidChar = directiveValue[0] == '\'' && directiveValue[valueLength - 1] && (valueLength == 3 || (valueLength == 4 && directiveValue[1] == '\\'));
		if (!isValueValidHex && !isValueValidChar)		
		{
			printf("Greska u parsiranju direktive %s, %s nije validna vrednost!", token, directiveValue);
			exit(-1);
		}
		byteCount = byteCount + CHARSIZE;
	}
	addToCurrentCollectionsCount(sectionsCollection, byteCount);
	freeTokenList(tokenList);
}

void parseWordDirective(char *token, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0, byteCount = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		valueLength = strlen(directiveValue);
		char isValueValidHex = directiveValue[0] == '0' && directiveValue[1] == 'x' && valueLength > 2 && valueLength < 7;
		char isValueValidNum = directiveValue[0] == '\'' && directiveValue[valueLength - 1] && (valueLength == 3 || (valueLength == 4 && directiveValue[1] == '\\'));
		if (!isValueValidHex && !isValueValidNum)
		{
			printf("Greska u parsiranju direktive %s, %s nije validna vrednost!", token, directiveValue);
			exit(-1);
		}
		byteCount = byteCount + WORDSIZE;
	}
	addToCurrentCollectionsCount(sectionsCollection, byteCount);
	freeTokenList(tokenList);
}

void parseAlignDirective(char *token, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int alignNumber = 0, counter, round;
	char *ptr;
	if (tokenList->size == 2)
	{
		char *directiveValue = rightTrim(tokenList->tokens[1], ",");
		alignNumber = strtol(directiveValue, &ptr, 10);
		if (alignNumber > 0)
		{
			counter = getCurrentCollectionsCount(sectionsCollection);
			round = (int)pow(2, alignNumber);
			if (counter % round != 0)
			{
				addToCurrentCollectionsCount(sectionsCollection, round - (counter % round));
			}
		}
	}
	freeTokenList(tokenList);
}

void parseSkipDirective(char *token, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int byteCount = 0;
	char *ptr;
	if (tokenList->size == 2)
	{
		char *directiveValue = rightTrim(tokenList->tokens[1], ",");
		byteCount = strtol(directiveValue, &ptr, 10);
	}
	addToCurrentCollectionsCount(sectionsCollection, byteCount);
	freeTokenList(tokenList);
}
