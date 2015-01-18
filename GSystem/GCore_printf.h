#ifndef _GCORE_PRINTF_H
#define _GCORE_PRINTF_H

void _gods_internal_debug_printf_file( const char* chars, int nCharCount );
void _gods_internal_debug_printf_debugger( const char* chars );
void _gods_internal_debug_printf_console( const char* chars );

//#define FORCE_STD_PRINTF_DEBUG	

#if !(defined( _DEBUG ) || defined( DEBUG ))
#define USE_FILE_DEBUG_PRINTF
#endif

#if defined( USE_FILE_DEBUG_PRINTF ) && !defined( FORCE_STD_PRINTF_DEBUG )
#define _gods_internal_debug_printf( chars, nCharCount ) _gods_internal_debug_printf_file( chars, nCharCount )
#define _gods_internal_debug_wprintf( chars, nCharCount )  OutputDebugStringW( chars )
#elif (defined( _WIN32 ) || defined( WIN32 )) && !defined( FORCE_STD_PRINTF_DEBUG )
#define _gods_internal_debug_printf( chars, nCharCount ) _gods_internal_debug_printf_debugger( chars )
#define _gods_internal_debug_wprintf( chars, nCharCount )  OutputDebugStringW( chars )
#else // I use this because I don't have the debugger attached during release build test. Anyway it should be replaced with a proper fprintf to an open log stream.
#define _gods_internal_debug_printf( chars, nCharCount ) _gods_internal_debug_printf_console( chars )
#define _gods_internal_debug_wprintf( chars, nCharCount ) wprintf( L"%s", chars )
#endif

#define gods_printf( TEXT, format, ... )											\
{																					\
	static char __dbg__buffer_[256]={0};											\
	static size_t __dbg_lenMy = sprintf_s( __dbg__buffer_, "[%s (%i)] " TEXT		\
		, __FILE__																	\
		, __LINE__ );																\
	_gods_internal_debug_printf( __dbg__buffer_, (int)__dbg_lenMy );				\
	char __dbg__buffer_2[1024]={0};													\
	size_t __dbg_lenMy_2 = sprintf_s( __dbg__buffer_2								\
		, format "\n"																\
		, __VA_ARGS__ );															\
	_gods_internal_debug_printf( __dbg__buffer_2, (int)__dbg_lenMy_2 );				\
}

#define gods_wprintf( TEXT, format, ... )											\
{																					\
	static char __dbg__buffer_[256]={0};											\
	static size_t __dbg_lenMy = sprintf_s( __dbg__buffer_, "[%s (%i)] " TEXT		\
		, __FILE__																	\
		, __LINE__ );																\
	_gods_internal_debug_printf( __dbg__buffer_, (int)__dbg_lenMy );				\
	static wchar_t __dbg__wbuffer_[1024]={0};										\
	size_t __dbg_lenMy_2 = swprintf_s( __dbg__wbuffer_								\
		, format L"\n"																\
		, __VA_ARGS__ );															\
	_gods_internal_debug_wprintf( __dbg__wbuffer_, (int)__dbg_lenMy_2 );			\
}

#ifdef DEBUG_PRINTF_ENABLED
#	if defined( ANDROID )
#		define debug_printf( ... )			__android_log_print( ANDROID_LOG_DEBUG, "(idc)", __FILE__ ": " __VA_ARGS__ );
#		define debug_wprintf( ... )
#	else
#		define debug_printf( format, ... )	gods_printf( "INFO: ", format, __VA_ARGS__ )
#		define debug_wprintf( format, ... )	gods_wprintf( "INFO: ", format, __VA_ARGS__ )
#	endif
#else
#	define debug_printf( ... ) 
#	define debug_wprintf( ... ) 
#endif

#ifdef WARNING_PRINTF_ENABLED
#	if defined( ANDROID )
#		define warning_printf( ... )			__android_log_print(ANDROID_LOG_WARN, "(lesser wtf)", __FILE__ ": " __VA_ARGS__  );
#		define warning_wprintf( ... ) 
#	elif defined( WIN32 ) || defined ( _WIN32 )
#		define warning_printf( format, ... )	gods_printf( "WARNING! ", format, __VA_ARGS__ )
#		define warning_wprintf( format, ... )	gods_wprintf( "WARNING! ", format, __VA_ARGS__ )
#	else 
#		define warning_printf( format, ... )	gods_printf( "WARNING! ", format, __VA_ARGS__ )
#		define warning_wprintf( format, ... )	gods_wprintf( "WARNING! ", format, __VA_ARGS__ )
#	endif
#else
#	define warning_printf( ... ) 
#	define warning_wprintf( ... ) 
#endif

