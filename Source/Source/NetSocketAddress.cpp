#include <NetSocketAddress.hpp>
#include <cstring>
#include <netdb.h>

namespace TERMINAL
{
	NetSocketAddress::NetSocketAddress( )
	{
		this->GetAsSockAddrIn( )->sin_family = AF_INET;
		this->GetAsSockAddrIn( )->sin_addr.s_addr = INADDR_ANY;
		this->GetAsSockAddrIn( )->sin_port = 0;
	}

	NetSocketAddress::NetSocketAddress( const T_UINT32 p_Address,
		const T_UINT16 p_Port )
	{
		this->GetAsSockAddrIn( )->sin_family = AF_INET;
		this->GetAsSockAddrIn( )->sin_addr.s_addr = p_Address;// htonl( p_Address );
		this->GetAsSockAddrIn( )->sin_port = htons( p_Port );
	}

	NetSocketAddress::NetSocketAddress( const struct sockaddr &p_SockAddr )
	{
		memcpy( &m_SockAddr, &p_SockAddr, sizeof( m_SockAddr ) );
	}

	T_MEMSIZE NetSocketAddress::GetSize( ) const
	{
		return sizeof( m_SockAddr );
	}

	size_t NetSocketAddress::GetHash( ) const
	{
		return( GetAsSockAddrIn( )->sin_addr.s_addr |
			( ( static_cast< uint32_t >(
				GetAsSockAddrIn( )->sin_port ) ) << 13 ) |
			m_SockAddr.sa_family );
	}

	bool NetSocketAddress::operator==( const NetSocketAddress &p_Other ) const
	{
		return(
			( m_SockAddr.sa_family == p_Other.m_SockAddr.sa_family ) &&
			( this->GetAsSockAddrIn( )->sin_port ==
				p_Other.GetAsSockAddrIn( )->sin_port ) &&
			( this->GetAsSockAddrIn( )->sin_addr.s_addr ==
				p_Other.GetAsSockAddrIn( )->sin_addr.s_addr ) );
	}

	sockaddr_in *NetSocketAddress::GetAsSockAddrIn( )
	{
		return reinterpret_cast< sockaddr_in * >( &m_SockAddr );
	}

	const sockaddr_in *NetSocketAddress::GetAsSockAddrIn( ) const
	{
		return reinterpret_cast< const sockaddr_in * >( &m_SockAddr );
	}

	NetSocketAddressPtr NetSocketAddressFactory::CreateIPv4(
		const std::string &p_Address )
	{
		auto Colon = p_Address.find_last_of( ':' );
		std::string Host, Service;
		
		if( Colon != std::string::npos )
		{
			Host = p_Address.substr( 0, Colon );
			Service = p_Address.substr( Colon + 1 );
		}
		else
		{
			Host = p_Address;
			Service = "0";
		}

		addrinfo Hints;
		memset( &Hints, 0, sizeof( Hints ) );
		Hints.ai_family = AF_INET;

		addrinfo *pResult;
		int Error = getaddrinfo( Host.c_str( ), Service.c_str( ), &Hints,
			&pResult );

		if( Error != 0 && pResult != nullptr )
		{
			freeaddrinfo( pResult );
			return nullptr;
		}

		while( !pResult->ai_addr && pResult->ai_next )
		{
			pResult = pResult->ai_next;
		}

		if( !pResult->ai_addr )
		{
			freeaddrinfo( pResult );
			return nullptr;
		}

		auto Return =
			std::make_shared< NetSocketAddress >( *pResult->ai_addr );

		freeaddrinfo( pResult );

		return Return;
	}
}

