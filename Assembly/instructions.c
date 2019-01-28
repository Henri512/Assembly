#include <string.h>
#include "instructions.h"
#include "parser_helpers.h"
#include "constants.h"
#include "section_helpers.h"
#include "instruction_helpers.h"
#include "string_helpers.h"
#include "constants_helper.h"

// First pass methods
void parseInstruction(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	char byteCount;
	TokenList *tokenList = getInstructionsTokens(token);
	byteCount = getInstructionSize(tokenList);
	printf("\tInstruction %s has byteCount %d\n\r", token, byteCount);
	addToCurrentCollectionsCount(sectionsCollection, (int)byteCount);
}

// Second pass methods
void parseInstructionSecondPass(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	InstructionData *instructionData = getNewInstructionData();
	TokenList *tokenList = getInstructionsTokens(token);
	fillInstructionData(instructionData, tokenList, symbolTableEntryList, sectionsCollection);

	// addToCurrentCollectionsCount(sectionsCollection, instructionData->byteCount);
}

void fillInstructionData(InstructionData *instructionData, TokenList *tokenList, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection)
{
	instructionData->byteCount = getInstructionSize(tokenList);
	instructionData->name = getNewString(strlen(tokenList->tokens[0]) + 1);
	strcpy(instructionData->name, tokenList->tokens[0]);
	if (tokenList->size >= 2)
	{
		instructionData->dst = getNewString(strlen(tokenList->tokens[1]) + 1);
		strcpy(instructionData->dst, tokenList->tokens[1]);
	}
	if (tokenList->size == 3)
	{
		instructionData->src = getNewString(strlen(tokenList->tokens[2]) + 1);
		strcpy(instructionData->src, tokenList->tokens[2]);
	}

	// setovanje operacionog koda instrukcije
	setInstructionOpCode(instructionData);

	// setovanje tipa adresiranja za jedan ili oba operanda
	setIntructionOperandsAddressing(instructionData, symbolTableEntryList);

	// ako instrukcija ima 2 operanda
	if (isTwoOperandsInstruction(instructionData->opCode))
	{
		processTwoOperandsInstruction(instructionData, symbolTableEntryList, sectionsCollection);
	}
	// ako je u pitanju Jmp ili Ret instruckcija
	else if (isRetOrJmpInstruction(instructionData->opCode))
	{
		processRetOrJmpInstructions(instructionData, symbolTableEntryList, sectionsCollection);
	}
	// ako je u pitanju instrukcija sa jednim operandom, pop, push ili call
	else if (isOneOperandInstruction(instructionData->opCode))
	{
		processOneOperandInstructions(instructionData, symbolTableEntryList, sectionsCollection);
	}
	// inace je instrukcija bez operanda, iret
	else
	{
		processIRetInstruction(instructionData, symbolTableEntryList);
	}

	addInstructionToSectionContent(instructionData, symbolTableEntryList, sectionsCollection);
	freeInstructionData(instructionData);
}

void setIntructionOperandsAddressing(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList)
{
	if (instructionData->dst || instructionData->src)
	{
		if (instructionData->opCode == Ret || instructionData->opCode == Iret)
		{
			printf("Instrukcije \'ret\' i \'iret\' ne mogu sadrzati operande! Kraj izvrsavanja!\n\r");
			exit(-1);
		}
		if (instructionData->dst)
		{
			setIntructionOperandAddressing(instructionData, symbolTableEntryList, Dst);
		}
		if (instructionData->src)
		{
			setIntructionOperandAddressing(instructionData, symbolTableEntryList, Src);
			if (instructionData->dstAddresingCode != RegDir && instructionData->srcAddressingCode != RegDir)
			{
				printf("U instrukciji \'%s %s, %s\' oba operanda zahtevaju dodatna 2 bajta a to nije dozvoljeno! Kraj izvrsavanja!\n\r",
					instructionData->name, instructionData->dst, instructionData->src);
				exit(-1);
			}
		}
	}
}

void processTwoOperandsInstruction(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection)
{
	// dst
	processOneOperand(instructionData, symbolTableEntryList, sectionsCollection, 0);
	// src
	processOneOperand(instructionData, symbolTableEntryList, sectionsCollection, 1);
}

