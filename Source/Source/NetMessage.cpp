#include <NetMessage.hpp>
#include <iostream>
#include <cassert>
#include <cstring>

namespace TERMINAL
{
	NetMessage::NetMessage( T_BYTE *p_pBuffer, const T_MEMSIZE p_Length ) :
		m_Overflow( false ),
		m_MaxSize( p_Length ),
		m_Size( 0 ),
		m_ReadPosition( 0 ),
		m_pData( p_pBuffer )
	{
	}

	NetMessage::~NetMessage( )
	{
		// Nothing to do
	}

	void NetMessage::Clear( )
	{
		m_Size = 0;
		m_ReadPosition = 0;
		m_Overflow = false;
	}

	void NetMessage::AddSequences( NetClient *p_pClient )
	{
		// Later...
	}

	void NetMessage::Write( const void *p_pData, const T_MEMSIZE p_Length )
	{
		this->CopyToInternalBuffer( p_pData, p_Length );
	}

	void NetMessage::WriteByte( const T_BYTE p_Byte )
	{
		this->CopyToInternalBuffer( &p_Byte, T_SIZEOF_BYTE );
	}

	void NetMessage::WriteInt16( const T_SINT16 p_Int16 )
	{
		this->CopyToInternalBuffer( &p_Int16, T_SIZEOF_SINT16 );
	}

	void NetMessage::WriteUInt16( const T_UINT16 p_UInt16 )
	{
		this->CopyToInternalBuffer( &p_UInt16, T_SIZEOF_UINT16 );
	}

	void NetMessage::WriteInt32( const T_SINT32 p_Int32 )
	{
		this->CopyToInternalBuffer( &p_Int32, T_SIZEOF_SINT32 );
	}

	void NetMessage::WriteUInt32( const T_UINT32 p_UInt32 )
	{
		this->CopyToInternalBuffer( &p_UInt32, T_SIZEOF_UINT32 );
	}

	void NetMessage::WriteFloat32( const T_FLOAT32 p_Float )
	{
		this->CopyToInternalBuffer( &p_Float, T_SIZEOF_FLOAT32 );
	}

	void NetMessage::WriteString( const char *p_pString )
	{
		if( p_pString )
		{
			this->Write( p_pString, strlen( p_pString ) + 1 );
		}
	}

	void NetMessage::WriteString( const char *p_pString,
		const T_MEMSIZE p_Length )
	{
		if( p_pString )
		{
			this->Write( p_pString, p_Length );
		}
	}

	void NetMessage::BeginReading( )
	{
		m_ReadPosition = 0;
	}

	void NetMessage::Read( void *p_pData, const T_MEMSIZE p_Length )
	{
		this->CopyToExternalBuffer( p_pData, p_Length );
	}

	T_BYTE NetMessage::ReadByte( )
	{
		T_BYTE Byte;

		this->CopyToExternalBuffer( &Byte, T_SIZEOF_BYTE );

		return Byte;
	}

	T_SINT16 NetMessage::ReadInt16( )
	{
		T_SINT16 Int16;

		this->CopyToExternalBuffer( &Int16, T_SIZEOF_SINT16 );

		return Int16;
	}

	T_UINT16 NetMessage::ReadUInt16( )
	{
		T_UINT16 UInt16;

		this->CopyToExternalBuffer( &UInt16, T_SIZEOF_UINT16 );

		return UInt16;
	}

	T_SINT32 NetMessage::ReadInt32( )
	{
		T_SINT32 Int32;

		this->CopyToExternalBuffer( &Int32, T_SIZEOF_SINT32 );

		return Int32;
	}

	T_UINT32 NetMessage::ReadUInt32( )
	{
		T_UINT32 UInt32;

		this->CopyToExternalBuffer( &UInt32, T_SIZEOF_UINT32 );

		return UInt32;
	}

	T_FLOAT32 NetMessage::ReadFloat32( )
	{
		float Float;

		this->CopyToExternalBuffer( &Float, T_SIZEOF_FLOAT32 );

		return Float;
	}

	void NetMessage::ReadString( char *p_pString )
	{
		if( p_pString )
		{
			T_MEMSIZE StringLength = 0;
			T_MEMSIZE Position = m_ReadPosition;
			char Char = m_pData[ Position++ ];

			while( Char != '\0' )
			{
				Char = m_pData[ Position++ ];
			}

			// Add one to account for the null char
			this->CopyToExternalBuffer( p_pString, StringLength + 1 );
		}
	}

	void NetMessage::ReadString( char *p_pString, const T_MEMSIZE p_Length )
	{
		if( p_pString )
		{
			this->CopyToExternalBuffer( p_pString, p_Length );
		}
	}

	bool NetMessage::GetOverflow( ) const
	{
		return m_Overflow;
	}

	T_MEMSIZE NetMessage::GetSize( ) const
	{
		return m_Size;
	}

	T_UINT32 NetMessage::CopyToInternalBuffer( const void *p_pSource,
		const T_MEMSIZE p_Size )
	{
		memcpy( this->GetBufferPosition( p_Size ), p_pSource, p_Size );

		return 0;
	}

	T_UINT32 NetMessage::CopyToExternalBuffer( void *p_pDestination,
		const T_MEMSIZE p_Size )
	{
		if( p_Size + m_Size > m_MaxSize )
		{
			std::cout << "[TERMINAL::NetMessage::CopyToExternalBuffer] "
				"<ERROR> Requested size out of memory bounds" << std::endl;

			return 1;
		}

		memcpy( p_pDestination, &m_pData[ m_ReadPosition ], p_Size );

		m_ReadPosition += p_Size;

		return 0;
	}

	T_BYTE *NetMessage::GetBufferPosition( const T_MEMSIZE p_Length )
	{
		T_BYTE *pBuffer;

		if( ( p_Length + m_Size ) > m_MaxSize )
		{
			if( p_Length > m_MaxSize )
			{
				// Need to keep daemon in mind when doing this
				std::cout << "[TERMINAL::NetMessage::GetBufferPosition] "
					"<ERROR> Requested length is greater than total allowable "
					"message length" << std::endl;

				assert( false );
			}

			this->Clear( );
			m_Overflow = true;
		}

		pBuffer = m_pData + p_Length;
		m_Size += p_Length;

		return pBuffer;
	}
}

