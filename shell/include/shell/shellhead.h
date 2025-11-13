#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <shell/betterarr.h>

#define SIZE 128
#define SSIZE 16
#define PROMPT_GREEN "\033[32m"
#define PROMPT_BLUE "\033[34m"
#define PROMPT_YELLOW "\033[33m"
#define PROMPT_RED "\033[31m"
#define PROMPT_RESET "\033[0m"

// global variable
extern btrint jobs;

// function declaration
char *getPrompt();            // free
char **parseline(char *line); // free
void eval(char **args);
void sigintHandler(int sig);
void setup_readline();
void sigchildHandler(int sig);
int builtinCmd(char **args);
void executeCmd(char **args, int *argcFEC); // argcFEC == argcForEveryCommand