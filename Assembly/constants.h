#pragma once
#define ENDDIRECTIVE ".end"
#define GLOBALDIRECTIVE ".global"
#define EXTERNDIRECTIVE ".extern"

#define CHARDIRECTIVE ".char"
#define WORDDIRECTIVE ".word"
#define LONGDIRECTIVE ".long"
#define ALIGNDIRECTIVE ".align"
#define SKIPDIRECTIVE ".skip"
#define ALIGNDIRFILLVALUE '\0'

#define PCRELRELOCATION "R_386_PC16"
#define CHARRELOCATION "R_386_8"
#define WORDRELOCATION "R_386_16"
#define LONGRELOCATION "R_386_32"
#define ABSRELOCATION WORDRELOCATION

#define CHARSIZE 1
#define WORDSIZE 2
#define LONGSIZE 4

#define INSTRUCTIONSCOUNT 18
#define SECTIONSCOUNT 4

#define SECTIONSIZE 50

#define PC "r7"
#define SP "r6"

#define VALUEWILDCARD '&'
#define ADRLOCWILDCARD '*'
#define PCRELINDWILDCARD '$'

#define UNKNOWNLABELOFFSET -1

static const char *Instructions[] = { "add", "sub", "mul", "div", "cmp", "and", "or", "not", "test", "push", "pop", "call", "iret", "mov", "shl", "shr", "ret", "jmp" };

static const char *Registers[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };

static const char *Sections[] = { ".text", ".data", ".rodata", ".bss", "unknown" };

static const char *RelocationTypes[] = { "R_386_PC16", "R_386_16", "R_386_8", "R_386_16", "R_386_32" };

enum SectionType { Local, Global };

enum SectionsEnum { Text, Data, RoData, Bss, Unknown };

enum OpCodes { Add, Sub, Mul, Div, Cmp, And, Or, Not, Test, Push, Pop, Call, Iret, Mov, Shl, Shr, Ret, Jmp };

// Equal, Not equal, Greater, Always
enum ConditionCodes { Eq, Ne, Gt, Al };

enum OperandTypes { Dst, Src };

enum RelocationTypesEnum { PCREL, ABS, CHARDIR, WORDDIR, LONGDIR };

enum AddressingCodes { Imm, RegDir, Mem, RegInd };