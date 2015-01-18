
#include "GMemory_core.h"
#include "GCore_function_macros.h"

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

#include <stdlib.h>
#if defined(ANDROID)
#elif defined( _WIN32 ) || defined(WIN32)
#include <Windows.h>
#include <crtdbg.h>
#endif

using namespace god;

//__GDEFINE_CREATEDATA_FUNCTION_MEMBER_INIT( __GODS(BUFFER), Buffer );
	//__GDECLARE_INLINE_CORE_STRUCTURE_DEALLOCATOR( __GODS(BUFFER), Buffer )
static uint32_t			__g_nBufferArraySize = 0;
static uint32_t			__g_nBufferCount = 0;
static int32_t			__g_nBufferCreatedRefs = 0;
static int32_t			__g_nBufferAcquiredRefs = 0;
static int32_t			__g_nBufferFreedRefs = 0;
static __GODS(BUFFER)**	__g_lstBuffer = 0;
DECLARE_CRITICAL_SECTION(Buffer);

struct __CBufferDestructor
{
	__CBufferDestructor()
	{
		GPROFILE_METHOD("__C""Buffer""Destructor", "__C""Buffer""Destructor");
	}
	~__CBufferDestructor()
	{
		/* WARNING! Enabling the following line causes a memory leak!*/
		/* This is because the profiler/debugger relies on this function to work		*/
		/* GPROFILE_METHOD( "__C""Buffer""Destructor", "~__C""Buffer""Destructor" );	*/
		DELETE_CRITICAL_SECTION(Buffer);
		if (__g_lstBuffer)
		{
			debug_printf("Preparing to deallocate %u __GODS(BUFFER) instances (%u bytes)", __g_nBufferCount, (sizeof(__GODS(BUFFER)) + sizeof(uint32_t))*__g_nBufferCount);
			debug_printf("%u __GODS(BUFFER) instances created (%u bytes)", __g_nBufferCreatedRefs, (sizeof(__GODS(BUFFER)) + sizeof(uint32_t))*__g_nBufferCreatedRefs);
			debug_printf("%u __GODS(BUFFER) instances acquired (%u bytes)", __g_nBufferAcquiredRefs, (sizeof(__GODS(BUFFER)) + sizeof(uint32_t))*__g_nBufferAcquiredRefs);
			debug_printf("%u __GODS(BUFFER) instances freed (%u bytes)", __g_nBufferFreedRefs, (sizeof(__GODS(BUFFER)) + sizeof(uint32_t))*__g_nBufferFreedRefs);
			while (__g_nBufferCount)
			{
				free(__g_lstBuffer[--__g_nBufferCount]);
			}
			debug_printf("Deallocation finished");
			free(__g_lstBuffer);
			__g_lstBuffer = (__GBUFFER_DATA**)0xFFFFFFFFUL;
		}
	}
};


__GDEFINE_ACQUIREDATA_FUNCTION(__GODS(BUFFER), Buffer);
__GDEFINE_FREEARRAY_FUNCTION(__GODS(BUFFER));

static __CBufferDestructor	__g_BufferDestructor;

void god::gcreateData(__GODS(BUFFER)** p)
{
	/*GPROFILE_FUNCTION();*/
	INIT_CRITICAL_SECTION(Buffer);
	__GODS(BUFFER)* newData = 0;
	ENTER_CRITICAL_SECTION(Buffer);
	if (__g_nBufferCount)
	{
		newData = __g_lstBuffer[--__g_nBufferCount];
		LEAVE_CRITICAL_SECTION(Buffer);
		if (newData)
		{
			if (newData->__nRefCount != 0)
			{
				errmsg_refcountnonnull();
				_CrtDbgBreak();
			}
			newData->__nRefCount = 1;
			/*newData->__pfreeData				= gfreeData;*/
			memset(((char*)newData) + newData->__kDataOffset, 0, newData->__kDataSize);
			if (BINFIBO != (*((uint32_t*)(newData + 1))))
			{
				errmsg_buffover();
				_CrtDbgBreak();
			}
		}
		else
		{
			errmsg_heapcorrupt();
			_CrtDbgBreak();
		}
	}
	else
	{
		LEAVE_CRITICAL_SECTION(Buffer)
			newData = (__GODS(BUFFER)*)malloc(sizeof(__GODS(BUFFER)) + sizeof(uint32_t));
		if (0 != newData)
		{
			newData->__nRefCount = 1;
			newData->__pfreeData = gfreeData;
			*((uint32_t*)(newData + 1)) = BINFIBO;
			memset(((char*)newData) + newData->__kDataOffset, 0, newData->__kDataSize);
		}
		else
		{
			errmsg_alloc();
		}
	}
	gcore_ptr<__GODS(BUFFER)*> oldData = *p;
	(*p) = newData;
	if (newData)
		__g_nBufferCreatedRefs++;
};


__GCORE_STRUCT_INIT_STATIC_MEMBERS( BUFFER, Buffer );

