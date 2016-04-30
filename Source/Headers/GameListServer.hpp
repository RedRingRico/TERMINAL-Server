#ifndef __TERMINAL_GAMELISTSERVER_HPP__
#define __TERMINAL_GAMELISTSERVER_HPP__

#include <NetServer.hpp>
#include <vector>

namespace TERMINAL
{
	struct GAME_SERVER
	{
		// Only support IPv4 for now
		uint32_t	IP;
		uint16_t	Port;
	};

	typedef std::vector< GAME_SERVER > ServerList;

	/*class GameList
	{
	public:
		GameList( );
		GameList( const GameList &p_Original );
		~GameList( );

		int GetPageCount( ) const;

	private:
		ServerList	m_ServerList;
	};*/

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

		T_UINT32	m_ClientToken;
		ServerList	m_ServerList;
	};
}

#endif /* __TERMINAL_GAMELISTSERVER_HPP__ */

