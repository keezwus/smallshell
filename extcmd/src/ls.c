#include <extcmd/syshead.h>

int main(int argc, char *argv[])
{
    int opt, flagL = 0, hasArg = 1;
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
    char cwd[SIZE];

    if (optind = argc)
    {
        hasArg = 0;
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getCwdErr");
        }
    }
    if (flagL)
    {
        if (hasArg)
            dir = opendir(argv[optind]);
        else
            dir = opendir(cwd);

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
        if (hasArg)
            dir = opendir(argv[optind]);
        else
            dir = opendir(cwd);

        if (dir == NULL)
        {
            perror("opendir");
            return EXIT_FAILURE;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            printf("%s ", entry->d_name);
        }
        printf("\n");
        closedir(dir);
        return EXIT_SUCCESS;
    }
}