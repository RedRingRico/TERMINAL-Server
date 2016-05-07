#include <GameServer.hpp>
#include <NetMessage.hpp>
#include <iostream>

namespace TERMINAL
{
	GameServer::GameServer( ) :
		m_Name( "UNKNOWN" ),
		m_Players( 0 ),
		m_MaxPlayers( 0 ),
		m_TimeSinceLastUpdate( 0ULL )
	{
	}

	GameServer::~GameServer( )
	{
	}

	T_UINT32 GameServer::Initialise( const std::string &p_Address )
	{
		NetServer::Initialise( p_Address );

		m_Socket = CreateUDPSocket( SOCKET_ADDRESS_FAMILY_INET );

		// The IP address should be user-modifiable via a configuration file
		NetSocketAddress Address( INADDR_ANY, htons( 50002 ) );

		m_Socket->Bind( Address );

		return 0;
	}

	T_UINT32 GameServer::Update( const T_UINT32 p_TimeDelta,
		struct timeval *p_pTimeOut )
	{
		NetServer::Update( p_TimeDelta, p_pTimeOut );

		// Check if the heartbeat has expired
		m_TimeSinceLastUpdate += p_TimeDelta;
		
		// One minute
		if( m_TimeSinceLastUpdate > 10000000ULL )
		{
			T_BYTE MessageData[ MAX_PACKET_SIZE ];
			std::cout << "Updating time" << std::endl;
			m_TimeSinceLastUpdate = 0ULL;

			NetMessage ServerInfo( MessageData, MAX_PACKET_SIZE );
			ServerInfo.WriteUInt32( PACKET_TYPE_REGISTERSERVER );
			ServerInfo.WriteString( m_Name );
			ServerInfo.WriteUInt16( m_Players );
			ServerInfo.WriteUInt16( m_MaxPlayers );
			ServerInfo.WriteByte( 0 );
			ServerInfo.WriteByte( 0 );
			ServerInfo.WriteUInt16( 0 );
			ServerInfo.WriteUInt16( 0 );
			ServerInfo.WriteUInt16( 0 );
			ServerInfo.WriteUInt16( 0 );

			NetSocketAddressPtr ListServer =
				NetSocketAddressFactory::CreateIPv4(
					"list.dreamcast.live:50001" );

			m_Socket->SendTo( ServerInfo.GetBuffer( ) , ServerInfo.GetSize( ),
				*ListServer );
		}

		return 0;
	}

	void GameServer::ProcessPacket( NetMessage &p_Message,
		const NetSocketAddress &p_Address )
	{
	}
}

