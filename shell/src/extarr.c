#include <shell/shellhead.h>

static void *extarr(void *ptr, int arrsize, int itemsize)
{
    void *newptr = realloc(ptr, arrsize * itemsize);
    if (newptr == NULL)
    {
        free(ptr);
        perror("realloc failed in extarr");
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