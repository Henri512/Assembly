#pragma once

typedef struct elfSymbol {
	int name;
	int value;
	int size;
	char type : 4,
		binding : 4;
	char section;
	struct elfSymbol *next;
} ElfSymbol;
