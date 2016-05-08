#include <iostream>
#include <GitVersion.hpp>
#include <GameListServer.hpp>
#include <GameServer.hpp>
#include <Time.hpp>
#include <vector>
#include <unistd.h>

void PrintUsage( );

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
	std::vector< TERMINAL::NetServer * > Servers;

	if( p_Argc == 1 )
	{
		PrintUsage( );

		return 0;
	}

	std::cout << "Configured for the following services:" << std::endl;
	std::cout << "\tAuthentication" << std::endl;
	std::cout << "\tLobby" << std::endl;
	std::cout << "\tGame" << std::endl;


	TERMINAL::GameListServer ListServer;
	TERMINAL::GameServer GameServer;

	ListServer.Initialise( "IGNORE" );
	GameServer.Initialise( "" );

	TERMINAL::T_UINT64 PreviousTime = TERMINAL::GetTimeInMicroseconds( );

	while( KeyPress( ) == -1 )
	{
		TERMINAL::T_UINT64 NewTime = TERMINAL::GetTimeInMicroseconds( );
		TERMINAL::T_UINT64 TimeDiff = NewTime - PreviousTime;
		ListServer.Update( TimeDiff, nullptr );
		GameServer.Update( TimeDiff, nullptr );
		PreviousTime = NewTime;
	}

	ListServer.Terminate( );

	return 0;
}

void PrintUsage( )
{
	std::cout << std::endl << std::endl;

	std::cout << "Hello and welcome to this server-side program to launch one "
		"of many services." << std::endl << std::endl;
	
	std::cout << "It would appear that you have made an error in your "
		"selection; fear not, for I " << std::endl << "will help you to "
		"correct your mistake!" << std::endl << std::endl;
	
	std::cout << "In order to successfully deploy this program, you will need "
		"to select one of " << std::endl << "two services with the "
		"following:" << std::endl;
	std::cout << "Flag | Description" << std::endl;
	std::cout << "-g     Starts a game server" << std::endl;
	std::cout << "-l     Starts a game list server" << std::endl << std::endl;

	std::cout << "That is all.  Good day to you." << std::endl << std::endl;
}

