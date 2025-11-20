#include <shell/shellhead.h>

void sigintHandler(int sig)
{
    int oldErr = errno;
    if (jobs.a[0] != 0)
    {
        killpg(jobs.a[0], SIGINT);
        jobs.a[0] = 0;
    }
    write(STDOUT_FILENO, "\n", 1);
    errno = oldErr;
}

void sigchildHandler(int sig)
{
    int oldErr = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = oldErr;
}