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

char **parseline(char *line) // free
{
    char **args = malloc(SSIZE * sizeof(char *));
    for (int i = 0; i < SSIZE; i++)
    {
        args[i] = malloc(SIZE * sizeof(char));
    }
    int isStart = 1;
    int assize = SSIZE;
    int argc = 0;
    int asize = SIZE;
    int arglen = 0;
    for (int i = 0; line[i] != '\0'; i++)
    {
        if (assize - 1 <= argc)
        {
            args = extarrchar(args, assize * 2);
            assize *= 2;
        }
        else
        {
            if (isStart)
            {
                if (isspace(line[i]))
                    continue;
                else
                    isStart = 0;
            }
            else
            {
                if (isspace(line[i]))
                {
                    /* code */
                }

                if (arglen - 1 <= asize)
                {
                    args[argc] = extchar(args[argc], asize * 2);
                    asize *= 2;
                }
                else
                {
                    args[argc][arglen] = line[i];
                    arglen++;
                }
            }
        }
    }
}