#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s directory...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i)
    {
        if (mkdir(argv[i], 0755) == -1)
        {
            perror(argv[i]);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
