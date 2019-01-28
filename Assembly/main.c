#include "sections.h"
#include "helpers.h"
#include "parser_first.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser_second.h"
#include "section_helpers.h"
#include "string_helpers.h"
#include "object_file_writer.h"

int main(int argc, char* argv[])
{
	int dummy = 0, length = 0;
	char *outputFileName = NULL, *tmp = NULL;
	SectionsCollection *sectionsCollection = getEmptySectionsCollection();
	SymbolTableEntryList *symbolTableEntryList = getEmptySymbolTableEntryList();
	if (argc < 2)
	{
		printf("Niste uneli naziv fajla!");
		exit(-1);
	}
	if (!fileExists(argv[1]))
	{
		printf("Fajl sa zadatim imenom ne postoji!");
		exit(-1);
	}	

	executeFirstPass(symbolTableEntryList, sectionsCollection, argv[1]);

	resetCounters(sectionsCollection);

	executeSecondPass(symbolTableEntryList, sectionsCollection, argv[1]);

	writeDataToFile(sectionsCollection, symbolTableEntryList, argv[1]);

	int i = scanf("%d", &dummy);

	return 0;
}
