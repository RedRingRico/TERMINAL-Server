#ifndef __TERMINAL_GAMELISTSERVER_HPP__
#define __TERMINAL_GAMELISTSERVER_HPP__

#include <NetServer.hpp>
#include <vector>
#include <microhttpd.h>
#include <string>

namespace TERMINAL
{
	const T_MEMSIZE GAME_SERVER_PACKET_SIZE = 6;
	struct GAME_SERVER
	{
		std::string	Name;
		// Only support IPv4 for now
		uint32_t	IP;
		uint16_t	Port;
		uint8_t		Players;
		uint8_t		MaxPlayers;
		uint8_t		Map;
		uint8_t		GameType;
		// Assume 4 teams, this will most likely only be two, though
		int16_t		Scores[ 4 ];
	};

	typedef std::vector< GAME_SERVER > ServerList;

	class GameListServer : public NetServer
	{
	public:
		GameListServer( );
		virtual ~GameListServer( );

		virtual T_UINT32 Initialise( const std::string &p_Address ) override;
		virtual T_UINT32 Update( const T_UINT32 p_TimeDelta,
			struct timeval *p_pTimeOut ) override;
		virtual void ProcessPacket( NetMessage &p_Message,
			const NetSocketAddress &p_Address ) override;

	private:
		void SendServerListPacket( const NetSocketAddress &p_Address,
			ServerList::const_iterator p_StartPoint );

		void UpdateWebPage( );

		T_UINT32			m_ClientToken;
		ServerList			m_ServerList;
		struct MHD_Daemon	*m_pDaemonHTTP;
	};
}

#endif /* __TERMINAL_GAMELISTSERVER_HPP__ */

