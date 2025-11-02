#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <stdarg.h>

#define SIZE 128
#define SSIZE 16
#define PROMPT_GREEN "\033[32m"
#define PROMPT_BLUE "\033[34m"
#define PROMPT_YELLOW "\033[33m"
#define PROMPT_RED "\033[31m"
#define PROMPT_RESET "\033[0m"

// function declaration
char *getPrompt();                         // free
char **parseline(char *line);              // free
char *extchar(char *str, int size);        // free
char *betterstrcat(const char *str1, ...); // free and end with NULL