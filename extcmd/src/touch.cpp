#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i)
    {
        FILE *fp = fopen(argv[i], "a");
        if (fp == NULL)
        {
            perror(argv[i]);
            return EXIT_FAILURE;
        }
        fclose(fp);
    }

    return EXIT_SUCCESS;
}
