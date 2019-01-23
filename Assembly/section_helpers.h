#pragma once
#include "sections.h"

char isTokenLabel(char *token);

char isTokenDirective(char *token);

void initializeSectionIfEmpty(SectionsCollection *sectionsCollection, int index, int start, int size);

char getTokenSectionValue(char *token);

char getTokenInstructionValue(char *token);

char *getInstructionFromToken(char *token);
