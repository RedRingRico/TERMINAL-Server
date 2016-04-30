#ifndef __TERMINAL_NETSOCKET_HPP__
#define __TERMINAL_NETSOCKET_HPP__

#include <DataTypes.hpp>
#include <sys/socket.h>
#include <memory>
#include <vector>

namespace TERMINAL
{
	class NetSocketUDP;
	class NetSocketAddress;

	const T_MEMSIZE MAX_PACKET_SIZE			= 1400;
	const T_MEMSIZE MAX_PACKETS_PER_FRAME	= 16;

	typedef std::shared_ptr< NetSocketUDP > NetSocketUDPPtr;

	enum SOCKET_ADDRESS_FAMILY
	{
		SOCKET_ADDRESS_FAMILY_INET = AF_INET,
		SOCKET_ADDRESS_FAMILY_INET6 = AF_INET6
	};

	class NetSocketUDP
	{
	public:
		~NetSocketUDP( );

		T_UINT32 Bind( NetSocketAddress &p_BindAddress );
		T_SINT32 SendTo( const void *p_pData, const T_MEMSIZE p_Length,
			const NetSocketAddress &p_To );
		T_SINT32 ReceiveFrom( void *p_pBuffer, const T_MEMSIZE p_Length,
			NetSocketAddress &p_From );

		int GetSocket( ) const;

	private:
		NetSocketUDP( const int p_Socket );

		friend NetSocketUDPPtr CreateUDPSocket(
			const SOCKET_ADDRESS_FAMILY p_Family );
		friend fd_set *FillSetFromArray( fd_set &p_pOutSet,
			const std::vector< NetSocketUDPPtr > *p_pSockets );
		friend void FillArrayFromSet(
			std::vector< NetSocketUDPPtr > *p_pOutSockets,
			const std::vector< NetSocketUDPPtr > *p_pInSockets,
			const fd_set &p_FDSet );

		int m_Socket;
	};

	NetSocketUDPPtr CreateUDPSocket( const SOCKET_ADDRESS_FAMILY p_Family );
	fd_set *FillSetFromArray( fd_set &p_pOutSet,
		const std::vector< NetSocketUDPPtr > *p_pSockets );
	void FillArrayFromSet( std::vector< NetSocketUDPPtr > *p_pOutSockets,
		const std::vector< NetSocketUDPPtr > *p_pInSockets,
		const fd_set &p_FDSet );
	T_SINT32 Select( const std::vector< NetSocketUDPPtr > *p_pReadSetIn,
		std::vector< NetSocketUDPPtr > *p_pReadSetOut,
		const std::vector< NetSocketUDPPtr > *p_pWriteSetIn,
		std::vector< NetSocketUDPPtr > *p_pWriteSetOut,
		const std::vector< NetSocketUDPPtr > *p_pExceptSetIn,
		std::vector< NetSocketUDPPtr > *p_pExceptSetOut,
		struct timeval *p_pTimeOut );
}

#endif // __TERMINAL_NETSOCKET_HPP__
