#pragma once
#include "sections.h"

char isTokenLabel(char *token);

char isTokenDirective(char *token);

int getLabelTokenSize(char *label);

void initializeSectionIfEmpty(SectionsCollection *sectionsCollection, int index, int start, int size);
