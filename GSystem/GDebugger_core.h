/// GDebugger_core.h
///
/// Declares data structures and methods for logging runtime information
///
/// This file is part of the Game Oriented Development Software Kit
/// 2013 (c) Pablo Ariel Zorrilla Cepeda
///
#include "GMemory_core.h"
//#include "GCore_class_macros.h"

#ifdef ANDROID
	#include <android/log.h>
	#include <stdlib.h>
	#include <memory.h>
#elif defined( WIN32 )
	#include <crtdbg.h>
	#include <Windows.h>
#endif

#ifndef _GDEBUGGER_CORE_H
#define _GDEBUGGER_CORE_H

namespace god
{
#pragma pack( push, 1 )
#define MAX_CLASS_NAME_LENGTH	60
#define MAX_METHOD_NAME_LENGTH	100
	GDECLARE_CORE_STRUCT( METHODDEBUG, MethodDebug,
		uint32_t		CallStackIndex;	
		uint64_t		TimestampEnter;		
		uint64_t		TimestampExit;		
		const void*		InstanceAddress;	// 
		char			ClassName[MAX_CLASS_NAME_LENGTH];	// 
		char			MethodName[MAX_METHOD_NAME_LENGTH];	// 
	);
#ifdef _WIN32
#undef GetClassName
#endif

	bool genableDebugger( bool bEnable );
	bool genabledDebugger();

	class CGDbgMethod
	{
	public:
		CGDbgMethod( const void* pInstanceAddress, const char* wszClassName, const char* wszMethodName );
		virtual ~CGDbgMethod( void );

		GODS(METHODDEBUG)			GetMethodData( void );
		static GODS(METHODDEBUG)*	GetMethodStack( void );
		static uint32_t				GetMethodStackSize( void );

	private:
		GODS(METHODDEBUG)			m_MethodData;
	protected:
		CGDbgMethod( void );
		CGDbgMethod& operator=( const CGDbgMethod& );
	};
#pragma pack( pop )

#ifdef GPROFILE_ENABLED
#	define GPROFILE_STATIC_METHOD( wszClassName, wszMethodName )	god::CGDbgMethod __dbgMethod(0, wszClassName, wszMethodName)
#	define GPROFILE_METHOD( wszClassName, wszMethodName )			god::CGDbgMethod __dbgMethod(this, wszClassName, wszMethodName)
#	define GPROFILE_FUNCTION()										god::CGDbgMethod __dbgMethod(0, 0, __FUNCTION__)
#else
#	define GPROFILE_STATIC_METHOD(...)
#	define GPROFILE_METHOD(...)		  
#	define GPROFILE_FUNCTION(...)	  
#endif

};


#endif // _GDEBUGGER_CORE_H