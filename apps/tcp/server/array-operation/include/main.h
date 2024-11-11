#pragma once

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "net-utils.h"
#include "shared-data.h"

#define DEFAULT_PORT 7701
#define DEFAULT_QUEUE 20

extern SOCKET server_socket;

void free_socket();

int start(int argc, char* argv[]);

void usage(const char* exe_name);

int init_server(short port, int queue_size);

int process_connection();

int process_request(struct NumberOperationData* request, struct NumberOperationResult* response);

double avg_of(double* data);

double max_of(double* data);

double min_of(double* data);