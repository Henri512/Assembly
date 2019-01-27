#pragma once
#include "constants.h"

// SymbolTableEntry methods declaration
typedef struct symbolTableEntry
{
	char *name;
	enum SectionsEnum section;
	enum SectionType sectionType;
	int offset;
	int num;
	struct symbolTableEntry *next;
} SymbolTableEntry;

typedef struct symbolTableEntryList
{
	int count;
	struct symbolTableEntry *list;
} SymbolTableEntryList;

SymbolTableEntry* makeSymbolTableEntry(char *name, char section, int offset);

void addSymbolTableEntry(SymbolTableEntryList *list, SymbolTableEntry *entry);

SymbolTableEntry* getSymbolTableEntryByIndex(SymbolTableEntry *list, int index);

int SymbolTableEntrySize(SymbolTableEntry *list);

void freeSymbolTableEntryList(SymbolTableEntry *list);

// RelocationData methods declaration
typedef struct relocationData
{
	char sectionType;
	int offset;
	int size;
	enum RelocationTypesEnum relocationType;
	int num;
	struct relocationData *next;
} RelocationData;

RelocationData* makeRelocationData(char sectionType, int offset, char relocationType, int num);

RelocationData* getRelocationDataByIndex(RelocationData *list, int index);

int RelocationDataSize(RelocationData *list);

void freeRelocationDataList(RelocationData *list);

// SectionContent methods declaration
typedef struct sectionContent
{
	char content[SECTIONSIZE];
	int count;
	struct sectionContent *next;
} SectionContent;

SectionContent *makeNewSectionContent();

SectionContent *getSectionContentByIndex(SectionContent *list, int index);

int SectonContentSize(SectionContent *list);

void freeSectionContentList(SectionContent *list);

// SectionData methods declaration
typedef struct sectionData
{
	int index;
	int start;
	int size;
	char marked;
	struct sectionContent *content;
	struct relocationData *relData;
	struct sectionData *next;
} SectionData;

SectionData* makeSectionData(int index, int start, int size, SectionContent *content);

SectionData* getSectionDataByIndex(SectionData *list, int index);

int SectionDataSize(SectionData *list);

void freeSectionDataList(SectionData *list);

// SectionsCollection methods declaration
typedef struct sectionsCollection
{
	struct sectionData *dataDataSection;
	struct sectionData *roDataSection;
	struct sectionData *textDataSection;
	struct sectionData *bssDataSection;
	enum SectionsEnum currentSection;
} SectionsCollection;

SectionsCollection* getEmptySectionsCollection();

SymbolTableEntryList *getEmptySymbolTableEntryList();

void freeSectionsCollection(SectionsCollection *sectionsCollection);

void addCharContentToSection(SectionData *sectionData, char content);

SymbolTableEntry *getSymbolByName(SymbolTableEntryList *list, char *symbolName);

void setGlobal(char *symbolName, SymbolTableEntryList *symbolTableEntryList);
