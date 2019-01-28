#pragma once

char *leftTrim(char *str, const char *seps);

char *rightTrim(char *str, const char *seps);

char startsWith(const char * prefix, const char * string);

char isStringHexChar(char *string, int length);

char isStringRegularChar(char *string, int length);

char getCharFromDirectiveValue(char *directive);

void printBinaryChar(char c);

char *getNewString(int size);

char *replaceChar(char *str, char find, char replace);
