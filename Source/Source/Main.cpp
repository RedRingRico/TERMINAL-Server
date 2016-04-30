#include <iostream>
#include <GitVersion.hpp>
#include <GameListServer.hpp>
#include <unistd.h>

// Move this later
int KeyPress( )
{
	static char KeyPressed;
	struct timeval Wait;
	int CharsRead;
	fd_set Mask;

	FD_SET( 0, &Mask );
	Wait.tv_sec = 0;
	Wait.tv_usec = 0;

	if( select( 1, &Mask, 0, 0, &Wait ) )
	{
		CharsRead = read( 0, &KeyPressed, 1 );

		if( CharsRead == 1 )
		{
			return ( ( int )KeyPressed );
		}
	}

	return -1;
}

int main( int p_Argc, char **p_ppArgv )
{
	std::cout << "[TERMINAL] Server" << std::endl;
	std::cout << "Version: " << GIT_BUILD_VERSION << std::endl;
	std::cout << "Configured for the following services:" << std::endl;
	std::cout << "\tAuthentication" << std::endl;
	std::cout << "\tLobby" << std::endl;
	std::cout << "\tGame" << std::endl;

	TERMINAL::GameListServer Server;

	Server.Initialise( "IGNORE" );

	while( KeyPress( ) == -1 )
	{
		Server.ProcessIncomingPackets( );
	}

	Server.Terminate( );

	return 0;
}

