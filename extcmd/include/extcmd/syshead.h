#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>

#define SIZE 128
#define SSIZE 16
#define PROMPT_GREEN "\033[32m"
#define PROMPT_BLUE "\033[34m"
#define PROMPT_YELLOW "\033[33m"
#define PROMPT_RED "\033[31m"
#define PROMPT_RESET "\033[0m"
#define MYPATH "build/extcmd"