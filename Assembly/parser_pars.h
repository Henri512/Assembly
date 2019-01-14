#pragma once
#include "sections.h"

void executeFirstPass(SymbolTableEntry *symbolTableEntry, SectionsCollection *sectionsCollection, char *inputFilePath);

void parseTokenFirstPass(SymbolTableEntry *symbolTableEntry, SectionsCollection *sectionsCollection, char *token);

void parseSection(SectionsCollection *sectionsCollection, char *token);

void parseLabel(SymbolTableEntry *symbolTableEntry, SectionsCollection *sectionsCollection, char *token);
