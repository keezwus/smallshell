#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>

#define SIZE 128
#define SSIZE 16

// function declaration
char *getPrompt();            // free
char **parseline(char *line); // free

int main()
{
    while (1)
    {
        char *prompt = getPrompt();
        char *line = readline(prompt);
        free(prompt);
        char **arg = parseline(line);
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