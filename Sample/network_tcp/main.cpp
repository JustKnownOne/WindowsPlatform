#include <iostream>
#include <conio.h>
#include <string>

#include "network.h"

enum mode_id
{
	SERVER,
	CLIENT,
};

int main(void)
{
	int mode;
	do {
		std::cout << "0. Server" << std::endl;
		std::cout << "1. Client" << std::endl;

		std::cin >> mode;
		if (mode != SERVER && mode != CLIENT)
		{
			std::cin.clear();
			std::cin.ignore(100, '\n');
		}
	} while (mode != SERVER && mode != CLIENT);

	if (mode == SERVER)
	{
		std::string port;
		std::cout << "=======Server mode=======" << std::endl;
		std::cout << "port to open : ";
		std::cin >> port;

		Server test;

		if (test.Open(port.c_str()))
		{
			while (test.Ready())
			{
				test.AllocSocket();
			}

			test.Close();
		}
	}
	else if (mode == CLIENT)
	{
		std::cout << "=======Client mode=======" << std::endl;
		std::string ip, port;
		std::cout << "IP : ";
		std::cin >> ip;
		std::cout << "port : ";
		std::cin >> port;

		Client robot;

		if (robot.Connect(ip.c_str(), port.c_str()))
		{
		}
	}
	else
	{
		std::cout << "invalid command." << std::endl;
	}

	_getch();
	return 0;
}
