#ifndef __TERMIANL_NETMESSAGE_HPP__
#define __TERMINAL_NETMESSAGE_HPP__

#include <DataTypes.hpp>
#include <NetState.hpp>

namespace TERMINAL
{
	const T_SINT8 TMESSAGE_CONNECT		= -100;
	const T_SINT8 TMESSAGE_DISCONNECT	= -101;
	const T_SINT8 TMESSAGE_ADDCLIENT	= -102;
	const T_SINT8 TMESSAGE_REMOVECLIENT	= -103;
	const T_SINT8 TMESSAGE_PING			= -104;

	// Forward declarations
	class NetClient;
	class NetServer;
	class NetSocket;

	class NetMessage
	{
	public:
		NetMessage( T_BYTE *p_pBuffer, const T_MEMSIZE p_Length );
		~NetMessage( );

		void Clear( );
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

		bool GetOverflow( ) const;
		T_MEMSIZE GetSize( ) const;
		
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
	};
}

#endif // __TERMINAL_NETMESSAGE_HPP__

