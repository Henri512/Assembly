#include <stdio.h>
#include "sections.h"
#include "object_file_writer.h"
#include "parser_helpers.h"
#include "constants.h"


void writeDataToFile(SectionsCollection *sectionsCollection, SymbolTableEntryList *symbolTableEntryList, char *inputFileName)
{
	int length = strlen(inputFileName);
	char *outputFileName = getNewString(length + 1);
	strcpy(outputFileName, inputFileName);
	outputFileName[length - 1] = 'o';

	FILE *outputFile = getOutputFile(outputFileName);

	writeSymbolTableEntryList(symbolTableEntryList, outputFile);

	if (sectionsCollection->textDataSection->marked)
	{
		writeSectionToFile(sectionsCollection->textDataSection, outputFile);
	}
	if (sectionsCollection->dataDataSection->marked)
	{
		writeSectionToFile(sectionsCollection->dataDataSection, outputFile);
	}
	if (sectionsCollection->roDataSection->marked)
	{
		writeSectionToFile(sectionsCollection->roDataSection, outputFile);
	}
	if (sectionsCollection->bssDataSection->marked)
	{
		writeSectionToFile(sectionsCollection->bssDataSection, outputFile);
	}

	fclose(outputFile);
}

void writeSymbolTableEntryList(SymbolTableEntryList *symbolTableEntryList, FILE *file)
{
	char *title = "Tabela simbola";
	fprintf(file, "%-23s%-35s%s\n\n", "###", title, "###");
	fprintf(file, "%-11s%-11s%-12s%-16s%-10s\n", "Naziv", "Sekcija", "Vrednost", "Tip Sekcije", "Redni Broj");
	SymbolTableEntry *entry = symbolTableEntryList->list;
	while (entry)
	{
		writeSymbolTableEntry(entry, file);
		entry = entry->next;
	}
}

void writeSymbolTableEntry(SymbolTableEntry *entry, FILE *file)
{
	char *sectionType;
	switch (entry->sectionType)
	{
	case Global:
		sectionType = "global";
		break;
	case Local:
		sectionType = "local";
		break;
	default:
		sectionType = "unknown";
		break;
	}
	fprintf(file, "%-11s%-11s%08x%-7s%-17s%-10d\n", entry->name, Sections[entry->section], entry->offset, " ", sectionType, entry->num);
}

void writeSectionToFile(SectionData *sectionData, FILE *file)
{
	RelocationData *relDataEntry = sectionData->relData;
	char *relTitle = "Relokacioni zapisi za ", *sectionName = Sections[sectionData->index], *contentTitle = "Sadrzaj ";
	fprintf(file, "\n\n%-5s%-1s%-1s%-10s%s\n", "###", relTitle, sectionName, " sekciju ", "###");
	if (relDataEntry)
	{
		fprintf(file, "\n%-16s%-14s%-12s\n", "Ofset", "Tip", "Vrednost");
	}
	else
	{
		fprintf(file, "%-1s%-1s%-10s\n", "Nema relokacionih zapisa za ", sectionName, " sekciju.");
	}
	while (relDataEntry)
	{
		fprintf(file, "%08x%-6s%-20s%-12d\n", relDataEntry->offset, " ", RelocationTypes[relDataEntry->relocationType], relDataEntry->num);
		relDataEntry = relDataEntry->next;
	}

	fprintf(file, "\n\n%-5s%-1s%-1s%-10s%s\n\n", "###", contentTitle, sectionName, " sekcije ", "###");
	writeContentEntry(sectionData->content, file);

}

void writeContentEntry(SectionContent *content, FILE *file)
{
	int i, global = 1;
	SectionContent *currentContent = content;
	while (currentContent)
	{
		for (i = 0; i < content->count; i++)
		{
			fprintf(file, "%02x ", content->content[i] & 0xFF);
			if (global && (global % 2 == 0) && !(global % 8 == 0))
			{
				fprintf(file, "  ");
			}
			if (global && (global % 8 == 0))
			{
				fprintf(file, "\n");
			}
			global++;
		}
		currentContent = currentContent->next;
	}
}
