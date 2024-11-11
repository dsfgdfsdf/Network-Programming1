#include "ns-lookup.h"

int main(int argc, char* argv[])
{
	atexit(cleanup);

	if (init())
	{
		printf("Error init networking\n");
		return -1;
	}

	if (start(argc, argv))
	{
		printf("Error dns name resolving");
		return -2;
	}

	return 0;
}