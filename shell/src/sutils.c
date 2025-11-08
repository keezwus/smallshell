#include <shell/shellhead.h>

void setup_readline()
{
    rl_variable_bind("horizontal-scroll-mode", "1");
    rl_variable_bind("colored-completion-prefix", "1");
}

int builtinCmd(char **args)
{
    if (args[0] == NULL)
    {
        return 1;
    }
    if (strcmp(args[0], "exit") == 0)
    {
        exit(0);
    }
    if (strcmp(args[0], "cd") == 0)
    {
        if (chdir(args[1]) == -1)
        {
            perror("chdirErr");
        }
        return 1;
    }
    if (strcmp(args[0], "pwd") == 0)
    {
        char cwd[SIZE];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getCwdErr");
        }
        else
        {
            printf("%s\n", cwd);
        }
        return 1;
    }
    if (strcmp(args[0], "echo") == 0)
    {
        for (int i = 1; args[i] != NULL; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
        return 1;
    }
    if (strcmp(args[0], "kill") == 0)
    {
        if (args[1] == NULL)
        {
            fprintf(stderr, "kill: missing pid\n");
            return 1;
        }
        else if (kill(atoi(args[1]), SIGKILL) == -1)
        {
            perror("killErr");
        }
        return 1;
    }
    if (strcmp(args[0], "&") == 0)
    {
        return 1;
    }
    return 0;
}
