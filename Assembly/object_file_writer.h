#pragma once

void writeSectionToFile(SectionData *sectionData, FILE *file);

void writeContentEntry(SectionContent *content, FILE *file);

void writeDataToFile(SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList, char *inputFileName);

void writeSymbolTableEntryList(SymbolTableEntryList *symbolTableEntryList, FILE *file);

void writeSymbolTableEntry(SymbolTableEntry *entry, FILE *file);
