#include "string_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *leftTrim(char *string, const char *separators)
{
	size_t totrim;
	if (separators == NULL) {
		separators = "\t\n\v\f\r ";
	}
	totrim = strspn(string, separators);
	if (totrim > 0) {
		size_t len = strlen(string);
		if (totrim == len) {
			string[0] = '\0';
		}
		else {
			memmove(string, string + totrim, len + 1 - totrim);
		}
	}
	return string;

}

char *rightTrim(char *string, const char *separators)
{
	int i;
	if (separators == NULL) {
		separators = "\t\n\v\f\r ";
	}
	i = strlen(string) - 1;
	while (i >= 0 && strchr(separators, string[i]) != NULL) {
		string[i] = '\0';
		i--;
	}
	return string;
}

char startsWith(const char *prefix, const char *string)
{
	return strncmp(prefix, string, strlen(prefix)) == 0;
}

char isStringHexChar(char *string, int length)
{
	return length > 2 && length < 5 && string[0] == '0' && (string[1] == 'x' || string[1] == 'X');
}

char isStringRegularChar(char *string, int length)
{
	return length == 1 || (length == 2 && string[0] == '\\');
}

char getCharFromDirectiveValue(char *directive)
{
	char result = directive[0];
	if (directive[0] == '\\')
	{
		switch (directive[1]) {
		case 'a':
			result = 0x07;
			break;
		case 'b':
			result = 0x08;
			break;
		case 'f':
			result = 0x0c;
			break;
		case 'n':
			result = 0x0a;
			break;
		case 'r':
			result = 0x0d;
			break;
		case 't':
			result = 0x09;
			break;
		case 'v':
			result = 0x0b;
			break;
		default:
			result = 0;
		}
	}
	return result;
}

void printBinaryChar(char c)
{
	for (int i = 7; i >= 0; --i)
	{
		putchar((c & (1 << i)) ? '1' : '0');
	}
	putchar('\n');
}

char *getNewString(int size)
{
	char *newString = (char*)malloc(sizeof(char) * size);
	if (newString == NULL)
	{
		printf("Greska u alokaciji memorije! Kraj izvrsavanja!\n\r");
		exit(-1);
	}
	return newString;
}

char* replaceChar(char* str, char find, char replace) {
	char *currentPos = strchr(str, find);
	while (currentPos) {
		*currentPos = replace;
		currentPos = strchr(currentPos, find);
	}
	return str;
}