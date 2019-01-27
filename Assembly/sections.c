#include "sections.h"
#include <stdlib.h>
#include <stdio.h>

// SymbolTableEntry methods
SymbolTableEntry* makeSymbolTableEntry(char* name, char section, int offset)
{
	SymbolTableEntry *symbolTableEntry;
	if (!name || name == "")
	{
		printf("Naziv labele ne moze biti prazan!\n\r");
		exit(-1);
	}

	symbolTableEntry = malloc(sizeof(SymbolTableEntry));
	if (!symbolTableEntry)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	symbolTableEntry->name = name;
	symbolTableEntry->section = section;
	symbolTableEntry->sectionType = Local;
	symbolTableEntry->offset = offset;

	symbolTableEntry->next = NULL;
	return symbolTableEntry;
}

void addSymbolTableEntry(SymbolTableEntryList * list, SymbolTableEntry * entry)
{
	SymbolTableEntry *last = list->list;
	if (last == NULL)
	{
		entry->num = 0;
		list->list = entry;
		list->count = 1;
	}
	else
	{
		while (last->next)
		{
			last = last->next;
		}

		entry->num = list->count;
		list->count++;
		last->next = entry;
	}
}

SymbolTableEntryList *getEmptySymbolTableEntryList()
{
	SymbolTableEntryList *newSymbolTableEntryList = (SymbolTableEntryList*)malloc(sizeof(SymbolTableEntryList));
	if (!newSymbolTableEntryList)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	newSymbolTableEntryList->count = 0;
	newSymbolTableEntryList->list = NULL;
}

SymbolTableEntry* getSymbolTableEntryByIndex(SymbolTableEntry* list, int index)
{
	if (!list)
	{
		exit(-1);
	}

	int counter = 0;
	while (counter < index && list->next)
	{
		counter++;
		list = list->next;
	}

	if (!list->next && counter < index)
	{
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!\n\r");
		exit(-1);
	}
	else
	{
		return list;
	}
}

int SymbolTableEntrySize(SymbolTableEntry* list)
{
	int size = 0;
	if (list)
	{
		while (list)
		{
			size++;
			list = list->next;
		}
	}
	return size;
}

void freeSymbolTableEntryList(SymbolTableEntry* list)
{
	SymbolTableEntry *old;
	while (list)
	{
		old = list;
		list = list->next;
		free(old->name);
		free(old);
	}
}

// RelocationData methods
RelocationData* makeRelocationData(char sectionType, int offset, char relocationType, int num)
{
	RelocationData *relocationData = (RelocationData*)malloc(sizeof(RelocationData));
	if (!relocationData)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	relocationData->sectionType = sectionType == Local ? 'l' : 'g';
	relocationData->offset = offset;
	relocationData->relocationType = relocationType;
	relocationData->num = num;
	relocationData->size = 0;

	relocationData->next = NULL;
	return relocationData;
}

RelocationData* getRelocationDataByIndex(RelocationData* list, int index)
{
	if (!list)
	{
		exit(-1);
	}

	int counter = 0;
	while (counter < index && list->next)
	{
		counter++;
		list = list->next;
	}

	if (!list->next && counter < index)
	{
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!\n\r");
		exit(-1);
	}
	else
	{
		return list;
	}
}

int RelocationDataSize(RelocationData *list)
{
	int size = 0;
	if (list)
	{
		while (list)
		{
			size++;
			list = list->next;
		}
	}
	return size;
}

void freeRelocationDataList(RelocationData *list)
{
	RelocationData *old;
	while (list)
	{
		old = list;
		list = list->next;
		free(old);
	}
}

// SectionContent methods

SectionContent* makeNewSectionContent()
{
	SectionContent *newContent = (SectionContent*)malloc(sizeof(SectionContent));
	if (!newContent)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	// memset(newContent->content, '\0', SECTIONSIZE);
	newContent->count = 0;
	newContent->next = NULL;
	return newContent;
}

SectionContent *getSectionContentByIndex(SectionContent *list, int index)
{
	if (!list)
	{
		exit(-1);
	}

	int counter = 0;
	while (counter < index && list->next)
	{
		counter++;
		list = list->next;
	}

	if (!list->next && counter < index)
	{
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!\n\r");
		exit(-1);
	}
	else
	{
		return list;
	}
}

int SectonContentSize(SectionContent * list)
{
	int size = 0;
	if (list)
	{
		while (list)
		{
			size++;
			list = list->next;
		}
	}
	return size;
}

