#include "GCore_printf.h"
#include <stdio.h>
#include <string.h>

#include <time.h>
#if defined( ANDROID )
#include <android\log.h>

#define GLOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,  \
											 "GODS", \
											 __VA_ARGS__))
#define GLOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  \
											 "GODS", \
											 __VA_ARGS__))
#define GLOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  \
											 "GODS", \
											 __VA_ARGS__))
#define GLOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  \
											 "GODS", \
											 __VA_ARGS__))

#elif defined( WIN32 ) || defined( _WIN32 )
#include <Windows.h>
#endif

void _gods_internal_debug_printf_file( const char* chars, int nCharCount )
{																					
	static char __debug_file_name[32];												
	static char temp[8];															
	static time_t __today_0 = time(0);		
#if defined( WIN32 ) // || defined( _WIN32 )
	static tm __debug_tm;															
	static errno_t __errMy	= gmtime_s(&__debug_tm, &__today_0);					
	static errno_t errMt_0 = sprintf_s(__debug_file_name, "log__");											
	static errno_t errMt_1 = _itoa_s(__debug_tm.tm_year+1900, temp, 10);											
	static errno_t errMt_2 = strcat_s(__debug_file_name, temp);												
	static errno_t errMt_3 = strcat_s(__debug_file_name, "__");												
	static errno_t errMt_4 = _itoa_s(__debug_tm.tm_mon+1, temp, 10);											
	static errno_t errMt_5 = strcat_s(__debug_file_name, temp);												
	static errno_t errMt_6 = strcat_s(__debug_file_name, "__");												
	static errno_t errMt_7 = _itoa_s(__debug_tm.tm_mday, temp, 10);											
	static errno_t errMt_8 = strcat_s(__debug_file_name, temp);												
	static errno_t errMt_9 = strcat_s(__debug_file_name, ".txt");											
	FILE* __debug_fp = 0;															
	errno_t ferrMy;
	OutputDebugStringA( chars );
	if( 2 == (ferrMy=fopen_s( &__debug_fp, __debug_file_name, "ab" )) )				
	{																				
		if( 0 == fopen_s( &__debug_fp, __debug_file_name, "wb" ) )					
		{																			
			fwrite( chars, sizeof( char ), nCharCount, __debug_fp );					
			fclose( __debug_fp );													
		}																			
	}																				
	else if( ferrMy == 0 )															
	{																				
		fwrite( chars, sizeof( char ), nCharCount, __debug_fp );						
		fclose( __debug_fp );														
	}																				
#else
	static tm* __debug_tm = gmtime(&__today_0);
	static int32_t errMt_0 = sprintf(__debug_file_name, "log__");
	static int32_t errMt_1 = snprintf(temp, 8, "%d", __debug_tm->tm_year+1900, temp);
	static const char* errMt_2 = strcat(__debug_file_name, temp);
	static const char* errMt_3 = strcat(__debug_file_name, "__");
	static int32_t errMt_4 = snprintf(temp, 8, "%d", __debug_tm->tm_mon+1, temp);
	static const char* errMt_5 = strcat(__debug_file_name, temp);
	static const char* errMt_6 = strcat(__debug_file_name, "__");
	static int32_t errMt_7 = snprintf(temp, 8, "%d", __debug_tm->tm_mday, temp);
	static const char* errMt_8 = strcat(__debug_file_name, temp);
	static const char* errMt_9 = strcat(__debug_file_name, ".txt");
	GLOGE( chars );
	FILE* __debug_fp = fopen(  __debug_file_name, "ab" );
	if( 0 == __debug_fp )
	{
		if( __debug_fp = fopen( __debug_file_name, "wb" ) )
		{
			fwrite( chars, sizeof( char ), nCharCount, __debug_fp );
			fclose( __debug_fp );
		}
	}
	else //if( ferrMy == 0 )
	{
		fwrite( chars, sizeof( char ), nCharCount, __debug_fp );
		fclose( __debug_fp );
	}
#endif 
}

void _gods_internal_debug_printf_debugger( const char* chars )
{
#if defined( ANDROID )
	GLOGE( chars );
#elif defined(WIN32) || defined(_WIN32)
	OutputDebugStringA( chars );
#endif
};

void _gods_internal_debug_printf_console( const char* chars )
{
	printf("%s", chars);
};
