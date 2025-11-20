#include <shell/shellhead.h>

// global variable
btrint jobs;
sigset_t newMask, oldMask;

int main()
{
    setup_readline();
    btrintinit(&jobs);
    btrintadd(&jobs, 0); // placeholder for foreground process
    sigemptyset(&newMask);
    sigaddset(&newMask, SIGCHLD);

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
            else if (line[i] == '|' || line[i] == '<' || line[i] == '>' || line[i] == '&')
            {
                isstart = 1;
                args[argc] = malloc(2); // leak a little, but is a little so probably fine
                args[argc][0] = line[i];
                args[argc][1] = '\0';
                argc++;
                line[i] = '\0';
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

    sigprocmask(SIG_BLOCK, &newMask, &oldMask);
    int pgid = pipeworks(args);
    if (bg)
    {
        btrintadd(&jobs, pgid);
        sigprocmask(SIG_UNBLOCK, &newMask, &oldMask);
        return;
    }
    else
    {
        jobs.a[0] = pgid;
        while (waitpid(-pgid, NULL, 0) > 0)
            ;
    }
    sigprocmask(SIG_UNBLOCK, &newMask, &oldMask);
}

int execCmd(char **args, int pgid, int inFd, int outFd) // pgid currently unused
{
    if (isBuiltinCmd(args))
    {
        // save original stdin and stdout and redirect
        int oldStdin = -1, oldStdout = -1;
        if (inFd != STDIN_FILENO)
        {
            oldStdin = dup(STDIN_FILENO);
            dup2(inFd, STDIN_FILENO);
            close(inFd);
        }
        if (outFd != STDOUT_FILENO)
        {
            oldStdout = dup(STDOUT_FILENO);
            dup2(outFd, STDOUT_FILENO);
            close(outFd);
        }

        builtinCmd(args);

        if (oldStdin != -1)
        {
            dup2(oldStdin, STDIN_FILENO);
            close(oldStdin);
        }
        if (oldStdout != -1)
        {
            dup2(oldStdout, STDOUT_FILENO);
            close(oldStdout);
        }
        return 0;
    }

    pid_t pid = fork();
    setpgid(pid, pgid);
    if (pid == 0)
    {
        sigprocmask(SIG_UNBLOCK, &newMask, &oldMask); // got to deal with mask even in child proces

        if (inFd != STDIN_FILENO)
        {
            dup2(inFd, STDIN_FILENO);
            close(inFd);
        }
        if (outFd != STDOUT_FILENO)
        {
            dup2(outFd, STDOUT_FILENO);
            close(outFd);
        }
        // i don know why but not doing this may cause problem
        // oh,now i understand
        // shit
        if (execvp(args[0], args) < 0)
        {
            perror("ExecErr");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if (inFd != STDIN_FILENO)
            close(inFd);
        if (outFd != STDOUT_FILENO)
            close(outFd);

        return pid;
    }
}

int pipeworks(char **args)
{
    btrint sessions; // records starting index of each command
    btrintinit(&sessions);
    btrintadd(&sessions, 0);

    btrint pipes; // records pipe fds
    btrintinit(&pipes);
    btrintadd(&pipes, STDIN_FILENO);
    btrintadd(&pipes, STDOUT_FILENO);
    int fd[2];

    for (int i = 0; args[i] != NULL; i++)
    {
        if (!strcmp(args[i], "|"))
        {
            args[i] = NULL;
            btrintadd(&sessions, i + 1);
            pipe(fd);
            btrintins(&pipes, pipes.c - 1, fd[1]); // insert read end before stdout
            btrintins(&pipes, pipes.c - 1, fd[0]); // insert write end before stdout
        }
        else if (!strcmp(args[i], "<"))
        {
            args[i] = NULL;
            fd[1] = open(args[i + 1], O_RDONLY);
            pipes.a[0] = fd[1];
            i++;
        }
        else if (!strcmp(args[i], ">"))
        {
            args[i] = NULL;
            fd[0] = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            pipes.a[pipes.c - 1] = fd[0];
            i++;
        }
    }

    if (sessions.c * 2 != pipes.c)
    {
        fprintf(stderr, "pipeworksErr: mismatch number of commands and pipes\n");
        free(sessions.a);
        free(pipes.a);
        return -1;
    }

    int pgid = 0;

    for (int i = 0; i < sessions.c; i++)
    {
        if (i == 0)
        {
            pgid = execCmd(args + sessions.a[i], 0,
                           pipes.a[i * 2], pipes.a[i * 2 + 1]);
        }
        else
        {
            execCmd(args + sessions.a[i], pgid,
                    pipes.a[i * 2], pipes.a[i * 2 + 1]);
        }
    }

    free(sessions.a);
    free(pipes.a);
    return pgid;
}