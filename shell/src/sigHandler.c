#include <shell/shellhead.h>

void sigintHandler(int sig)
{
    if (jobs.a[0] != 0)
    {
        killpg(jobs.a[0], SIGINT);
        jobs.a[0] = 0;
    }
    write(STDOUT_FILENO, "\n", 1);
}

void sigchildHandler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}