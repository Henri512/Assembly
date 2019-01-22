#pragma once
#include "sections.h"

void parseAsciiDirectives(char *token, char hasZeroByte, SectionsCollection *sectionsCollection);

void parseCharWordLongDirectives(char *token, SectionsCollection *sectionsCollection, char size);

void parseCharDirective(char *token, SectionsCollection *sectionsCollection);

void parseWordDirective(char *token, SectionsCollection *sectionsCollection);

void parseAlignDirective(char *token, SectionsCollection *sectionsCollection);

void parseSkipDirective(char *token, SectionsCollection *sectionsCollection);