void processOneOperand(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char operandIndex)
{
	char *additionalValue = NULL;
	SymbolTableEntry *symbol = NULL;
	char *op = operandIndex == 0 ? instructionData->dst : instructionData->src;
	char addrCode = operandIndex == 0 ? instructionData->dstAddresingCode : instructionData->srcAddressingCode;
	char isOperandPsw = !strcmp(op, "psw");
	char opLength = strlen(op);
	// parse second operand
	switch (addrCode)
	{
	case Imm:
		if (operandIndex == 0 && instructionData->opCode != Test && instructionData->opCode != Cmp)
		{
			printf("Instrukcija \'%s %s, %s\' sadrzi neposredno adresiranje za prvi operand a to nije dozvoljeno! Kraj izvrsavanja!\n\r",
				instructionData->name, instructionData->dst, instructionData->src);
			exit(-1);
		}
		// add r5, &label
		if (op[0] == '&')
		{
			additionalValue = getNewString(opLength);
			strcpy(additionalValue, op + 1);
			if (isOperandNumber(additionalValue))
			{
				printf("Instrukcija \'%s %s\' ne sadrzi validan neposredni operand!\n\r", instructionData->name, op);
				exit(-1);
			}
			// ako je operand psw i adresiranje Imm setujemo da se koristi registar r7
			if (!strcmp(additionalValue, "psw"))
			{
				free(op);
				op = getNewString(3);
				strcpy(op, "r7");
				if (operandIndex == 0)
				{
					instructionData->dst = op;
				}
				else
				{
					instructionData->src = op;
				}
			}
			else
			{
				processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
			}
			free(additionalValue);
		}
		// cmp r3, 20
		else
		{
			if (!strcmp(op, "psw"))
			{
				printf("Instrukcija \'%s %s\' ne sadrzi validan neposredni operand!\n\r", instructionData->name, op);
				exit(-1);
			}
			instructionData->additionalWord = getOperandNumericValue(op);
		}
		break;
	case RegInd:
		// and 20, &label
		if (op[0] == '&')
		{
			additionalValue = getNewString(opLength);
			strcpy(additionalValue, op + 1);
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		// or r4, r5[20]; mul 10, r4[label]
		else
		{
			additionalValue = getNewString(opLength - 3);
			strncpy(additionalValue, op + 3, opLength - 4);
			additionalValue[opLength - 4] = '\0';
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		free(additionalValue);
		break;
	case Mem:
		// add *20, r5
		if (op[0] == '*')
		{
			additionalValue = getNewString(opLength);
			strcpy(additionalValue, op + 1);
			if (!isOperandNumber(additionalValue))
			{
				printf("Instrukcija \'%s %s\' ne sadrzi validan operand!\n\r", instructionData->name, op);
				exit(-1);
			}
			instructionData->additionalWord = getOperandNumericValue(additionalValue);
		}
		// add label, r5
		else
		{
			additionalValue = getNewString(opLength + 1);
			strcpy(additionalValue, op);
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		free(additionalValue);
		break;
	}
}

void processRetOrJmpInstructions(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection)
{
	char *label = NULL;
	// instrukcija se menja sa pop pc tj pop r7
	if (instructionData->opCode == Ret)
	{
		if (instructionData->dst)
		{
			printf("Instrukcija \'ret\' ne moze sadrzati operande!\n\r");
			exit(-1);
		}
		instructionData->opCode = Pop;
		instructionData->dst = getNewString(3);
		strcpy(instructionData->dst, PC);
		instructionData->byteCount = 2;
	}
	// instrukcija se menja sa add ili mov
	// TODO
	else
	{
		// jmp $label ili jmp r7[label] = add r7, &label
		if (instructionData->dstAddresingCode == RegInd &&
			(instructionData->dst[0] == '$' || (instructionData->dst[0] == 'r' && instructionData->dst[1] == '7')))
		{
			instructionData->opCode = Add;
			// jmp $label = add r7, &label
			if (instructionData->dst[0] == '$')
			{
				label = getNewString(strlen(instructionData->dst));
				strcpy(label, instructionData->dst + 1);
				processLabelValue(label, instructionData, symbolTableEntryList, sectionsCollection, 1);
				instructionData->src = getNewString(strlen(instructionData->dst) + 1);
				instructionData->src[0] = '&';
				strcpy(instructionData->src + 1, label);
				free(label);
			}
			else
			{
				label = getNewString(strlen(instructionData->dst) - 3);
				strncpy(label, instructionData->dst + 3, strlen(instructionData->dst) - 3);
				label[strlen(instructionData->dst) - 4] = '\0';
				// jmp r7[20] = add r7, 20
				if (isOperandNumber(label))
				{
					instructionData->additionalWord = (getOperandNumericValue(label) & 0xFFFF);
					instructionData->src = label;
				}
				// jmp r7[label] = add r7, &label
				else
				{
					processLabelValue(label, instructionData, symbolTableEntryList, sectionsCollection, 1);
					instructionData->src = getNewString(strlen(label) + 2);
					instructionData->src[0] = '&';
					strcpy(instructionData->src + 1, label);
					free(label);
				}
			}
		}
		// jmp *50
		else if (instructionData->dst[0] == '*')
		{
			label = getNewString(strlen(instructionData->dst));
			strcpy(label, instructionData->dst + 1);
			instructionData->additionalWord = (getOperandNumericValue(label) & 0xFFFF);
			instructionData->src = label;
		}
		// jmp 50 = mov r7, 50; jmp &label = mov r7, &label
		else
		{
			instructionData->opCode = Mov;
			// jmp &label = mov r7, &label
			if (instructionData->dst[0] == '&')
			{
				label = getNewString(strlen(instructionData->dst));
				strcpy(label, instructionData->dst + 1);
				processLabelValue(label, instructionData, symbolTableEntryList, sectionsCollection, 0);
				instructionData->src = getNewString(strlen(instructionData->dst) + 2);
				instructionData->src[0] = '&';
				strcpy(instructionData->src + 1, label);
				free(label);
			}
			// jmp 50 = mov r7, 50;
			else if (isOperandNumber(instructionData->dst))
			{
				instructionData->additionalWord = (getOperandNumericValue(label) & 0xFFFF);
				instructionData->src = label;
			}
			// jmp label = invalid
			else
			{
				printf("Instrukcija \'%s %s\' nije validna!\n\r", instructionData->name, instructionData->dst);
				exit(-1);
			}
		}
		instructionData->srcAddressingCode = Imm;
		instructionData->dst = getNewString(3);
		strcpy(instructionData->dst, "r7");
		instructionData->dstAddresingCode = RegDir;
	}
}

// Push, Pop, Call
void processOneOperandInstructions(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection)
{
	char isOperandPsw = !strcmp(instructionData->dst, "&psw");
	char length = strlen(instructionData->dst);
	char *additionalValue = NULL;
	SymbolTableEntry *symbol = NULL;
	char *operand = instructionData->dst;
	// SymbolTableEntry *symbol = NULL;
	switch (instructionData->dstAddresingCode)
	{
		// pop &psw, push 20, call 10, push &label
	case Imm:
		// ako je pop instrukcija sa neposrednim operandom, jedino je dozvoljeno 'pop &psw'
		if (instructionData->opCode == Pop && !isOperandPsw)
		{
			printf("Instrukcija pop ne moze sadrzati neposredni operand!\n\r");
			exit(-1);
		}
		// push 20
		if (!isOperandPsw && operand[0] != '&')
		{
			int operandNum = getOperandNumericValue(operand);
			instructionData->additionalWord = operandNum;
		}
		// call &label, push &psw
		else
		{
			additionalValue = getNewString(length);
			strcpy(additionalValue, operand + 1);
			if (isOperandNumber(additionalValue))
			{
				printf("Instrukcija \'%s %s\'ne sadrzi validan neposredni operand!\n\r", instructionData->name, operand);
				exit(-1);
			}
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
			free(additionalValue);
		}
		break;
	case RegInd:
		// pop &label, push &label
		if (operand[0] == '&')
		{
			additionalValue = getNewString(length);
			strcpy(additionalValue, operand + 1);
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		// call r5[20], call r4[label]
		else
		{
			additionalValue = getNewString(length - 3);
			strncpy(additionalValue, operand + 3, length - 4);
			additionalValue[length - 4] = '\0';
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		free(additionalValue);
		break;
		// push label, pop label, call *20
	case Mem:
		additionalValue = getNewString(length);
		if (operand[0] == '*')
		{
			additionalValue = getNewString(length);
			strcpy(additionalValue, operand + 1);
			if (!isOperandNumber(additionalValue))
			{
				printf("Instrukcija \'%s %s\' ne sadrzi validan operand!\n\r", instructionData->name, operand);
				exit(-1);
			}
			instructionData->additionalWord = getOperandNumericValue(additionalValue);
		}
		else
		{
			strcpy(additionalValue, operand, length);
			processLabelValue(additionalValue, instructionData, symbolTableEntryList, sectionsCollection, 0);
		}
		free(additionalValue);
		break;
	}
}

void processLabelValue(char *labelValue, InstructionData *instructionData,
	SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char isPcRelAddr)
{
	SymbolTableEntry *symbol = NULL, *sectionSymbol = NULL;
	char *tmpLabel = NULL;
	int offset = getCurrentOffset(sectionsCollection);
	if (isOperandNumber(labelValue))
	{
		instructionData->additionalWord = getOperandNumericValue(labelValue);
	}
	else
	{
		symbol = getSymbolByName(symbolTableEntryList, labelValue);
		sectionSymbol = getSymbolByName(symbolTableEntryList, getSectionValue(symbol ? symbol->section : Unknown));
		// ako simbol postoji gledamo da li je lokalni ili globalni
		if (symbol)
		{
			// ako simbol nije iz tekuce sekcije
			if (symbol->section != sectionsCollection->currentSection)
			{
				// ako se radi o pc relativnom adresiranju - jmp $label
				if (isPcRelAddr)
				{
					switch (symbol->sectionType)
					{
					case Local:
						instructionData->additionalWord = symbol->offset - WORDSIZE;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, PCREL, sectionSymbol->num);
						break;
					case Global:
						instructionData->additionalWord = -WORDSIZE;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, PCREL, symbol->num);
						break;
					case Unknown:
						instructionData->additionalWord = -WORDSIZE;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, PCREL, sectionSymbol->num);
						break;
					}
				}
				// ako nije pc relativno adresiranje onda se pravi apsolutna relokacija
				else
				{
					switch (symbol->sectionType)
					{
					case Local:
						instructionData->additionalWord = symbol->offset;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, ABS, sectionSymbol->num);
						break;
					case Global:
						instructionData->additionalWord = 0;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, ABS, symbol->num);
						break;
					case Unknown:
						instructionData->additionalWord = 0;
						addNewRelocationData(sectionsCollection, symbol->section, offset + WORDSIZE, ABS, symbol->num);
						break;
					}
				}
			}
		}
		// ako simbol nije u tabeli simbola dodajemo ga kao globalni i pravimo realokacioni zapis
		else
		{
			// handle non existing labels handle(symbolTableEntryList)
			tmpLabel = getNewString(strlen(labelValue) + 1);
			strcpy(tmpLabel, labelValue);
			int num = addNewRelDataToSymbolTableList(symbolTableEntryList, sectionsCollection, tmpLabel, Unknown, UNKNOWNLABELOFFSET);
			if (isPcRelAddr)
			{
				instructionData->additionalWord = -WORDSIZE;
				addNewRelocationData(sectionsCollection, Unknown, offset + WORDSIZE, PCREL, sectionSymbol->num);
			}
			else
			{
				instructionData->additionalWord = 0;
				addNewRelocationData(sectionsCollection, Unknown, offset + WORDSIZE, ABS, num);
			}
		}
	}
}

