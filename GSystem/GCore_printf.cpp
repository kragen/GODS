#include "GCore_printf.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <time.h>
#if defined(ANDROID) || defined(__linux__)
    #ifdef ANDROID
        #include <android/log.h>
        #define GLOG(...) ((void)__android_log_print(__VA_ARGS__))
    #else // regular Linux
        #define GLOG(level, gods, ...) ((void)fprintf(stderr, __VA_ARGS__))
    #endif

    #define GLOGD(...) GLOG(ANDROID_LOG_DEBUG, "GODS", __VA_ARGS__)
    #define GLOGI(...) GLOG(ANDROID_LOG_INFO, "GODS", __VA_ARGS__)
    #define GLOGW(...) GLOG(ANDROID_LOG_WARN, "GODS", __VA_ARGS__)
    #define GLOGE(...) GLOG(ANDROID_LOG_ERROR, "GODS", __VA_ARGS__)

#else // Windows
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
	static int errMt_0 = snprintf(__debug_file_name, sizeof(__debug_file_name),
				      "log__%d__%d__%d.txt",
				      __debug_tm.tm_year+1900,
				      __debug_tm.tm_mon+1,
				      __debug_tm.tm_mday); 
							
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
	UNUSED static int32_t errMt_0 = sprintf(__debug_file_name, "log__");
	UNUSED static int32_t errMt_1 = snprintf(temp, 8, "%d", __debug_tm->tm_year+1900);
	UNUSED static const char* errMt_2 = strcat(__debug_file_name, temp);
	UNUSED static const char* errMt_3 = strcat(__debug_file_name, "__");
	UNUSED static int32_t errMt_4 = snprintf(temp, 8, "%d", __debug_tm->tm_mon+1);
	UNUSED static const char* errMt_5 = strcat(__debug_file_name, temp);
	UNUSED static const char* errMt_6 = strcat(__debug_file_name, "__");
	UNUSED static int32_t errMt_7 = snprintf(temp, 8, "%d", __debug_tm->tm_mday);
	UNUSED static const char* errMt_8 = strcat(__debug_file_name, temp);
	UNUSED static const char* errMt_9 = strcat(__debug_file_name, ".txt");
	GLOGE( chars );
	FILE* __debug_fp = fopen(  __debug_file_name, "ab" );
	if( 0 == __debug_fp )
	{
		if( (__debug_fp = fopen( __debug_file_name, "wb" )) )
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
