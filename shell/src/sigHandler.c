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

    if (jobs.a[0] == -1) // race condition a little, but at least it won't collect my foreground job now (btw i wrote != first but ai helped me find out)
    {
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;
    }

    errno = oldErr;
}