#pragma once
#include "sections.h"

void executeSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *inputFilePath);

void *parseTokenSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void *parseLabelSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void parseDirectiveSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);