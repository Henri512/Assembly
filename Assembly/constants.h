#pragma once
#define ENDDIRECTIVE ".end"
#define GLOBALDIRECTIVE ".globl"
#define SECTIONDIRECTIVE ".section"

enum Sections { Uninitialized, Text, RoData, Data, Bss, SymTab, RelText, RelData, Debug, StrTab };

enum OpCodes { Add, Sub, Mul, Div, Cmp, And, Or, Not, Test, Push, Pop, Call, Iret, Mov, Shl, Shr };

// Equal, Not equal, Greater, Always
enum ConditionCodes { Eq, Ne, Gt, Al };

//Operandi se kodiraju tako što najviša dva bita određuju tip adresiranja. U tipovima adresiranja u
//kojima je potreban registar, broj registra zapisan je u preostala 3 bita operanda. U slučaju da je za
//adresiranje potreban i podatak koji predstavlja adresu, pomeraj ili neposrednu vrednost, zapisuje se u 2
//bajta neposredno posle instrukcije. U jednoj instrukciji samo jedan operand može da zahteva 2 dodatna
//bajta. Instrukcije koje zahtevaju dva podatka predstavljaju grešku. Takođe, sve kombinacije instrukcija i
//operanada, za koje ne postoji razumno tumačenje, smatrati greškom.

enum AddressingCodes { Imm, RegDir, Mem, RegInd };