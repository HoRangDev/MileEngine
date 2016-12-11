#pragma once
#pragma warning(disable : 4251)
#pragma warning(disable : 4512)
#pragma warning(disable : 4996)

#include <iostream>
#include <string>

#define MILE_LIBRARYEXPORT __declspec(dllexport)
#define MILE_LIBRARYIMPORT __declspec(dllimport)

#ifdef MILE_BUILD
#define MILE_API          MILE_LIBRARYEXPORT
#else
#define MILE_API          MILE_LIBRARYIMPORT
#endif

#define MILE_FORCEINLINE  __forceinline

#define MS_ALIGN(N)       __declspec(align(N))
#define ALGIN_4			  MS_ALIGN(4)
#define ALGIN_8			  MS_ALIGN(8)
#define ALGIN_16          MS_ALIGN(16)

#ifndef ASSERT
#ifdef _DEBUG
#include <cassert>
#define ASSERT(Condition) { if( !(Condition) ) { __debugbreak( ); } }
#define ASSERT_MSG(Condition, Message) \
	do  \
	{ \
		if( !(Condition) ) \
		{					\
			std::cerr << "#Assertion " #Condition " failed in " << __FILE__ \
					  << " line " << __LINE__ << ": " << Message << std::endl; \
			ASSERT( Condition );\
		}					\
	} while(false)
#else
#define ASSERT(Condition) ((void)0)
#define ASSERT_MSG(Condition, Message)
#endif
#endif

#define UNUSED_PARAM(Param) ((void)Param)

/** ��ȣ���� �⺻ Ÿ�� */
using uint8 = unsigned char;
using uint16 = unsigned short int;
using uint32 = unsigned int;
using uint64 = unsigned long long;

/** ��ȣ�ִ� �⺻ Ÿ�� */
using int8 = signed char;
using int16 = signed short int;
using int32 = signed int;
using int64 = signed long long;

/** ���� Ÿ�� */
using ANSICHAR = char;
using WIDECHAR = wchar_t;

template<typename Ty>
class TypeDisplayer;