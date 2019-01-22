#include "parser_pars.h"
#include "parser_helpers.h"
#include "string_helpers.h"
#include "constants_helper.h"
#include "section_helpers.h"
#include "directives.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void executeFirstPass(SymbolTableEntry *symbolTableEntry, SectionsCollection *sectionsCollection, char *inputFilePath)
{
	FILE *inputFile = getInputFile(inputFilePath);
	const size_t line_size = 100;
	char *line = (char*)malloc(line_size);
	if (line == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

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
	else if (isTokenLabel(token))
	{
		parseLabel(symbolTableList, sectionsCollection, token);
	}
	else if (isTokenDirective(token))
	{
		parseDirective(symbolTableList, sectionsCollection, token);
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
	if (section == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
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
			printf("\tGreska u parsiranju %s naredbe!\n\r\tSekcija %s nije validna, kraj asembliranja!\n\r", token, section);
			exit(-1);
		}
		initializeSectionIfEmpty(sectionsCollection, 0, 0, 0);
	}
}

void parseLabel(SymbolTableEntry *symbolTableList, SectionsCollection *sectionsCollection, char *token)
{
	SymbolTableEntry *newSymbolTableEntry = NULL;
	int labelLength = 0, labelTokenSize = 0, tokenLength = strlen(token), offset = 0;
	char *label;

	// slucaj kad je prazna labela na liniji
	if (token[tokenLength - 1] == ':')
	{
		label = (char*)malloc(tokenLength);
		if (label == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}

		strncpy(label, token, tokenLength - 1);
		label[tokenLength - 1] = '\n';
	}
	// slucaj kad labela ima i sadrzaj na istoj liniji
	else
	{
		labelLength = strcspn(token, ":") + 1;
		label = (char*)malloc(labelLength);
		int labelTokenLength = tokenLength - labelLength - 1;
		if (label == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}

		strncpy(label, token, labelLength - 1);
		label[labelLength - 2] = '\n';

		char *labelContentToken = (char*)malloc(sizeof(char) * labelTokenLength);
		if (labelContentToken == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}
		strncpy(labelContentToken, token + labelLength, labelTokenLength);

		parseTokenFirstPass(symbolTableList, sectionsCollection, labelContentToken);
	}

	offset = sectionsCollection->textDataSection ? sectionsCollection->textDataSection->size : 0;
	
	// dodavanje labele u tabelu simbola
	if (symbolTableList == NULL)
	{
		symbolTableList = makeSymbolTableEntry(label, sectionsCollection->currentSection, offset, 0);
	}
	else
	{
		SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(label, sectionsCollection->currentSection, offset, symbolTableList->num + 1);
		symbolTableList->next = newSymbolTableEntry;
	}
}

void parseDirective(SymbolTableEntry *symbolTableList, SectionsCollection *sectionsCollection, char *token)
{
	if (startsWith(GLOBALDIRECTIVE, token))
	{
		// nista se ne radi u prvom prolazu
	}
	else if (startsWith(ASCIZDIRECTIVE, token))
	{
		parseAsciiDirectives(token, 1, sectionsCollection);
	}
	else if (startsWith(ASCIIDIRECTIVE, token))
	{
		parseAsciiDirectives(token, 0, sectionsCollection);
	}
	else if (startsWith(CHARDIRECTIVE, token))
	{
		parseCharWordLongDirectives(token, sectionsCollection, CHARSIZE);
	}
	else if (startsWith(WORDDIRECRTIVE, token))
	{
		parseCharWordLongDirectives(token, sectionsCollection, WORDSIZE);
	}
	else if (startsWith(LONGDIRECTIVE, token))
	{
		parseCharWordLongDirectives(token, sectionsCollection, LONGSIZE);
	}
	else if (startsWith(ALIGNDIRECTIVE, token))
	{
		parseAlignDirective(token, sectionsCollection);
	}
	else if (startsWith(SKIPDIRECTIVE, token))
	{
		parseSkipDirective(token, sectionsCollection);
	}
}
