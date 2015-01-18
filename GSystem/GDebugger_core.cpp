/// This file is part of the Game Oriented Development Software Kit
/// Copyright © 2013: Pablo Ariel Zorrilla Cepeda
#include "GCore_constants.h"
#include "GCore_function_macros.h"

#if defined(ANDROID)
#elif defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

//	enabling profiling and debug on this file will make the runtime crash because of the debugger trying to debug itself recursively
#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

//#define GODS_DISABLE_DEBUGGER

using namespace god;

static int64_t				__g_nStartTime				= 0;
static int64_t				__g_nCountsPerSecond		= 0;
static double				__g_fSecondsPerCount		= 0;

static uint32_t				__g_nStackMethodCount		= 0;
static uint32_t				__g_nStackMethodArraySize	= 0;
static GODS(METHODDEBUG)*	__g_lstStackMethod			= 0;
static bool					__g_bEnableGDebugger		= false;

__GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD(METHODDEBUG,MethodDebug);

struct __CStackMethodDestructor
{
	__CStackMethodDestructor()
	{
		//GPROFILE_METHOD( "__CStackMethodDestructor", "__CStackMethodDestructor" );
	}
	~__CStackMethodDestructor()	
	{
		// WARNING! Enabling the following line causes a memory leak! This is because the profiler/debugger relies on this instance for memory management
		// GPROFILE_METHOD( "__CStackMethodDestructor", "~__CStackMethodDestructor" );
		if(	__g_lstStackMethod )
		{
			debug_printf("Preparing to deallocate %u GMETHODDEBUG_DATA instances", __g_nStackMethodCount);	
			while(__g_nStackMethodCount)
			{
				__g_nStackMethodCount--;
				free(__g_lstStackMethod[__g_nStackMethodCount]);
			}
			free( __g_lstStackMethod );	
			__g_lstStackMethod = 0;	
			debug_printf("Deallocation finished");	
		}
	}
};	
//#ifndef GInitDebugger
//void god::GInitDebugger()
//{
//#ifdef _WIN32
//	QueryPerformanceFrequency( (LARGE_INTEGER*)&__g_nCountsPerSecond );
//	QueryPerformanceCounter( (LARGE_INTEGER*)&__g_nStartTime );
//	__g_fSecondsPerCount = 1.0/__g_nCountsPerSecond;
//#else
//	//throw("Not implemented\n");
//#endif
//
//	// __g_nStackMethodIndex		= 0;
//	// __g_nStackMethodCount		= 0;
//	// __g_nStackMethodArraySize	= 0;
//	// __g_lstStackMethod			= 0;
//};
//#endif



bool god::genableDebugger( bool bEnable )
{
	return __g_bEnableGDebugger = bEnable;
};

bool god::genabledDebugger()
{
	return __g_bEnableGDebugger;
};

CGDbgMethod::CGDbgMethod( const void* pInstanceAddress, const char* wszClassName, const char* wszMethodName )
:m_MethodData(0)
{
	if( !__g_bEnableGDebugger )
		return;
#ifndef GODS_DISABLE_DEBUGGER
	if( 0 == __g_fSecondsPerCount )
	{
#ifdef _WIN32
		QueryPerformanceFrequency( (LARGE_INTEGER*)&__g_nCountsPerSecond );
		QueryPerformanceCounter( (LARGE_INTEGER*)&__g_nStartTime );
		__g_fSecondsPerCount = 1.0/__g_nCountsPerSecond;
#	elif defined( ANDROID )
		__g_nCountsPerSecond	= 33000000;
		__g_fSecondsPerCount = 1.0/__g_nCountsPerSecond;
#	else
//		throw("Not implemented\n");
#	endif	
	}
//	__g_nStackMethodIndex++;
	gcreateData( &m_MethodData );
	m_MethodData->CallStackIndex	= __g_nStackMethodCount;
	m_MethodData->InstanceAddress	= pInstanceAddress;
	if( wszClassName )
	{
		char* dst = &m_MethodData->ClassName[0];
		while( *dst++ = *wszClassName++ );
	}
	if( wszMethodName )
	{
		char* dst = &m_MethodData->MethodName[0];
		while( *dst++ = *wszMethodName++ );
	}

	gresizePointerArray( sizeof( GODS(METHODDEBUG) ), __g_nStackMethodCount+1, &__g_nStackMethodArraySize, (void**)&__g_lstStackMethod, 40 );
	__g_lstStackMethod[__g_nStackMethodCount] = m_MethodData;
	__g_nStackMethodCount++;

#	ifdef WIN32
	printf( "Entering: " );
	printInfoString( m_MethodData );
	printf( "\n" );
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_MethodData->TimestampEnter );
#	elif defined( ANDROID )
	__android_log_print(ANDROID_LOG_INFO, "GODS", "Entering method: " );
	printInfoString( m_MethodData );
