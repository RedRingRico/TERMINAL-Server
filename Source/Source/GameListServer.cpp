#include <GameListServer.hpp>
#include <Time.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

namespace TERMINAL
{
	static int GLS_AnswerConnection( void *p_pCls,
		struct MHD_Connection *p_pConnection, const char *p_pURL,
		const char *p_pMethod, const char *p_pVersion,
		const char *p_pUploadData, size_t *p_pUploadDataSize,
		void **p_ppConCls );

	GameListServer::GameListServer( ) :
		m_ClientToken( 1 )
	{
		this->UpdateWebPage( );

		// This should be moved into some kind of initialise member function
		m_pDaemonHTTP = MHD_start_daemon( MHD_USE_SELECT_INTERNALLY, 8080,
			nullptr, nullptr, &GLS_AnswerConnection, nullptr, MHD_OPTION_END );
	}

	GameListServer::~GameListServer( )
	{
		MHD_stop_daemon( m_pDaemonHTTP );
	}

	T_UINT32 GameListServer::Initialise( const std::string &p_Address )
	{
		NetServer::Initialise( p_Address );

		m_Socket = CreateUDPSocket( SOCKET_ADDRESS_FAMILY_INET );

		// The IP address should be user-modifiable via a configuration file
		NetSocketAddress Address( INADDR_ANY, htons( 50001 ) );

		m_Socket->Bind( Address );

		return 0;
	}

	T_UINT32 GameListServer::Update( T_UINT32 p_TimeDelta,
		struct timeval *p_pTimeOut )
	{
		NetServer::Update( p_TimeDelta, p_pTimeOut );

		auto ServerItr = m_ServerList.begin( );
		bool UpdateWebPage = false;

		while( ServerItr != m_ServerList.end( ) )
		{
			GAME_SERVER &Server = ( *ServerItr );

			if( Server.ServerTimeOut < m_ElapsedTime )
			{
				ServerItr = m_ServerList.erase( ServerItr );
				UpdateWebPage = true;

				continue;
			}

			++ServerItr;
		}

		if( UpdateWebPage == true )
		{
			this->UpdateWebPage( );
		}

		return 1;
	}


	void GameListServer::ProcessPacket( NetMessage &p_Message,
		const NetSocketAddress &p_Address )
	{
		T_UINT32 PacketType = p_Message.ReadUInt32( );

		switch( PacketType )
		{
			case PACKET_TYPE_LISTREQUEST:
			{
				// Resend the welcome packet
				GAME_SERVER Seed;

				// Determine where to start in the game list
				// IP 0.0.0.0 with port 0 is the first request
				// Any other IP address and port combination is where to
				// continue from
				Seed.IP = p_Message.ReadInt32( );
				Seed.Port = p_Message.ReadUInt16( );

				ServerList::const_iterator ListStart = m_ServerList.begin( );

				if( ( Seed.IP != 0 ) && ( Seed.Port != 0 ) )
				{
					while( ListStart != m_ServerList.end( ) )
					{
						if( ( *ListStart ).IP == Seed.IP
							&& ( *ListStart ).Port == Seed.Port )
						{
							break;
						}
						++ListStart;
					}
				}

				this->SendServerListPacket( p_Address, ListStart );

				break;
			}
			case PACKET_TYPE_REGISTERSERVER:
			{
				GAME_SERVER NewServer;

				NewServer.IP = p_Address.GetIP( );
				NewServer.Port = p_Address.GetPort( );
				p_Message.ReadString( NewServer.Name );
				NewServer.Players = p_Message.ReadUInt16( );
				NewServer.MaxPlayers = p_Message.ReadUInt16( );
				NewServer.Map = p_Message.ReadByte( );
				NewServer.GameType = p_Message.ReadByte( );
				NewServer.Scores[ 0 ] = p_Message.ReadUInt16( );
				NewServer.Scores[ 1 ] = p_Message.ReadUInt16( );
				NewServer.Scores[ 2 ] = p_Message.ReadUInt16( );
				NewServer.Scores[ 3 ] = p_Message.ReadUInt16( );
				NewServer.ServerTimeOut = m_ElapsedTime +
					GAME_SERVER_TIMEOUT;

				// Find the server and update it if it's not present, otherwise
				// add it
				auto ServerItr = m_ServerList.begin( );
				while( ServerItr != m_ServerList.end( ) )
				{
					if( ( NewServer.IP == ( *ServerItr ).IP ) &&
						( NewServer.Port == ( *ServerItr ).Port ) )
					{
						GAME_SERVER &Server = ( *ServerItr );

						Server.Name = NewServer.Name;
						Server.Players = NewServer.Players;
						Server.MaxPlayers = NewServer.MaxPlayers;
						Server.Map = NewServer.Map;
						Server.GameType = NewServer.GameType;
						Server.Scores[ 0 ] = NewServer.Scores[ 0 ];
						Server.Scores[ 1 ] = NewServer.Scores[ 1 ];
						Server.Scores[ 2 ] = NewServer.Scores[ 2 ];
						Server.Scores[ 3 ] = NewServer.Scores[ 3 ];
						Server.ServerTimeOut = NewServer.ServerTimeOut;

						break;
					}

					++ServerItr;
				}

				if( ServerItr == m_ServerList.end( ) )
				{
					m_ServerList.push_back( NewServer );
				}

				this->UpdateWebPage( );

				break;
			}
			default:
			{
				std::cout << "Unknown packet type: " << PacketType <<
					std::endl;
				break;
			}
		}
	}

