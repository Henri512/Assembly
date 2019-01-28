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
		if (!sectionsCollection->textDataSection->marked)
		{
			sectionsCollection->textDataSection->marked = 1;
		}
		else
		{
			printf("\tSekcija %s je vec jednom pojavila u fajlu! Kraj asembliranja.", Sections[Text]);
			exit(-1);
		}
		break;
	case RoData:
		if (!sectionsCollection->roDataSection->marked)
		{
			sectionsCollection->roDataSection->marked = 1;
		}
		else
		{
			printf("\tSekcija %s je vec jednom pojavila u fajlu! Kraj asembliranja.", Sections[RoData]);
			exit(-1);
		}
		break;
	case Data:
		if (!sectionsCollection->dataDataSection->marked)
		{
			sectionsCollection->dataDataSection->marked = 1;
		}
		else
		{
			printf("\tSekcija %s je vec jednom pojavila u fajlu! Kraj asembliranja.", Sections[Data]);
			exit(-1);
		}
		break;
	case Bss:
		if (!sectionsCollection->bssDataSection->marked)
		{
			sectionsCollection->bssDataSection->marked = 1;
		}
		else
		{
			printf("\tSekcija %s je vec jednom pojavila u fajlu! Kraj asembliranja.", Sections[Bss]);
			exit(-1);
		}
		break;
	default:
		break;
	}
}

char labelExists(SymbolTableEntryList *symbolTableEntryList, char *labelName)
{
	SymbolTableEntry *current = symbolTableEntryList->list;
	char exists = 0;
	while (current)
	{
		if (!strcmp(current->name, labelName))
		{
			exists = 1;
		}
		current = current->next;
	}
	return exists;
}

SymbolTableEntry *getEntryByLabelName(SymbolTableEntryList *symbolTableEntryList, char *labelName)
{
	SymbolTableEntry *current = symbolTableEntryList->list, *entry = NULL;
	while (current)
	{
		if (!strcmp(current->name, labelName))
		{
			entry = current;
		}
		current = current->next;
	}
	return entry;
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
			if (!strncmp(instruction + length - 3, "_eq", 3) ||
				!strncmp(instruction + length - 3, "_ne", 3) ||
				!strncmp(instruction + length - 3, "_gt", 3))
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
	while (token[i] != ' ' && token[i] != '\t')
	{
		i++;
	}
	char *instruction = (char*)malloc(sizeof(char) * (i + 1));
	strncpy(instruction, token, i);
	instruction[i] = '\0';
	return instruction;
}

void resetCounters(SectionsCollection *sectionsCollection)
{
	if (sectionsCollection->bssDataSection)
	{
		sectionsCollection->bssDataSection->size = 0;
		sectionsCollection->bssDataSection->start = 0;
		sectionsCollection->bssDataSection->marked = 0;
	}
	if (sectionsCollection->dataDataSection)
	{
		sectionsCollection->dataDataSection->size = 0;
		sectionsCollection->dataDataSection->start = 0;
		sectionsCollection->dataDataSection->marked = 0;
	}
	if (sectionsCollection->roDataSection)
	{
		sectionsCollection->roDataSection->size = 0;
		sectionsCollection->roDataSection->start = 0;
		sectionsCollection->roDataSection->marked = 0;
	}
	if (sectionsCollection->textDataSection)
	{
		sectionsCollection->textDataSection->size = 0;
		sectionsCollection->textDataSection->start = 0;
		sectionsCollection->textDataSection->marked = 0;
	}
}


void addToCurrentCollectionsCount(SectionsCollection *sectionsCollection, int size)
{
	switch (sectionsCollection->currentSection)
	{
	case Text:
		sectionsCollection->textDataSection->size += size;
		break;
	case RoData:
		sectionsCollection->roDataSection->size += size;
		break;
	case Data:
		sectionsCollection->dataDataSection->size += size;
		break;
	case Bss:
		sectionsCollection->bssDataSection->size += size;
		break;
	}
}

