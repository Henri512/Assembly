#pragma once
#include "sections.h"

void parseAsciiDirectives(char *token, char hasZeroByte, SectionsCollection *sectionsCollection);

void parseExternDirective(char *token, SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList);

void addExternDirectiveToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, char *directiveValue);

void parseCharWordLongDirectives(char *token, SectionsCollection *sectionsCollection, char size);

void parseAlignDirective(char *token, SectionsCollection *sectionsCollection);

void parseSkipDirective(char *token, SectionsCollection *sectionsCollection);

void parseGlobalDirective(char *token, SymbolTableEntryList *symbolTableEntryList);

void parseCharWordLongDirectivesSP(SymbolTableEntryList *symbolTableEntryList, char *token, SectionsCollection *sectionsCollection, char size, char *oldLabel);

void parseAlignDirectiveSP(char *token, SectionsCollection *sectionsCollection);

void parseSkipDirectiveSP(char *token, SectionsCollection *sectionsCollection);

void validateAndAddCWLDirectiveContent(SectionsCollection *sectionsCollection, char *directiveValue, char size, int length);

void addLabelOffsetToContent(SectionsCollection *sectionsCollection, int offset, char size);

void addDirectiveRelDataToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *label, enum SectionsEnum section, int offset);

void handleLabelInCharWordLongDirectives(SectionsCollection *sectionsCollection, int directiveSizeInBytes, enum SectionsEnum section, int labelNum);

void addLabelToContent(SectionsCollection *sectionsCollection, int offset, char size);

void parseAsciiDirectivesSP(char *token, char hasZeroByte, SectionsCollection *sectionsCollection);
