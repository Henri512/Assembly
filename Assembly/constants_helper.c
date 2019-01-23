#include "constants_helper.h"
#include <stdio.h>

char *getSectionValue(enum SectionsEnum section)
{
	char *value = NULL;
	switch (section)
	{
	case Text:
		value = "text";
		break;
	case RoData:
		value = "roData";
		break;
	case Data:
		value = "data";
		break;
	case Bss:
		value = "bss";
		break;
	default:
		value = "Not Found!";
		break;
	}
	return value;
}

char *getOpCodeValue(enum OpCodes opcode)
{
	char *value = NULL;
	switch (opcode)
	{
	case Add:
		value = "add";
		break;
	case Sub:
		value = "add";
		break;
	case Mul:
		value = "add";
		break;
	case Div:
		value = "add";
		break;
	case Cmp:
		value = "add";
		break;
	case And:
		value = "add";
		break;
	case Or:
		value = "add";
		break;
	case Not:
		value = "add";
		break;
	case Test:
		value = "add";
		break;
	case Push:
		value = "add";
		break;
	case Pop:
		value = "add";
		break;
	case Call:
		value = "add";
		break;
	case Iret:
		value = "add";
		break;
	case Mov:
		value = "add";
		break;
	case Shl:
		value = "add";
		break;
	case Shr:
		value = "add";
		break;
	default:
		value = "Not Found!";
		break;
	}
	return value;
}
