#include "main.h"

void usage(const char* exe_name)
{
	printf("Usage:\n");
	printf("\t%s -h <host> -p <port>\n", exe_name);
}

int start(int argc, char* argv[])
{
	char host[2048] = "";

	int port;

	if (argc >= 3)
	{
		char arg_line[4 * 1024] = "";

		combine_arg_line(arg_line, argv, 1, argc);

		int ret = sscanf(arg_line, "-h %s -p %d", host, &port);

		if (ret < 2) {
			usage(argv[0]);
			return -1;
		}
	}
	else {
		printf("Enter server address (-h <host> -p <port>): ");

		int ret = scanf("-h %s -p %d", host, &port);

		if (ret < 2)
		{
			usage(argv[0]);

			return -2;
		}
	}

	return init_client(host, port);
}

int init_client(const char* host, short port)
{
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (client_socket <= 0)
	{
		printf("Cannot create client socket\n");
		return -1;
	}

	printf("Socket created\n");

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	char target_host[2048] = "";
	resolve_address(host, target_host);

	server_address.sin_addr.s_addr = inet_addr(target_host);

	if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address))) {
		printf("Cannot connect to port %s:%d\n", target_host, port);
		return -2;
	}

	printf("Success connection to server: %s:%d\n", target_host, port);

	return process_connection(client_socket);
}

int process_connection(SOCKET client_socket)
{
	struct NumberOperationData request;

	printf("\nEnter your list of numbers: ");
	for (int i = 0; i < 2; i++) {
		if (scanf("%lf", &request.data[i]) != 1) {
			printf("Error reading input\n");
			return -12;
		}
	}
	for (int i = 2; i < 1024; i++) {
		request.data[i] = NAN;
	}

	int ret = send(client_socket, (char*)&request, sizeof(request), 0);
	if (ret <= 0) {
		printf("Sending data error\n");
		return -13;
	}

	printf("====> Sent: [%d bytes]\n", ret);

	struct NumberOperationResult response;
	ret = recv(client_socket, (char*)&response, sizeof(response), 0);
	if (ret <= 0) {
		printf("Receiving data error\n");
		return -14;
	}

	printf("<==== Received: [%d bytes]\n", ret);

	printf("Average value: %.3f\n", response.avg);
	printf("Maximal value: %.3f\n", response.max);
	printf("Minimal value: %.3f\n", response.min);

	return closesocket(client_socket);
}




