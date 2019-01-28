#include "instructions.h"
#include "instruction_helpers.h"
#include "string_helpers.h"

char getInstructionSize(TokenList *tokenList)
{
	char *dst = NULL, *src = NULL;
	char byteCount = 2, isSrcRegOrPsw = 0, isDstReg = 0;
	if (tokenList->size >= 2)
	{
		dst = strtok(tokenList->tokens[1], ",");
		isDstReg = isRegOperand(dst);
		byteCount = isDstReg ? 2 : 4;
	}
	if (tokenList->size == 3)
	{
		src = tokenList->tokens[2];
		isSrcRegOrPsw = (isRegOperand(src) || !strcmp(src, "psw"));
		byteCount = isSrcRegOrPsw ? byteCount : 4;
	}
	return byteCount;
}

char isRegOperand(char *operand)
{
	return strlen(operand) == 2 &&
		((operand[0] == 'r' || (operand[1] >= '0' && operand[1] <= '7')
			|| operand == "sp" || operand == "pc"));
}

char isTwoOperandsInstruction(enum OpCodes opCode)
{
	return (opCode >= Add && opCode <= Not) || (opCode >= Mov && opCode <= Shr);
}

char isOneOperandInstruction(enum OpCodes opCode)
{
	return opCode >= 9 && opCode <= 11;
}

char isRetOrJmpInstruction(enum OpCodes opCode)
{
	return opCode >= Ret;
}

InstructionData *getNewInstructionData()
{
	InstructionData *instructionData = (InstructionData*)malloc(sizeof(InstructionData));
	if (instructionData == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}

	instructionData->additionalWord = 0;
	instructionData->byteCount = 0;
	instructionData->dst = NULL;
	instructionData->src = NULL;
	instructionData->name = NULL;
	instructionData->opCode = 0;
	instructionData->dstAddresingCode = Imm;
	instructionData->srcAddressingCode = Imm;
	instructionData->condition = Eq;
	return instructionData;
}

void freeInstructionData(InstructionData *instructionData)
{
	if (instructionData)
	{
		if (instructionData->name)
		{
			free(instructionData->name);
		}
		if (instructionData->dst)
		{
			free(instructionData->dst);
		}
		if (instructionData->src)
		{
			free(instructionData->src);
		}
	}
}

int getOperandNumericValue(char *operand)
{
	int num;
	char *pEnd;
	num = strtol(operand, &pEnd, 0);
	return num;
}

char isOperandNumber(char *operand)
{
	char isNum = 1, i;
	for (i = 0; i < strlen(operand); i++)
	{
		if (operand[i] < '0' || operand[i] > '9')
		{
			isNum = 0;
		}
	}
	return isNum;
}

void setIntructionOperandAddressing(InstructionData *instructionData, SymbolTableEntryList *symbolTableEntryList, enum OperandTypes operandType)
{
	int length = 0;
	enum AddressingCodes operandAddressing = Imm;
	char *operand = operandType == Dst ? instructionData->dst : instructionData->src;
	char opCode = instructionData->opCode;
	length = strlen(operand);
	if (operand)
	{
		// push 20; add r0, &x
		if (isOperandNumber(operand) || operand[0] == '&')
		{
			if (operandType == Dst &&
				opCode != Cmp && opCode != Test && 
					opCode != Push && opCode != Call && opCode != Jmp)
			{
				printf("Odredisni operand ne moze biti neposredna vrednost!\n\r");
				exit(-1);
			}
			else
			{
				operandAddressing = Imm;
			}
		}
		else if (!strcmp(operand, "&psw"))
		{
			operandAddressing = Imm;
		}
		else if (!strcmp(operand, "psw"))
		{
			printf("\'psw\' nije validan operand (&psw jeste)!\n\r");
			exit(-1);
		}
		// r0-r7 ili sp
		else if ((length == 2 && operand[0] == 'r'
			&& operand[1] >= '0' && operand[1] <= '7')
			|| (length == 2 && (!strcmp(operand, "sp") || !strcmp(operand, "pc"))))
		{
			operandAddressing = RegDir;
		}
		// r0-r7 [x] ili sp[x] ili $x
		else if ((length >= 5 &&
			((operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7') || (operand[0] == 's' && operand[1] == 'p'))
			&& operand[2] == '[' && operand[length - 1] == ']')
			|| (length >= 2 && operand[0] == PCRELINDWILDCARD))
		{
			operandAddressing = RegInd;
		}
		// &x ili *20 ili x
		else
		{
			operandAddressing = Mem;
		}

		if (operandType == Dst)
		{
			instructionData->dstAddresingCode = operandAddressing;
		}
		else
		{
			instructionData->srcAddressingCode = operandAddressing;
		}
	}
}

void setInstructionOpCode(InstructionData *instructionData)
{
	char *instruction = instructionData->name;
	int length = strlen(instruction);
	for (int i = 0; i < INSTRUCTIONSCOUNT; i++)
	{
		if (startsWith(Instructions[i], instruction))
		{
			if (!strcmp(instruction + length - 3, "_eq"))
			{
				instructionData->opCode = i;
				instructionData->condition = Eq;
			}
			else if (!strcmp(instruction + length - 3, "_ne"))
			{
				instructionData->opCode = i;
				instructionData->condition = Ne;
			}
			else if (!strcmp(instruction + length - 3, "_gt"))
			{
				instructionData->opCode = i;
				instructionData->condition = Gt;
			}
			else if (!strcmp(Instructions[i], instruction))
			{
				instructionData->opCode = i;
				instructionData->condition = Al;
			}
		}
	}
}

void addNewRelocationData(SectionsCollection *sectionsCollection, enum SectionsEnum sectionType, int offset, enum RelocationTypes relType, int num)
{
	RelocationData *relData = makeRelocationData(sectionType, offset, relType, num);
	
	switch (sectionsCollection->currentSection)
	{
	case Text:
		addNewRelocationDataToSection(sectionsCollection->textDataSection, relData);
		break;
	case RoData:
		addNewRelocationDataToSection(sectionsCollection->roDataSection, relData);
		break;
	case Data:
		addNewRelocationDataToSection(sectionsCollection->dataDataSection, relData);
		break;
	case Bss:
		addNewRelocationDataToSection(sectionsCollection->bssDataSection, relData);
		break;
	}
}

void addNewRelocationDataToSection(SectionData *sectionData, RelocationData *relocationData)
{
	RelocationData *tmp = sectionData->relData;
	if (sectionData->relData == NULL)
	{
		sectionData->relData = relocationData;
	}
	else
	{
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = relocationData;
	}
}