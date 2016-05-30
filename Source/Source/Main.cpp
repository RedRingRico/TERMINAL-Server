#include <iostream>
#include <GitVersion.hpp>
#include <GameListServer.hpp>
#include <GameServer.hpp>
#include <Time.hpp>
#include <vector>
#include <cstring>
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

	for( int Cmd = 1; Cmd < p_Argc; ++Cmd )
	{
		if( ( p_ppArgv[ Cmd ][ 0 ] == '-' ) &&
			( p_ppArgv[ Cmd ][ 1 ] != '-' ) )
		{
			size_t ArgIndex = 1;

			while( p_ppArgv[ Cmd ][ ArgIndex ] != '\0' )
			{
				char ArgChar = p_ppArgv[ Cmd ][ ArgIndex ];

				if( ArgChar == 'g' )
				{
					TERMINAL::NetServer *pServer = new TERMINAL::GameServer( );
					Servers.push_back( pServer );
					std::cout << "\tGame" << std::endl;
				}
				if( ArgChar == 'l' )
				{
					TERMINAL::NetServer *pServer =
						new TERMINAL::GameListServer( );
					Servers.push_back( pServer );
					std::cout << "\tGame List" << std::endl;
				}

				++ArgIndex;
			}
		}
		else // Doube-tack version
		{
			if( strcmp( "--gameserver", p_ppArgv[ Cmd ] ) == 0 )
			{
				TERMINAL::NetServer *pServer = new TERMINAL::GameServer( );
				Servers.push_back( pServer );
				std::cout << "\tGame" << std::endl;
			}

			if( strcmp( "--listserver", p_ppArgv[ Cmd ] ) == 0 )
			{
				TERMINAL::NetServer *pServer = new TERMINAL::GameListServer( );
				Servers.push_back( pServer );
				std::cout << "\tGame List" << std::endl;
			}
		}
	}

	auto Server = Servers.begin( );

	while( Server != Servers.end( ) )
	{
		( *Server )->Initialise( "??" );
		++Server;
	}

	TERMINAL::T_UINT64 PreviousTime = TERMINAL::GetTimeInMicroseconds( );

	while( KeyPress( ) == -1 )
	{
		TERMINAL::T_UINT64 NewTime = TERMINAL::GetTimeInMicroseconds( );
		TERMINAL::T_UINT64 TimeDiff = NewTime - PreviousTime;

		auto Server = Servers.begin( );

		while( Server != Servers.end( ) )
		{
			( *Server )->Update( TimeDiff, nullptr );
			++Server;
		}

		PreviousTime = NewTime;
	}

	Server = Servers.begin( );

	while( Server != Servers.end( ) )
	{
		std::cout << "Deleting server" << std::endl;
		( *Server )->Terminate( );
		delete ( *Server );
		++Server;
	}

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
	std::cout << "Flag          Description" << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << "--gameserver  Starts a game server" << std::endl;
	std::cout << "-g" << std::endl;
	std::cout << "--listserver  Starts a game list server" << std::endl;
	std::cout << "-l" << std::endl << std::endl;

	std::cout << "That is all.  Good day to you." << std::endl << std::endl;
}

