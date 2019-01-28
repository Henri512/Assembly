#pragma once
#include "constants.h"
#include "instructions.h"
#include "parser_helpers.h"
#include "instructions.h"

char getInstructionSize(TokenList *tokenList);

char isRegOperand(char *operand);

char isTwoOperandsInstruction(enum OpCodes opCode);

char isOneOperandInstruction(enum OpCodes opCode);

char isRetOrJmpInstruction(enum OpCodes opCode);

InstructionData *getNewInstructionData();

void freeInstructionData(InstructionData *instructionData);

int getOperandNumericValue(char *operand);

char isOperandNumber(char *operand);

void setInstructionOpCode(InstructionData *instructionData);

void addNewRelocationData(SectionsCollection *sectionsCollection, enum SectionsEnum sectionType, int offset, enum RelocationTypesEnum relType, int num);

void addNewRelocationDataToSection(SectionData *sectionData, RelocationData *relocationData);

void setIntructionOperandAddressing(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, enum OperandTypes operandType);
