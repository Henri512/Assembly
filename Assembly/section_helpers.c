#include "section_helpers.h"
#include "string_helpers.h"
#include "constants.h"
#include "sections.h"
#include <string.h>

char isTokenLabel(char *token)
{
	char isTokenLabel = token[strlen(token) - 1] == ':'
		|| strchr(token, ':') != NULL;
	return isTokenLabel;
}

char isTokenDirective(char *token)
{
	return token[0] == '.';
}

void initializeSectionIfEmpty(SectionsCollection *sectionsCollection, int index, int start, int size)
{
	switch (sectionsCollection->currentSection)
	{
	case Text:
		if (!sectionsCollection->textDataSection)
		{
			sectionsCollection->textDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case RoData:
		if (!sectionsCollection->roDataSection)
		{
			sectionsCollection->roDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case Data:
		if (!sectionsCollection->dataDataSection)
		{
			sectionsCollection->dataDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case Bss:
		if (!sectionsCollection->bssDataSection)
		{
			sectionsCollection->bssDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	default:
		break;
	}
}

char getTokenSectionValue(char *token) 
{
	char result = -1;
	for (int i = 0; i < SECTIONSCOUNT; i++)
	{
		if (startsWith(Sections[i], token))
		{
			result = i;
		}
	}
	return result;
}

char getTokenInstructionValue(char *token)
{
	char result = -1;
	char *instruction = getInstructionFromToken(token);
	int length = strlen(instruction);
	for (int i = 0; i <INSTRUCTIONSCOUNT; i++)
	{
		if (!strcmp(Instructions[i], instruction))
		{
			result = i;
		}
		else if (startsWith(Instructions[i], instruction))
		{
			if (!strcmp(instruction + length - 3, "_eq") ||
				!strcmp(instruction + length - 3, "_ne") ||
				!strcmp(instruction + length - 3, "_gt"))
			{
				result = i;
			}
			else
			{
				printf("\tNepostojeca instrukcija \'%s\', kraj asembliranja!", instruction);
				exit(-1);
			}
		}
	}
	free(instruction);
	return result;
}

char *getInstructionFromToken(char *token)
{
	int i = 0;
	while (token[i] != ' ')
	{
		i++;
	}
	char *instruction = (char*)malloc(sizeof(char) * (i + 1));
	strncpy(instruction, token, i);
	instruction[i] = '\0';
	return instruction;
}
