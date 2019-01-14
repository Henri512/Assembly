#pragma once
#include <stdio.h>

char *getTokenFromLine(char *line);

char isEndToken(char *token);

char isValidToken(char *token);

FILE *getInputFile(char *filePath);