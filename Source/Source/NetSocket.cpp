#include <NetSocket.hpp>
#include <NetSocketAddress.hpp>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <errno.h>

namespace TERMINAL
{
	NetSocketUDP::~NetSocketUDP( )
	{
		close( m_Socket );
	}

	T_UINT32 NetSocketUDP::Bind( NetSocketAddress &p_BindAddress )
	{
		int Error = bind( m_Socket, &p_BindAddress.m_SockAddr,
			p_BindAddress.GetSize( ) );

		if( Error != 0 )
		{
			return 1;
		}

		return 0;
	}

	T_SINT32 NetSocketUDP::SendTo( const void *p_pData,
		const T_MEMSIZE p_Length, const NetSocketAddress &p_To )
	{
		T_SINT32 SentBytes = sendto( m_Socket,
			static_cast< const char * >( p_pData ), p_Length, 0,
			&p_To.m_SockAddr, p_To.GetSize( ) );

		if( SentBytes >= 0 )
		{
			return SentBytes;
		}
		else
		{
			return -1;
		}
	}

	T_SINT32 NetSocketUDP::ReceiveFrom( void *p_pBuffer,
		const T_MEMSIZE p_Length, NetSocketAddress &p_From )
	{
		socklen_t FromLength = p_From.GetSize( );
		T_SINT32 ReceivedBytes = recvfrom( m_Socket,
			static_cast< char * >( p_pBuffer ), p_Length, 0,
			&p_From.m_SockAddr, &FromLength );
		
		if( ReceivedBytes >= 0 )
		{
			std::cout << "Got " << ReceivedBytes << " bytes" << std::endl;
			return ReceivedBytes;
		}
		else
		{
			if( errno == EWOULDBLOCK ||
				errno == EAGAIN )
			{
				return 0;
			}
			return -1;
		}
	}

	int NetSocketUDP::GetSocket( ) const
	{
		return m_Socket;
	}

	NetSocketUDP::NetSocketUDP( const int p_Socket ) :
		m_Socket( p_Socket )
	{
	}

	NetSocketUDPPtr CreateUDPSocket( const SOCKET_ADDRESS_FAMILY p_Family )
	{
		int Socket = socket( p_Family, SOCK_DGRAM, IPPROTO_UDP );

		if( Socket != T_INVALID_SOCKET )
		{
			// Temporarily set to non-blocking always
			int NonBlock = 1;
			ioctl( Socket, FIONBIO, &NonBlock );
			return NetSocketUDPPtr( new NetSocketUDP( Socket ) );
		}
		else
		{
			return nullptr;
		}
	}

	fd_set *FillSetFromArray( fd_set &p_FDSet,
		const std::vector< NetSocketUDPPtr > *p_pSockets )
	{
		if( p_pSockets )
		{
			FD_ZERO( &p_FDSet );

			for( const NetSocketUDPPtr &Socket : *p_pSockets )
			{
				FD_SET( Socket->m_Socket, &p_FDSet );
			}

			return &p_FDSet;
		}

		return nullptr;
	}

	void FillArrayFromSet( std::vector< NetSocketUDPPtr > *p_pOutSockets,
		const std::vector< NetSocketUDPPtr > *p_pInSockets,
		const fd_set &p_FDSet )
	{
		if( p_pInSockets && p_pOutSockets )
		{
			p_pOutSockets->clear( );

			for( const NetSocketUDPPtr &Socket : *p_pInSockets )
			{
				if( FD_ISSET( Socket->m_Socket, &p_FDSet ) )
				{
					p_pOutSockets->push_back( Socket );
				}
			}
		}
	}

	T_SINT32 Select( const std::vector< NetSocketUDPPtr > *p_pReadSetIn,
		std::vector< NetSocketUDPPtr > *p_pReadSetOut,
		const std::vector< NetSocketUDPPtr > *p_pWriteSetIn,
		std::vector< NetSocketUDPPtr > *p_pWriteSetOut,
		const std::vector< NetSocketUDPPtr > *p_pExceptSetIn,
		std::vector< NetSocketUDPPtr > *p_pExceptSetOut,
		struct timeval *p_pTimeOut )
	{
		fd_set Read, Write, Except;

		fd_set *pRead = FillSetFromArray( Read, p_pReadSetIn );
		fd_set *pWrite = FillSetFromArray( Write, p_pWriteSetIn );
		fd_set *pExcept = FillSetFromArray( Except, p_pExceptSetIn );

		int Return = select( 0, pRead, pWrite, pExcept, p_pTimeOut );

		if( Return > 0 )
		{
			FillArrayFromSet( p_pReadSetOut, p_pReadSetIn, Read );
			FillArrayFromSet( p_pWriteSetOut, p_pWriteSetIn, Write );
			FillArrayFromSet( p_pExceptSetOut, p_pExceptSetIn, Except );
		}

		return Return;
	}
}

