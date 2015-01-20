/// GCore_function_macros.h
///
/// Function definitions as macros for common functions for handling reference counting and save/load core structures to/from file/memory streams.
/// NOTES: 
/// - As this function contains function definitions, it should be include only in the .cpp files that require it, 
/// instead of including in a header file, in order to reduce compile times.
///
/// - These functions are low level functions written in C which were been tested thorougly and may require 
/// a good understanding of software engineering and resource management to be able to modify them properly.
/// 
/// This file is part of the Game Object Data System.
/// 2013 (c) Pablo Ariel Zorrilla Cepeda
#ifndef _GCORE_FUNCTION_MACROS_H
#define _GCORE_FUNCTION_MACROS_H

#include "GCore_macros.h"
#include "GCore_io_function_macros.h"
#include "GCore_memory_function_macros.h"

#define GCORE_STRUCT_HEADER_SIZE sizeof(god::refcount_t)
#define GCORE_STRUCT_FOOTER_SIZE sizeof(void*)
#define GCORE_STRUCT_DATA_SIZE(structName) (sizeof(structName)-(GCORE_STRUCT_FOOTER_SIZE+GCORE_STRUCT_HEADER_SIZE))
//#define GCORE_STRUCT_DATA_SIZE(structName) sizeof(S##Token)

//#define GCORE_INTERFACE_HEADER_SIZE (sizeof(god::refcount_t)+sizeof(uint16_t)*2)
//#define GCORE_INTERFACE_FOOTER_SIZE sizeof(void*)
//#define GCORE_INTERFACE_DATA_SIZE(structName) (sizeof(structName)-((GCORE_STRUCT_FOOTER_SIZE)+(GCORE_STRUCT_HEADER_SIZE)))


#define __GCORE_STRUCT_INIT_STATIC_MEMBERS(TOKEN, Token)																	\
	const god::cue_t	__GODS(TOKEN)::__kCue					= #TOKEN;													\
	const uint32_t		__GODS(TOKEN)::__kDataSize				= sizeof(S##Token);											\
	const uint8_t		__GODS(TOKEN)::__kCueLen				= (uint8_t)strlen(#TOKEN);									\
	const uint8_t		__GODS(TOKEN)::__kDataOffset			= GCORE_STRUCT_HEADER_SIZE;									\
	const uint8_t		__GODS(TOKEN)::__kDataTurbo				=	(uint8_t) (												\
																	( 0 == (sizeof(S##Token) % 8) ) ? 8 :					\
																	( 0 == (sizeof(S##Token) % 4) ) ? 4 : 					\
																	( 0 == (sizeof(S##Token) % 2) ) ? 2 : 					\
																	1 );													\
	const uint8_t		__GODS(TOKEN)::__kDataReserved			=	(uint8_t) (												\
																	( 0 == (sizeof(S##Token) % 8)) ? sizeof(S##Token) / 8 :	\
																	( 0 == (sizeof(S##Token) % 4)) ? sizeof(S##Token) / 4 :	\
																	( 0 == (sizeof(S##Token) % 2)) ? sizeof(S##Token) / 2 :	\
																	sizeof(S##Token) );										\
	template<> GODS(TOKEN)	const 			gbaselist_safe<GODS(TOKEN)>::INVALID	= 0;									\
	template<> GODS(TOKEN)					gbaselist_safe<GODS(TOKEN)>::_INVALID	= 0;									\
	template<> GODS(TOKEN)					gbase_ptr<GODS(TOKEN)>::INVALID		= 0;

//
#define GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TOKEN,Token)			\
	__GCORE_STRUCT_INIT_STATIC_MEMBERS(TOKEN,Token);								\
	__GDEFINE_CREATEDATA_FUNCTION_NO_MEMBER_INIT(__GODS(TOKEN),Token);				\
	__GDEFINE_FREEDATA_FUNCTION_NO_MEMBER_CLEANUP(__GODS(TOKEN),Token);				\
	__GDEFINE_ACQUIREDATA_FUNCTION(__GODS(TOKEN), Token);							\
	__GDEFINE_COPYDATA_FUNCTION_NO_MEMBER_POINTERS(__GODS(TOKEN));					\
	__GDEFINE_CLONEDATA_FUNCTION_NO_MEMBER_POINTERS(__GODS(TOKEN));					\
	__GDEFINE_COMPAREDATA_FUNCTION(__GODS(TOKEN));									\
	__GDEFINE_FILEMEM_SERIALIZATION_FUNCTIONS_NO_MEMBER_POINTERS(__GODS(TOKEN));	\
	__GDEFINE_FILEMEM_STREAMING_FUNCTIONS_NO_MEMBER_POINTERS(__GODS(TOKEN));		\
	__GDEFINE_FREEARRAY_FUNCTION(__GODS(TOKEN));									\
	__GDEFINE_AMIOWNING_DATA(__GODS(TOKEN));

#define GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD(TOKEN,Token)			\
	__GCORE_STRUCT_INIT_STATIC_MEMBERS(TOKEN,Token);								\
	__GDEFINE_CREATEDATA_FUNCTION_MEMBER_INIT(__GODS(TOKEN),Token);					\
	__GDEFINE_FREEDATA_FUNCTION_NO_MEMBER_CLEANUP(__GODS(TOKEN),Token);				\
	__GDEFINE_ACQUIREDATA_FUNCTION(__GODS(TOKEN),Token);							\
	__GDEFINE_COPYDATA_FUNCTION_NO_MEMBER_POINTERS(__GODS(TOKEN));					\
	__GDEFINE_CLONEDATA_FUNCTION_NO_MEMBER_POINTERS(__GODS(TOKEN));					\
	__GDEFINE_COMPAREDATA_FUNCTION(__GODS(TOKEN));									\
	__GDEFINE_FILEMEM_SERIALIZATION_FUNCTIONS_NO_MEMBER_POINTERS(__GODS(TOKEN));	\
	__GDEFINE_FILEMEM_STREAMING_FUNCTIONS_NO_MEMBER_POINTERS(__GODS(TOKEN));		\
	__GDEFINE_FREEARRAY_FUNCTION(__GODS(TOKEN));									\
	__GDEFINE_AMIOWNING_DATA(__GODS(TOKEN));


static const char* STRING_BOOL_TRUE = "true";
static const char* STRING_BOOL_FALSE = "false";
static const wchar_t* WSTRING_BOOL_TRUE = L"true";
static const wchar_t* WSTRING_BOOL_FALSE = L"false";

#define G_CSTRING_FROM_BOOL(b) ( (b) ? (STRING_BOOL_TRUE)	: (STRING_BOOL_FALSE)	)
#define G_WSTRING_FROM_BOOL(b) ( (b) ? (WSTRING_BOOL_TRUE)	: (WSTRING_BOOL_FALSE)	)


#endif // _GCORE_FUNCTION_MACROS_H