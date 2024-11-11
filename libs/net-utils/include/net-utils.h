#pragma once

#include "stdio.h"
#include "string.h"
#include "winsock2.h"

int init();

void cleanup();

int resolve_address(const char* src, char* dest);

int combine_arg_line(char* dest, const char* argv[], int start, int count);