#define __GDECLARE_INLINE_CORE_BUFFER_DEALLOCATOR( Token )																		\
	static uint32_t				__g_n##Token##ArraySize=0;																		\
	static uint32_t				__g_n##Token##Count = 0;																		\
	static GODS(BUFFER)*		__g_lst##Token = 0;																				\
	static int32_t				__g_n##Token##CreatedRefs = 0;																	\
	static int32_t				__g_n##Token##AcquiredRefs = 0;																	\
	static int32_t				__g_n##Token##FreedRefs = 0;																	\
	DECLARE_CRITICAL_SECTION(Token);																							\
																																\
	struct __C##Token##Destructor																								\
	{																															\
		__C##Token##Destructor()																								\
		{																														\
			GPROFILE_METHOD( "__C"#Token"Destructor", "__C"#Token"Destructor" );												\
			/*_CrtSetBreakAlloc(262);*/																							\
		}																														\
		~__C##Token##Destructor()																								\
		{																														\
			/* WARNING! Enabling the following line at __GDEFINE_CREATEDATA_FUNCTION_MEMBER_INIT causes a memory leak!*/		\
			/* This is because the profiler/debugger relies on this function to work		*/									\
			/* GPROFILE_METHOD( "__C"#Token"Destructor", "~__C"#Token"Destructor" );	*/										\
			if(	__g_lst##Token )																								\
			{																													\
				debug_printf("Preparing to deallocate %u __GODS(BUFFER) instances of type "#Token" (%u bytes).", __g_n##Token##Count, (sizeof(__GBUFFER_DATA)+sizeof(uint32_t))*__g_n##Token##Count);	\
				debug_printf("%u __GODS(BUFFER) instances created (%u bytes)", __g_n##Token##CreatedRefs, (sizeof(Token)+sizeof(uint32_t))*__g_n##Token##CreatedRefs);		\
				debug_printf("%u __GODS(BUFFER) instances acquired (%u bytes)", __g_n##Token##AcquiredRefs, (sizeof(Token)+sizeof(uint32_t))*__g_n##Token##AcquiredRefs);	\
				debug_printf("%u __GODS(BUFFER) instances freed (%u bytes)", __g_n##Token##FreedRefs, (sizeof(Token)+sizeof(uint32_t))*__g_n##Token##FreedRefs);			\
				size_t freedBytes = 0;																							\
				while(__g_n##Token##Count)																						\
				{																												\
					__g_n##Token##Count--;																						\
					if( __g_lst##Token[__g_n##Token##Count]->pByteArray	)														\
					{																											\
						freedBytes += __g_lst##Token[__g_n##Token##Count]->nSizeInBytes;										\
						free( __g_lst##Token[__g_n##Token##Count]->pByteArray );												\
					}																											\
					free(__g_lst##Token[__g_n##Token##Count]);																	\
				}																												\
				debug_printf("Deallocation finished. %u system bytes freed.", (int32_t)freedBytes);								\
				free( __g_lst##Token );																							\
				__g_lst##Token = 0;																								\
			}																													\
			DELETE_CRITICAL_SECTION(Token);																						\
		}																														\
	};	

#define __GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( Token )													\
																												\
	__GDECLARE_INLINE_CORE_BUFFER_DEALLOCATOR( Token );														\
																												\
	static __C##Token##Destructor	__g_##Token##Destructor;													\
																												\
namespace god																									\
{																												\
	god::error_t gcreate##Token( __GODS(BUFFER)** p, uint32_t nElementCount );							\
}																												\
																												\
god::error_t god::gcreate##Token( __GODS(BUFFER)** p, uint32_t nElementCount )							\
{																												\
	GPROFILE_FUNCTION();																						\
 	INIT_CRITICAL_SECTION(Token);																				\
 	GODS(BUFFER) newData=0;																					\
	uint32_t sizeInBytesRequested = (GTYPEID_TOTALBYTES(Token)*nElementCount);									\
	/*for( uint32_t iBuffer=0; iBuffer < __g_n##Token##Count; iBuffer++ )									*/		\
	/*{																										*/	\
	/*	if( __g_lst##Token[iBuffer]->nElementCount == nElementCount 										*/	\
	/*		|| __g_lst##Token[iBuffer]->nSizeInBytes == sizeInBytesRequested )								*/	\
	/*	{																									*/	\
	/*		newData = __g_lst##Token[iBuffer];																*/	\
	/*		__g_lst##Token[iBuffer] = __g_lst##Token[--__g_n##Token##Count];								*/	\
	/*		if( newData->__nRefCount != 0 )																*/	\
	/*		{	errmsg_refcountnonnull();	}																*/	\
	/*		newData->__nRefCount++;																		*/	\
	/*		newData->nElementCount = nElementCount;														*/	\
	/*		break;																							*/	\
	/*	}																									*/	\
	/*}																										*/	\
	if( 0 == newData )																							\
	{																											\
		ENTER_CRITICAL_SECTION(Token);																			\
		for( uint32_t iBuffer=0; iBuffer < __g_n##Token##Count; iBuffer++ )										\
		{																										\
			if( 																								\
					(__g_lst##Token[iBuffer]->nElementCount >= nElementCount 									\
						&& __g_lst##Token[iBuffer]->nElementCount <= (nElementCount+nElementCount*.5f)		\
					)																							\
					|| 																							\
					(__g_lst##Token[iBuffer]->nSizeInBytes >= sizeInBytesRequested								\
					&& __g_lst##Token[iBuffer]->nSizeInBytes <= (sizeInBytesRequested+sizeInBytesRequested*.5f)	\
					)																							\
				)																								\
			{																									\
				newData = __g_lst##Token[iBuffer];																\
				if( newData->__nRefCount != 0 )																\
				{																								\
					errmsg_refcountnonnull();																	\
					_CrtDbgBreak();																				\
				}																								\
				newData->__nRefCount++;																		\
				__g_lst##Token[iBuffer] = __g_lst##Token[--__g_n##Token##Count];								\
				newData->nElementCount = nElementCount;														\
				break;																							\
			}																									\
		}																										\
		LEAVE_CRITICAL_SECTION(Token);																			\
	}																											\
	error_t errMy=0;																							\
	if( 0 == newData )																							\
	{																											\
		gcreateData( &newData );																				\
		if( newData ) 																							\
		{																										\
			newData->DataFormat			= Token;																\
			newData->nElementCount		= nElementCount;														\
			newData->nRowElementCount	= 1;																	\
			newData->nSizeInBytes		= GTYPEID_TOTALBYTES(newData->DataFormat)*(uint32_t)(newData->nElementCount*1.5f);	\
			newData->pByteArray			= malloc( newData->nSizeInBytes );										\
			if( 0 == newData->pByteArray )																		\
			{																									\
				errMy = -1;																						\
				error_printf("Cannot allocate buffer! Out of memory? Size requested %u", newData->nSizeInBytes);	\
				gfreeData( &newData );																				\
			}																										\
		}																											\
		else																										\
		{																											\
			error_printf( "gcreateBuffer( " #Token "** ): malloc(%u) FAILED! Out of memory?\n"						\
				"Cannot allocate new " #Token ".", (uint32_t)sizeof( god::GBUFFER_DATA ) );							\
		}																											\
	}																												\
	GOPTR(BUFFER) oldData = *p;																						\
	(*p) = newData;																									\
	if(newData)																										\
		__g_n##Token##CreatedRefs++;																				\
	return errMy;																									\
};

					
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT2_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT4_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT8_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT16_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT32_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_INT64_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT1_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT2_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT4_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_3  );
//__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT8_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT16_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT32_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UINT64_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT4_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT8_NORM_16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT16_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_2  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_3  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_4  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_9  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM  );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT32_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_FLOAT64_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT4_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT8_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT16_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT32_NORM_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_16 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM_2 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM_3 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM_4 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM_9 );
__GDEFINE_CREATEBUFFER_FUNCTION_MEMBER_INIT( GDATATYPE_UFLOAT64_NORM_16 );

																												
	__GDECLARE_INLINE_CORE_BUFFER_DEALLOCATOR( GDATATYPE_UINT8_4 );												
																												
	static __CGDATATYPE_UINT8_4Destructor	__g_GDATATYPE_UINT8_4Destructor;									
																												
namespace god																									
{																												
	god::error_t gcreateGDATATYPE_UINT8_4( __GODS(BUFFER)** p, uint32_t nElementCount );						
}																												
																												
god::error_t god::gcreateGDATATYPE_UINT8_4( __GODS(BUFFER)** p, uint32_t nElementCount )						
{																												
	GPROFILE_FUNCTION();																						
 	GODS(BUFFER) newData=0;																					
	uint32_t sizeInBytesRequested = (GTYPEID_TOTALBYTES(GDATATYPE_UINT8_4)*nElementCount);						
	INIT_CRITICAL_SECTION(GDATATYPE_UINT8_4);											
	/*for( uint32_t iBuffer=0; iBuffer < __g_nGDATATYPE_UINT8_4Count; iBuffer++ )							*/	
	/*{																										*/	
	/*	if( __g_lstGDATATYPE_UINT8_4[iBuffer]->nElementCount == nElementCount 								*/	
	/*		|| __g_lstGDATATYPE_UINT8_4[iBuffer]->nSizeInBytes == sizeInBytesRequested )					*/	
	/*	{																									*/	
	/*		newData = __g_lstGDATATYPE_UINT8_4[iBuffer];													*/	
	/*		__g_lstGDATATYPE_UINT8_4[iBuffer] = __g_lstGDATATYPE_UINT8_4[--__g_nGDATATYPE_UINT8_4Count];	*/	
	/*		if( newData->__nRefCount != 0 )																*/	
	/*		{	errmsg_refcountnonnull();	}																*/	
	/*		newData->__nRefCount++;																		*/	
	/*		newData->nElementCount = nElementCount;														*/	
	/*		break;																							*/	
	/*	}																									*/	
	/*}																										*/	
	if( 0 == newData )																							
	{																											
		ENTER_CRITICAL_SECTION(GDATATYPE_UINT8_4);											
		for( uint32_t iBuffer=0; iBuffer < __g_nGDATATYPE_UINT8_4Count; iBuffer++ )								
		{																										
			if( 																								
					(__g_lstGDATATYPE_UINT8_4[iBuffer]->nElementCount >= nElementCount 							
						&& __g_lstGDATATYPE_UINT8_4[iBuffer]->nElementCount <= (nElementCount+nElementCount*.5f)
					)																							
					|| 																							
					(__g_lstGDATATYPE_UINT8_4[iBuffer]->nSizeInBytes >= sizeInBytesRequested					
					&& __g_lstGDATATYPE_UINT8_4[iBuffer]->nSizeInBytes <= (sizeInBytesRequested+sizeInBytesRequested*.5f)	
					)																							
				)																								
			{																									
				newData = __g_lstGDATATYPE_UINT8_4[iBuffer];																
				if( newData->__nRefCount != 0 )																
				{																							
					errmsg_refcountnonnull();																
					_CrtDbgBreak();																			
				}																							
				newData->__nRefCount++;																		
				__g_lstGDATATYPE_UINT8_4[iBuffer] = __g_lstGDATATYPE_UINT8_4[--__g_nGDATATYPE_UINT8_4Count];				
				newData->nElementCount = nElementCount;														
				break;																							
			}																									
		}																										
		LEAVE_CRITICAL_SECTION(GDATATYPE_UINT8_4);											
	}																											
	error_t errMy=0;																							
	if( 0 == newData )																							
	{																											
		gcreateData( &newData );																				
		if( newData ) 																							
		{																										
			newData->DataFormat		= GDATATYPE_UINT8_4;																
			newData->nElementCount		= nElementCount;														
			newData->nRowElementCount	= 1;																	
			newData->nSizeInBytes	= GTYPEID_TOTALBYTES(newData->DataFormat)*(uint32_t)(newData->nElementCount*1.5);
			newData->pByteArray	= malloc( newData->nSizeInBytes );											
			if( 0 == newData->pByteArray )																		
			{																									
				errMy = -1;																						
				error_printf("Cannot allocate buffer! Out of memory? Size requested %u", newData->nSizeInBytes);	
				gfreeData( &newData );																			
			}																									
		}																										
		else																									
		{																										
			error_printf( "gcreateBuffer( " "GDATATYPE_UINT8_4" "** ): malloc(%u) FAILED! Out of memory?\n"			
				"Cannot allocate new " "GDATATYPE_UINT8_4" ".", (uint32_t)sizeof( god::GBUFFER_DATA ) );			
		}																										
	}																											
	GOPTR(BUFFER) oldData = *p;																					
	(*p) = newData;																							
	if (newData)																										
		__g_nGDATATYPE_UINT8_4CreatedRefs++;																			
	return errMy;
};

					


#define __GDEFINE_FREE_TO_POINTER_STACK( TYPE, Token )								\
{																					\
	ENTER_CRITICAL_SECTION(TYPE##Token);											\
	if( gresizePointerArray( sizeof( GODS(BUFFER) ),								\
						__g_n##TYPE##Token##Count+1,								\
							&__g_n##TYPE##Token##ArraySize,							\
							(void**)&__g_lst##TYPE##Token, 							\
							__g_n##TYPE##Token##ArraySize							\
								+(uint32_t)(__g_n##TYPE##Token##ArraySize*.25f) )	\
							)														\
	{																				\
		if( oldBuffer->pByteArray )													\
			free( oldBuffer->pByteArray );											\
		free( oldBuffer );															\
	}																				\
	else																			\
	{																				\
		oldBuffer->__nRefCount--;													\
		__g_lst##TYPE##Token[__g_n##TYPE##Token##Count] = oldBuffer;				\
		__g_n##TYPE##Token##Count++;												\
	}																				\
	LEAVE_CRITICAL_SECTION(TYPE##Token);											\
	if (oldBuffer)																	\
		__g_n##TYPE##Token##FreedRefs++;											\
}

#define __GDEFINE_FREE_TO_POINTER_STACK_SWITCH( TYPE )																									\
switch( (*ppBufferData)->nElementCount )																												\
{																																						\
case 1		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __1		) break;																				\
case 2		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __2		) break;																				\
case 3		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __3		) break;																				\
case 4		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __4		) break;																				\
/*case 5		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __5	) break;*/																				\
/*case 6		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __6	) break;*/																				\
/*case 7		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __7	) break;*/																				\
/*case 8		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __8	) break;*/																				\
/*case 9		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __9	) break;*/																				\
/*case 10		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __10	) break;*/																				\
/*case 11		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __11	) break;*/																				\
/*case 12		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __12	) break;*/																				\
/*case 13		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __13	) break;*/																				\
/*case 14		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __14	) break;*/																				\
/*case 15		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __15	) break;*/																				\
case 16		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __16		) break;																				\
case 32		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __32		) break;																				\
case 64		:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __64		) break;																				\
case 128	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __128		) break;																			\
case 256	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __256		) break;																			\
case 512	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __512		) break;																			\
case 1024	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __1024		) break;																			\
/*case 2048	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __2048		) break;		*/																	\
/*case 4096	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __4096		) break;		*/																	\
/*case 8192	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __8192		) break;		*/																	\
/*case 16384	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __16384	) break;	*/																		\
/*case 32768	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __32768	) break;	*/																		\
/*case 65536	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __65536	) break;	*/																		\
/*case 131072	:	__GDEFINE_FREE_TO_POINTER_STACK( TYPE, __131072	) break;	*/																		\
default	:																																				\
	if( (*ppBufferData)->nElementCount < 64	)														{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 64		)}	\
	else if( (*ppBufferData)->nElementCount > 64	 && (*ppBufferData)->nElementCount < 128	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 128		)}	\
	else if( (*ppBufferData)->nElementCount > 128	 && (*ppBufferData)->nElementCount < 256	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 256		)}	\
	else if( (*ppBufferData)->nElementCount > 256	 && (*ppBufferData)->nElementCount < 512	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 512		)}	\
	else if( (*ppBufferData)->nElementCount > 512	 && (*ppBufferData)->nElementCount < 1024	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 1024	)}	\
	else if( (*ppBufferData)->nElementCount > 1024	 && (*ppBufferData)->nElementCount < 2048	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 2048	)}	\
	else if( (*ppBufferData)->nElementCount > 2048	 && (*ppBufferData)->nElementCount < 4096	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 4096	)}	\
	else if( (*ppBufferData)->nElementCount > 4096	 && (*ppBufferData)->nElementCount < 8192	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 8192	)}	\
	else if( (*ppBufferData)->nElementCount > 8192	 && (*ppBufferData)->nElementCount < 16384	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 16384	)}	\
	else if( (*ppBufferData)->nElementCount > 16384	 && (*ppBufferData)->nElementCount < 32768	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 32768	)}	\
	else if( (*ppBufferData)->nElementCount > 32768	 && (*ppBufferData)->nElementCount < 65536	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 65536	)}	\
	else if( (*ppBufferData)->nElementCount > 65536	 && (*ppBufferData)->nElementCount < 131072	)	{__GDEFINE_FREE_TO_POINTER_STACK( TYPE, 131072	)}	\
	else																																				\
	{																																					\
		if( gresizePointerArray( sizeof( god::_GBUFFER_DATA* ),																							\
								__g_n##TYPE##Count+1,																									\
								&__g_n##TYPE##ArraySize,																								\
								(void**)&__g_lst##TYPE, 																								\
								__g_n##TYPE##ArraySize+(uint32_t)(__g_n##TYPE##ArraySize*.5f) )															\
								)																														\
		{																																				\
			if( (*ppBufferData)->pByteArray )																											\
				free( (*ppBufferData)->pByteArray );																									\
			free(*ppBufferData);																														\
		}																																				\
		else																																			\
		{																																				\
			__g_lst##TYPE[__g_n##TYPE##Count] = *ppBufferData;																							\
			(*ppBufferData)->__nRefCount--;																												\
			__g_n##TYPE##Count++;																														\
		}																																				\
	}																																					\
}

