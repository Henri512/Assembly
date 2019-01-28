#include <stdio.h>
#include "parser_second.h"
#include "section_helpers.h"
#include "sections.h"
#include "constants_helper.h"
#include "instructions.h"
#include "directives.h"
#include "string_helpers.h"

void executeSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *inputFilePath)
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
		token = replaceChar(token, '\t', ' ');
		if (!isTokenComment(token) && !isTokenEmptyLine(token))
		{
			if (isEndToken(token))
			{
				printf("\tDoslo se do .end direktive. Kraj drugog prolaza.");
				break;
			}
			else if (!isValidToken(token))
			{
				printf("\tNaredba %s nije validna, kraj asembliranja!", token);
				exit(-1);
			}
			printf("In %s section:\n\r", getSectionValue(sectionsCollection->currentSection));

			parseTokenSecondPass(symbolTableEntryList, sectionsCollection, token, NULL);
		}
	}

	fclose(inputFile);
	if (line)
	{
		free(line);
	}
}

void parseTokenSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token, char *oldLabel)
{
	char instructionValue, sectionValue;
	if ((sectionValue = getTokenSectionValue(token)) > -1)
	{
		parseSection(sectionsCollection, symbolTableEntryList, sectionValue, NULL);
	}
	else if (isTokenLabel(token))
	{
		parseLabelSecondPass(symbolTableEntryList, sectionsCollection, token);
	}
	else if (isTokenDirective(token))
	{
		parseDirectiveSecondPass(symbolTableEntryList, sectionsCollection, token, oldLabel);
	}
	else if ((instructionValue = getTokenInstructionValue(token)) > -1)
	{
		parseInstructionSecondPass(symbolTableEntryList, sectionsCollection, token);
	}
	else
	{
		printf("\tObrada naredbe %s nije implementirana!\n\r", token);
	}
}

void parseLabelSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	int labelLength = 0, tokenLength = strlen(token);

	// slucaj kad labela ima i sadrzaj na istoj liniji
	if (token[tokenLength - 1] != ':')
	{
		labelLength = strcspn(token, ":") + 1;
		int labelTokenLength = tokenLength - labelLength + 1;

		char *labelContentToken = getNewString(labelTokenLength);
		char *oldLabel = getNewString(labelLength);
		strncpy(oldLabel, token, labelLength - 1);
		oldLabel[labelLength - 1] = '\0';
		if (labelContentToken == NULL)
		{
			printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
			exit(-1);
		}
		strncpy(labelContentToken, token + labelLength, labelTokenLength);
		labelContentToken[labelTokenLength - 1] = '\0';


		labelContentToken = leftTrim(labelContentToken, "\t ");		

		parseTokenSecondPass(symbolTableEntryList, sectionsCollection, labelContentToken, oldLabel);
	}
}

void parseDirectiveSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token, char *oldLabel)
{
	if (startsWith(GLOBALDIRECTIVE, token))
	{
		parseGlobalDirective(token, symbolTableEntryList);
	}
	else if (startsWith(CHARDIRECTIVE, token))
	{
		parseCharWordLongDirectivesSP(symbolTableEntryList, token, sectionsCollection, CHARSIZE, oldLabel);
	}
	else if (startsWith(WORDDIRECTIVE, token))
	{
		parseCharWordLongDirectivesSP(symbolTableEntryList, token, sectionsCollection, WORDSIZE, oldLabel);
	}
	else if (startsWith(LONGDIRECTIVE, token))
	{
		parseCharWordLongDirectivesSP(symbolTableEntryList, token, sectionsCollection, LONGSIZE, oldLabel);
	}
	else if (startsWith(ALIGNDIRECTIVE, token))
	{
		parseAlignDirectiveSP(token, sectionsCollection);
	}
	else if (startsWith(SKIPDIRECTIVE, token))
	{
		parseSkipDirectiveSP(token, sectionsCollection);
	}
}
