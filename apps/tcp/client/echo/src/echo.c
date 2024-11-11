#include "echo.h"

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
	char buffer[4 * 1024] = "";

	printf("Enter message: ");

	getc(stdin);

	int ret = scanf("%[^\n]s", buffer);

	if (ret < 1) {
		printf("Cannot read source message\n");
		return -10;
	}

	ret = send(client_socket, buffer, strlen(buffer), 0);

	if (ret <= 0)
	{
		printf("Sending data error\n");
		return -11;
	}

	printf("====> Sent: [%d bytes]\n", ret);

	memset(buffer, 0, sizeof(buffer));

	ret = recv(client_socket, buffer, sizeof(buffer), 0);

	printf("<==== Received echo: %s [%d bytes]\n", buffer, ret);

	return closesocket(client_socket);
}