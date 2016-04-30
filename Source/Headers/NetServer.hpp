#ifndef __TERMINAL_NETSERVER_HPP__
#define __TERMINAL_NETSERVER_HPP__

#include <DataTypes.hpp>
#include <NetClient.hpp>
#include <NetSocket.hpp>
#include <NetSocketAddress.hpp>
#include <NetProxyClient.hpp>
#include <sys/socket.h>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include <queue>

namespace TERMINAL
{
	class NetMessage;
	class NetSocketAddress;
	class NetClientProxy;

	class NetServer
	{
	public:
		NetServer( );
		virtual ~NetServer( );

		T_UINT32 Initialise( const std::string &p_Address );
		void Terminate( );

		void SendPing( );

		T_UINT32 GetPacket( void *p_pData, NetSocketAddress &p_From );

		void ProcessIncomingPackets( );
		void ReadIncomingPackets( );
		void ProcessMessageQueue( );
		virtual void ProcessPacket( NetMessage &p_Message,
			const NetSocketAddress &p_Address ) = 0;
		/*virtual void ProcessPacket( NetProxyClientPtr p_ProxyClient,
			NetMessage &p_Message ) = 0;*/

		T_UINT32 Update( const T_UINT32 p_TimeDelta,
			struct timeval *p_pTimeOut );

		void SendPackets( );
		void SendPacket( const NetMessage &p_Message,
			const NetSocketAddress &p_Address );

		bool Initialised( ) const;

	protected:
		typedef std::unordered_map< T_UINT32, NetProxyClientPtr >
			IDToClientMap;

		IDToClientMap					m_IDToClientMap;

	private:
		class ReceivedPacket
		{
		public:
			ReceivedPacket( NetMessage &p_Message,
				const NetSocketAddress &p_Addresss );
			const NetSocketAddress &GetAddress( ) const;
			NetMessage &GetMessage( );

		private:
			NetMessage			m_Message;
			NetSocketAddress	m_Address;
		};

		void ParseMessage( NetMessage &p_Message,
			struct sockaddr *p_pAddress );
		T_SINT32 CheckForTimeOut( void *p_pData, struct sockaddr *p_pFrom );
		
		T_UINT16						m_Port;
		NetSocketUDPPtr					m_Socket;
		bool							m_Initialised;
		std::vector< NetSocketUDPPtr >	m_ReadSockets;
		std::vector< NetSocketUDPPtr >	m_ReadableSockets;

		std::queue< ReceivedPacket, std::list<ReceivedPacket > > m_PacketQueue;
	};
}

#endif // __TERMINAL_NETSERVER_HPP__

