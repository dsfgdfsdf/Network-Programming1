#pragma once

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "net-utils.h"
#include "shared-data.h"

int start(int argc, char* argv[]);

void usage(const char* exe_name);

int init_client(const char* host, short port);

int process_connection(SOCKET client_socket);

int process_request(struct NumberOperationData* request, struct NumberOperationResult* response);
