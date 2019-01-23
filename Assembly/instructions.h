#pragma once
#include "sections.h"
#include "parser_helpers.h"

void parseInstruction(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

char getInstructionSize(TokenList *tokenList);

char isRegOperand(char *operand);
