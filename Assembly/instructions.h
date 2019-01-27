#pragma once
#include "sections.h"
#include "parser_helpers.h"
#include "constants.h"

typedef struct instructionData
{
	enum Instructions opCode;
	enum ConditionCodes condition;
	char byteCount;
	char *name;
	char *dst;
	char *src;
	enum AddressingCodes dstAddresingCode;
	enum AddressingCodes srcAddressingCode;
	int additionalWord;
} InstructionData;

void parseInstruction(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void parseInstructionSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token);

void fillInstructionData(InstructionData *instructionData, TokenList *tokenList, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection);

void addInstructionToSectionContent(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection);

void setIntructionOperandsAddressing(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList);

void processTwoOperandsInstruction(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection);

void processOneOperand(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char operandIndex);

void processRetOrJmpInstructions(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection);

void processOneOperandInstructions(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection);

void processIRetInstruction(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList);

void processLabelValue(char *labelValue, InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char isPcRelAddr);

void addOperandToContent(InstructionData *instructionData, int content, char operandIndex);

void addNewRelDataToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char * label, enum SectionEnum section, int offset);
