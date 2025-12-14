#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char buffer[4096];
    size_t bytes_read;

    if (argc == 1)
    {
        // Read from stdin
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0)
        {
            fwrite(buffer, 1, bytes_read, stdout);
        }
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "-") == 0)
            {
                // Read from stdin
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), stdin)) > 0)
                {
                    fwrite(buffer, 1, bytes_read, stdout);
                }
            }
            else
            {
                fp = fopen(argv[i], "r");
                if (fp == NULL)
                {
                    perror(argv[i]);
                    return EXIT_FAILURE;
                }
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
                {
                    fwrite(buffer, 1, bytes_read, stdout);
                }
                fclose(fp);
            }
        }
    }

    return EXIT_SUCCESS;
}