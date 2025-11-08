#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define LENGTH 16

char *extchar(char *str, int size);        // free
char **extarrchar(char **arr, int size);   // free
char *betterstrcat(const char *str1, ...); // free and end with NULL

// size for s and count for c
// note that count c is the number of used elements
struct btrgeneral
{
    void *a;
    int s;
    int c;
};

struct btrchar
{
    char *a;
    int s;
    int c;
};

struct btrptr
{
    void **a;
    int s;
    int c;
};

struct btrint
{
    int *a;
    int s;
    int c;
};

typedef struct btrchar btrchar;
typedef struct btrptr btrptr;
typedef struct btrint btrint;
typedef struct btrgeneral btrgeneral;