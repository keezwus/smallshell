#include <shell/betterarr.h>

static void *extarr(void *ptr, int arrsize, int itemsize)
{
    void *newptr = realloc(ptr, arrsize * itemsize);
    if (newptr == NULL)
    {
        free(ptr);
        exit(EXIT_FAILURE);
    }
    return newptr;
}

char *extchar(char *str, int size) // free
{
    return (char *)extarr((void *)str, size, sizeof(char));
}

char **extarrchar(char **arr, int size) // free
{
    return (char **)extarr((void *)arr, size, sizeof(char *));
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