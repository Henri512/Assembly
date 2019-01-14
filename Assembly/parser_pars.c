#include "parser_pars.h"
#include "parser_helpers.h"
#include "string_helpers.h"
#include "constants_helper.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void executeFirstPass(SymbolTableEntry *symbolTableEntry, SectionsCollection *sectionsCollection, char *inputFilePath)
{
	FILE *inputFile = getInputFile(inputFilePath);
	const size_t line_size = 100;
	char *line = (char*)malloc(line_size);

	while (fgets(line, line_size, inputFile) != NULL)
	{
		printf("In %s section:\n\r", getSectionValue(sectionsCollection->currentSection));
		char *token = getTokenFromLine(line);
		if (isEndToken(token))
		{
			printf("\tDoslo se do .end direktive. Kraj prvog prolaza.");
			break;
		}
		else if (!isValidToken(token))
		{
			printf("\tNaredba %s nije validna, kraj asembliranja!", token);
			exit(-1);
		}

		parseTokenFirstPass(symbolTableEntry, sectionsCollection, token);
	}

	fclose(inputFile);
	if (line)
	{
		free(line);
	}
}

void parseTokenFirstPass(SymbolTableEntry *symbolTableList, SectionsCollection *sectionsCollection, char *token)
{
	if (startsWith(SECTIONDIRECTIVE, token))
	{
		parseSection(sectionsCollection, token);
	}
	else if (token[strlen(token) - 1] == ':')
	{
		parseLabel(symbolTableList, sectionsCollection, token);
	}
	else
	{
		printf("\tObrada naredbe %s nije implementirana!\n\r", token);
	}
}

// enum Sections { Text, RoData, Data, Bss, SymTab, RelText, RelData, Debug, StrTab };
void parseSection(SectionsCollection *sectionsCollection, char *token)
{
	int tokenLength = strlen(token);
	char *section = (char*)malloc(tokenLength - 8);
	if (section)
	{
		strncpy(section, token + 9, tokenLength - 7);
		section[tokenLength - 7] = '\n';
		if (strcmp(section, "\".text\"") == 0)
		{
			sectionsCollection->currentSection = Text;
		}
		else if (strcmp(section, "\".rodata\"") == 0)
		{
			sectionsCollection->currentSection = RoData;
		}
		else if (strcmp(section, "\".data\"") == 0)
		{
			sectionsCollection->currentSection = Data;
		}
		else if (strcmp(section, "\".bss\"") == 0)
		{
			sectionsCollection->currentSection = Bss;
		}
		else if (strcmp(section, "\".symtab\"") == 0)
		{
			sectionsCollection->currentSection = SymTab;
		}
		else if (strcmp(section, "\".reltext\"") == 0)
		{
			sectionsCollection->currentSection = RelText;
		}
		else if (strcmp(section, "\".reldata\"") == 0)
		{
			sectionsCollection->currentSection = RelData;
		}
		else if (strcmp(section, "\".debug\"") == 0)
		{
			sectionsCollection->currentSection = Debug;
		}
		else if (strcmp(section, "\".strtab\"") == 0)
		{
			sectionsCollection->currentSection = StrTab;
		}
		else
		{
			printf("\tGreska u parsiranju %s naredbe!\n\r\tSekcija %s nije validna, kraj asembliranja!", token, section);
			exit(-1);
		}
	}
}

void parseLabel(SymbolTableEntry *symbolTableList, SectionsCollection *sectionsCollection, char *token)
{
	int tokenLength = strlen(token);
	char *label = (char*)malloc(tokenLength);
	SymbolTableEntry *newSymbolTableEntry = NULL;
	strncpy(label, token, tokenLength - 1);
	label[tokenLength - 1] = '\n';

	if (symbolTableList == NULL)
	{
		symbolTableList = makeSymbolTableEntry(label, sectionsCollection->currentSection, 0, 0);
	}
	else
	{
		SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(label, sectionsCollection->currentSection, 0, symbolTableList->num + 1);
		symbolTableList->next = newSymbolTableEntry;
	}
}
