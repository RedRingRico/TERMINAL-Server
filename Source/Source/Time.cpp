#include <Time.hpp>

namespace TERMINAL
{
	T_UINT64 GetTimeInMicroseconds( )
	{
		struct timespec Spec;

		clock_gettime( CLOCK_REALTIME, &Spec );
		T_UINT64 Sec, uSec;

		Sec = Spec.tv_sec * 1000000ULL;
		uSec = Spec.tv_nsec;

		return Sec + ( ( uSec / 1000ULL ) );
	}
}

