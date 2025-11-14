#include <shell/shellhead.h>

// global variable
btrint jobs;

int main()
{
    setup_readline();
    btrintinit(&jobs);
    btrintadd(&jobs, 0); // placeholder for foreground process

    if (signal(SIGINT, sigintHandler) == SIG_ERR)
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
            args = extarrchar(args, argsize);
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
    int bg = 0;
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "&") == 0)
        {
            bg = 1;
            args[i] = NULL;
            break;
        }
    }

    // btrint btrArgcFEC;
    // btrintinit(&btrArgcFEC);
    // for (size_t i = 0; args[i] != NULL; i++)
    // {
    //     if (args[i] == '|')
    //     {
    //         /* code */
    //         execlp
    //     }
    // }

    if (!builtinCmd(args))
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ForkErr");
            return;
        }
        else if (pid == 0)
        {
            setpgid(0, 0); // set child process group id to its own pid
            if (execvp(args[0], args) < 0)
            {
                perror("ExecErr");
                exit(EXIT_FAILURE);
            }
        }
        else if (bg)
        {
            btrintadd(&jobs, pid);
            return;
        }
        else
        {
            jobs.a[0] = pid;
            int status;
            waitpid(pid, &status, 0);
        }
    }

    // remember to free memory
    // free(btrArgcFEC.a);
}

int execCmd(char **args, int pgid, int inFd, int outFd) // return 0 for builtin, positive for forked pid
{
    // save original stdin and stdout and redirect
    int oldStdin = dup(STDIN_FILENO);
    int oldStdout = dup(STDOUT_FILENO);
    dup2(inFd, STDIN_FILENO);
    dup2(outFd, STDOUT_FILENO);
    close(inFd);
    close(outFd);

    if (!builtinCmd(args))
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("ForkErr");
            // restore original stdin and stdout
            dup2(oldStdin, STDIN_FILENO);
            dup2(oldStdout, STDOUT_FILENO);
            close(oldStdin);
            close(oldStdout);
            return -1;
        }
        else if (pid == 0)
        {
            setpgid(0, pgid); // set child process group id to the given pgid (0 if initializing)
            if (execvp(args[0], args) < 0)
            {
                perror("ExecErr");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // restore original stdin and stdout
            dup2(oldStdin, STDIN_FILENO);
            dup2(oldStdout, STDOUT_FILENO);
            close(oldStdin);
            close(oldStdout);
            return pid;
        }
    }
    // restore original stdin and stdout for builtin command
    dup2(oldStdin, STDIN_FILENO);
    dup2(oldStdout, STDOUT_FILENO);
    close(oldStdin);
    close(oldStdout);
}

// void setFd(int inFd,int outFd) //Fd minus zero means restore to the Fd before last redirection
// {
//     //save original stdin and stdout and redirect
//     static int oldStdin = dup(STDIN_FILENO);
//     static int oldStdout = dup(STDOUT_FILENO);
//     dup2(inFd, STDIN_FILENO);
//     dup2(outFd, STDOUT_FILENO);
//     close(inFd);
//     close(outFd);
// }