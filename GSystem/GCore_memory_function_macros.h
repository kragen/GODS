#define BINFIBO						1458552799L
#define errmsg_buffover()			error_printf("Buffer overrun detected!");
#define errmsg_refcountnull()		error_printf("Reference counter is zero! This is often because of a gcore_ptr being cast to either a GODS() or a GOSH() type and then released with gfreeData()/gfreeInterface(). As casting to a reference to the GODS/GOSH doesn't mean the pointer is being copied, the reference counter is not incremented.")
#define errmsg_refcountnonnull()	error_printf("Reference counter is non-zero! This is often because of a gcore_ptr being cast to either a GODS() or a GOSH() type and then released with gfreeData()/gfreeInterface(). As casting to a reference to the GODS/GOSH doesn't mean the pointer is being copied, the reference counter is not incremented.")
#define errmsg_heapcorrupt()		error_printf("Heap corruption detected!");

#define GODS_MTSUPPORT

#if defined(ANDROID)
#define DECLARE_CRITICAL_SECTION(...)
#define INIT_CRITICAL_SECTION(...)
#define ENTER_CRITICAL_SECTION(...)
#define LEAVE_CRITICAL_SECTION(...)
#define DELETE_CRITICAL_SECTION(...)
#elif((defined( WIN32 ) || defined( _WIN32 )) && defined(GODS_MTSUPPORT))
#include <Windows.h>
#define DECLARE_CRITICAL_SECTION(Token)	CRITICAL_SECTION _g__cs##Token##CriticalSection = {};
#define INIT_CRITICAL_SECTION(Token)		\
	static int inited = InitializeCriticalSectionAndSpinCount(&_g__cs##Token##CriticalSection, 0x00000400);
#define ENTER_CRITICAL_SECTION(Token) { if( _g__cs##Token##CriticalSection.LockCount != 0) EnterCriticalSection( &_g__cs##Token##CriticalSection );  }
#define LEAVE_CRITICAL_SECTION(Token) { if( _g__cs##Token##CriticalSection.LockCount != 0) LeaveCriticalSection( &_g__cs##Token##CriticalSection );  }
#define DELETE_CRITICAL_SECTION(Token){ if( _g__cs##Token##CriticalSection.LockCount != 0) DeleteCriticalSection( &_g__cs##Token##CriticalSection ); }
#else
#define DECLARE_CRITICAL_SECTION(...)
#define INIT_CRITICAL_SECTION(...)
#define ENTER_CRITICAL_SECTION(...) 
#define LEAVE_CRITICAL_SECTION(...) 
#define DELETE_CRITICAL_SECTION(...)
#endif

#if defined( ANDROID )
#define __GWINTERLOCKEDINCREMENT(nCount)	(++(nCount))
#define __GWINTERLOCKEDDECREMENT(nCount)	(--(nCount))
#elif (defined( _WIN32 ) || defined( WIN32 )) && (defined(GCORE_FUNCTIONS_INTERLOCKED) || defined(GODS_MTSUPPORT))
#define __GWINTERLOCKEDINCREMENT(nCount)	( InterlockedIncrement( &nCount ) )
#define __GWINTERLOCKEDDECREMENT(nCount)	( InterlockedDecrement( &nCount ) )
#else
#define __GWINTERLOCKEDINCREMENT(nCount)	(++(nCount))
#define __GWINTERLOCKEDDECREMENT(nCount)	(--(nCount))
#endif

#define __GDEFINE_AMIOWNING_DATA( _structName )							\
bool god::gwritableData( const _structName* in_CoreInstance )			\
{																		\
	return ( in_CoreInstance && in_CoreInstance->__nRefCount == 1 );	\
};

#define __GDEFINE_FREEARRAY_FUNCTION( structName )									\
void god::gfreeData( structName** in_lstCoreInstances, uint32_t* nElementCount )	\
{																					\
	while( *nElementCount )															\
		gfreeData( &in_lstCoreInstances[--(*nElementCount)] );						\
};


#define __GDEFINE_ACQUIREDATA_FUNCTION( structName, Token )	\
structName* god::gacquireData( structName* p )			\
{														\
	if( p )												\
	{													\
		if( 0 == p->__nRefCount )						\
		{												\
			errmsg_refcountnull();						\
			printInfoString(p);							\
			_CrtDbgBreak();								\
		}												\
		if( BINFIBO != *((uint32_t*)(p+1)) )			\
		{												\
			errmsg_buffover();							\
			_CrtDbgBreak();								\
		}												\
		__GWINTERLOCKEDINCREMENT( p->__nRefCount );		\
		if (p)											\
			__g_n##Token##AcquiredRefs++;				\
	}													\
	return p;											\
}
//
#define __GDEFINE_ACQUIREINTERFACE_FUNCTION( structName )	\
structName* god::gacquireInterface( structName* p )			\
{															\
	/*GPROFILE_FUNCTION();*/								\
	if( p )													\
	{														\
		if( 0 == p->__nRefCount )							\
		{													\
			errmsg_refcountnull();							\
			error_printf("%s", p->__kCue);					\
			_CrtDbgBreak();									\
		}													\
		__GWINTERLOCKEDINCREMENT( p->__nRefCount );			\
	}														\
	return p;												\
}

//--------------------------------------------------------------
#define __GDEFINE_FREEDATA_FUNCTION_NO_MEMBER_CLEANUP( structName, Token )									\
	void god::gfreeData( structName** _p )																	\
	{																										\
		/*GPROFILE_FUNCTION();*/																			\
		if( 0 == (*_p) )																					\
			return;																							\
		structName* p = *_p;																				\
		(*_p) = 0;																							\
																											\
		if( (p)->__pfreeData != gfreeData )																	\
		{																									\
			(p)->__pfreeData(&p);																			\
			return;																							\
		}																									\
																											\
		if( BINFIBO != *((uint32_t*)((p)+1)) )																\
		{																									\
			errmsg_buffover();																				\
			_CrtDbgBreak();																					\
		}																									\
		switch( (p)->__nRefCount )																			\
		{																									\
		default:																							\
			__GWINTERLOCKEDDECREMENT( (p)->__nRefCount );													\
			break;																							\
		case 0:																								\
			errmsg_refcountnull();																			\
			printInfoString(p);																				\
			_CrtDbgBreak();																					\
			(p)->__nRefCount = 1;																			\
		case 1:																								\
			ENTER_CRITICAL_SECTION(Token);																	\
			if( (0xFFFFFFFFUL == (uint32_t)(__g_lst##Token)) || gresizePointerArray( sizeof( structName* ),	\
									__g_n##Token##Count+1, 													\
									&__g_n##Token##ArraySize,												\
									(void**)&__g_lst##Token ) )												\
				free(p);																					\
			else																							\
			{																								\
				(p)->__nRefCount--;																			\
				__g_lst##Token[__g_n##Token##Count] = p;													\
				__g_n##Token##Count++;																		\
			}																								\
			LEAVE_CRITICAL_SECTION(Token);																	\
			if (p)																							\
				__g_n##Token##FreedRefs++;																	\
		}																									\
	};
//--------------------------------------------------------------
#define __GDEFINE_FREEINTERFACE_FUNCTION_RAW_DELETE( structName )	\
	void god::gfreeInterface( structName** _p )						\
	{																\
		/*GPROFILE_FUNCTION();*/									\
		if( 0 == (*_p) )											\
			return;													\
		structName* p = *_p;										\
		(*_p) = 0;													\
																	\
		if( (p)->__pfreeInterface != gfreeInterface )				\
		{															\
			(p)->__pfreeInterface(&p);								\
			return;													\
		}															\
																	\
		switch( (p)->__nRefCount )									\
		{															\
		default:													\
			__GWINTERLOCKEDDECREMENT( (p)->__nRefCount );			\
			break;													\
		case 0:														\
			errmsg_refcountnull();									\
			_CrtDbgBreak();											\
			(p)->__nRefCount	= 1;								\
		case 1:														\
			delete( (p) );											\
		};															\
	};

#define __GDECLARE_INLINE_CORE_STRUCTURE_DEALLOCATOR( structName, Token )														\
	static uint32_t			__g_n##Token##ArraySize=0;																			\
	static uint32_t			__g_n##Token##Count = 0;																			\
	static int32_t			__g_n##Token##CreatedRefs = 0;																		\
	static int32_t			__g_n##Token##AcquiredRefs = 0;																		\
	static int32_t			__g_n##Token##FreedRefs = 0;																		\
	static structName**		__g_lst##Token = 0;																					\
	DECLARE_CRITICAL_SECTION(Token);																							\
	struct __C##Token##Destructor																								\
	{																															\
		__C##Token##Destructor()																								\
		{																														\
			GPROFILE_METHOD( "__C"#Token"Destructor", "__C"#Token"Destructor" );												\
		 	INIT_CRITICAL_SECTION(Token);																						\
		}																														\
		~__C##Token##Destructor()																								\
		{																														\
			/* WARNING! Enabling the following line causes a memory leak!*/														\
			/* This is because the profiler/debugger relies on this function to work		*/									\
			/* GPROFILE_METHOD( "__C"#Token"Destructor", "~__C"#Token"Destructor" );	*/										\
			if(	__g_lst##Token )																								\
			{																													\
				debug_printf("Preparing to deallocate %u "#structName " instances (%u bytes)", __g_n##Token##Count, (sizeof(structName)+sizeof(uint32_t))*__g_n##Token##Count);	\
				debug_printf("%u "#structName " instances created (%u bytes)", __g_n##Token##CreatedRefs, (sizeof(structName)+sizeof(uint32_t))*__g_n##Token##CreatedRefs);		\
				debug_printf("%u "#structName " instances acquired (%u bytes)", __g_n##Token##AcquiredRefs, (sizeof(structName)+sizeof(uint32_t))*__g_n##Token##AcquiredRefs);	\
				debug_printf("%u "#structName " instances freed (%u bytes)", __g_n##Token##FreedRefs, (sizeof(structName)+sizeof(uint32_t))*__g_n##Token##FreedRefs);			\
				while (__g_n##Token##Count)																						\
				{																												\
					free( __g_lst##Token[--__g_n##Token##Count] );																\
				}																												\
				debug_printf("Deallocation finished");																			\
				free( __g_lst##Token );																							\
				__g_lst##Token = (structName**)0xFFFFFFFFUL;																	\
			}																													\
			DELETE_CRITICAL_SECTION(Token);																						\
		}																														\
	};

#define __GDEFINE_CREATEDATA_FUNCTION_MEMBER_INIT( structName, Token )															\
																																\
__GDECLARE_INLINE_CORE_STRUCTURE_DEALLOCATOR(structName,Token);																	\
																																\
	static __C##Token##Destructor	__g_##Token##Destructor;																	\
																																\
	void god::gcreateData( structName** p )																						\
	{																															\
		/*GPROFILE_FUNCTION();*/																								\
 		structName* newData=0;																									\
		ENTER_CRITICAL_SECTION(Token);																							\
		if( __g_n##Token##Count	)																								\
		{																														\
			newData = __g_lst##Token[--__g_n##Token##Count];																	\
			LEAVE_CRITICAL_SECTION(Token);																						\
			if( newData )																										\
			{																													\
				if( newData->__nRefCount != 0 )																					\
				{																												\
					errmsg_refcountnonnull();																					\
					_CrtDbgBreak();																								\
				}																												\
				newData->__nRefCount = 1;																						\
				/*newData->__pfreeData				= gfreeData;*/																\
				memset( ((char*)newData)+newData->__kDataOffset, 0, newData->__kDataSize );										\
				if( BINFIBO != ( *((uint32_t*)(newData+1)) ) )																	\
				{																												\
					errmsg_buffover();																							\
					_CrtDbgBreak();																								\
				}																												\
			}																													\
			else																												\
			{																													\
				errmsg_heapcorrupt();																							\
				_CrtDbgBreak();																									\
			}																													\
		}																														\
		else																													\
		{																														\
			LEAVE_CRITICAL_SECTION(Token);																						\
			newData = (structName*)malloc( sizeof( structName )+sizeof(uint32_t) );												\
			if( 0 != newData ) 																									\
			{																													\
				newData->__nRefCount	= 1;																					\
				newData->__pfreeData				= gfreeData;																\
				*((uint32_t*)(newData+1)) = BINFIBO;																			\
				memset( ((char*)newData)+newData->__kDataOffset, 0, newData->__kDataSize );										\
				/*structName##funcFree f	= &gfreeData;*/																		\
				/*newData->__deallocator = f; */																				\
			}																													\
			else																												\
			{																													\
				errmsg_alloc();																									\
			}																													\
		}																														\
		gcore_ptr<structName*> oldData = *p;																					\
		(*p) = newData;																											\
		if(newData)																												\
			__g_n##Token##CreatedRefs++;																							\
	};

#if defined( GFORCE_CORE_DATA_INIT )
	#define __GDEFINE_CREATEDATA_FUNCTION_NO_MEMBER_INIT __GDEFINE_CREATEDATA_FUNCTION_MEMBER_INIT 
#else
	#define __GDEFINE_CREATEDATA_FUNCTION_NO_MEMBER_INIT( structName, Token )													\
																																\
		__GDECLARE_INLINE_CORE_STRUCTURE_DEALLOCATOR( structName, Token )														\
																																\
		static __C##Token##Destructor	__g_##Token##Destructor;																\
																																\
		void god::gcreateData( structName ** p )																				\
		{																														\
			/*GPROFILE_FUNCTION();*/																							\
			structName* newData=0;																								\
			ENTER_CRITICAL_SECTION(Token);																					\
			if( __g_n##Token##Count	)																							\
			{																													\
				newData = __g_lst##Token[--__g_n##Token##Count];																\
				LEAVE_CRITICAL_SECTION(Token);																					\
				if( !newData )																									\
				{																												\
					errmsg_heapcorrupt();																						\
					_CrtDbgBreak();																								\
				}																												\
				else if( BINFIBO != (*((uint32_t*)(newData+1))) )																\
				{																												\
					errmsg_buffover();																							\
					_CrtDbgBreak();																								\
				}																												\
				else if( newData->__nRefCount != 0 )																			\
				{																												\
					errmsg_refcountnonnull();																					\
					_CrtDbgBreak();																								\
				}																												\
				else																											\
					newData->__nRefCount = 1;																					\
			}																													\
			else																												\
			{																													\
				LEAVE_CRITICAL_SECTION(Token);																					\
				newData = (structName*)malloc( sizeof( structName )+sizeof(uint32_t) );											\
				if( newData ) 																									\
				{																												\
					*((uint32_t*)(newData+1))	= BINFIBO;																		\
					newData->__nRefCount		= 1;																			\
					newData->__pfreeData		= gfreeData;																	\
				}																												\
				else																											\
				{																												\
					errmsg_alloc();																								\
				}																												\
			}																													\
			gcore_ptr<structName*> oldData = *p;																				\
			(*p) = newData;																										\
			if (newData)																										\
				__g_n##Token##CreatedRefs++;																						\
		};
#endif

#define __GDEFINE_CLONEDATA_FUNCTION_NO_MEMBER_POINTERS( structName )				\
	void god::gcloneData( structName** dst, const structName* src )					\
	{																				\
		GPROFILE_FUNCTION();														\
		if( 0 == src )																\
		{																			\
			gfreeData( dst );														\
			return;																	\
		}																			\
		gcore_ptr<structName*> newData;												\
		newData.create();															\
		/*if( newData )*/															\
			gcopyData( newData, src );												\
		/*else																		*/\
		/*{																			*/\
		/*	debug_printf("gcloneData() cannot create new "#structName" instance!");	*/\
		/*}																			*/\
		gcore_ptr<structName*> oldData = *dst;										\
		(*dst) = gacquireData( newData );											\
	}

#define __GDEFINE_COPYDATA_FUNCTION_NO_MEMBER_POINTERS( structName )						\
	void god::gcopyData( structName* dst, const structName* src )							\
	{																						\
		GPROFILE_FUNCTION();																\
		if( 0 == dst && 0 == src )															\
			return;																			\
		else if( 0 == dst )																	\
		{																					\
			error_printf("Cannot copy "#structName" contents into a null pointer!");		\
			return;																			\
		}																					\
		else if( 0 == src )																	\
		{																					\
			memset( ((char*)&dst->__nRefCount)+dst->__kDataOffset,							\
				0,																			\
				dst->__kDataSize );															\
		}																					\
		else																				\
		{																					\
			switch( structName::__kDataTurbo )												\
			{																				\
			case 8:																			\
			{																				\
				uint64_t* psrctmp = (uint64_t*)(((char*)src)+structName::__kDataOffset),	\
					*pdsttmp = (uint64_t*)(((char*)dst)+structName::__kDataOffset);			\
				for( uint32_t i=0; i<structName::__kDataReserved; i++ )						\
					if( psrctmp[i] != pdsttmp[i] )											\
						pdsttmp[i] = psrctmp[i];											\
			}																				\
			case 4:																			\
			{																				\
				uint32_t* psrctmp = (uint32_t*)(((char*)src)+structName::__kDataOffset),	\
					*pdsttmp = (uint32_t*)(((char*)dst)+structName::__kDataOffset);			\
				for( uint32_t i=0; i<structName::__kDataReserved; i++ )						\
					if( psrctmp[i] != pdsttmp[i] )											\
						pdsttmp[i] = psrctmp[i];											\
			}																				\
			default:																		\
				memcpy( ((char*)&dst->__nRefCount)+dst->__kDataOffset,						\
					((char*)&src->__nRefCount)+src->__kDataOffset,							\
					dst->__kDataSize );														\
			}																				\
		}																					\
		if( BINFIBO != *((uint32_t*)(src+1)) )												\
		{																					\
			errmsg_buffover();																\
			printInfoString(src);															\
			_CrtDbgBreak();																	\
		}																					\
		if( BINFIBO != *((uint32_t*)(dst+1)) )												\
		{																					\
			errmsg_buffover();																\
			printInfoString(dst);															\
			_CrtDbgBreak();																	\
		}																					\
	}

#define __GDEFINE_COMPAREDATA_FUNCTION( structName )							\
int32_t god::gcompareData( const structName* p1, const structName* p2 )			\
{																				\
	GPROFILE_FUNCTION();														\
	if( 0 == p1 && 0 == p2 )													\
		return 0;																\
	else if( 0 == p1 || 0 == p2 )												\
		return -1;																\
																				\
	return memcmp(((char*)&p1->__nRefCount)+p1->__kDataOffset,					\
		((char*)&p2->__nRefCount)+p2->__kDataOffset, structName::__kDataSize );	\
};
#if defined(ANDROID)
	//---------------------
	#define __GDEFINE_PRINTINFOSTRING_FUNCTIONA( _structName, format, ... )		\
		void god::printInfoString( const _structName* in_CoreInstance )			\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				warning_printf(#_structName"* is NULL.");						\
				return;															\
			}																	\
			data_printf(#_structName " at location 0x%p:\n"						\
				"nRefCount.....................: %i\n"							\
				format															\
				, in_CoreInstance												\
				, in_CoreInstance->__nRefCount									\
				, __VA_ARGS__													\
			);																	\
		};																		\
		void god::getInfoString( char* pOutputBuffer, uint32_t nBufferSize,		\
			const _structName* in_CoreInstance )								\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				sprintf( pOutputBuffer, "%s\n", "(null)" );		\
				return;															\
			}																	\
			sprintf( pOutputBuffer, format "\n", __VA_ARGS__	);	\
		};																		\
		void god::getInfoString( wchar_t* pOutputBuffer, uint32_t nBufferSize,	\
			const _structName* in_CoreInstance )								\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				/*swprintf( pOutputBuffer, L"%s\n", L"(null)" );*/	\
				return;															\
			}																	\
			char buffer[2048];													\
			sprintf( buffer, format "\n", __VA_ARGS__	);						\
			mbstowcs( pOutputBuffer, buffer, 2048 );							\
		};																	
#else
	//---------------------
	#define __GDEFINE_PRINTINFOSTRING_FUNCTIONA( _structName, format, ... )		\
		void god::printInfoString( const _structName* in_CoreInstance )			\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				warning_printf(#_structName"* is NULL.");						\
				return;															\
			}																	\
			data_printf(#_structName " at location 0x%p:\n"						\
				"nRefCount.....................: %i\n"							\
				format															\
				, in_CoreInstance												\
				, in_CoreInstance->__nRefCount									\
				, __VA_ARGS__													\
			);																	\
		};																		\
		void god::getInfoString( char* pOutputBuffer, uint32_t nBufferSize,		\
			const _structName* in_CoreInstance )								\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				sprintf_s( pOutputBuffer, nBufferSize, "%s\n", "(null)" );		\
				return;															\
			}																	\
			sprintf_s( pOutputBuffer, nBufferSize, format "\n", __VA_ARGS__	);	\
		};																		\
		void god::getInfoString( wchar_t* pOutputBuffer, uint32_t nBufferSize,	\
			const _structName* in_CoreInstance )								\
		{																		\
			if( 0 == in_CoreInstance )											\
			{																	\
				swprintf_s( pOutputBuffer, nBufferSize, L"%s\n", L"(null)" );	\
				return;															\
			}																	\
			char buffer[2048];													\
			sprintf_s( buffer, nBufferSize, format "\n", __VA_ARGS__	);		\
			size_t n;															\
			mbstowcs_s( &n, pOutputBuffer, nBufferSize, buffer, 2048 );			\
		};		

#define __GDEFINE_PRINTINFOSTRING_FUNCTIONW( _structName, format, ... )				\
	void god::printInfoString( const _structName* in_CoreInstance )					\
	{																				\
		if( 0 == in_CoreInstance )													\
		{																			\
			warning_printf(#_structName"* is NULL.");								\
			return;																	\
		}																			\
		data_printf(#_structName" at location 0x%p:\n", in_CoreInstance);			\
		data_wprintf(																\
			L"nRefCount.....................: %i\n"									\
			format																	\
			, in_CoreInstance->__nRefCount											\
			, __VA_ARGS__															\
		);																			\
	};																				\
	void god::getInfoString( wchar_t* pOutputBuffer, uint32_t nBufferSize,			\
		const _structName* in_CoreInstance )										\
	{																				\
		if( 0 == in_CoreInstance )													\
		{																			\
			swprintf_s( pOutputBuffer, nBufferSize, L"%s\n", L"(null)" );			\
			return;																	\
		}																			\
		swprintf_s( pOutputBuffer, nBufferSize, format L"\n", __VA_ARGS__	);		\
	};																				\
	void god::getInfoString( char* pOutputBuffer, uint32_t nBufferSize,				\
		const _structName* in_CoreInstance )										\
	{																				\
		if( 0 == in_CoreInstance )													\
		{																			\
			sprintf_s( pOutputBuffer, nBufferSize, "%s\n", "(null)" );				\
			return;																	\
		}																			\
		wchar_t buffer[2048];														\
		size_t n = swprintf_s( buffer, nBufferSize, format L"\n", __VA_ARGS__	);	\
		wcstombs_s( &n, pOutputBuffer, nBufferSize, buffer, n );					\
	};


#endif // ANDROID