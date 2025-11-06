#include <shell/shellhead.h>

void sigintHandler(int sig)
{
    write(STDOUT_FILENO, "\n", 1);
}

void sigchildHandler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}