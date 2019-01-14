#include "string_model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringData *addStringData(StringData *list, char *strData)
{
	StringData* stringData = makeStringData(strData);
	if (!stringData)
	{
		exit(-1);
	}
	if (!list)
	{
		list = stringData;
	}
	else
	{
		while (list->next)
		{
			list = list->next;
		}
		list->next = stringData;
		list = list->next;
	}
	return list;
}

StringData *makeStringData(char *strData)
{
	StringData *stringData;
	if (!strData)
	{
		printf("String data cannot be empty!");
		exit(-1);
	}

	stringData = malloc(sizeof(StringData));
	if (!stringData)
	{
		exit(-1);
	}
	stringData->data = malloc((strlen(strData) + 1) * sizeof(char));
	if (!stringData->data) {
		free(stringData);
		exit(-1);
	}

	strcpy(stringData->data, strData);
	stringData->next = 0;
	return stringData;
}

StringData *setRawString(StringData *elem, char *strData)
{
	if (!elem)
	{
		exit(-1);
	}
	elem->data = strData;
	return elem;
}

char *getRawString(StringData *list, int index)
{
	if (!list)
	{
		exit(-1);
	}

	int counter = 0;
	while (counter < index && list->next)
	{
		counter++;
		list = list->next;
	}

	if (!list->next && counter < index)
	{
		printf("End of list reached, element not found!");
		exit(-1);
	}
	else
	{
		return list->data;
	}	
}

int Size(StringData *list)
{
	int size = 0;
	if (list)
	{
		while (list)
		{
			size++;
			list = list->next;
		}
	}
	return size;
}

void freeList(StringData *list)
{
	StringData *old;
	while (list)
	{
		old = list;
		list = list->next;
		free(old->data);
		free(old);
	}
}
