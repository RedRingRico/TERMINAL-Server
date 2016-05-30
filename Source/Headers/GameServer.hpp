#ifndef __TERMINAL_GAMESERVER_HPP__
#define __TERMINAL_GAMESERVER_HPP__

#include <NetServer.hpp>
#include <NetSocketAddress.hpp>
#include <NetProxyClient.hpp>

namespace TERMINAL
{
	class GameServer : public NetServer
	{
	public:
		GameServer( );
		virtual ~GameServer( );

		virtual T_UINT32 Initialise( const std::string &p_Address ) override;

		virtual T_UINT32 Update( const T_UINT32 p_TimeDelta,
			struct timeval *p_pTimeOut ) override;
		virtual void ProcessPacket( NetMessage &p_Message,
			const NetSocketAddress &p_Address ) override;

	private:
		typedef std::unordered_map< T_UINT32, NetProxyClientPtr >
			IDToClientMap;
		typedef std::unordered_map< NetSocketAddress, NetProxyClientPtr >
			AddressToClientMap;

		void SendServerUpdate( );
		void ProcessPacket( NetProxyClientPtr p_pClient,
			NetMessage &p_Message );
		void HandlePacketFromNewClient( NetMessage &p_Message,
			const NetSocketAddress &p_Address );
		void SendWelcomePacket( NetProxyClientPtr p_pClient );
		void Unregister( );

		std::string	m_Name;
		T_UINT32	m_NextEntityIndex;
		T_UINT16	m_Players;
		T_UINT16	m_MaxPlayers;
		T_UINT64	m_TimeSinceLastUpdate;

		IDToClientMap		m_IDToClientMap;
		AddressToClientMap	m_AddressToClientMap;
	};
}

#endif /* __TERMINAL_GAMESERVER_HPP__ */

