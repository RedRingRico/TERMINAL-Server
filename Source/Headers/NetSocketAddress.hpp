#ifndef __TERMINAL_SOCKETADDRESS_HPP__
#define __TERMINAL_SOCKETADDRESS_HPP__

#include <DataTypes.hpp>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace TERMINAL
{
	class NetSocketAddress
	{
	public:
		NetSocketAddress( );
		NetSocketAddress( T_UINT32 p_Address, T_UINT16 p_Port );
		NetSocketAddress( const struct sockaddr &p_SockAddr );

		T_MEMSIZE GetSize( ) const;

		size_t GetHash( ) const;

		bool operator==( const NetSocketAddress &p_Other ) const;

	private:
		friend class NetSocketUDP;
		
		sockaddr_in *GetAsSockAddrIn( );
		const sockaddr_in *GetAsSockAddrIn( ) const;

		struct sockaddr m_SockAddr;
	};

	typedef std::shared_ptr< NetSocketAddress > NetSocketAddressPtr;

	class NetSocketAddressFactory
	{
	public:
		static NetSocketAddressPtr CreateIPv4( const std::string &p_Address );
	};
}

namespace std
{
	template< > struct hash< TERMINAL::NetSocketAddress >
	{
		size_t operator( )( const TERMINAL::NetSocketAddress &p_Address ) const
		{
			return p_Address.GetHash( );
		}
	};
}

#endif // __TERMINAL_SOCKETADDRESS_HPP__

