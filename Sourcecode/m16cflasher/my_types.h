#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <limits.h>

#ifndef FALSE
	#define FALSE (0)
#endif

#ifndef TRUE
	#define TRUE (-1)
#endif

typedef int my_bool;
typedef char my_int8;
typedef short my_int16;
typedef long my_int32;
typedef unsigned char my_uint8;
typedef unsigned short my_uint16;
typedef unsigned long my_uint32;

//MS Visual Studio C++ compiler?
#ifdef _MSC_VER
	//MS Visual Studio C++ compiler.
	#ifdef WIN32
		typedef __int64 my_int64;
		typedef unsigned __int64 my_uint64;

		#ifndef MY_INT64
			#define MY_INT64 my_int64
		#endif
		#ifndef MY_UINT64
			#define MY_UINT64 my_uint64
		#endif
	#endif
#else
	//Long long 64 bit type would only work if we're using C99 compilers.
	//LLONG_MAX macro is defined in C99.
	//Exists?
	#ifdef LLONG_MAX
		//C99 compilers.
		typedef signed long long my_int64;

		#ifndef MY_INT64
			#define MY_INT64 my_int64
		#endif
	#endif
	#ifdef ULONG_MAX
		//All other C99 compilers.
		typedef unsigned long long my_uint64;

		#ifndef MY_UINT64
			#define MY_UINT64 my_uint64
		#endif
	#endif
#endif

#ifdef MY_INT64
	typedef my_int64 my_intx;
#else
	typedef my_int32 my_intx;
#endif

#ifdef MY_UINT64
	typedef my_uint64 my_uintx;
#else
	typedef my_uint32 my_uintx;
#endif

#endif
