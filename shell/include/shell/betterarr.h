#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char *extchar(char *str, int size);        // free
char **extarrchar(char **arr, int size);   // free
char *betterstrcat(const char *str1, ...); // free and end with NULL

struct btrarr
{
    void *a;
    int s;
};

typedef struct btrarr btrarr;