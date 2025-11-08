#include <shell/betterarr.h>

static void *extarr(void *ptr, int currentSize, int itemsize)
{
    void *newptr = realloc(ptr, 2 * currentSize * itemsize);
    if (newptr == NULL)
    {
        free(ptr);
        exit(EXIT_FAILURE);
    }
    return newptr;
}

// extpart
char *extchar(char *str, int size) // free
{
    return (char *)extarr((void *)str, size, sizeof(char));
}

char **extarrchar(char **arr, int size) // free
{
    return (char **)extarr((void *)arr, size, sizeof(char *));
}

static void extbtrptr(btrptr *bptr) // free
{
    bptr->a = extarr(bptr->a, bptr->s, sizeof(void *));
}

static void extbtrchar(btrchar *bptr) // free
{
    bptr->a = extarr(bptr->a, bptr->s, sizeof(char));
}

static void extbtrint(btrint *bptr) // free
{
    bptr->a = extarr(bptr->a, bptr->s, sizeof(int));
}

// btrinit
btrchar btrcharinit()
{
    btrchar bptr;
    bptr.a = (char *)malloc(LENGTH * sizeof(char));
    if (bptr.a == NULL)
    {
        exit(EXIT_FAILURE);
    }
    bptr.s = LENGTH;
    bptr.c = 0;
    return bptr;
}

btrptr btrptrinit()
{
    btrptr bptr;
    bptr.a = (void **)malloc(LENGTH * sizeof(void *));
    if (bptr.a == NULL)
    {
        exit(EXIT_FAILURE);
    }
    bptr.s = LENGTH;
    bptr.c = 0;
    return bptr;
}

btrint btrintinit()
{
    btrint bptr;
    bptr.a = (int *)malloc(LENGTH * sizeof(int));
    if (bptr.a == NULL)
    {
        exit(EXIT_FAILURE);
    }
    bptr.s = LENGTH;
    bptr.c = 0;
    return bptr;
}

// btradd
void btrcharadd(btrchar *bptr, char c)
{
    if (bptr->c >= bptr->s)
    {
        extbtrchar(bptr);
        bptr->s *= 2;
    }
    bptr->a[bptr->c] = c;
    bptr->c++;
}

void btrptradd(btrptr *bptr, void *p)
{
    if (bptr->c >= bptr->s)
    {
        extbtrptr(bptr);
        bptr->s *= 2;
    }
    bptr->a[bptr->c] = p;
    bptr->c++;
}

void btrintadd(btrint *bptr, int val)
{
    if (bptr->c >= bptr->s)
    {
        extbtrint(bptr);
        bptr->s *= 2;
    }
    bptr->a[bptr->c] = val;
    bptr->c++;
}

// void btarrAdd

char *betterstrcat(const char *str1, ...)
{
    va_list args;
    va_start(args, str1);

    // calculate total length
    size_t totLen = strlen(str1);
    const char *s = NULL;
    while ((s = va_arg(args, const char *)) != NULL)
    {
        totLen += strlen(s);
    }
    va_end(args);

    // allocate memory
    char *result = (char *)malloc(totLen + 1);
    if (result == NULL)
    {
        exit(EXIT_FAILURE);
    }

    // concatenate strings
    strcpy(result, str1);
    va_start(args, str1);
    while ((s = va_arg(args, const char *)) != NULL)
    {
        strcat(result, s);
    }
    va_end(args);

    return result;
}