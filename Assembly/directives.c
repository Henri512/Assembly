#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "directives.h"
#include "sections.h"
#include "parser_helpers.h"
#include "string_helpers.h"
#include "section_helpers.h"
#include "instruction_helpers.h"
#include "constants_helper.h"

// First Pass
void parseExternDirective(char *token, SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		addExternDirectiveToSymbolTableList(symbolTableEntryList, directiveValue);
	}
	freeTokenList(tokenList);
}

void addExternDirectiveToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, char *directiveValue)
{
	if (labelExists(symbolTableEntryList, directiveValue))
	{
		printf("Simbol %s vec postoji!\n\r", directiveValue);
		exit(-1);
	}

	SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(directiveValue, Unknown, 0);
	newSymbolTableEntry->sectionType = Global;

	addSymbolTableEntry(symbolTableEntryList, newSymbolTableEntry);
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

// Second Pass
void parseGlobalDirective(char *token, SymbolTableEntryList *symbolTableEntryList)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int i, valueLength = 0, byteCount = 0;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",");
		setGlobal(directiveValue, symbolTableEntryList);
	}
	freeTokenList(tokenList);
}

void parseCharWordLongDirectivesSP(SymbolTableEntryList *symbolTableEntryList, char *token, SectionsCollection *sectionsCollection, char size, char *oldLabel)
{
	TokenList *tokenList = getInstructionsTokens(token);
	SymbolTableEntry *entry = NULL, *sectionSymbol = NULL;
	int i, valueLength = 0, byteCount = 0, offset = 0;;
	for (i = 1; i < tokenList->size; i++)
	{
		char *directiveValue = rightTrim(tokenList->tokens[i], ",\'");
		directiveValue = leftTrim(tokenList->tokens[i], "\'");
		valueLength = strlen(directiveValue);
		entry = getEntryByLabelName(symbolTableEntryList, directiveValue[0] == '&' ? directiveValue + 1 : directiveValue);
		// ako se referencira labela u direktivi proveravamo da li je potrebno dodati realokacioni zapis
		if (entry)
		{
			// slucaj kada labela samu sebe referencira, d: .long &d
			if (oldLabel && !strcmp(entry->name, oldLabel))
			{
				int offset = getCurrentOffset(sectionsCollection);
				sectionSymbol = getSymbolByName(symbolTableEntryList, getSectionValue(entry->section));
				addNewRelocationData(sectionsCollection, entry->section, offset, ABS, sectionSymbol->num);
				free(oldLabel);
			}
			else
			{
				handleLabelInCharWordLongDirectives(sectionsCollection, size, entry->section, entry->num, oldLabel);
			}
			offset = sectionsCollection->currentSection != entry->section ? 0 : entry->offset;
			addLabelOffsetToContent(sectionsCollection,offset, size);
		}
		else
		{
			validateAndAddCWLDirectiveContent(sectionsCollection, directiveValue, size, valueLength);
		}
	}
	freeTokenList(tokenList);
}

void parseAlignDirectiveSP(char *token, SectionsCollection *sectionsCollection)
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
				int i = round - (counter % round);
				for (i; i > 0; i--)
				{
					addCharContentToCurrentSection(sectionsCollection, ALIGNDIRFILLVALUE);
				}
			}
		}
	}
	freeTokenList(tokenList);
}

void parseSkipDirectiveSP(char *token, SectionsCollection *sectionsCollection)
{
	TokenList *tokenList = getInstructionsTokens(token);
	int byteCount = 0;
	char *ptr;
	if (tokenList->size == 2)
	{
		char *directiveValue = rightTrim(tokenList->tokens[1], ",");
		byteCount = strtol(directiveValue, &ptr, 10);
		int i = byteCount;
		for (i; i > 0; i--)
		{
			addCharContentToCurrentSection(sectionsCollection, '\0');
		}
	}
	freeTokenList(tokenList);
}

