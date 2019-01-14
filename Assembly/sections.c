#include "sections.h"
#include <stdlib.h>
#include <stdio.h>

// SymbolTableEntry methods
SymbolTableEntry* makeSymbolTableEntry(char* name, char sectionType, int offset, int num)
{
	SymbolTableEntry *symbolTableEntry;
	if (!name || name == "")
	{
		printf("Naziv labele ne moze biti prazan!");
		exit(-1);
	}

	symbolTableEntry = malloc(sizeof(SymbolTableEntry));
	if (!symbolTableEntry)
	{
		exit(-1);
	}

	symbolTableEntry->name = name;
	symbolTableEntry->sectionType = sectionType;
	symbolTableEntry->offset = offset;
	symbolTableEntry->num = num;

	symbolTableEntry->next = NULL;
	return symbolTableEntry;
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
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!");
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
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!");
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
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!");
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
		printf("Doslo se do kraja liste, element sa zadatim indeksom ne postoji!");
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
	if (newSectionsCollection == NULL)
	{
		exit(-1);
	}
	return newSectionsCollection;
}

void freeSectionsCollection(SectionsCollection* sectionsCollection)
{
	if (sectionsCollection != NULL)
	{
		free(sectionsCollection->bssDataSection);
		free(sectionsCollection->dataDataSection);
		free(sectionsCollection->relocationDataSection);
		free(sectionsCollection->roDataSection);
		free(sectionsCollection->textDataSection);
		free(sectionsCollection);
	}
}
