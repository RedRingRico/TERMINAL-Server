#include <GameListServer.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>

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
		// Populate the list with fake addresses
		GAME_SERVER TestServer;

		TestServer.IP = inet_addr( "192.168.2.1" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.2" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.3" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.4" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.5" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.6" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.7" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.8" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.9" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.10" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.11" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.12" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.13" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.14" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		TestServer.IP = inet_addr( "192.168.2.15" );
		TestServer.Port = htons( 10002U );
		m_ServerList.push_back( TestServer );

		this->UpdateWebPage( );

		// This should be moved into some kind of initialise member function
		m_pDaemonHTTP = MHD_start_daemon( MHD_USE_SELECT_INTERNALLY, 8080,
			nullptr, nullptr, &GLS_AnswerConnection, nullptr, MHD_OPTION_END );
	}

	GameListServer::~GameListServer( )
	{
		MHD_stop_daemon( m_pDaemonHTTP );
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
				std::cout << "Received list request" << std::endl;
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
					std::cout << "Sending servers" << std::endl;

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
			if( ( 1400 - List.GetSize( ) ) >= sizeof( GAME_SERVER ) )
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
			( ( 1400 - List.GetSize( ) ) >= sizeof( GAME_SERVER ) ) )
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
		WebPage << 
			"<html><head><title>[TERMINAL] Game List Server</title></head>"
			"<body>";

		for( const GAME_SERVER &Server : m_ServerList )
		{
			char Address[ INET_ADDRSTRLEN ];

			inet_ntop( AF_INET, &Server.IP, Address, INET_ADDRSTRLEN );

			WebPage << "Server: " <<  Address << ":" <<
				htons( Server.Port ) << "<br/>";
		}

		WebPage << "</body></html>";

		fwrite( WebPage.str( ).c_str( ), WebPage.str( ).size( ), 1,
			pWebPage );

		fclose( pWebPage );

		rename( "index.html.tmp", "index.html" );
	}

	static int GLS_AnswerConnection( void *p_pCls,
		struct MHD_Connection *p_pConnection, const char *p_pURL,
		const char *p_pMethod, const char *p_pVersion,
		const char *p_pUploadData, size_t *p_pUploadDataSize,
		void **p_ppConCls )
	{
		FILE *pWebPageFile;
		char *pWebPageContent;
		long WebPageSize;

		pWebPageFile = fopen( "index.html", "r" );

		fseek( pWebPageFile, 0, SEEK_END );
		WebPageSize = ftell( pWebPageFile );
		rewind( pWebPageFile );

		pWebPageContent = new char [ WebPageSize + 1 ];
		pWebPageContent[ WebPageSize ] = '\0';

		fread( pWebPageContent, 1, WebPageSize, pWebPageFile );

		struct MHD_Response *pResponse;
		int Return;

		pResponse = MHD_create_response_from_buffer( strlen( pWebPageContent ),
			( void * )pWebPageContent, MHD_RESPMEM_PERSISTENT );

		Return = MHD_queue_response( p_pConnection, MHD_HTTP_OK, pResponse );
		MHD_destroy_response( pResponse );

		fclose( pWebPageFile );

		delete [ ] pWebPageContent;

		return Return;
	}
}

