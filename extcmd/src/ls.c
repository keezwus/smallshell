#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    int opt, flagL = 0;
    while ((opt = getopt(argc, argv, "l")) != -1)
    {
        switch (opt)
        {
        case 'l':
            flagL = 1;
            break;
        default:
            fprintf(stderr, PROMPT_RED "Usage: %s [-l] <directory_path>\n" PROMPT_RESET, argv[0]);
            return EXIT_FAILURE;
        }
    }

    DIR *dir;
    struct dirent *entry;

    if (optind > argc)
    {
        fprintf(stderr, PROMPT_RED "Usage: %s [-l] <directory_path>\n" PROMPT_RESET, argv[0]);
        return EXIT_FAILURE;
    }
    else if (flagL)
    {
        dir = opendir(argv[optind]);
        struct stat fileStat;
        if (dir == NULL)
        {
            perror("opendir");
            return EXIT_FAILURE;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            stat(entry->d_name, &fileStat);
            // printf()
        }
        closedir(dir);
        return EXIT_SUCCESS;
    }
    else
    {
        dir = opendir(argv[optind]);
        if (dir == NULL)
        {
            perror("opendir");
            return EXIT_FAILURE;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            printf("%s", entry->d_name);
        }
        printf("\n");
        closedir(dir);
        return EXIT_SUCCESS;
    }
}