void god::gfreeData( GODS(BUFFER)* pBufferData )
{
	GPROFILE_FUNCTION();
	if( 0 == (*pBufferData) ) 
		return;

	typedef void(*funcType)(GODS(BUFFER)*);
	if( (*pBufferData)->__pfreeData != (funcType)gfreeData )
	{
		(*pBufferData)->__pfreeData( pBufferData );
		return;
	}

	ENTER_CRITICAL_SECTION(Buffer);
	GODS(BUFFER) oldBuffer = *pBufferData;
	*pBufferData = 0;
	LEAVE_CRITICAL_SECTION(Buffer);

	switch( oldBuffer->__nRefCount )
	{
	default:
		__GWINTERLOCKEDDECREMENT( oldBuffer->__nRefCount );
		break;
	case 0:
		errmsg_refcountnull();
		printInfoString( oldBuffer );
		_CrtDbgBreak();
	case 1:
		switch( oldBuffer->DataFormat )
		{
		// Most used come first
		case GDATATYPE_INT8				: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8				,); break;
		case GDATATYPE_INT16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16				,); break;	
		case GDATATYPE_INT32			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32				,); break;	
		case GDATATYPE_INT64			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64				,); break;	
		case GDATATYPE_UINT8			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8				,); break;	
		case GDATATYPE_UINT16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16				,); break;
		case GDATATYPE_UINT32			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32				,); break;
		case GDATATYPE_UINT64			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64				,); break;
		case GDATATYPE_FLOAT32			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32				,); break;
		case GDATATYPE_FLOAT64			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64				,); break;
		// Vector types are also used often																,
		case GDATATYPE_FLOAT32_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_2			,); break;	
		case GDATATYPE_FLOAT32_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_3			,); break;	
		case GDATATYPE_FLOAT32_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_4			,); break;	
		case GDATATYPE_FLOAT64_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_2			,); break;	
		case GDATATYPE_FLOAT64_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_3			,); break;	
		case GDATATYPE_FLOAT64_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_4			,); break;	
		case GDATATYPE_UFLOAT32			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32			,); break;
		case GDATATYPE_UFLOAT32_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_2			,); break;
		case GDATATYPE_UFLOAT32_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_3			,); break;
		case GDATATYPE_UFLOAT32_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_4			,); break;
		case GDATATYPE_UFLOAT64			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64			,); break;
		case GDATATYPE_UFLOAT64_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_2			,); break;
		case GDATATYPE_UFLOAT64_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_3			,); break;
		case GDATATYPE_UFLOAT64_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_4			,); break;
			// Float vector types are more common than integral											,
		case GDATATYPE_INT8_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_2				,); break;
		case GDATATYPE_INT16_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_2				,); break;
		case GDATATYPE_INT32_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_2				,); break;
		case GDATATYPE_INT64_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_2				,); break;
		case GDATATYPE_INT8_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_3				,); break;
		case GDATATYPE_INT16_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_3				,); break;
		case GDATATYPE_INT32_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_3				,); break;
		case GDATATYPE_INT64_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_3				,); break;
		case GDATATYPE_INT8_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_4				,); break;
		case GDATATYPE_INT16_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_4				,); break;
		case GDATATYPE_INT32_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_4				,); break;
		// 																								,
		case GDATATYPE_UINT8_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_2				,); break;
		case GDATATYPE_UINT16_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_2			,); break;
		case GDATATYPE_UINT32_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_2			,); break;
		case GDATATYPE_UINT64_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_2			,); break;
		case GDATATYPE_UINT8_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_3				,); break;
		case GDATATYPE_UINT16_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_3			,); break;
		case GDATATYPE_UINT32_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_3			,); break;
		case GDATATYPE_UINT64_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_3			,); break;
		case GDATATYPE_UINT8_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_4				,); break;
		case GDATATYPE_UINT16_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_4			,); break;
		case GDATATYPE_UINT32_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_4			,); break;
		// Matrix types are less used than others but are also used.									,
		case GDATATYPE_FLOAT32_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_9			,); break;	
		case GDATATYPE_FLOAT32_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_16			,); break;
		case GDATATYPE_FLOAT64_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_9			,); break;	
		case GDATATYPE_FLOAT64_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_16			,); break;
		case GDATATYPE_UFLOAT64_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_9			,); break;
		case GDATATYPE_UFLOAT64_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_16			,); break;
		case GDATATYPE_UFLOAT32_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_9			,); break;
		case GDATATYPE_UFLOAT32_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_16			,); break;
		// Other types that match the architecture alignment are also used often than others.			,
		case GDATATYPE_INT8_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_9				,); break;
		case GDATATYPE_INT8_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_16				,); break;
		case GDATATYPE_INT8_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM			,); break;	
		case GDATATYPE_INT8_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM_2			,); break;
		case GDATATYPE_INT8_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM_3			,); break;
		case GDATATYPE_INT8_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM_4			,); break;
		case GDATATYPE_INT8_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM_9			,); break;
		case GDATATYPE_INT8_NORM_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT8_NORM_16		,); break;
		case GDATATYPE_INT16_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_9				,); break;
		case GDATATYPE_INT16_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_16			,); break;
		case GDATATYPE_INT16_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM			,); break;
		case GDATATYPE_INT16_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM_2		,); break;
		case GDATATYPE_INT16_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM_3		,); break;
		case GDATATYPE_INT16_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM_4		,); break;
		case GDATATYPE_INT16_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM_9		,); break;
		case GDATATYPE_INT16_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT16_NORM_16		,); break;	
		case GDATATYPE_INT32_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_9				,); break;
		case GDATATYPE_INT32_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_16			,); break;
		case GDATATYPE_INT32_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM			,); break;
		case GDATATYPE_INT32_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM_2		,); break;
		case GDATATYPE_INT32_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM_3		,); break;
		case GDATATYPE_INT32_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM_4		,); break;
		case GDATATYPE_INT32_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM_9		,); break;
		case GDATATYPE_INT32_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT32_NORM_16		,); break;	
		case GDATATYPE_INT64_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_4				,); break;
		case GDATATYPE_INT64_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_9				,); break;
		case GDATATYPE_INT64_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_16			,); break;
		case GDATATYPE_INT64_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM			,); break;
		case GDATATYPE_INT64_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM_2		,); break;
		case GDATATYPE_INT64_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM_3		,); break;
		case GDATATYPE_INT64_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM_4		,); break;
		case GDATATYPE_INT64_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM_9		,); break;
		case GDATATYPE_INT64_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT64_NORM_16		,); break;	
		//
		case GDATATYPE_UINT8_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_9				,); break;
		case GDATATYPE_UINT8_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_16			,); break;
		case GDATATYPE_UINT8_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM			,); break;
		case GDATATYPE_UINT8_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM_2		,); break;
		case GDATATYPE_UINT8_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM_3		,); break;
		case GDATATYPE_UINT8_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM_4		,); break;
		case GDATATYPE_UINT8_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM_9		,); break;
		case GDATATYPE_UINT8_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT8_NORM_16		,); break;	
		case GDATATYPE_UINT16_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_9			,); break;
		case GDATATYPE_UINT16_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_16			,); break;	
		case GDATATYPE_UINT16_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM			,); break;
		case GDATATYPE_UINT16_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM_2		,); break;	
		case GDATATYPE_UINT16_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM_3		,); break;	
		case GDATATYPE_UINT16_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM_4		,); break;	
		case GDATATYPE_UINT16_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM_9		,); break;	
		case GDATATYPE_UINT16_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT16_NORM_16		,); break;
		case GDATATYPE_UINT32_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_9			,); break;
		case GDATATYPE_UINT32_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_16			,); break;	
		case GDATATYPE_UINT32_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM			,); break;
		case GDATATYPE_UINT32_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM_2		,); break;	
		case GDATATYPE_UINT32_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM_3		,); break;	
		case GDATATYPE_UINT32_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM_4		,); break;	
		case GDATATYPE_UINT32_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM_9		,); break;	
		case GDATATYPE_UINT32_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT32_NORM_16		,); break;
		case GDATATYPE_UINT64_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_4			,); break;
		case GDATATYPE_UINT64_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_9			,); break;
		case GDATATYPE_UINT64_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_16			,); break;	
		case GDATATYPE_UINT64_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM			,); break;
		case GDATATYPE_UINT64_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM_2		,); break;	
		case GDATATYPE_UINT64_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM_3		,); break;	
		case GDATATYPE_UINT64_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM_4		,); break;	
		case GDATATYPE_UINT64_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM_9		,); break;	
		case GDATATYPE_UINT64_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT64_NORM_16		,); break;
		//																								,
		case GDATATYPE_FLOAT16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16				,); break;
		case GDATATYPE_FLOAT16_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_2			,); break;	
		case GDATATYPE_FLOAT16_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_3			,); break;	
		case GDATATYPE_FLOAT16_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_4			,); break;	
		case GDATATYPE_FLOAT16_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_9			,); break;	
		case GDATATYPE_FLOAT16_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_16			,); break;
		case GDATATYPE_FLOAT16_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM		,); break;
		case GDATATYPE_FLOAT16_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM_2		,); break;
		case GDATATYPE_FLOAT16_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM_3		,); break;
		case GDATATYPE_FLOAT16_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM_4		,); break;
		case GDATATYPE_FLOAT16_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM_9		,); break;
		case GDATATYPE_FLOAT16_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT16_NORM_16		,); break;
		case GDATATYPE_FLOAT32_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM		,); break;
		case GDATATYPE_FLOAT32_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM_2		,); break;
		case GDATATYPE_FLOAT32_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM_3		,); break;
		case GDATATYPE_FLOAT32_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM_4		,); break;
		case GDATATYPE_FLOAT32_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM_9		,); break;
		case GDATATYPE_FLOAT32_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT32_NORM_16		,); break;
		case GDATATYPE_FLOAT64_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM		,); break;
		case GDATATYPE_FLOAT64_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM_2		,); break;
		case GDATATYPE_FLOAT64_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM_3		,); break;
		case GDATATYPE_FLOAT64_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM_4		,); break;
		case GDATATYPE_FLOAT64_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM_9		,); break;
		case GDATATYPE_FLOAT64_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT64_NORM_16		,); break;
		// Less used types are last.																	,
		case GDATATYPE_INT2				: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2				,); break;
		case GDATATYPE_INT2_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_2				,); break;
		case GDATATYPE_INT2_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_3				,); break;
		case GDATATYPE_INT2_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_4				,); break;
		case GDATATYPE_INT2_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_9				,); break;
		case GDATATYPE_INT2_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_16				,); break;
		case GDATATYPE_INT2_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM			,); break;	
		case GDATATYPE_INT2_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM_2			,); break;
		case GDATATYPE_INT2_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM_3			,); break;
		case GDATATYPE_INT2_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM_4			,); break;
		case GDATATYPE_INT2_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM_9			,); break;
		case GDATATYPE_INT2_NORM_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT2_NORM_16		,); break;
		case GDATATYPE_INT4				: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4				,); break;
		case GDATATYPE_INT4_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_2				,); break;
		case GDATATYPE_INT4_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_3				,); break;
		case GDATATYPE_INT4_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_4				,); break;
		case GDATATYPE_INT4_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_9				,); break;
		case GDATATYPE_INT4_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_16				,); break;
		case GDATATYPE_INT4_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM			,); break;	
		case GDATATYPE_INT4_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM_2			,); break;
		case GDATATYPE_INT4_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM_3			,); break;
		case GDATATYPE_INT4_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM_4			,); break;
		case GDATATYPE_INT4_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM_9			,); break;
		case GDATATYPE_INT4_NORM_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_INT4_NORM_16		,); break;
		case GDATATYPE_UINT1			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1				,); break;	
		case GDATATYPE_UINT1_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1_2				,); break;
		case GDATATYPE_UINT1_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1_3				,); break;
		case GDATATYPE_UINT1_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1_4				,); break;
		case GDATATYPE_UINT1_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1_9				,); break;
		case GDATATYPE_UINT1_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT1_16			,); break;
		case GDATATYPE_UINT2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2				,); break;	
		case GDATATYPE_UINT2_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_2				,); break;
		case GDATATYPE_UINT2_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_3				,); break;
		case GDATATYPE_UINT2_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_4				,); break;
		case GDATATYPE_UINT2_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_9				,); break;
		case GDATATYPE_UINT2_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_16			,); break;
		case GDATATYPE_UINT2_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM			,); break;
		case GDATATYPE_UINT2_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM_2		,); break;
		case GDATATYPE_UINT2_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM_3		,); break;
		case GDATATYPE_UINT2_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM_4		,); break;
		case GDATATYPE_UINT2_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM_9		,); break;
		case GDATATYPE_UINT2_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT2_NORM_16		,); break;	
		case GDATATYPE_UINT4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4				,); break;	
		case GDATATYPE_UINT4_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_2				,); break;
		case GDATATYPE_UINT4_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_3				,); break;
		case GDATATYPE_UINT4_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_4				,); break;
		case GDATATYPE_UINT4_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_9				,); break;
		case GDATATYPE_UINT4_16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_16			,); break;
		case GDATATYPE_UINT4_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM			,); break;
		case GDATATYPE_UINT4_NORM_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM_2		,); break;
		case GDATATYPE_UINT4_NORM_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM_3		,); break;
		case GDATATYPE_UINT4_NORM_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM_4		,); break;
		case GDATATYPE_UINT4_NORM_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM_9		,); break;
		case GDATATYPE_UINT4_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UINT4_NORM_16		,); break;	
		// Floating point types hardly used																,
		case GDATATYPE_FLOAT4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4				,); break;
		case GDATATYPE_FLOAT4_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_2			,); break;
		case GDATATYPE_FLOAT4_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_3			,); break;
		case GDATATYPE_FLOAT4_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_4			,); break;
		case GDATATYPE_FLOAT4_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_9			,); break;
		case GDATATYPE_FLOAT4_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_16			,); break;	
		case GDATATYPE_FLOAT4_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM			,); break;
		case GDATATYPE_FLOAT4_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM_2		,); break;	
		case GDATATYPE_FLOAT4_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM_3		,); break;	
		case GDATATYPE_FLOAT4_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM_4		,); break;	
		case GDATATYPE_FLOAT4_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM_9		,); break;	
		case GDATATYPE_FLOAT4_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT4_NORM_16		,); break;
		case GDATATYPE_FLOAT8			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8				,); break;
		case GDATATYPE_FLOAT8_2			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_2			,); break;
		case GDATATYPE_FLOAT8_3			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_3			,); break;
		case GDATATYPE_FLOAT8_4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_4			,); break;
		case GDATATYPE_FLOAT8_9			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_9			,); break;
		case GDATATYPE_FLOAT8_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_16			,); break;	
		case GDATATYPE_FLOAT8_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM			,); break;
		case GDATATYPE_FLOAT8_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM_2		,); break;	
		case GDATATYPE_FLOAT8_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM_3		,); break;	
		case GDATATYPE_FLOAT8_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM_4		,); break;	
		case GDATATYPE_FLOAT8_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM_9		,); break;	
		case GDATATYPE_FLOAT8_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_FLOAT8_NORM_16		,); break;
		case GDATATYPE_UFLOAT4			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4				,); break;
		case GDATATYPE_UFLOAT4_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_2			,); break;	
		case GDATATYPE_UFLOAT4_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_3			,); break;	
		case GDATATYPE_UFLOAT4_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_4			,); break;	
		case GDATATYPE_UFLOAT4_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_9			,); break;	
		case GDATATYPE_UFLOAT4_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_16			,); break;
		case GDATATYPE_UFLOAT4_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM		,); break;
		case GDATATYPE_UFLOAT4_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM_2		,); break;
		case GDATATYPE_UFLOAT4_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM_3		,); break;
		case GDATATYPE_UFLOAT4_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM_4		,); break;
		case GDATATYPE_UFLOAT4_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM_9		,); break;
		case GDATATYPE_UFLOAT4_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT4_NORM_16		,); break;
		case GDATATYPE_UFLOAT8			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8				,); break;
		case GDATATYPE_UFLOAT8_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_2			,); break;	
		case GDATATYPE_UFLOAT8_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_3			,); break;	
		case GDATATYPE_UFLOAT8_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_4			,); break;	
		case GDATATYPE_UFLOAT8_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_9			,); break;	
		case GDATATYPE_UFLOAT8_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_16			,); break;
		case GDATATYPE_UFLOAT8_NORM		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM		,); break;
		case GDATATYPE_UFLOAT8_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM_2		,); break;
		case GDATATYPE_UFLOAT8_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM_3		,); break;
		case GDATATYPE_UFLOAT8_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM_4		,); break;
		case GDATATYPE_UFLOAT8_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM_9		,); break;
		case GDATATYPE_UFLOAT8_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT8_NORM_16		,); break;
		case GDATATYPE_UFLOAT16			: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16			,); break;
		case GDATATYPE_UFLOAT16_2		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_2			,); break;
		case GDATATYPE_UFLOAT16_3		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_3			,); break;
		case GDATATYPE_UFLOAT16_4		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_4			,); break;
		case GDATATYPE_UFLOAT16_9		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_9			,); break;
		case GDATATYPE_UFLOAT16_16		: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_16			,); break;
		case GDATATYPE_UFLOAT16_NORM	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM		,); break;	
		case GDATATYPE_UFLOAT16_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM_2		,); break;
		case GDATATYPE_UFLOAT16_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM_3		,); break;
		case GDATATYPE_UFLOAT16_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM_4		,); break;
		case GDATATYPE_UFLOAT16_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM_9		,); break;
		case GDATATYPE_UFLOAT16_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT16_NORM_16	,); break;
		case GDATATYPE_UFLOAT32_NORM	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM		,); break;	
		case GDATATYPE_UFLOAT32_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM_2		,); break;
		case GDATATYPE_UFLOAT32_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM_3		,); break;
		case GDATATYPE_UFLOAT32_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM_4		,); break;
		case GDATATYPE_UFLOAT32_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM_9		,); break;
		case GDATATYPE_UFLOAT32_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT32_NORM_16	,); break;
		case GDATATYPE_UFLOAT64_NORM	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM		,); break;	
		case GDATATYPE_UFLOAT64_NORM_2	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM_2		,); break;
		case GDATATYPE_UFLOAT64_NORM_3	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM_3		,); break;
		case GDATATYPE_UFLOAT64_NORM_4	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM_4		,); break;
		case GDATATYPE_UFLOAT64_NORM_9	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM_9		,); break;
		case GDATATYPE_UFLOAT64_NORM_16	: __GDEFINE_FREE_TO_POINTER_STACK(GDATATYPE_UFLOAT64_NORM_16	,); break;
		case god::GDATATYPE_UNKNOWN:
		default:
			if( oldBuffer->pByteArray )
			{
				free( oldBuffer->pByteArray );
				oldBuffer->pByteArray = 0;
			}
			ENTER_CRITICAL_SECTION(Buffer);
			if( gresizePointerArray( sizeof( GODS(BUFFER) ),
									__g_nBufferCount+1, 
									&__g_nBufferArraySize,
									(void**)&__g_lstBuffer, 
									__g_nBufferArraySize+(uint32_t)(__g_nBufferArraySize*.25f) ) 
									)
			{
				free(oldBuffer);
			}
			else
			{
				__GWINTERLOCKEDDECREMENT(oldBuffer->__nRefCount);
				__g_lstBuffer[__g_nBufferCount] = oldBuffer;
				__g_nBufferCount++;
			}
			if (oldBuffer)
				__g_nBufferFreedRefs++;
			LEAVE_CRITICAL_SECTION(Buffer);
		};// switch( type)
	}; // switch( refcount )
	oldBuffer = 0;
}


