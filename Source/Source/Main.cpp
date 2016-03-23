#include <iostream>
#include <GitVersion.hpp>

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "[TERMINAL] Server" << std::endl;
	std::cout << "Version: " << GIT_BUILD_VERSION << std::endl;
	std::cout << "Configured for the following services:" << std::endl;
	std::cout << "\tAuthentication" << std::endl;
	std::cout << "\tLobby" << std::endl;
	std::cout << "\tGame" << std::endl;

	return 0;
}

