#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser_first.h"
#include "parser_helpers.h"
#include "string_helpers.h"
#include "constants_helper.h"
#include "section_helpers.h"
#include "directives.h"
#include "instructions.h"

void executeFirstPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *inputFilePath)
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
		char *token = getTokenFromLine(line);
		if (!isTokenComment(token) && !isTokenEmptyLine(token))
		{
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
			printf("In %s section:\n\r", getSectionValue(sectionsCollection->currentSection));

			parseTokenFirstPass(symbolTableEntryList, sectionsCollection, token);
		}
	}

	fclose(inputFile);
	if (line)
	{
		free(line);
	}
}

void parseTokenFirstPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	char instructionValue, sectionValue;
	if ((sectionValue = getTokenSectionValue(token)) > -1)
	{
		parseSection(sectionsCollection, sectionValue);
	}
	else if (isTokenLabel(token))
	{
		parseLabel(symbolTableEntryList, sectionsCollection, token);
	}
	else if (isTokenDirective(token))
	{
		parseDirective(symbolTableEntryList, sectionsCollection, token);
	}
	else if ((instructionValue = getTokenInstructionValue(token)) > -1)
	{
		parseInstruction(symbolTableEntryList, sectionsCollection, token);
	}
	else
	{
		printf("\tObrada naredbe %s nije implementirana!\n\r", token);
	}
}

// strncpy(dest, src + beginIndex, endIndex - beginIndex);
void parseLabel(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	SymbolTableEntry *newSymbolTableEntry = NULL;
	int labelLength = 0, tokenLength = strlen(token), offset = 0;
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
		label[tokenLength - 1] = '\0';

		addLabelToSymbolTableList(symbolTableEntryList, sectionsCollection, label);
	}
	// slucaj kad labela ima i sadrzaj na istoj liniji
	else
	{
		labelLength = strcspn(token, ":") + 1;
		label = (char*)calloc(labelLength, sizeof(char));
		int labelTokenLength = tokenLength - labelLength + 1;
		if (label == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}

		strncpy(label, token, labelLength);
		label[labelLength - 1] = '\0';

		char *labelContentToken = (char*)calloc(labelTokenLength, sizeof(char));
		if (labelContentToken == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}
		strncpy(labelContentToken, token + labelLength, labelTokenLength);
		labelContentToken[labelTokenLength - 1] = '\0';

		labelContentToken = leftTrim(labelContentToken, "\t ");
		addLabelToSymbolTableList(symbolTableEntryList, sectionsCollection, label);

		parseTokenFirstPass(symbolTableEntryList, sectionsCollection, labelContentToken);
	}
}

void addLabelToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *label)
{
	if (labelExists(symbolTableEntryList, label))
	{
		printf("\tLabela %s vec postoji!\n\r", label);
		exit(-1);
	}

	int offset = getCurrentCollectionsCount(sectionsCollection);
	SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(label, sectionsCollection->currentSection, offset);

	addSymbolTableEntry(symbolTableEntryList, newSymbolTableEntry);
}

void parseDirective(SymbolTableEntry *symbolTableList, SectionsCollection *sectionsCollection, char *token)
{
	if (startsWith(GLOBALDIRECTIVE, token))
	{
		// nista se ne radi u prvom prolazu
	}
	else if (startsWith(EXTERNDIRECTIVE, token))
	{
		parseExternDirective(token, sectionsCollection, symbolTableList);
	}
	/*else if (startsWith(ASCIZDIRECTIVE, token))
	{
		parseAsciiDirectives(token, 1, sectionsCollection);
	}
	else if (startsWith(ASCIIDIRECTIVE, token))
	{
		parseAsciiDirectives(token, 0, sectionsCollection);
	}*/
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
