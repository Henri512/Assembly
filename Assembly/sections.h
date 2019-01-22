#pragma once
#include "constants.h"

// SymbolTableEntry methods declaration
typedef struct symbolTableEntry
{
	char *name;
	char sectionType;
	int offset;
	int num;
	struct symbolTableEntry *next;
} SymbolTableEntry;

SymbolTableEntry* makeSymbolTableEntry(char *name, char sectionType, int offset, int num);

SymbolTableEntry* getSymbolTableEntryByIndex(SymbolTableEntry *list, int index);

int SymbolTableEntrySize(SymbolTableEntry *list);

void freeSymbolTableEntryList(SymbolTableEntry *list);

// RelocationData methods declaration
typedef struct relocationData
{
	char sectionType;
	int offset;
	int size;
	char relocationType;
	struct relocationData *next;
} RelocationData;

RelocationData* makeRelocationData(char sectionType, int offset, char relocationType);

RelocationData* getRelocationDataByIndex(RelocationData *list, int index);

int RelocationDataSize(RelocationData *list);

void freeRelocationDataList(RelocationData *list);

// SectionContent methods declaration
typedef struct sectionContent
{
	char *content;
	int size;
	struct sectionContent *next;
} SectionContent;

SectionContent* makeSectionContent(char *content, int size);

SectionContent* getSectionContentByIndex(SectionContent *list, int index);

int SectonContentSize(SectionContent *list);

void freeSectionContentList(SectionContent *list);

// SectionData methods declaration
typedef struct sectionData
{
	int index;
	int start;
	int size;
	struct sectionContent *content;
	struct sectionData *next;
} SectionData;

// SectionData *addSectionData(SectionData *list, SectionData *entry);

SectionData* makeSectionData(int index, int start, int size, SectionContent *content);

SectionData* getSectionDataByIndex(SectionData *list, int index);

int SectionDataSize(SectionData *list);

void freeSectionDataList(SectionData *list);

// SectionsCollection methods declaration
typedef struct sectionsCollection
{
	struct relocationData *relocationDataSection;
	struct sectionData *dataDataSection;
	struct sectionData *roDataSection;
	struct sectionData *textDataSection;
	struct sectionData *bssDataSection;
	enum Sections currentSection;
} SectionsCollection;

SectionsCollection* getEmptySectionsCollection();

void freeSectionsCollection(SectionsCollection *sectionsCollection);

void addToCurrentCollectionsCount(SectionsCollection *sectionsCollection, int size);

int getCurrentCollectionsCount(SectionsCollection *sectionsCollection);
