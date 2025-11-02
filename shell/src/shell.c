#include <shell/shellhead.h>

int main()
{
    setup_readline();

    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        perror("signal error");

    if (signal(SIGCHLD, sigchildHandler) == SIG_ERR)
        perror("signal error");

    while (1)
    {
        char *prompt = getPrompt();
        char *line = readline(prompt);
        add_history(line);
        if (line == NULL)
        {
            exit(0);
        }
        free(prompt);
        char **arg = parseline(line);

        eval(arg);

        free(line);
        free(arg);
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
    int argc = 0, argsize = SSIZE;
    while (isspace(*line))
        line++;
    int inquote = 0;
    int isstart = 1;
    for (size_t i = 0; line[i] != '\0'; i++)
    {
        if (argsize - 2 <= argc)
        {
            args = extarrchar(args, argsize * 2);
            argsize *= 2;
        }
        if (line[i] == '\"')
        {
            inquote = !inquote;
            line[i] = '\0';
            isstart = 1;
            continue;
        }
        if (inquote)
        {
            if (isstart)
            {
                args[argc] = &line[i];
                argc++;
                isstart = 0;
            }
            continue;
        }
        if (!inquote)
        {
            if (isspace(line[i]))
            {
                line[i] = '\0';
                isstart = 1;
            }
            else
            {
                if (isstart)
                {
                    args[argc] = &line[i];
                    argc++;
                    isstart = 0;
                }
            }

            if (line[i] == '|' || line[i] == '<' || line[i] == '>' || line[i] == '&')
            {
                isstart = 1;
                args[argc] = malloc(2);
                args[argc][0] = line[i];
                args[argc][1] = '\0';
                argc++;
                line[i] = '\0';
            }
        }
    }
    args[argc] = NULL;
    return args;
}

void eval(char **args)
{
    if (args[0] == NULL)
    {
        return;
    }
    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    else
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ForkErr");
            return;
        }
        else if (pid == 0)
        {
            if (execvp(args[0], args) < 0)
            {
                perror("ExecErr");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
}