#	else
	//throw("Not implemented\n");
#	endif	// WIN32
	
#endif // GODS_DISABLE_DEBUGGER
};

CGDbgMethod::~CGDbgMethod( void )
{
// 	if( !__g_bEnableGDebugger )
// 		return; this will cause a memory leak
#ifndef GODS_DISABLE_DEBUGGER
//	__g_nStackMethodIndex--;
#	ifdef WIN32
	if( !m_MethodData )
		return;
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_MethodData->TimestampExit );
	printf( "Exiting: " );
	printInfoString( m_MethodData );
	printf( "\n" );
#	elif defined( ANDROID )
	__android_log_print(ANDROID_LOG_INFO, "GODS", "Exiting method: " );
	printInfoString( m_MethodData );
#	else
	//throw("Not implemented\n");
#	endif	

	gfreeData( &m_MethodData );
	if( __g_lstStackMethod )
	{
		__g_nStackMethodCount--;
		if( 0 == __g_nStackMethodCount )
		{
			//__g_nStackMethodArraySize	= 0;
#	ifdef WIN32
			uint64_t nExitTime;
			QueryPerformanceCounter( (LARGE_INTEGER*)&nExitTime );
			uint64_t TimestampDiff = nExitTime-__g_nStartTime;
			long double DiffInMilliseconds = TimestampDiff*1000*(__g_fSecondsPerCount);
			debug_printf( "Total execution time: %f\n", DiffInMilliseconds );
#	endif // WIN32
			//free( __g_lstStackMethod );
			//__g_lstStackMethod = 0;
		}
	}
	else
	{
		error_printf("A class destructor was called before the constructor initialized the object.");
		//throw(L"A class destructor was called before the constructor initialized the object.");
	}
#endif // GODS_DISABLE_DEBUGGER
};

GODS(METHODDEBUG)	CGDbgMethod::GetMethodData( void )
{
	return gacquireData( m_MethodData );
};

GODS(METHODDEBUG)*	CGDbgMethod::GetMethodStack( void )
{
	
	return __g_lstStackMethod;
};

uint32_t			CGDbgMethod::GetMethodStackSize( void )
{
	return __g_nStackMethodCount;
};

