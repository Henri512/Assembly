#include "sections.h"
#include <stdlib.h>
#include <stdio.h>

// SymbolTableEntry methods
SymbolTableEntry* makeSymbolTableEntry(char* name, char sectionType, int offset)
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
	symbolTableEntry->sectionType = sectionType;
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
RelocationData* makeRelocationData(char sectionType, int offset, char relocationType)
{
	RelocationData *relocationData = (RelocationData*)malloc(sizeof(RelocationData));
	if (!relocationData)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	relocationData->sectionType = sectionType;
	relocationData->offset = offset;
	relocationData->relocationType = relocationType;

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

int RelocationDataSize(RelocationData* list)
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

void freeRelocationDataList(RelocationData * list)
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
SectionContent * makeSectionContent(char * content, int size)
{
	SectionContent *sectionContent;

	sectionContent = malloc(sizeof(SectionContent));
	if (!sectionContent)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	sectionContent->content = content;
	sectionContent->size = size;

	sectionContent->next = NULL;
	return sectionContent;
}

SectionContent* getSectionContentByIndex(SectionContent* list, int index)
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

	sectionData->index = index;
	sectionData->start = start;
	sectionData->size = size;
	sectionData->content = content;

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
	newSectionsCollection->bssDataSection = NULL;
	newSectionsCollection->dataDataSection = NULL;
	// newSectionsCollection->relocationDataSection = NULL;
	newSectionsCollection->roDataSection = NULL;
	newSectionsCollection->textDataSection = NULL;
	newSectionsCollection->currentSection = Text;

	return newSectionsCollection;
}

void freeSectionsCollection(SectionsCollection* sectionsCollection)
{
	if (sectionsCollection != NULL)
	{
		free(sectionsCollection->bssDataSection);
		free(sectionsCollection->dataDataSection);
		// free(sectionsCollection->relocationDataSection);
		free(sectionsCollection->roDataSection);
		free(sectionsCollection->textDataSection);
		free(sectionsCollection);
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
	default:

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
	default:

		break;
	}
	return counter;
}