int getCurrentCollectionsCount(SectionsCollection *sectionsCollection)
{
	int counter = 0;
	switch (sectionsCollection->currentSection)
	{
	case Text:
		counter = sectionsCollection->textDataSection->size;
		break;
	case RoData:
		counter = sectionsCollection->roDataSection->size;
		break;
	case Data:
		counter = sectionsCollection->dataDataSection->size;
		break;
	case Bss:
		counter = sectionsCollection->bssDataSection->size;
		break;
	}
	return counter;
}

int getCurrentOffset(SectionsCollection *sectionsCollection)
{
	int counter = 0;
	switch (sectionsCollection->currentSection)
	{
	case Text:
		counter = sectionsCollection->textDataSection ? sectionsCollection->textDataSection->size + sectionsCollection->textDataSection->start : 0;
		break;
	case RoData:
		counter = sectionsCollection->roDataSection ? sectionsCollection->roDataSection->size + sectionsCollection->roDataSection->start : 0;
		break;
	case Data:
		counter = sectionsCollection->dataDataSection ? sectionsCollection->dataDataSection->size + sectionsCollection->dataDataSection->start : 0;
		break;
	case Bss:
		counter = sectionsCollection->bssDataSection ? sectionsCollection->bssDataSection->size + sectionsCollection->bssDataSection->start : 0;
		break;
	}
	return counter;
}

// enum Sections { Text, Data, RoData, Bss };
void parseSection(SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList, char section, char *token)
{
	int currentOffset = getCurrentOffset(sectionsCollection);
	sectionsCollection->currentSection = section;
	initializeSectionIfEmpty(sectionsCollection, 0, currentOffset, 0);
	// ako je token NULL onda je u pitanju drugi prolaz i nista se ne radi, inace se sekcija dodaje u tabelu simbola
	if (token)
	{
		if (labelExists(symbolTableEntryList, token))
		{
			printf("\tLabela %s vec postoji!\n\r", token);
			exit(-1);
		}

		int offset = getCurrentCollectionsCount(sectionsCollection);
		char *sectionName = getNewString(strlen(token) + 1);
		char section = 0, i;
		strcpy(sectionName, token);
		for (i = 0; i < 4; i++)
		{
			if (!strcmp(token, Sections[i]))
			{
				section = i;
			}
		}
		SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(sectionName, section, offset);

		addSymbolTableEntry(symbolTableEntryList, newSymbolTableEntry);
	}	
}

void addCharContentToCurrentSection(SectionsCollection *sectionsCollection, char content)
{
	switch (sectionsCollection->currentSection)
	{
	case Text:
		addCharContentToSection(sectionsCollection->textDataSection, content);
		break;
	case RoData:
		addCharContentToSection(sectionsCollection->roDataSection, content);
		break;
	case Data:
		addCharContentToSection(sectionsCollection->dataDataSection, content);
		break;
	case Bss:
		addCharContentToSection(sectionsCollection->bssDataSection, content);
		break;
	}
}

void addIntContentToCurrentSection(SectionsCollection *sectionsCollection, int content)
{
	char firstByte = (char)content & 0xff;
	char secondByte = (char)(content >> 8) & 0xff;
	char thirdByte = (char)(content >> 16) & 0xff;
	char fourthByte = (char)(content >> 24) & 0xff;
	addCharContentToCurrentSection(sectionsCollection, firstByte);
	addCharContentToCurrentSection(sectionsCollection, secondByte);
	addCharContentToCurrentSection(sectionsCollection, thirdByte);
	addCharContentToCurrentSection(sectionsCollection, fourthByte);
}

void addWordContentToCurrentSection(SectionsCollection *sectionsCollection, int content)
{
	char firstByte = (char)content & 0xff;
	char secondByte = (char)(content >> 8) & 0xff;
	addCharContentToCurrentSection(sectionsCollection, firstByte);
	addCharContentToCurrentSection(sectionsCollection, secondByte);
}
