#include "instructions.h"
#include "parser_helpers.h"
#include "constants.h"
#include "section_helpers.h"

void parseInstruction(SymbolTableEntryList *symbolTableEntryList, SectionsCollection *sectionsCollection, char *token)
{
	char byteCount;
	TokenList *tokenList = getInstructionsTokens(token);
	byteCount = getInstructionSize(tokenList);
	printf("\tInstruction %s has byteCount %d\n\r", token, byteCount);
	addToCurrentCollectionsCount(sectionsCollection, (int)byteCount);
}

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