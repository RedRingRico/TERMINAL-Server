#ifndef __TERMINAL_GAMESERVER_HPP__
#define __TERMINAL_GAMESERVER_HPP__

#include <NetServer.hpp>

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
		void SendServerUpdate( );

		std::string	m_Name;
		T_UINT16	m_Players;
		T_UINT16	m_MaxPlayers;
		T_UINT64	m_TimeSinceLastUpdate;
	};
}

#endif /* __TERMINAL_GAMESERVER_HPP__ */

