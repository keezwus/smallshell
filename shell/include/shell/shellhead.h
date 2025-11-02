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