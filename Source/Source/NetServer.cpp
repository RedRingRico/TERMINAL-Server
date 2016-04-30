#include <NetServer.hpp>
#include <NetSocketAddress.hpp>
#include <iostream>
#include <sys/time.h>
#include <cstring>

namespace TERMINAL
{
	NetServer::NetServer( ) :
		m_Port( 0 ),
		m_Initialised( false )
	{
	}

	NetServer::~NetServer( )
	{
	}

	T_UINT32 NetServer::Initialise( const std::string &p_Address )
	{
		m_Socket = CreateUDPSocket( SOCKET_ADDRESS_FAMILY_INET );

		// The IP address should be user-modifiable via a configuration file
		NetSocketAddress Address( INADDR_ANY, 50001 );

		m_Socket->Bind( Address );

		m_Initialised = true;

		return 0;
	}

	void NetServer::Terminate( )
	{
	}

	void NetServer::ProcessIncomingPackets( )
	{
		this->ReadIncomingPackets( );
		this->ProcessMessageQueue( );
	}

	void NetServer::ReadIncomingPackets( )
	{
		static T_BYTE PacketData[ MAX_PACKET_SIZE ];
		T_MEMSIZE PacketSize = sizeof( PacketData );
		memset( PacketData, 0, sizeof( PacketData ) );
		NetMessage ReceivedMessage( PacketData, PacketSize );
		NetSocketAddress FromAddress;
		T_MEMSIZE ReceivedPacketCount = 0;
		T_MEMSIZE TotalBytesReadCount = 0;

		while( ReceivedPacketCount < MAX_PACKETS_PER_FRAME )
		{
			T_SINT32 ReceivedBytes = m_Socket->ReceiveFrom( PacketData,
				PacketSize, FromAddress );

			if( ReceivedBytes == 0 )
			{
				// Nothing left
				break;
			}
			else if( ReceivedBytes < 0 )
			{
				// Error, handle it
			}
			else
			{
				std::cout << "Got a live one!" << std::endl;
				// Reset the message so it is like new
				ReceivedMessage.Reset( ReceivedBytes );
				++ReceivedPacketCount;
				TotalBytesReadCount += ReceivedBytes;
				m_PacketQueue.emplace( ReceivedMessage, FromAddress );
			}
		}
	}

	void NetServer::ProcessMessageQueue( )
	{
		if( m_PacketQueue.empty( ) == false )
		{
			std::cout << "Something in the qeueue" << std::endl;
			ReceivedPacket &Next = m_PacketQueue.front( );

			ProcessPacket( Next.GetMessage( ), Next.GetAddress( ) );
			m_PacketQueue.pop( );
		}
	}


	T_UINT32 NetServer::Update( const T_UINT32 p_TimeDelta,
		struct timeval *p_pTimeOut )
	{
		if( m_Initialised == false )
		{
			return -1;
		}

		//NetMessage Message;

		/*if( Select( &m_ReadSockets, &m_ReadableSockets,
			nullptr, nullptr,
			nullptr, nullptr,
			p_pTimeOut ) )*/
		{
			//for( const NetSocketUDPPtr &Socket : m_ReadableSockets )
			{
				/*Socket->ReceiveFrom( pData, DataLen, SockAddr );
				this->ProcessMessage( pData, SockAddr );*/
			}
		}

		return 0;
	}

	void NetServer::SendPacket( const NetMessage &p_Message,
			const NetSocketAddress &p_Address )
	{
		m_Socket->SendTo( p_Message.GetBuffer( ), p_Message.GetSize( ),
			p_Address );
	}


	NetServer::ReceivedPacket::ReceivedPacket( NetMessage &p_Message,
		const NetSocketAddress &p_Address ) :
		m_Message( p_Message ),
		m_Address( p_Address )
	{
		std::cout << "Set message to size: " << m_Message.GetSize( ) <<
			std::endl;
	}

	const NetSocketAddress &NetServer::ReceivedPacket::GetAddress( ) const
	{
		return m_Address;
		void ReadIncomingPackets( );
		void ProcessMessageQueue( );
	}

	NetMessage &NetServer::ReceivedPacket::GetMessage( )
	{
		return m_Message;
	}
}