void validateAndAddCWLDirectiveContent(SectionsCollection *sectionsCollection, char *directiveValue, char size, int length)
{
	char isValid = 0, isHexChar, *pEnd;
	long num = -1;
	switch (size)
	{
	case CHARSIZE:
		isValid = isStringRegularChar(directiveValue, length);
		isHexChar = isStringHexChar(directiveValue, length);
		if (isValid)
		{
			char value = getCharFromDirectiveValue(directiveValue);
			addCharContentToCurrentSection(sectionsCollection, value);
		}
		else if (isHexChar)
		{
			num = strtol(directiveValue, &pEnd, 16);
			addCharContentToCurrentSection(sectionsCollection, (char)num);
		}
		else
		{
			printf("Greska u parsiranju direktive, %s nije validna vrednost!", directiveValue);
			exit(-1);
		}
		break;
	case WORDSIZE:
		num = strtol(directiveValue, &pEnd, 0);
		isValid = num != 0 && num != LONG_MAX && num != LONG_MIN;
		if (isValid)
		{
			addCharContentToCurrentSection(sectionsCollection, (char)num & 0xff);
			addCharContentToCurrentSection(sectionsCollection, (char)(num >> 8) & 0xff);
		}
		else
		{
			printf("Greska u parsiranju direktive, %s nije validna vrednost!", directiveValue);
			exit(-1);
		}
		break;
	case LONGSIZE:
		num = strtol(directiveValue, &pEnd, 0);

		isValid = num != 0 && num != LONG_MAX && num != LONG_MIN;
		if (isValid)
		{
			char firstByte = (char)num & 0xff;
			char secondByte = (char)(num >> 8) & 0xff;
			char thirdByte = (char)(num >> 16) & 0xff;
			char fourthByte = (char)(num >> 24) & 0xff;
			// printBinaryChar(firstByte);
			// printBinaryChar(secondByte);
			addCharContentToCurrentSection(sectionsCollection, firstByte);
			addCharContentToCurrentSection(sectionsCollection, secondByte);
			addCharContentToCurrentSection(sectionsCollection, thirdByte);
			addCharContentToCurrentSection(sectionsCollection, fourthByte);
		}
		else
		{
			printf("Greska u parsiranju direktive, %s nije validna vrednost!", directiveValue);
			exit(-1);
		}
		break;
	}
}

void addLabelOffsetToContent(SectionsCollection *sectionsCollection, int offset, char size)
{
	switch (size)
	{
	case CHARSIZE:
		addCharContentToCurrentSection(sectionsCollection, (char)offset & 0xff);
		break;
	case WORDSIZE:
		addCharContentToCurrentSection(sectionsCollection, (char)offset & 0xff);
		addCharContentToCurrentSection(sectionsCollection, (char)(offset >> 8) & 0xff);
		break;
	case LONGSIZE:
		addCharContentToCurrentSection(sectionsCollection, (char)offset & 0xff);
		addCharContentToCurrentSection(sectionsCollection, (char)(offset >> 8) & 0xff);
		addCharContentToCurrentSection(sectionsCollection, (char)(offset >> 16) & 0xff);
		addCharContentToCurrentSection(sectionsCollection, (char)(offset >> 24) & 0xff);
		break;
	}
}

void addDirectiveRelDataToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection,
	char *label, enum SectionEnum section, int offset)
{
	if (!labelExists(symbolTableEntryList, label))
	{
		SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(label, section, offset);
		newSymbolTableEntry->sectionType = Global;

		addSymbolTableEntry(symbolTableEntryList, newSymbolTableEntry);
	}
}

void handleLabelInCharWordLongDirectives(SectionsCollection *sectionsCollection, int directiveSizeInBytes, enum SectionEnum section, int labelNum)
{
	/*if (sectionsCollection->currentSection != section)
	{*/
	int offset = getCurrentOffset(sectionsCollection);
	char relocationType = CHARDIR;
	if (directiveSizeInBytes == WORDSIZE)
	{
		relocationType++;
	}
	else if (directiveSizeInBytes == LONGSIZE)
	{
		relocationType += 2;
	}
	addNewRelocationData(sectionsCollection, section, offset, relocationType, labelNum);
	/*}*/
}