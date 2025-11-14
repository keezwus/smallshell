#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define LENGTH 16

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

// extend array functions
char *extchar(char *str, int size);
char **extarrchar(char **arr, int size);

// initialize functions
void btrcharinit(btrchar *bptr);
void btrptrinit(btrptr *bptr);
void btrintinit(btrint *bptr);

// add element functions
void btrcharadd(btrchar *bptr, char c);
void btrptradd(btrptr *bptr, void *p);
void btrintadd(btrint *bptr, int val);

// delete element functions
void btrchardel(btrchar *bptr, int index);
void btrptrdel(btrptr *bptr, int index);
void btrintdel(btrint *bptr, int index);

// insert element functions
void btrcharins(btrchar *bptr, int index, char c);
void btrptrins(btrptr *bptr, int index, void *p);
void btrintins(btrint *bptr, int index, int val);

// string concatenation function
char *betterstrcat(const char *str1, ...);
