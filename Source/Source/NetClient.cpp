#include <NetClient.hpp>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

namespace TERMINAL
{
    NetClient::NetClient( ) :
    	m_ConnectionState( T_CONNECTIONSTATE_DISCONNECTED ),
	m_OutgoingSequence( 1 ),
	m_IncomingSequence( 0 ),
	m_AcknowledgedSequence( 0 ),
	m_DroppedPackets( 0 ),
	m_ServerPort( 0 ),
	m_Index( 0 ),
	m_PingSent( 0 ),
	m_Latency( 0 ),
	m_LastMessageTime( 0 ),
	m_Initialised( false ),
	m_Message( m_MessageBuffer, sizeof( m_MessageBuffer ) )
    {
	memset( m_MessageBuffer, 0, sizeof( m_MessageBuffer ) );
    }

    NetClient::~NetClient( )
    {
    }

    T_UINT32 NetClient::GetID( ) const
    {
	return m_ID;
    }
}

