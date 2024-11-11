#include "ns-lookup.h"

int init()
{
	WSADATA ws;

	return WSAStartup(MAKEWORD(2, 2), &ws);
}

void cleanup()
{
	if (WSACleanup())
	{
		printf("Error cleanup WinSock2\n");
		return -2;
	}
}

int start(int argc, char** argv)
{
	char name[256];

	memset(name, 0, sizeof(name));

	if (argc >= 2)
	{
		strcpy(name, argv[1]);
	}
	else
	{
		printf("Enter DNS name:");
		int ret = scanf("%s", name);
		if (ret < 1)
		{
			return -3;
		}
	}

	return resolve_dns(name);
}

int resolve_dns(const char* name)
{
	struct hostent* he = gethostbyname(name);

	if (he == NULL)
	{
		return -5;
	}

	printf("Resolved: %s\n", he->h_name);

	printf("IPv4 address list: \n");

	struct in_addr** addr_list = (struct in_addr**)he->h_addr_list;
	int i;
	for (i = 0; addr_list[i] != 0; ++i)
	{
		char ip[64];

		memset(ip, 0, sizeof(ip));

		printf("\t%s\n", inet_ntoa(*addr_list[i]));
	}

	return 0;
}
