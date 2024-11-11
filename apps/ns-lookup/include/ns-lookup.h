#pragma once

#include "stdio.h"
#include "string.h"
#include "winsock2.h"
#include "windows.h"

int init();

void cleanup();

int start(int argc, char* argv[]);

int resolve_dns(const char* name);
