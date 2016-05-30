#include <GameServer.hpp>
#include <NetMessage.hpp>
#include <iostream>

namespace TERMINAL
{
	GameServer::GameServer( ) :
		m_Name( "UNKNOWN" ),
		m_NextEntityIndex( 0UL ),
		m_Players( 0U ),
		m_MaxPlayers( 16U ),
		m_TimeSinceLastUpdate( 0ULL )
	{
	}

	GameServer::~GameServer( )
	{
		this->Unregister( );
	}

	T_UINT32 GameServer::Initialise( const std::string &p_Address )
	{
		NetServer::Initialise( p_Address );

		m_Socket = CreateUDPSocket( SOCKET_ADDRESS_FAMILY_INET );

		// The IP address should be user-modifiable via a configuration file
		NetSocketAddress Address( INADDR_ANY, htons( 50002 ) );

		m_Socket->Bind( Address );

		this->SendServerUpdate( );

		return 0;
	}

	T_UINT32 GameServer::Update( const T_UINT32 p_TimeDelta,
		struct timeval *p_pTimeOut )
	{
		NetServer::Update( p_TimeDelta, p_pTimeOut );

		// Check if the heartbeat has expired
		m_TimeSinceLastUpdate += p_TimeDelta;
		
		// One minute
		if( m_TimeSinceLastUpdate > 60000000ULL )
		{
			this->SendServerUpdate( );
		}

		return 0;
	}

	void GameServer::ProcessPacket( NetMessage &p_Message,
		const NetSocketAddress &p_Address )
	{
		auto Client = m_AddressToClientMap.find( p_Address );

		std::cout << "Got a new packet" << std::endl;

		if( Client == m_AddressToClientMap.end( ) )
		{
			std::cout << "New client" << std::endl;
			this->HandlePacketFromNewClient( p_Message, p_Address );
		}
		else
		{
			std::cout << "Known client" << std::endl;
			this->ProcessPacket( ( *Client ).second, p_Message );
		}
	}

	void GameServer::SendServerUpdate( )
	{
		T_BYTE MessageData[ MAX_PACKET_SIZE ];
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
#if defined ( TERMINAL_BUILD_DEBUG )
				"dev.list.dreamcast.live:50001"
#else
				"list.dreamcast.live:50001"
#endif // TERMINAL_BUILD_DEBUG
				);

		m_Socket->SendTo( ServerInfo.GetBuffer( ) , ServerInfo.GetSize( ),
			*ListServer );
	}

	void GameServer::ProcessPacket( NetProxyClientPtr p_pClient,
		NetMessage &p_Message )
	{
		T_UINT32 PacketType = p_Message.ReadUInt32( );

		switch( PacketType )
		{
			case PACKET_TYPE_CLIENTJOIN:
			{
				this->SendWelcomePacket( p_pClient );

				break;
			}
			case PACKET_TYPE_CLIENTLEAVE:
			{
				break;
			}
			default:
			{
				std::cout << "Unknown packet received: 0x" << std::hex <<
					PacketType << std::dec << std::endl;
			}
		}
	}

	void GameServer::HandlePacketFromNewClient( NetMessage &p_Message,
		const NetSocketAddress &p_Address )
	{
		T_UINT32 PacketType = p_Message.ReadUInt32( );

		if( PacketType == PACKET_TYPE_CLIENTJOIN )
		{
			if( m_Players >= m_MaxPlayers )
			{
				// Tell the client that they cannot join, too many players
				return;
			}

			std::string PlayerName;
			p_Message.ReadString( PlayerName );

			NetProxyClientPtr NewClient = std::make_shared< NetProxyClient >(
				p_Address, PlayerName, m_NextEntityIndex++ );

			m_AddressToClientMap[ p_Address ] = NewClient;
			m_IDToClientMap[ NewClient->GetID( ) ] = NewClient;

			++m_Players;

			this->SendWelcomePacket( NewClient );
			this->SendServerUpdate( );
		}
		else
		{
			std::cout << "Bad packet from unknown client" << std::endl;
		}
	}

	void GameServer::SendWelcomePacket( NetProxyClientPtr p_pClient )
	{
		NetMessage NewClient( nullptr, 1400 );

		NewClient.WriteUInt32( PACKET_TYPE_CLIENTWELCOME );
		NewClient.WriteUInt32( p_pClient->GetID( ) );

		this->SendPacket( NewClient, p_pClient->GetSocketAddress( ) );
	}

	void GameServer::Unregister( )
	{
		T_BYTE MessageData[ MAX_PACKET_SIZE ];
		m_TimeSinceLastUpdate = 0ULL;

		NetMessage ServerInfo( MessageData, MAX_PACKET_SIZE );
		ServerInfo.WriteUInt32( PACKET_TYPE_UNREGISTERSERVER );

		NetSocketAddressPtr ListServer =
			NetSocketAddressFactory::CreateIPv4(
#if defined ( TERMINAL_BUILD_DEBUG )
				"dev.list.dreamcast.live:50001"
#else
				"list.dreamcast.live:50001"
#endif // TERMINAL_BUILD_DEBUG
				);

		m_Socket->SendTo( ServerInfo.GetBuffer( ) , ServerInfo.GetSize( ),
			*ListServer );
	}
}

