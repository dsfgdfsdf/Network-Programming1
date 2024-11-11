#include "main.h"

SOCKET server_socket = -1;

void free_socket()
{
	if (server_socket > 0)
	{
		closesocket(server_socket);
	}
}

void usage(const char* exe_name)
{
	printf("Usage:\n");
	printf("\t%s -p <port> -q <queue_size>", exe_name);
}

int start(int argc, char* argv[])
{
	int port = DEFAULT_PORT;
	int queue_size = DEFAULT_QUEUE;

	if (argc >= 3)
	{
		char arg_line[128];
		memset(arg_line, 0, sizeof(arg_line));
		combine_arg_line(arg_line, argv, 1, argc);
		int ret = sscanf(arg_line, "-p %d -q %d", &port, &queue_size);

		if (ret < 1) {
			usage(argv[0]);
			return -1;
		}
	}

	return init_server(port, queue_size);
}

int init_server(short port, int queue_size)
{
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket <= 0)
	{
		printf("Cannot create socket\n");
		return -1;
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_socket, (struct sockaddr*)&address, sizeof(address))) {
		printf("Cannot bind socket to port %d\n", port);
		return -2;
	}

	if (listen(server_socket, queue_size))
	{
		printf("Cannot listen socket on port %d\n", port);
		return -3;
	}

	printf("Server running on port %d\n", port);

	return process_connections();
}

int process_connections()
{
	while (1)
	{
		struct sockaddr_in client_addr;
		int len = sizeof(client_addr);

		SOCKET client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len);
		if (client_socket <= 0)
		{
			printf("Error incoming connection\n");
			continue;
		}

		thrd_t client_thread;
		if (thrd_create(&client_thread, process_connection, (void*)(intptr_t)client_socket) != thrd_success)
		{
			printf("Failed to create thread for client\n");
			closesocket(client_socket);
			continue;
		}

		thrd_detach(client_thread);  // Let the thread clean up after itself.
	}

	return 0;
}

int process_connection(void* arg)
{
	SOCKET client_socket = (SOCKET)(intptr_t)arg;

	if (client_socket <= 0)
	{
		printf("Error with client connection\n");
		return -1;
	}

	struct sockaddr_in client_addr;
	int len = sizeof(client_addr);
	getsockname(client_socket, (struct sockaddr*)&client_addr, &len);

	printf("Established connection from: %s\n", inet_ntoa(client_addr.sin_addr));

	struct NumberOperationData request;
	int ret = recv(client_socket, (char*)&request, sizeof(request), 0);
	if (ret <= 0)
	{
		printf("Receiving data error\n");
		closesocket(client_socket);
		return -1;
	}

	printf("<==== Received: [%d bytes]\n", ret);

	struct NumberOperationResult response;
	process_request(&request, &response);

	ret = send(client_socket, (char*)&response, sizeof(response), 0);
	if (ret <= 0)
	{
		printf("Error sending response\n");
		closesocket(client_socket);
		return -1;
	}

	printf("====> Sent: [%d bytes]\n", ret);

	closesocket(client_socket);
	return 0;
}

int process_request(struct NumberOperationData* request, struct NumberOperationResult* response)
{
	response->avg = avg_of(request->data);
	response->max = max_of(request->data);
	response->min = min_of(request->data);
	return 0;
}

double avg_of(double* data) {
	double sum = 0;
	int count = 0;

	for (; !isnan(data[count]); ++count)
	{
		sum += data[count];
	}
	return sum / count;
}

double max_of(double* data) {
	double max = NAN;

	for (int i = 0; !isnan(data[i]); ++i)
	{
		if (isnan(max) || max < data[i]) {
			max = data[i];
		}
	}
	return max;
}

double min_of(double* data) {
	double min = NAN;

	for (int i = 0; !isnan(data[i]); ++i)
	{
		if (isnan(min) || min > data[i]) {
			min = data[i];
		}
	}
	return min;
}