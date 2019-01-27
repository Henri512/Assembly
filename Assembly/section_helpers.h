#pragma once
#include "sections.h"

char isTokenLabel(char *token);

char isTokenDirective(char *token);

void initializeSectionIfEmpty(SectionsCollection *sectionsCollection, int index, int start, int size);

char labelExists(SymbolTableEntryList *symbolTableEntryList, char *labelName);

SymbolTableEntry *getEntryByLabelName(SymbolTableEntryList *symbolTableEntryList, char *labelName);

char getTokenSectionValue(char *token);

char getTokenInstructionValue(char *token);

char *getInstructionFromToken(char *token);

void resetCounters(SectionsCollection *sectionsCollection);

void addToCurrentCollectionsCount(SectionsCollection *sectionsCollection, int size);

int getCurrentCollectionsCount(SectionsCollection *sectionsCollection);

int getCurrentOffset(SectionsCollection *sectionsCollection);

void parseSection(SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList, char section, char *token);

void addCharContentToCurrentSection(SectionsCollection *sectionsCollection, char content);

void addContentToCurrentSection(SectionsCollection *sectionsCollection, char *content, int size);

void addIntContentToCurrentSection(SectionsCollection *sectionsCollection, int content);

void addWordContentToCurrentSection(SectionsCollection *sectionsCollection, int content);
