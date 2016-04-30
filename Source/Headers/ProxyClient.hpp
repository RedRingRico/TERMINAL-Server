#ifndef __TERMINAL_NETPROXYCLIENT_HPP__
#define __TERMINAL_NETPROXYCLIENT_HPP__

#include <DataTypes.hpp>
#include <memory>
#include <string>

namespace TERMINAL
{
	class NetProxyClient
	{
	public:
		NetProxyClient( );
		~NetProxyClient( );

	private:
		std::string	m_Name;
	};

	typedef std::shared_ptr< NetProxyClient > NetProxyClientPtr;
}

#endif // __TERMINAL_NETPROXYCLIENT_HPP__

