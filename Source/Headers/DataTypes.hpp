#ifndef __TERMINAL_DATATYPES_HPP__
#define __TERMINAL_DATATYPES_HPP__

#include <cstdint>
#include <cstddef>

namespace TERMINAL
{
	typedef int8_t		T_SINT8;
	typedef uint8_t		T_UINT8;
	typedef int16_t		T_SINT16;
	typedef uint16_t	T_UINT16;
	typedef int32_t		T_SINT32;
	typedef uint32_t	T_UINT32;
	typedef float		T_FLOAT32;

	typedef size_t		T_MEMSIZE;

	typedef T_UINT8		T_BYTE;

	const T_MEMSIZE T_SIZEOF_SINT8 = sizeof( T_SINT8 );
	const T_MEMSIZE T_SIZEOF_UINT8 = sizeof( T_UINT8 );
	const T_MEMSIZE T_SIZEOF_SINT16 = sizeof( T_SINT16 );
	const T_MEMSIZE T_SIZEOF_UINT16 = sizeof( T_UINT16 );
	const T_MEMSIZE T_SIZEOF_SINT32 = sizeof( T_SINT32 );
	const T_MEMSIZE T_SIZEOF_UINT32 = sizeof( T_UINT32 );
	const T_MEMSIZE T_SIZEOF_FLOAT32 = sizeof( T_FLOAT32 );
	const T_MEMSIZE T_SIZEOF_MEMSIZE = sizeof( T_MEMSIZE );
	const T_MEMSIZE T_SIZEOF_BYTE = sizeof( T_BYTE );

	const int T_INVALID_SOCKET = -1;
}

#endif // __TERMINAL_DATATYPES_HPP__

