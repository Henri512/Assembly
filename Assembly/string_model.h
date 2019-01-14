#pragma once

typedef struct stringData 
{
	char *data;
	struct stringData *next;
} StringData;

StringData *addStringData(StringData *list, char *strData);

StringData *makeStringData(char *strData);

StringData *setRawString(StringData *elem, char *strData);

char *getRawString(StringData *list, int index);

int Size(StringData *list);

void freeList(StringData *list);
