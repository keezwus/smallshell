#include <shell/shellhead.h>

// void sigintHandler(int sig)
// {
//     // sigintReceived = 1;
//     write(STDOUT_FILENO, "\n", 1);
// }

void sigchildHandler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}