void freeSectionContentList(SectionContent * list)
{
	SectionContent *old;
	while (list)
	{
		old = list;
		list = list->next;
		free(old);
	}
}

// SectionData methods
SectionData* makeSectionData(int index, int start, int size, SectionContent *content)
{
	SectionData *sectionData = (SectionData*)malloc(sizeof(SectionData));
	if (!sectionData)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	if (!content)
	{
		sectionData->content = makeNewSectionContent();
	}
	else
	{
		sectionData->content = content;
	}

	sectionData->index = index;
	sectionData->start = start;
	sectionData->size = size;
	sectionData->marked = 0;

	sectionData->relData = NULL;
	sectionData->next = NULL;
	return sectionData;
}

SectionData* getSectionDataByIndex(SectionData* list, int index)
{
	if (!list)
	{
		exit(-1);
	}

	int counter = 0;
	while (counter < index && list->next)
	{
		counter++;
		list = list->next;
	}

	if (!list->next && counter < index)
	{
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!\n\r");
		exit(-1);
	}
	else
	{
		return list;
	}
}

int SectionDataSize(SectionData* list)
{
	int size = 0;
	if (list)
	{
		while (list)
		{
			size++;
			list = list->next;
		}
	}
	return size;
}

void freeSectionDataList(SectionData* list)
{
	SectionData *old;
	while (list)
	{
		old = list;
		list = list->next;
		free(old->content);
		free(old);
	}
}

SectionsCollection* getEmptySectionsCollection()
{
	SectionsCollection *newSectionsCollection = (SectionsCollection*)malloc(sizeof(SectionsCollection));
	if (!newSectionsCollection)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	newSectionsCollection->textDataSection = makeSectionData(Text, 0, 0, NULL);
	newSectionsCollection->bssDataSection = makeSectionData(Bss, 0, 0, NULL);
	newSectionsCollection->dataDataSection = makeSectionData(Data, 0, 0, NULL);
	newSectionsCollection->roDataSection = makeSectionData(RoData, 0, 0, NULL);
	newSectionsCollection->currentSection = Text;

	return newSectionsCollection;
}

void freeSectionsCollection(SectionsCollection* sectionsCollection)
{
	if (sectionsCollection != NULL)
	{
		free(sectionsCollection->bssDataSection);
		free(sectionsCollection->dataDataSection);
		free(sectionsCollection->roDataSection);
		free(sectionsCollection->textDataSection);
		free(sectionsCollection);
	}
}

void addCharContentToSection(SectionData *sectionData, char content)
{
	// ako content nije inicijalizovan
	if (!sectionData->content)
	{
		sectionData->content = makeNewSectionContent();
		sectionData->content->content[sectionData->content->count++] = content;
	}

	SectionContent *sectionContent = sectionData->content;

	// ako ima mesta u prvom content-u
	if (sectionContent->count < SECTIONSIZE)
	{
		sectionContent->content[sectionContent->count++] = content;
	}
	// ako je prvi pun a sledeci nije inicijalizovan
	else if (!sectionContent->next)
	{
		sectionContent->next = makeNewSectionContent();
		sectionContent = sectionContent->next;
		sectionContent->content[sectionContent->count++] = content;
	}
	// ako nema mesta u prvom content-u trazi se sledeci gde ima mesta ili ce se napraviti sledeci
	else if (sectionContent->count == SECTIONSIZE && sectionContent->next)
	{
		while (sectionContent->count == SECTIONSIZE && sectionContent->next)
		{
			sectionContent = sectionContent->next;
		}
		if (sectionContent->count < SECTIONSIZE)
		{
			sectionContent->content[sectionContent->count++] = content;
		}
		else
		{
			sectionContent->next = makeNewSectionContent();
			sectionContent = sectionContent->next;
			sectionContent->content[sectionContent->count++] = content;
		}
	}
	sectionData->size++;
}

SymbolTableEntry *getSymbolByName(SymbolTableEntryList* list, char *symbolName)
{
	SymbolTableEntry *symbol = list->list;
	while (symbol && strcmp(symbol->name, symbolName))
	{
		symbol = symbol->next;
	}
	return symbol;
}

void setGlobal(char *symbolName, SymbolTableEntryList *symbolTableEntryList)
{
	SymbolTableEntry *symbolEntry = getSymbolByName(symbolTableEntryList, symbolName);
	if (symbolEntry)
	{
		symbolEntry->sectionType = Global;
	}
	else
	{
		printf("Doslo se do kraja liste, simbol sa nazivom \'%s\' ne postoji!\n\r", symbolName);
		exit(-1);
	}
}