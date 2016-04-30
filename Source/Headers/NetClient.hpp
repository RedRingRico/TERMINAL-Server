#ifndef __TERMINAL_NETCLIENT_HPP__
#define __TERMINAL_NETCLIENT_HPP__

#include <DataTypes.hpp>
#include <NetState.hpp>
#include <NetMessage.hpp>
#include <NetSocket.hpp>
#include <sys/socket.h>

namespace TERMINAL
{
    // Forward declarations

    class NetClient
    {
	public:
		NetClient( );
		~NetClient( );

		void SetDroppedPackets( const T_UINT16 m_DroppedPackets );
		T_UINT32 GetDroppedPakcets( ) const;

		T_UINT32 GetID( ) const;

	private:
		void DumpBuffers( );
		void ParseMessage( NetMessage *p_pMessage );
		
		T_CONNECTIONSTATE	m_ConnectionState;
		T_UINT16			m_OutgoingSequence;
		T_UINT16			m_IncomingSequence;
		T_UINT16			m_AcknowledgedSequence;
		T_UINT16			m_DroppedPackets;
		T_UINT16			m_ServerPort;
		// Only good for IPv4!
		char				m_ServerIP[ 16 ];
		// Index of the client, assigned by server
		T_UINT16			m_Index;
		// Gamer name
		char				m_Name[ 16 ];
		// Replace with NetSocket?
		NetSocketUDPPtr		m_Socket;
		struct sockaddr		m_SocketAddress;
		T_UINT32			m_PingSent;
		T_UINT16			m_Latency;
		T_UINT32			m_LastMessageTime;
		bool				m_Initialised;
		NetMessage			m_Message;
		T_BYTE				m_MessageBuffer[ MAX_PACKET_SIZE ];
		T_UINT32			m_ID;
    };
}

#endif // __TERMINAL_NETCLIENT_HPP__