void processIRetInstruction(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList)
{
	// nista se ne radi, vec je sve namesteno za upis instrukcije u sekciju
}

void addInstructionToSectionContent(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection)
{
	int content = 0;
	char hasFirstOperand = instructionData->dst != NULL;
	char hasSecondOperand = instructionData->src != NULL;
	// first two bits - condition
	content = content | (instructionData->condition & 0x03);

	// bits 2-5 - opCode
	content = content | ((instructionData->opCode << 2) & 0x3C);

	// add operands if they exist
	if (hasFirstOperand)
	{
		content = content | ((instructionData->dstAddresingCode & 0x3) << 9);
		content = addOperandToContent(instructionData, content, 0);
	}
	if (hasSecondOperand)
	{
		content = content | ((instructionData->srcAddressingCode & 0x3) << 14);
		content = addOperandToContent(instructionData, content, 1);
	}

	if (instructionData->byteCount == 2)
	{
		addWordContentToCurrentSection(sectionsCollection, content);
	}
	else
	{
		addIntContentToCurrentSection(sectionsCollection, content);
	}
}

int addOperandToContent(InstructionData *instructionData, int content, char operandIndex)
{
	char *operand = operandIndex == 0 ? instructionData->dst : instructionData->src;
	enum AddressingCodes addrCode = operandIndex == 0 ? instructionData->dstAddresingCode : instructionData->srcAddressingCode;
	char shift = operandIndex == 0 ? 6 : 11, regNum = 0;
	switch (addrCode)
	{
	case Imm:
		if (!strcmp(operand, "&psw"))
		{
			content = content | ((7 & 0x7) << shift);
		}
		else
		{
			content = content | ((instructionData->additionalWord & 0xFFFF) << 16);
		}
		break;
	case RegDir:
		regNum = operand[0] == 'r' ? operand[1] - '0' : (strcmp(operand, "pc") ? 7 : 6);
		content = content | ((regNum & 0x7) << shift);
		break;
	case Mem:
		content = content | ((instructionData->additionalWord & 0xFFFF) << 16);
		break;
	case RegInd:
		regNum = operand[0] == 'r' ? operand[1] - '0' : (startsWith("pc", operand) ? 7 : 6);
		content = content | ((regNum & 0x7) << shift);
		content = content | ((instructionData->additionalWord & 0xFFFF) << 16);
		break;
	}
	return content;
}

int addNewRelDataToSymbolTableList(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection,
	char *label, enum SectionEnum section, int offset)
{
	char *newLabel = NULL;
	int num = -1;
	if (!labelExists(symbolTableEntryList, label))
	{
		newLabel = getNewString(strlen(label) + 1);
		strcpy(newLabel, label);
		SymbolTableEntry *newSymbolTableEntry = makeSymbolTableEntry(newLabel, section, offset);
		newSymbolTableEntry->sectionType = Global;

		addSymbolTableEntry(symbolTableEntryList, newSymbolTableEntry);
		num = newSymbolTableEntry->num;
	}
	return num;
}