#if defined( ANDROID )
void god::printInfoString( const __GODS(METHODDEBUG)* MethodData )
{
	if( MethodData->TimestampExit )
	{
		uint64_t TimestampDiff = MethodData->TimestampExit-MethodData->TimestampEnter;
		long double DiffInMilliseconds = TimestampDiff*1000*(__g_fSecondsPerCount);
		if( MethodData->ClassName[0] )
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"ClassName				: %s\n"
				"MethodName				: %s()\n"
				"InstanceAddress			: 0x%p\n"
				"CallStackIndex			: %u\n"
				//"TimestampEnter			: %I64u\n"
				//"TimestampExit			: %I64u\n"
				//"TimestampDiff			: %I64u\n"
				"DiffInMilliseconds		: %LG\n"
				, MethodData
				, MethodData->ClassName
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				//, MethodData->TimestampExit
				//, TimestampDiff
				, DiffInMilliseconds
				);
		}
		else
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"MethodName				: %s()\n"
				"InstanceAddress			: 0x%p\n"
				"CallStackIndex			: %u\n"
				//"TimestampEnter			: %I64u\n"
				//"TimestampExit			: %I64u\n"
				//"TimestampDiff			: %I64u\n"
				"DiffInMilliseconds		: %LG\n"
				, MethodData
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				//, MethodData->TimestampExit
				//, TimestampDiff
				, DiffInMilliseconds

				);
		}
	}
	else
	{
		if( MethodData->ClassName[0] )
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"ClassName				: %s\n"
				"MethodName				: %s()\n"
				"InstanceAddress		: 0x%p\n"
				"CallStackIndex		: %u\n"
				//"TimestampEnter		: %I64u\n"
				, MethodData
				, MethodData->ClassName
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				);
		}
		else
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"MethodName			: %s()\n"
				"InstanceAddress		: 0x%p\n"
				"CallStackIndex		: %u\n"
				//"TimestampEnter		: %I64u\n"
				, MethodData
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				);
		}
	}
};
#elif defined(WIN32) || defined(_WIN32)
__GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(METHODDEBUG),
	"ClassName.....................: %s\n"
	"MethodName....................: %s()\n"
	"InstanceAddress...............: 0x%p\n"
	"CallStackIndex................: %u\n"
	"TimestampEnter................: %I64u\n"
	"TimestampExit.................: %I64u\n"
	"TimestampDiff.................: %I64u\n"
	"DiffInMilliseconds............: %LG"
	, in_CoreInstance->ClassName
	, in_CoreInstance->MethodName
	, in_CoreInstance->InstanceAddress
	, in_CoreInstance->CallStackIndex
	, in_CoreInstance->TimestampEnter
	, in_CoreInstance->TimestampExit
	, in_CoreInstance->TimestampExit-in_CoreInstance->TimestampEnter
	, (in_CoreInstance->TimestampExit-in_CoreInstance->TimestampEnter)*1000*(__g_fSecondsPerCount)
);
#else
void god::printInfoString( const __GODS(METHODDEBUG)* MethodData )
{
	if( MethodData->TimestampExit )
	{
		uint64_t TimestampDiff = MethodData->TimestampExit-MethodData->TimestampEnter;
		long double DiffInMilliseconds = TimestampDiff*1000*(__g_fSecondsPerCount);
		if( MethodData->ClassName[0] )
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"ClassName				: %s\n"
				"MethodName				: %s()\n"
				"InstanceAddress			: 0x%p\n"
				"CallStackIndex			: %u\n"
				//"TimestampEnter			: %I64u\n"
				//"TimestampExit			: %I64u\n"
				//"TimestampDiff			: %I64u\n"
				"DiffInMilliseconds		: %LG\n"
				, MethodData
				, MethodData->ClassName
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				//, MethodData->TimestampExit
				//, TimestampDiff
				, DiffInMilliseconds
				);
		}
		else
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"MethodName				: %s()\n"
				"InstanceAddress			: 0x%p\n"
				"CallStackIndex			: %u\n"
				//"TimestampEnter			: %I64u\n"
				//"TimestampExit			: %I64u\n"
				//"TimestampDiff			: %I64u\n"
				"DiffInMilliseconds		: %LG\n"
				, MethodData
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				//, MethodData->TimestampExit
				//, TimestampDiff
				, DiffInMilliseconds

				);
		}
	}
	else
	{
		if( MethodData->ClassName[0] )
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"ClassName				: %s\n"
				"MethodName				: %s()\n"
				"InstanceAddress		: 0x%p\n"
				"CallStackIndex		: %u\n"
				//"TimestampEnter		: %I64u\n"
				, MethodData
				, MethodData->ClassName
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				);
		}
		else
		{
#ifdef ANDROID
			__android_log_print(ANDROID_LOG_INFO, "Call Stack", "MethodData at address 0x%p:\n"
#else
			printf("MethodData at address 0x%p:\n"
#endif
				"MethodName			: %s()\n"
				"InstanceAddress		: 0x%p\n"
				"CallStackIndex		: %u\n"
				//"TimestampEnter		: %I64u\n"
				, MethodData
				, MethodData->MethodName
				, MethodData->InstanceAddress
				, MethodData->CallStackIndex
				//, MethodData->TimestampEnter
				);
		}
	}
};
#endif // WIN32
