#include <shell/shellhead.h>

int main()
{
    while (1)
    {
        char *prompt = getPrompt();
        char *line = readline(prompt);
        free(prompt);
        // char **arg = parseline(line);
        // free(line);

        // free(arg);
    }
    return 0;
}

char *getPrompt() // free
{
    char cwd[SIZE];
    char hostname[SIZE];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getCwdErr");
        return NULL;
    }

    if (gethostname(hostname, sizeof(hostname)) == -1)
    {
        perror("getHostNameErr");
        return NULL;
    }

    struct passwd *user = getpwuid(getuid());
    if (user == NULL)
    {
        perror("getUserInfoErr");
        return NULL;
    }

    // generate user@hostname aaa/bb/cwd > style prompt
    return betterstrcat(PROMPT_GREEN,
                        user->pw_name,
                        PROMPT_RESET,
                        "@",
                        PROMPT_BLUE,
                        hostname,
                        " ",
                        PROMPT_YELLOW,
                        cwd,
                        PROMPT_RESET,
                        " >",
                        NULL);
}