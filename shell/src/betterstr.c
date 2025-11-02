#include <shell/shellhead.h>

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
        perror("malloc failed in betterstrcat");
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