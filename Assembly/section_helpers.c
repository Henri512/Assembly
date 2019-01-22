#include "section_helpers.h"
#include "constants.h"
#include "sections.h"
#include <string.h>

char isTokenLabel(char *token)
{
	char isTokenLabel = token[strlen(token) - 1] == ':'
		|| strchr(token, ':') != NULL;
	return isTokenLabel;
}

char isTokenDirective(char *token)
{
	return token[0] == '.';
}

int getLabelTokenSize(char *label)
{
	int size = -1;
	if (strcmp(label, CHARDIRECTIVE))
	{
		size = 1;
	}
	else if (strcmp(label, LONGDIRECTIVE))
	{
		size = 4;
	}
	else if (strcmp(label, SKIPDIRECTIVE))
	{

	}
	else if (strcmp(label, ALIGNDIRECTIVE))
	{

	}
	return size;
}

void initializeSectionIfEmpty(SectionsCollection *sectionsCollection, int index, int start, int size)
{
	switch (sectionsCollection->currentSection)
	{
	case Uninitialized:

		break;
	case Text:
		if (!sectionsCollection->textDataSection)
		{
			sectionsCollection->textDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case RoData:
		if (!sectionsCollection->roDataSection)
		{
			sectionsCollection->roDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case Data:
		if (!sectionsCollection->dataDataSection)
		{
			sectionsCollection->dataDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case Bss:
		if (!sectionsCollection->bssDataSection)
		{
			sectionsCollection->bssDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case SymTab:

		break;
	case RelText:

		break;
	case RelData:
		if (!sectionsCollection->relocationDataSection)
		{
			sectionsCollection->relocationDataSection = makeSectionData(index, start, size, NULL);
		}
		break;
	case Debug:

		break;
	case StrTab:

		break;
	default:

		break;
	}
}
