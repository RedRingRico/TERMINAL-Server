#include <NetProxyClient.hpp>

namespace TERMINAL
{
	NetProxyClient::NetProxyClient( const NetSocketAddress &p_Address,
		const std::string &p_Name, const T_UINT32 p_ID ) :
		m_SocketAddress( p_Address ),
		m_Name( p_Name ),
		m_ID( p_ID )
	{
	}

	NetProxyClient::~NetProxyClient( )
	{
	}

	const NetSocketAddress &NetProxyClient::GetSocketAddress( ) const
	{
		return m_SocketAddress;
	}

	T_UINT32 NetProxyClient::GetID( ) const
	{
		return m_ID;
	}

	/*const std::string &GetName( ) const
	{
		return m_Name;
	}*/
}