god::error_t god::gcreateBuffer( GDATA_TYPE DataFormat, GRESOURCE_USAGE Usage, uint32_t nElementCount, god::GODS(BUFFER)* out_pBuffer )
{
	if( Usage == GUSAGE_TEXT )
		nElementCount += 1;

	error_t errMy = 0;
	GODS(BUFFER) newBuffer = 0;
	switch( DataFormat )
	{
	case GDATATYPE_UINT8			: errMy = gcreateGDATATYPE_UINT8			(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16			: errMy = gcreateGDATATYPE_UINT16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32			: errMy = gcreateGDATATYPE_UINT32			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64			: errMy = gcreateGDATATYPE_UINT64			(&newBuffer, nElementCount); break;

	case GDATATYPE_INT8				: errMy = gcreateGDATATYPE_INT8				(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16			: errMy = gcreateGDATATYPE_INT16			(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT32			: errMy = gcreateGDATATYPE_INT32			(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT64			: errMy = gcreateGDATATYPE_INT64			(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT32			: errMy = gcreateGDATATYPE_FLOAT32			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_2		: errMy = gcreateGDATATYPE_FLOAT32_2		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT32_3		: errMy = gcreateGDATATYPE_FLOAT32_3		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT32_4		: errMy = gcreateGDATATYPE_FLOAT32_4		(&newBuffer, nElementCount); break;	

	case GDATATYPE_UINT8_2			: errMy = gcreateGDATATYPE_UINT8_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_3			: errMy = gcreateGDATATYPE_UINT8_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_4			: errMy = gcreateGDATATYPE_UINT8_4			(&newBuffer, nElementCount); break;

	case GDATATYPE_INT8_2			: errMy = gcreateGDATATYPE_INT8_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_3			: errMy = gcreateGDATATYPE_INT8_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_4			: errMy = gcreateGDATATYPE_INT8_4			(&newBuffer, nElementCount); break;

	case GDATATYPE_INT16_2			: errMy = gcreateGDATATYPE_INT16_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_3			: errMy = gcreateGDATATYPE_INT16_3			(&newBuffer, nElementCount); break;

	case GDATATYPE_INT32_2			: errMy = gcreateGDATATYPE_INT32_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_3			: errMy = gcreateGDATATYPE_INT32_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_4			: errMy = gcreateGDATATYPE_INT32_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_9		: errMy = gcreateGDATATYPE_FLOAT32_9		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT32_16		: errMy = gcreateGDATATYPE_FLOAT32_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_2			: errMy = gcreateGDATATYPE_INT64_2			(&newBuffer, nElementCount); break;

	case GDATATYPE_UINT32_2			: errMy = gcreateGDATATYPE_UINT32_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32_3			: errMy = gcreateGDATATYPE_UINT32_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32_4			: errMy = gcreateGDATATYPE_UINT32_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64			: errMy = gcreateGDATATYPE_FLOAT64			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_2		: errMy = gcreateGDATATYPE_FLOAT64_2		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT64_3		: errMy = gcreateGDATATYPE_FLOAT64_3		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT64_4		: errMy = gcreateGDATATYPE_FLOAT64_4		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT64_9		: errMy = gcreateGDATATYPE_FLOAT64_9		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT64_16		: errMy = gcreateGDATATYPE_FLOAT64_16		(&newBuffer, nElementCount); break;


	case GDATATYPE_INT8_9			: errMy = gcreateGDATATYPE_INT8_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_16			: errMy = gcreateGDATATYPE_INT8_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_NORM		: errMy = gcreateGDATATYPE_INT8_NORM		(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT8_NORM_2		: errMy = gcreateGDATATYPE_INT8_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_NORM_3		: errMy = gcreateGDATATYPE_INT8_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_NORM_4		: errMy = gcreateGDATATYPE_INT8_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_NORM_9		: errMy = gcreateGDATATYPE_INT8_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT8_NORM_16		: errMy = gcreateGDATATYPE_INT8_NORM_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_4			: errMy = gcreateGDATATYPE_INT16_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_9			: errMy = gcreateGDATATYPE_INT16_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_16			: errMy = gcreateGDATATYPE_INT16_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM		: errMy = gcreateGDATATYPE_INT16_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM_2		: errMy = gcreateGDATATYPE_INT16_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM_3		: errMy = gcreateGDATATYPE_INT16_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM_4		: errMy = gcreateGDATATYPE_INT16_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM_9		: errMy = gcreateGDATATYPE_INT16_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT16_NORM_16	: errMy = gcreateGDATATYPE_INT16_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT32_9			: errMy = gcreateGDATATYPE_INT32_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_16			: errMy = gcreateGDATATYPE_INT32_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM		: errMy = gcreateGDATATYPE_INT32_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM_2		: errMy = gcreateGDATATYPE_INT32_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM_3		: errMy = gcreateGDATATYPE_INT32_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM_4		: errMy = gcreateGDATATYPE_INT32_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM_9		: errMy = gcreateGDATATYPE_INT32_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT32_NORM_16	: errMy = gcreateGDATATYPE_INT32_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT64_3			: errMy = gcreateGDATATYPE_INT64_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_4			: errMy = gcreateGDATATYPE_INT64_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_9			: errMy = gcreateGDATATYPE_INT64_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_16			: errMy = gcreateGDATATYPE_INT64_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM		: errMy = gcreateGDATATYPE_INT64_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM_2		: errMy = gcreateGDATATYPE_INT64_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM_3		: errMy = gcreateGDATATYPE_INT64_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM_4		: errMy = gcreateGDATATYPE_INT64_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM_9		: errMy = gcreateGDATATYPE_INT64_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT64_NORM_16	: errMy = gcreateGDATATYPE_INT64_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT8_9			: errMy = gcreateGDATATYPE_UINT8_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_16			: errMy = gcreateGDATATYPE_UINT8_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM		: errMy = gcreateGDATATYPE_UINT8_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM_2		: errMy = gcreateGDATATYPE_UINT8_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM_3		: errMy = gcreateGDATATYPE_UINT8_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM_4		: errMy = gcreateGDATATYPE_UINT8_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM_9		: errMy = gcreateGDATATYPE_UINT8_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT8_NORM_16	: errMy = gcreateGDATATYPE_UINT8_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_2			: errMy = gcreateGDATATYPE_UINT16_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT16_3			: errMy = gcreateGDATATYPE_UINT16_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT16_4			: errMy = gcreateGDATATYPE_UINT16_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT16_9			: errMy = gcreateGDATATYPE_UINT16_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT16_16		: errMy = gcreateGDATATYPE_UINT16_16		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_NORM		: errMy = gcreateGDATATYPE_UINT16_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT16_NORM_2	: errMy = gcreateGDATATYPE_UINT16_NORM_2	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_NORM_3	: errMy = gcreateGDATATYPE_UINT16_NORM_3	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_NORM_4	: errMy = gcreateGDATATYPE_UINT16_NORM_4	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_NORM_9	: errMy = gcreateGDATATYPE_UINT16_NORM_9	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT16_NORM_16	: errMy = gcreateGDATATYPE_UINT16_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32_9			: errMy = gcreateGDATATYPE_UINT32_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32_16		: errMy = gcreateGDATATYPE_UINT32_16		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT32_NORM		: errMy = gcreateGDATATYPE_UINT32_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT32_NORM_2	: errMy = gcreateGDATATYPE_UINT32_NORM_2	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT32_NORM_3	: errMy = gcreateGDATATYPE_UINT32_NORM_3	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT32_NORM_4	: errMy = gcreateGDATATYPE_UINT32_NORM_4	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT32_NORM_9	: errMy = gcreateGDATATYPE_UINT32_NORM_9	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT32_NORM_16	: errMy = gcreateGDATATYPE_UINT32_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_2			: errMy = gcreateGDATATYPE_UINT64_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_3			: errMy = gcreateGDATATYPE_UINT64_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_4			: errMy = gcreateGDATATYPE_UINT64_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_9			: errMy = gcreateGDATATYPE_UINT64_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_16		: errMy = gcreateGDATATYPE_UINT64_16		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT64_NORM		: errMy = gcreateGDATATYPE_UINT64_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT64_NORM_2	: errMy = gcreateGDATATYPE_UINT64_NORM_2	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT64_NORM_3	: errMy = gcreateGDATATYPE_UINT64_NORM_3	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT64_NORM_4	: errMy = gcreateGDATATYPE_UINT64_NORM_4	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT64_NORM_9	: errMy = gcreateGDATATYPE_UINT64_NORM_9	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT64_NORM_16	: errMy = gcreateGDATATYPE_UINT64_NORM_16	(&newBuffer, nElementCount); break;

	case GDATATYPE_FLOAT16			: errMy = gcreateGDATATYPE_FLOAT16			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_2		: errMy = gcreateGDATATYPE_FLOAT16_2		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT16_3		: errMy = gcreateGDATATYPE_FLOAT16_3		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT16_4		: errMy = gcreateGDATATYPE_FLOAT16_4		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT16_9		: errMy = gcreateGDATATYPE_FLOAT16_9		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT16_16		: errMy = gcreateGDATATYPE_FLOAT16_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM		: errMy = gcreateGDATATYPE_FLOAT16_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM_2	: errMy = gcreateGDATATYPE_FLOAT16_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM_3	: errMy = gcreateGDATATYPE_FLOAT16_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM_4	: errMy = gcreateGDATATYPE_FLOAT16_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM_9	: errMy = gcreateGDATATYPE_FLOAT16_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT16_NORM_16	: errMy = gcreateGDATATYPE_FLOAT16_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM		: errMy = gcreateGDATATYPE_FLOAT32_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM_2	: errMy = gcreateGDATATYPE_FLOAT32_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM_3	: errMy = gcreateGDATATYPE_FLOAT32_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM_4	: errMy = gcreateGDATATYPE_FLOAT32_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM_9	: errMy = gcreateGDATATYPE_FLOAT32_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT32_NORM_16	: errMy = gcreateGDATATYPE_FLOAT32_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM		: errMy = gcreateGDATATYPE_FLOAT64_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM_2	: errMy = gcreateGDATATYPE_FLOAT64_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM_3	: errMy = gcreateGDATATYPE_FLOAT64_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM_4	: errMy = gcreateGDATATYPE_FLOAT64_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM_9	: errMy = gcreateGDATATYPE_FLOAT64_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT64_NORM_16	: errMy = gcreateGDATATYPE_FLOAT64_NORM_16	(&newBuffer, nElementCount); break;

	case GDATATYPE_UFLOAT16			: errMy = gcreateGDATATYPE_UFLOAT16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_2		: errMy = gcreateGDATATYPE_UFLOAT16_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_3		: errMy = gcreateGDATATYPE_UFLOAT16_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_4		: errMy = gcreateGDATATYPE_UFLOAT16_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_9		: errMy = gcreateGDATATYPE_UFLOAT16_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_16		: errMy = gcreateGDATATYPE_UFLOAT16_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_NORM	: errMy = gcreateGDATATYPE_UFLOAT16_NORM	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT16_NORM_2	: errMy = gcreateGDATATYPE_UFLOAT16_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_NORM_3	: errMy = gcreateGDATATYPE_UFLOAT16_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_NORM_4	: errMy = gcreateGDATATYPE_UFLOAT16_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_NORM_9	: errMy = gcreateGDATATYPE_UFLOAT16_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT16_NORM_16	: errMy = gcreateGDATATYPE_UFLOAT16_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32			: errMy = gcreateGDATATYPE_UFLOAT32			(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_2		: errMy = gcreateGDATATYPE_UFLOAT32_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_3		: errMy = gcreateGDATATYPE_UFLOAT32_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_4		: errMy = gcreateGDATATYPE_UFLOAT32_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_9		: errMy = gcreateGDATATYPE_UFLOAT32_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_16		: errMy = gcreateGDATATYPE_UFLOAT32_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_NORM	: errMy = gcreateGDATATYPE_UFLOAT32_NORM	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT32_NORM_2	: errMy = gcreateGDATATYPE_UFLOAT32_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_NORM_3	: errMy = gcreateGDATATYPE_UFLOAT32_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_NORM_4	: errMy = gcreateGDATATYPE_UFLOAT32_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_NORM_9	: errMy = gcreateGDATATYPE_UFLOAT32_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT32_NORM_16	: errMy = gcreateGDATATYPE_UFLOAT32_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64			: errMy = gcreateGDATATYPE_UFLOAT64			(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_2		: errMy = gcreateGDATATYPE_UFLOAT64_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_3		: errMy = gcreateGDATATYPE_UFLOAT64_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_4		: errMy = gcreateGDATATYPE_UFLOAT64_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_9		: errMy = gcreateGDATATYPE_UFLOAT64_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_16		: errMy = gcreateGDATATYPE_UFLOAT64_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_NORM	: errMy = gcreateGDATATYPE_UFLOAT64_NORM	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT64_NORM_2	: errMy = gcreateGDATATYPE_UFLOAT64_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_NORM_3	: errMy = gcreateGDATATYPE_UFLOAT64_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_NORM_4	: errMy = gcreateGDATATYPE_UFLOAT64_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_NORM_9	: errMy = gcreateGDATATYPE_UFLOAT64_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT64_NORM_16	: errMy = gcreateGDATATYPE_UFLOAT64_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT1			: errMy = gcreateGDATATYPE_UINT1			(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT1_2			: errMy = gcreateGDATATYPE_UINT1_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT1_3			: errMy = gcreateGDATATYPE_UINT1_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT1_4			: errMy = gcreateGDATATYPE_UINT1_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT1_9			: errMy = gcreateGDATATYPE_UINT1_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT1_16			: errMy = gcreateGDATATYPE_UINT1_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2			: errMy = gcreateGDATATYPE_UINT2			(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT2_2			: errMy = gcreateGDATATYPE_UINT2_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_3			: errMy = gcreateGDATATYPE_UINT2_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_4			: errMy = gcreateGDATATYPE_UINT2_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_9			: errMy = gcreateGDATATYPE_UINT2_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_16			: errMy = gcreateGDATATYPE_UINT2_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM		: errMy = gcreateGDATATYPE_UINT2_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM_2		: errMy = gcreateGDATATYPE_UINT2_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM_3		: errMy = gcreateGDATATYPE_UINT2_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM_4		: errMy = gcreateGDATATYPE_UINT2_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM_9		: errMy = gcreateGDATATYPE_UINT2_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT2_NORM_16	: errMy = gcreateGDATATYPE_UINT2_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT4			: errMy = gcreateGDATATYPE_UINT4			(&newBuffer, nElementCount); break;	
	case GDATATYPE_UINT4_2			: errMy = gcreateGDATATYPE_UINT4_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_3			: errMy = gcreateGDATATYPE_UINT4_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_4			: errMy = gcreateGDATATYPE_UINT4_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_9			: errMy = gcreateGDATATYPE_UINT4_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_16			: errMy = gcreateGDATATYPE_UINT4_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM		: errMy = gcreateGDATATYPE_UINT4_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM_2		: errMy = gcreateGDATATYPE_UINT4_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM_3		: errMy = gcreateGDATATYPE_UINT4_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM_4		: errMy = gcreateGDATATYPE_UINT4_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM_9		: errMy = gcreateGDATATYPE_UINT4_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_UINT4_NORM_16	: errMy = gcreateGDATATYPE_UINT4_NORM_16	(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT2				: errMy = gcreateGDATATYPE_INT2				(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_2			: errMy = gcreateGDATATYPE_INT2_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_3			: errMy = gcreateGDATATYPE_INT2_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_4			: errMy = gcreateGDATATYPE_INT2_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_9			: errMy = gcreateGDATATYPE_INT2_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_16			: errMy = gcreateGDATATYPE_INT2_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_NORM		: errMy = gcreateGDATATYPE_INT2_NORM		(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT2_NORM_2		: errMy = gcreateGDATATYPE_INT2_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_NORM_3		: errMy = gcreateGDATATYPE_INT2_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_NORM_4		: errMy = gcreateGDATATYPE_INT2_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_NORM_9		: errMy = gcreateGDATATYPE_INT2_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT2_NORM_16		: errMy = gcreateGDATATYPE_INT2_NORM_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4				: errMy = gcreateGDATATYPE_INT4				(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_2			: errMy = gcreateGDATATYPE_INT4_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_3			: errMy = gcreateGDATATYPE_INT4_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_4			: errMy = gcreateGDATATYPE_INT4_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_9			: errMy = gcreateGDATATYPE_INT4_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_16			: errMy = gcreateGDATATYPE_INT4_16			(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_NORM		: errMy = gcreateGDATATYPE_INT4_NORM		(&newBuffer, nElementCount); break;	
	case GDATATYPE_INT4_NORM_2		: errMy = gcreateGDATATYPE_INT4_NORM_2		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_NORM_3		: errMy = gcreateGDATATYPE_INT4_NORM_3		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_NORM_4		: errMy = gcreateGDATATYPE_INT4_NORM_4		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_NORM_9		: errMy = gcreateGDATATYPE_INT4_NORM_9		(&newBuffer, nElementCount); break;
	case GDATATYPE_INT4_NORM_16		: errMy = gcreateGDATATYPE_INT4_NORM_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4			: errMy = gcreateGDATATYPE_FLOAT4			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_2			: errMy = gcreateGDATATYPE_FLOAT4_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_3			: errMy = gcreateGDATATYPE_FLOAT4_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_4			: errMy = gcreateGDATATYPE_FLOAT4_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_9			: errMy = gcreateGDATATYPE_FLOAT4_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_16		: errMy = gcreateGDATATYPE_FLOAT4_16		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT4_NORM		: errMy = gcreateGDATATYPE_FLOAT4_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT4_NORM_2	: errMy = gcreateGDATATYPE_FLOAT4_NORM_2	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT4_NORM_3	: errMy = gcreateGDATATYPE_FLOAT4_NORM_3	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT4_NORM_4	: errMy = gcreateGDATATYPE_FLOAT4_NORM_4	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT4_NORM_9	: errMy = gcreateGDATATYPE_FLOAT4_NORM_9	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT4_NORM_16	: errMy = gcreateGDATATYPE_FLOAT4_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8			: errMy = gcreateGDATATYPE_FLOAT8			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_2			: errMy = gcreateGDATATYPE_FLOAT8_2			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_3			: errMy = gcreateGDATATYPE_FLOAT8_3			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_4			: errMy = gcreateGDATATYPE_FLOAT8_4			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_9			: errMy = gcreateGDATATYPE_FLOAT8_9			(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_16		: errMy = gcreateGDATATYPE_FLOAT8_16		(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT8_NORM		: errMy = gcreateGDATATYPE_FLOAT8_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_FLOAT8_NORM_2	: errMy = gcreateGDATATYPE_FLOAT8_NORM_2	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT8_NORM_3	: errMy = gcreateGDATATYPE_FLOAT8_NORM_3	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT8_NORM_4	: errMy = gcreateGDATATYPE_FLOAT8_NORM_4	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT8_NORM_9	: errMy = gcreateGDATATYPE_FLOAT8_NORM_9	(&newBuffer, nElementCount); break;	
	case GDATATYPE_FLOAT8_NORM_16	: errMy = gcreateGDATATYPE_FLOAT8_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4			: errMy = gcreateGDATATYPE_UFLOAT4			(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_2		: errMy = gcreateGDATATYPE_UFLOAT4_2		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT4_3		: errMy = gcreateGDATATYPE_UFLOAT4_3		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT4_4		: errMy = gcreateGDATATYPE_UFLOAT4_4		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT4_9		: errMy = gcreateGDATATYPE_UFLOAT4_9		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT4_16		: errMy = gcreateGDATATYPE_UFLOAT4_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM		: errMy = gcreateGDATATYPE_UFLOAT4_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM_2	: errMy = gcreateGDATATYPE_UFLOAT4_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM_3	: errMy = gcreateGDATATYPE_UFLOAT4_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM_4	: errMy = gcreateGDATATYPE_UFLOAT4_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM_9	: errMy = gcreateGDATATYPE_UFLOAT4_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT4_NORM_16	: errMy = gcreateGDATATYPE_UFLOAT4_NORM_16	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8			: errMy = gcreateGDATATYPE_UFLOAT8			(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_2		: errMy = gcreateGDATATYPE_UFLOAT8_2		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT8_3		: errMy = gcreateGDATATYPE_UFLOAT8_3		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT8_4		: errMy = gcreateGDATATYPE_UFLOAT8_4		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT8_9		: errMy = gcreateGDATATYPE_UFLOAT8_9		(&newBuffer, nElementCount); break;	
	case GDATATYPE_UFLOAT8_16		: errMy = gcreateGDATATYPE_UFLOAT8_16		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM		: errMy = gcreateGDATATYPE_UFLOAT8_NORM		(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM_2	: errMy = gcreateGDATATYPE_UFLOAT8_NORM_2	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM_3	: errMy = gcreateGDATATYPE_UFLOAT8_NORM_3	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM_4	: errMy = gcreateGDATATYPE_UFLOAT8_NORM_4	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM_9	: errMy = gcreateGDATATYPE_UFLOAT8_NORM_9	(&newBuffer, nElementCount); break;
	case GDATATYPE_UFLOAT8_NORM_16	: errMy = gcreateGDATATYPE_UFLOAT8_NORM_16	(&newBuffer, nElementCount); break;

	default:
		error_printf("Cannot create a buffer of unknown type!");
		error_printf(
			"IsSigned: %s\n"
			"IsNormalized: %s\n"
			"IsFloat: %s\n"
			"IsBigEndian: %s\n"
			"ElementCount: %u\n"
			"ElementSize: %u\n"
			"ElementBytes: %u\n"
			"TotalBytes: %u"
			, GTYPEID_ISSIGNED( DataFormat )		? "true" : "false"
			, GTYPEID_ISNORMALIZED( DataFormat ) 	? "true" : "false"
			, GTYPEID_ISFLOAT( DataFormat ) 		? "true" : "false"
			, GTYPEID_ISBIGENDIAN( DataFormat ) 	? "true" : "false"
			, (int)GTYPEID_ELEMENTCOUNT( DataFormat )
			, (int)GTYPEID_ELEMENTSIZE( DataFormat ) 
			, (int)GTYPEID_ELEMENTBYTES( DataFormat ) 
			, (int)GTYPEID_TOTALBYTES( DataFormat ) 
			);
		return -1;
	}
	if( newBuffer )
	{
		if( GUSAGE_TEXT == (newBuffer->Usage = Usage) )
		{
			//memset( (*out_pBuffer)->pByteArray, 0, (*out_pBuffer)->nSizeInBytes );
			newBuffer->nElementCount--;
		}
	}
	GOPTR(BUFFER) oldBuffer = *out_pBuffer;
	*out_pBuffer = newBuffer;
	return errMy;
};
//



//#define __GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( TYPE, Tag )												\
//	__GDECLARE_INLINE_CORE_BUFFER_DEALLOCATOR( ##TYPE##Tag );												\
//																											\
//	static __C##TYPE##Tag##Destructor	__g_##TYPE##Tag##Destructor;										\
//																											\
//namespace god																								\
//{																											\
//	god::error_t gcreate##TYPE##Tag( __GODS(BUFFER)** p, uint32_t nElementCount );							\
//}																											\
//error_t god::gcreate##TYPE##Tag( __GODS(BUFFER)** p, uint32_t nElementCount )								\
//{																											\
//	GPROFILE_FUNCTION(TYPE##Tag);																			\
// 	INIT_CRITICAL_SECTION(TYPE##Tag);																		\
//	GODS(BUFFER) newData=0;																				\
//	uint32_t sizeInBytesRequested = (GTYPEID_TOTALBYTES(TYPE)*nElementCount);								\
//	ENTER_CRITICAL_SECTION(Token);																			\
//	for( uint32_t iBuffer=0; iBuffer < __g_n##TYPE##Tag##Count; iBuffer++ )									\
//	{																										\
//		if( __g_lst##TYPE##Tag[iBuffer]->nElementCount == nElementCount 									\
//		|| __g_lst##TYPE##Tag[iBuffer]->nSizeInBytes == sizeInBytesRequested )								\
//		{																									\
//			newData = __g_lst##TYPE##Tag[iBuffer];															\
//			if( newData->__nRefCount != 0 )																\
//			{																								\
//				errmsg_refcountnonnull();																	\
//				_CrtDbgBreak();																				\
//			}																								\
//			newData->__nRefCount++;																		\
//			__g_lst##TYPE##Tag[iBuffer] = __g_lst##TYPE##Tag[--__g_n##TYPE##Count];							\
//			newData->nElementCount = nElementCount;														\
//			break;																							\
//		}																									\
//	}																										\
//	LEAVE_CRITICAL_SECTION(Token);																			\
//	if( 0 == newData )																						\
//	{																										\
//		ENTER_CRITICAL_SECTION(Token);																		\
//		for( uint32_t iBuffer=0; iBuffer < __g_n##TYPE##Tag##Count; iBuffer++ )								\
//		{																									\
//			if( 																							\
//					(__g_lst##TYPE##Tag[iBuffer]->nElementCount > nElementCount 							\
//						&& __g_lst##TYPE##Tag[iBuffer]->nElementCount <= (nElementCount+nElementCount*.5f)	\
//					)																						\
//					|| 																						\
//					(__g_lst##TYPE##Tag[iBuffer]->nSizeInBytes > sizeInBytesRequested						\
//						&& __g_lst##TYPE##Tag[iBuffer]->nSizeInBytes <= (sizeInBytesRequested*.5f)			\
//					)																						\
//				)																							\
//			{																								\
//				newData = __g_lst##TYPE##Tag[iBuffer];														\
//				__g_lst##TYPE##Tag[iBuffer] = __g_lst##TYPE##Tag[--__g_n##TYPE##Tag##Count];				\
//				if( newData->__nRefCount != 0 )															\
//				{																							\
//					errmsg_refcountnonnull();																\
//					_CrtDbgBreak();																			\
//				}																							\
//				newData->__nRefCount++;																	\
//				newData->nElementCount = nElementCount;													\
//				break;																						\
//			}																								\
//		}																									\
//		LEAVE_CRITICAL_SECTION(Token);																		\
//	}																										\
//	error_t errMy=0;																						\
//	if( 0 == newData )																						\
//	{																										\
//		gcreateData( &newData );																			\
//		if( 0 != newData ) 																				\
//		{																									\
//			newData->DataFormat	= TYPE;																	\
//			newData->nElementCount	= nElementCount;														\
//			newData->nSizeInBytes	= GTYPEID_TOTALBYTES(newData->DataFormat)*newData->nElementCount;		\
//			newData->nRowElementCount	= 1;																\
//			newData->pByteArray	= malloc( newData->nSizeInBytes );										\
//			if( 0 == newData->pByteArray )																	\
//			{																								\
//				errMy = -1;																					\
//				error_printf("Cannot allocate buffer! Out of memory? Size requested %u",newData->nSizeInBytes);										\
//				gfreeData( &newData );																		\
//			}																								\
//		}																									\
//		else																								\
//		{																									\
//			error_printf( "gcreateBuffer( " #TYPE "** ): malloc(%u) FAILED! Out of memory?\n"				\
//				"Cannot allocate new " #TYPE ".", (uint32_t)sizeof( __GODS(BUFFER) ) );						\
//		}																									\
//	}																										\
//	GOPTR(BUFFER) oldData = (*p);																			\
//	(*p) = newData;																							\
//	if (newData)																							\
//		__g_n##TYPE##Tag##CreatedRefs++;																			\
//	return errMy;																							\
//};
//
//#define __GDEFINE_CREATEBUFFER_FUNCTION_SUPER( Token )				\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token,				)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __1			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __2			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __3			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __4			)		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __5		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __6		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __7		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __8		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __9		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __10		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __11		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __12		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __13		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __14		)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __15		)*/		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __16		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __32		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __64		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __128		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __256		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __512		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __1024		)		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __2048	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __4096	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __8192	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __16384	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __32768	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __65536	)*/		\
///*__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, __131072	)*/		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 16			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 32			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 64			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 128			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 256			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 512			)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 1024		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 2048		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 4096		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 8192		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 16384		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 32768		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 65536		)		\
//__GDEFINE_CREATEBUFFER_FUNCTION_GENERIC( Token, 131072		)