	void GameListServer::SendServerListPacket(
		const NetSocketAddress &p_Address,
		ServerList::const_iterator p_StartPoint )
	{
		NetMessage List( nullptr, 1400 );

		List.WriteUInt32( PACKET_TYPE_LISTRESPONSE );

		if( p_StartPoint == m_ServerList.begin( ) )
		{
			std::cout << "Sending server start" << std::endl;
			// The first item to send to the client
			List.WriteInt32( 0xFFFFFFFF );
			List.WriteUInt16( 0xFFFF );
		}

		while( p_StartPoint != m_ServerList.end( ) )
		{
			std::cout << "Sending server" << std::endl;
			if( ( 1400 - List.GetSize( ) ) >= GAME_SERVER_PACKET_SIZE )
			{
				// Write the server
				List.WriteInt32( ( *p_StartPoint ).IP );
				List.WriteUInt16( ( *p_StartPoint ).Port );

				++p_StartPoint;
			}
			else
			{
				break;
			}
		}

		if( ( p_StartPoint == m_ServerList.end( ) ) &&
			( ( 1400 - List.GetSize( ) ) >= GAME_SERVER_PACKET_SIZE ) )
		{
			std::cout << "Sending end of list" << std::endl;
			// Send the end of list indicator
			List.WriteInt32( 0 );
			List.WriteUInt16( 0 );
		}

		this->SendPacket( List, p_Address );
	}

	void GameListServer::UpdateWebPage( )
	{

		FILE *pWebPage;

		pWebPage = fopen( "index.html.tmp", "w" );

		std::stringstream WebPage;
		WebPage << "<!DOCTYPE html>"
			"<html><head><title>[TERMINAL] | Game List Server</title>"
			"<link href='https://fonts.googleapis.com/css?family=Audiowide' "
			"rel='stylesheet' type='text/css'>"
			"<style type=text/css>body{color:#53FEFF}"
			"@font-face{font-family:WhiteRabbit;src: url('whitrabt.ttf');}"
			"div{font-family:AudioWide;text-align:center;}"
			"td{text-align:left}</style></head>"
			"<body bgcolor='#111111'>";
		WebPage << "<div>[TERMINAL] | Game List</div>";
		WebPage << "<br/><br/>";
		WebPage << "<div><table style='width:100%'>";
		WebPage << "<tr><td>Name</td><td>Address</td><td>Players</td></tr>";

		for( const GAME_SERVER &Server : m_ServerList )
		{
			char Address[ INET_ADDRSTRLEN ];

			inet_ntop( AF_INET, &Server.IP, Address, INET_ADDRSTRLEN );

			WebPage << "<tr><td>" << Server.Name << "</td>"
				"<td>" <<  Address << ":" <<
				htons( Server.Port ) << "</td><td>" << +Server.Players <<
				"/" << +Server.MaxPlayers << "</td></tr>";
		}
		WebPage << "</table></div>";

		WebPage << "</body></html>";

		fprintf( pWebPage, "%s", WebPage.str( ).c_str( ) );

		fclose( pWebPage );

		rename( "index.html.tmp", "index.html" );
	}

	static ssize_t GLS_ReadWebPage( void *p_pCls, uint64_t p_Position,
		char *p_pBuffer, size_t p_Maximum )
	{
		FILE *pFile = ( FILE * )p_pCls;

		fseek( pFile, p_Position, SEEK_SET );
		return fread( p_pBuffer, 1, p_Maximum, pFile );
	}

	static void GLS_FreeWebPageCallback( void *p_pCls )
	{
		FILE *pFile = ( FILE * )p_pCls;

		fclose( pFile );
	}

	static int GLS_AnswerConnection( void *p_pCls,
		struct MHD_Connection *p_pConnection, const char *p_pURL,
		const char *p_pMethod, const char *p_pVersion,
		const char *p_pUploadData, size_t *p_pUploadDataSize,
		void **p_ppConCls )
	{
		FILE *pWebPageFile;

		pWebPageFile = fopen( "index.html", "r" );

		int FD = fileno( pWebPageFile );
		struct stat FileStat;

		fstat( FD, &FileStat );

		struct MHD_Response *pResponse = MHD_create_response_from_callback(
			FileStat.st_size, 32 * 1024, &GLS_ReadWebPage, pWebPageFile,
			&GLS_FreeWebPageCallback );

		int Return = MHD_queue_response( p_pConnection, MHD_HTTP_OK,
			pResponse );
		MHD_destroy_response( pResponse );

		return Return;
	}
}