#ifdef ERROR_PRINTF_ENABLED
#	if defined( ANDROID )
#		define error_printf( ... )			__android_log_print(ANDROID_LOG_ERROR, "(wtf)", __FILE__ ": " __VA_ARGS__  );
#		define error_wprintf( ... ) 
#	elif defined( WIN32 ) || defined ( _WIN32 )
#		define error_printf( format, ... )	{ gods_printf( __FUNCTION__ ": ERROR! ", format, __VA_ARGS__ ) ; /*Beep( (uint32_t)strlen( format )*15, 50 );*/ /*_CrtDbgBreak();*/ }
#		define error_wprintf( format, ... )	{ gods_wprintf( __FUNCTION__ ": ERROR! ", format, __VA_ARGS__ ); /*Beep( (uint32_t)wcslen( format )*15, 50 );*/ /*_CrtDbgBreak();*/ }
#	else
#		define error_printf( format, ... )	{ gods_printf( "ERROR! ", format, __VA_ARGS__ ) ; }
#		define error_wprintf( format, ... )	{ gods_wprintf( "ERROR! ", format, __VA_ARGS__ ); }
#	endif
#else
#	define error_printf( ... ) 
#	define error_wprintf( ... ) 
#endif

#ifdef DATA_PRINTF_ENABLED
#	if defined( ANDROID )
#		define data_printf( ... )			__android_log_print(ANDROID_LOG_DEBUG, "data", __FILE__ ": " __VA_ARGS__  );
#		define data_wprintf( ... ) 
#	else 
#		define data_printf( format, ... )	gods_printf( "(data): \n", format, __VA_ARGS__ )
#		define data_wprintf( format, ... )	gods_wprintf( "(data): \n", format, __VA_ARGS__ )
#	endif
#else
#	define data_printf( ... ) 
#	define data_wprintf( ... ) 
#endif


#ifdef RENDER_PRINTF_ENABLED
#	if defined( ANDROID )
#		define render_printf( ... )				__android_log_print(ANDROID_LOG_DEBUG, "(render)", __FILE__ ": " __VA_ARGS__  );
#		define render_wprintf( ... ) 
#	else
#		define render_printf( format, ... )		gods_printf( "(render) ", format, __VA_ARGS__ )
#		define render_wprintf( format, ... )	gods_printf( "(render) ", format, __VA_ARGS__ )
#	endif
#else
#	define render_printf( ... ) 
#	define render_wprintf( ... ) 
#endif

#ifdef SOUND_PRINTF_ENABLED
#	if defined( ANDROID )
#		define sound_printf( ... )			__android_log_print(ANDROID_LOG_DEBUG, "(sound)", __FILE__ ": " __VA_ARGS__  );
#		define sound_wprintf( ... ) 
#	else
#		define sound_printf( format, ... )	gods_printf( "(sound) ", format, __VA_ARGS__ )
#		define sound_wprintf( format, ... )	gods_printf( "(sound) ", format, __VA_ARGS__ )
#	endif
#else
#	define sound_printf( ... ) 
#	define sound_wprintf( ... ) 
#endif

#ifdef VERBOSE_PRINTF_ENABLED
#	if defined( ANDROID )
#		define verbose_printf( ... )			__android_log_print( ANDROID_LOG_DEBUG, "(verbose)", __FILE__ ": " __VA_ARGS__ );
#		define verbose_wprintf( ... )
#	else
#		define verbose_printf( format, ... )	gods_printf( "(verbose) ", format, __VA_ARGS__ )
#		define verbose_wprintf( format, ... )	gods_wprintf( "(verbose) ", format, __VA_ARGS__ )
#	endif
#else
#	define verbose_printf( ... ) 
#	define verbose_wprintf( ... ) 
#endif

#define errmsg_alloc()			error_printf("Failed to allocate! Out of memory?")
#define errmsg_fileread()		error_printf("Failed to read from file!")
#define errmsg_filewrite()		error_printf("Failed to write to file!")

#endif // _GCORE_PRINTF_H