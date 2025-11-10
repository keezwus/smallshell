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
static void btrinit(btrgeneral *bptr, int itemsize)
{
    bptr->a = malloc(LENGTH * itemsize);
    if (bptr->a == NULL)
    {
        exit(EXIT_FAILURE);
    }
    bptr->s = LENGTH;
    bptr->c = 0;
}

void btrcharinit(btrchar *bptr)
{
    btrinit((btrgeneral *)bptr, sizeof(char));
}

void btrptrinit(btrptr *bptr)
{
    btrinit((btrgeneral *)bptr, sizeof(void *));
}

void btrintinit(btrint *bptr)
{
    btrinit((btrgeneral *)bptr, sizeof(int));
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

// btrdel
static void btrdel(btrgeneral *bptr, int index, int itemsize)
{
    if (index < 0 || index >= bptr->c)
    {
        return;
    }
    if (index == bptr->c - 1)
    {
        bptr->c--;
        return;
    }
    memmove((char *)bptr->a + index * itemsize,
            (char *)bptr->a + (index + 1) * itemsize,
            (bptr->c - index - 1) * itemsize);
    bptr->c--;
}

void btrchardel(btrchar *bptr, int index)
{
    btrdel((btrgeneral *)bptr, index, sizeof(char));
}

void btrptrdel(btrptr *bptr, int index)
{
    btrdel((btrgeneral *)bptr, index, sizeof(void *));
}

void btrintdel(btrint *bptr, int index)
{
    btrdel((btrgeneral *)bptr, index, sizeof(int));
}

// betterstrcat
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