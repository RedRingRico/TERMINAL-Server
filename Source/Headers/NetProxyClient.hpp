#ifndef __TERMINAL_NETPROXYCLIENT_HPP__
#define __TERMINAL_NETPROXYCLIENT_HPP__

#include <DataTypes.hpp>
#include <NetSocketAddress.hpp>
#include <memory>
#include <string>

namespace TERMINAL
{
	class NetProxyClient
	{
	public:
		NetProxyClient( const NetSocketAddress &p_Address,
			const std::string &p_Name, const T_UINT32 p_ID );
		~NetProxyClient( );

		const NetSocketAddress &GetSocketAddress( ) const;
		T_UINT32 GetID( ) const;
		const std::string &GetName( ) const;

	private:
		NetSocketAddress	m_SocketAddress;
		std::string			m_Name;
		T_UINT32			m_ID;
	};

	typedef std::shared_ptr< NetProxyClient > NetProxyClientPtr;
}

#endif // __TERMINAL_NETPROXYCLIENT_HPP__

