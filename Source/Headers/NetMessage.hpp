#ifndef __TERMINAL_NETMESSAGE_HPP__
#define __TERMINAL_NETMESSAGE_HPP__

#include <DataTypes.hpp>
#include <NetState.hpp>
#include <string>

namespace TERMINAL
{
	const T_SINT8 TMESSAGE_CONNECT		= -100;
	const T_SINT8 TMESSAGE_DISCONNECT	= -101;
	const T_SINT8 TMESSAGE_ADDCLIENT	= -102;
	const T_SINT8 TMESSAGE_REMOVECLIENT	= -103;
	const T_SINT8 TMESSAGE_PING			= -104;

	// Request for the game server list
	const T_UINT32 PACKET_TYPE_LISTREQUEST		= 0x00000100;
	// Tells the client about the servers available
	const T_UINT32 PACKET_TYPE_LISTRESPONSE		= 0x00000101;
	// Game server requesting to be on the list
	const T_UINT32 PACKET_TYPE_REGISTERSERVER	= 0x00011100;
	// Game server quit
	const T_UINT32 PACKET_TYPE_UNREGISTERSERVER	= 0x00011101;
	// Client join request
	const T_UINT32 PACKET_TYPE_CLIENTJOIN		= 0x00011000;
	// Welcome the client
	const T_UINT32 PACKET_TYPE_CLIENTWELCOME	= 0x00010110;

	// Forward declarations
	class NetClient;
	class NetServer;
	class NetSocket;

	class NetMessage
	{
	public:
		NetMessage( T_BYTE *p_pBuffer, const T_MEMSIZE p_Length );
		NetMessage( const NetMessage &p_Other );
		~NetMessage( );

		void Clear( );
		void Reset( const T_MEMSIZE p_Length );
		void AddSequences( NetClient *p_pClient );

		void Write( const void *p_pData, const T_MEMSIZE p_Length );
		void WriteByte( const T_BYTE p_Byte );
		void WriteInt16( const T_SINT16 p_Int16 );
		void WriteUInt16( const T_UINT16 p_UInt16 );
		void WriteInt32( const T_SINT32 p_Int32 );
		void WriteUInt32( const T_UINT32 p_UInt32 );
		void WriteFloat32( const T_FLOAT32 p_Float );
		// Must be a null-terminated string
		void WriteString( const char *p_pString );
		void WriteString( const char *p_pString, const T_MEMSIZE p_Length );
		void WriteString( const std::string &p_String );

		void BeginReading( );
		void Read( void *p_pData, const T_MEMSIZE p_Length );
		T_BYTE ReadByte( );
		T_SINT16 ReadInt16( );
		T_UINT16 ReadUInt16( );
		T_SINT32 ReadInt32( );
		T_UINT32 ReadUInt32( );
		T_FLOAT32 ReadFloat32( );
		// Must be a null-terminated string
		void ReadString( char *p_pString );
		void ReadString( char *p_pString, const T_MEMSIZE p_Length );
		void ReadString( std::string &p_String );

		bool GetOverflow( ) const;
		T_MEMSIZE GetSize( ) const;

		const T_BYTE *GetBuffer( ) const;
		
	private:
		T_UINT32 CopyToInternalBuffer( const void *p_pSource,
			const T_MEMSIZE p_Size );
		T_UINT32 CopyToExternalBuffer( void *p_pDestination,
			const T_MEMSIZE p_Size );
		T_BYTE *GetBufferPosition( const T_MEMSIZE p_Length );

		bool 		m_Overflow;
		T_MEMSIZE	m_MaxSize;
		T_MEMSIZE	m_Size;
		T_MEMSIZE	m_ReadPosition;
		T_BYTE		*m_pData;
		bool		m_InternalBuffer;
	};
}

#endif // __TERMINAL_NETMESSAGE_HPP__

