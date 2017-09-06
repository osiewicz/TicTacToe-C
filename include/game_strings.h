
#pragma once

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

void eprintf(char *fmt,...);
int load_strings(char *language,char ***destination);

