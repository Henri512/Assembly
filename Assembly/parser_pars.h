#pragma once
#include "sections.h"

void executeFirstPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *inputFilePath);

void *parseTokenFirstPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void parseSection(SectionsCollection *sectionsCollection, char section);

void *parseLabel(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void *addLabelToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *label);

void parseDirective(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);
