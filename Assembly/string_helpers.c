#include "string_helpers.h"
#include <stdio.h>
#include <string.h>

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
