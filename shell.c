#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 256

// function declaration
char *myReadline(char *prompt); // free
char *getPrompt();              // free

int main()
{
    printf("%s", getPrompt());
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

    char prompt[SIZE];
    struct passwd *user = getpwuid(getuid());
    if (user == NULL)
    {
        perror("getUserInfoErr");
        return NULL;
    }

    // generate user@hostname aaa/bb/cwd > style prompt
    strcpy(prompt, user->pw_name);
    strcat(prompt, "@");
    strcat(prompt, hostname);
    strcat(prompt, " ");
    strcat(prompt, cwd);
    strcat(prompt, " ");
    strcat(prompt, ">");

    return strdup(prompt);
}

char *myReadline(char *prompt)
{
    // set terminal to raw mode
    struct termios origTerm, rawTerm;
    if (tcgetattr(STDIN_FILENO, &origTerm) != 0)
    {
        perror("tcGetAttrErr");
        return NULL;
    }
    rawTerm = origTerm;
    rawTerm.c_lflag &= ~(ECHO | ICANON);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTerm) != 0)
    {
        perror("tcSetAttrErr");
        return NULL;
    }

    // put prompt
    if (prompt)
    {
        printf("s", prompt);
        fflush(stdout);
    }

    // readline
    char line[SIZE];
    char ch;
    int pos = 0;
    while (1)
    {
        if (read(STDIN_FILENO, &ch, 1) != 1)
        {
            perror("readErr");
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTerm);
            return NULL;
        }

        if (ch == '\n')
        {
            line[pos] = '\0';
            write(STDIN_FILENO, '\n', 1);
            if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTerm) != 0)
            {
                perror("tcSetAttrErr");
                return strdup(line);
            }
        }
        else if (ch == 127)
        {
            if (pos > 0)
            {
                pos--;
                write(STDIN_FILENO, "\b \b", 3);
            }
        }
        else if (ch == 27)
        {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1))
                continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1))
                continue;
            if (seq[0] == '[')
            {
                if (seq[1] == 'D')
                {
                    write(STDIN_FILENO, "\b", 1);
                }

                if (seq[1] == 'C')
                {
                    write(STDIN_FILENO, "\b", 1);
                }
            }
        }
    }
}