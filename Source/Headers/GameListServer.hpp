#ifndef __TERMINAL_GAMELISTSERVER_HPP__
#define __TERMINAL_GAMELISTSERVER_HPP__

#include <NetServer.hpp>
#include <vector>
#include <microhttpd.h>

namespace TERMINAL
{
	struct GAME_SERVER
	{
		// Only support IPv4 for now
		uint32_t	IP;
		uint16_t	Port;
	};

	typedef std::vector< GAME_SERVER > ServerList;

	class GameListServer : public NetServer
	{
	public:
		GameListServer( );
		virtual ~GameListServer( );

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

