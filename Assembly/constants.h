#pragma once
#define ENDDIRECTIVE ".end"
#define GLOBALDIRECTIVE ".global"
#define SECTIONDIRECTIVE ".section"
#define ASCIZDIRECTIVE ".asciz"
#define ASCIIDIRECTIVE ".ascii"

#define CHARDIRECTIVE ".char"
#define WORDDIRECRTIVE ".word"
#define LONGDIRECTIVE ".long"
#define ALIGNDIRECTIVE ".align"
#define SKIPDIRECTIVE ".skip"

#define CHARSIZE 1
#define WORDSIZE 2
#define LONGSIZE 4

#define INSTRUCTIONSCOUNT 18
#define SECTIONSCOUNT 4

#define PC "r7"
#define SP "r6"

static const char *Instructions[] = { "add", "sub", "mul", "div", "cmp", "and", "or", "not", "test", "push", "pop", "call", "iret", "mov", "shl", "shr", "ret", "jmp" };

static const char *Registers[] = { "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7" };

static const char *Sections[] = { ".text", ".data", ".rodata", ".bss" };

enum SectionsEnum { Text, Data, RoData, Bss };

enum OpCodes { Add, Sub, Mul, Div, Cmp, And, Or, Not, Test, Push, Pop, Call, Iret, Mov, Shl, Shr, Ret, Jmp };

// Equal, Not equal, Greater, Always
enum ConditionCodes { Eq, Ne, Gt, Al };

//Operandi se kodiraju tako što najviša dva bita određuju tip adresiranja. U tipovima adresiranja u
//kojima je potreban registar, broj registra zapisan je u preostala 3 bita operanda. U slučaju da je za
//adresiranje potreban i podatak koji predstavlja adresu, pomeraj ili neposrednu vrednost, zapisuje se u 2
//bajta neposredno posle instrukcije. U jednoj instrukciji samo jedan operand može da zahteva 2 dodatna
//bajta. Instrukcije koje zahtevaju dva podatka predstavljaju grešku. Takođe, sve kombinacije instrukcija i
//operanada, za koje ne postoji razumno tumačenje, smatrati greškom.

enum AddressingCodes { Imm, RegDir, Mem, RegInd };