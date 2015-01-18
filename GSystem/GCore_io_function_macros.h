
#define __GDEFINE_DUMMY_READWRITE_FUNCTIONS( structName )								\
uint32_t god::fileReadData(structName** v,uint32_t c,FILE * f)							\
{																						\
	error_printf("Cannot \"read\" a "#structName" instance!");							\
	return -1;																			\
}																						\
uint32_t god::fileWriteData(structName * const * v,uint32_t n,FILE * f, structName * b)	\
{																						\
	error_printf("Cannot \"write\" a "#structName" instance!");							\
	return -1;																			\
}																						\
uint32_t god::memReadData(structName * *,uint32_t ,void const *out)						\
{																						\
	return fileReadData( (structName**)0, 0, 0 );										\
}																						\
uint32_t god::memWriteData(structName * const *,uint32_t ,void *, structName *)			\
{																						\
	return fileWriteData( (structName**)0, 0, 0, 0 );									\
}

#define __GDEFINE_DUMMY_SERIALIZATION_FUNCTIONS( structName )					\
uint32_t god::fileDeserializeData(structName** v,uint32_t c,FILE * f)			\
{																				\
	error_printf("Cannot \"deserialize\" a "#structName" instance!");			\
	return -1;																	\
}																				\
uint32_t god::fileSerializeData(structName * const * v, uint32_t n,FILE * f)	\
{																				\
	error_printf("Cannot \"serialize\" a "#structName" instance!");				\
	return -1;																	\
}																				\
uint32_t god::memDeserializeData(structName * *,uint32_t ,void const *out)		\
{																				\
	return fileDeserializeData( (structName**)0, 0, 0 );						\
}																				\
uint32_t god::memSerializeData(structName * const *,uint32_t ,void *)			\
{																				\
	return fileSerializeData( (structName**)0, 0, 0 );							\
}
#define __GDEFINE_DUMMY_IO_FUNCTIONS( structName )			\
	__GDEFINE_DUMMY_SERIALIZATION_FUNCTIONS( structName )	\
	__GDEFINE_DUMMY_READWRITE_FUNCTIONS( structName )

//--------------------------------------------- Streaming function macros

#define __GDEFINE_MEMWRITE_FUNCTION_NO_MEMBER_POINTERS( structName )																					\
	uint32_t god::memWriteData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, void* out_pMemoryBuffer, structName* in_DefaultData )	\
	{																																\
		GPROFILE_FUNCTION();																										\
		if( 0 == nDefinitionCount || 0 == in_DefinitionList )																		\
		{																															\
			error_printf("memWriteData(): Invalid parameters!\n"																	\
				#structName "**		: 0x%p\n"																						\
				"uint32_t			: %i\n"																							\
				"void*				: 0x%p"																							\
				, in_DefinitionList																									\
				, (int)nDefinitionCount																								\
				, out_pMemoryBuffer																									\
				);																													\
			return 0;	/* nothing written	*/																						\
		}																															\
		gcore_ptr<structName*> DefaultData = gacquireData( in_DefaultData );														\
		if( 0 == in_DefaultData )																									\
		{																															\
			DefaultData.create();																									\
			if( !DefaultData )																										\
			{																														\
				error_printf("gcreateData() returned NULL! Out of memory? Cannot write data to file.");								\
				return 0;																											\
			};																														\
			memset( ((char*)&DefaultData->__nRefCount)+structName::__kDataOffset, 0, structName::__kDataSize );						\
		}																															\
																																	\
		uint32_t i, nSkipped=0, byteIndex=0;																						\
		structName* pInstanceToSave=0;																								\
		for( i=0; i< nDefinitionCount; i++ )																						\
		{																															\
			/* Select instance values or default values if instance is NULL */														\
			if( 0 == in_DefinitionList[i] )																							\
			{																														\
				pInstanceToSave = DefaultData;																						\
				nSkipped++;																											\
			}																														\
			else																													\
				pInstanceToSave = in_DefinitionList[i];																				\
																																	\
			if( out_pMemoryBuffer )																									\
				memcpy( &((char*)out_pMemoryBuffer)[byteIndex],																		\
					((char*)&pInstanceToSave->__nRefCount)+structName::__kDataOffset, 												\
					structName::__kDataSize );																						\
			byteIndex += structName::__kDataSize;																					\
		}																															\
		debug_printf("%i " #structName " instances written to memory stream, %i written with default values.", i, nSkipped);		\
		return byteIndex;																											\
	};

#define __GDEFINE_MEMREAD_FUNCTION_NO_MEMBER_POINTERS( structName )																	\
	uint32_t god::memReadData( structName** out_DefinitionList, uint32_t nDefinitionCount, const void* in_pMemoryBuffer )			\
	{																																\
		GPROFILE_FUNCTION();																										\
		if( 0 == in_pMemoryBuffer )																									\
		{																															\
			error_printf("memReadData(): Invalid parameters!"																		\
				#structName "**		: 0x%p\n"																						\
				"uint32_t		: %i\n"																								\
				"void*			: 0x%p"																								\
				, out_DefinitionList																								\
				, (int) nDefinitionCount																							\
				, in_pMemoryBuffer																									\
				);																													\
			return 0;	/* no source file to read from	*/																			\
		}																															\
																																	\
		uint32_t i, j, nLoaded=0, byteIndex=0, nSkipped=0;																			\
		gcore_ptr<structName*>	newData, oldData;																					\
		glist<structName*> lstLoadedData( nDefinitionCount );																		\
																																	\
		for( i=0; i< nDefinitionCount; i++ )																						\
		{																															\
			newData.create();																										\
			if( !newData )																											\
			{																														\
				error_printf("Cannot create new " #structName " instance! Terminating function...");								\
				return byteIndex;																									\
			}																														\
																																	\
			memcpy( newData.get_data_address(),																						\
				&((char*)in_pMemoryBuffer)[byteIndex], structName::__kDataSize );													\
			byteIndex += structName::__kDataSize;																					\
			for( j=0; j<nLoaded; j++ )																								\
			{																														\
				if( 0 == gcompareData(lstLoadedData[j], newData) )																	\
				{																													\
 					newData = gacquireData( lstLoadedData[j] );																		\
 					nSkipped++;																										\
 					break;																											\
				}																													\
			}																														\
			if( j == nLoaded )																										\
			{																														\
				lstLoadedData[nLoaded] = gacquireData( newData );																	\
				nLoaded++;																											\
			}																														\
																																	\
			if( out_DefinitionList )																								\
			{																														\
				oldData = out_DefinitionList[i];																					\
				out_DefinitionList[i] = gacquireData( newData );																	\
			}																														\
		}																															\
		debug_printf("%i " #structName " instances read from memory stream, %i referenced.", i, nSkipped);							\
		return byteIndex;																											\
	};

#if defined( ANDROID )
	#define __GDEFINE_FILEWRITE_FUNCTION_NO_MEMBER_POINTERS( structName )																		\
	uint32_t god::fileWriteData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp, structName* in_DefaultData )	\
	{																														\
		GPROFILE_FUNCTION();																								\
		if( 0 == out_fp || 0 == in_DefinitionList )																			\
		{																													\
			error_printf("fileWriteData(): Invalid parameters!\n"															\
				#structName "**		: 0x%p\n"																				\
				"uint32_t			: %i\n"																					\
				"FILE*				: 0x%p"																					\
				, in_DefinitionList																							\
				, (int) nDefinitionCount																					\
				, out_fp																									\
				);																											\
			return 0;	/* no source to read from */																		\
		}																													\
																															\
		gcore_ptr<structName*> InstanceToSave, DefaultData = gacquireData( in_DefaultData );								\
		if( 0 == in_DefaultData )																							\
		{																													\
			DefaultData.create();																							\
			if( !DefaultData )																								\
			{																												\
				error_printf("gcreateData() returned NULL! Out of memory? Cannot write data to file.");						\
				return 0;																									\
			};																												\
			memset( DefaultData.get_data_address(), 0, structName::__kDataSize );											\
		}																													\
																															\
		uint32_t i, nSkipped=0;																								\
		for( i=0; i< nDefinitionCount; i++ )																				\
		{																													\
			/* Select instance values or default values if instance is NULL */												\
			if( 0 == in_DefinitionList[i] )																					\
			{																												\
				InstanceToSave = DefaultData;																				\
				nSkipped++;																									\
			}																												\
			else																											\
				InstanceToSave = gacquireData( in_DefinitionList[i] );														\
																															\
			if( fwrite( InstanceToSave.get_data_address(),																	\
				structName::__kDataSize, 1, out_fp ) < 1 )																	\
			{																												\
				error_printf( "Error saving " #structName " instance at location %p:\n" 									\
					, in_DefinitionList[i]																					\
					);																										\
				printInfoString( in_DefinitionList[i] );																	\
				error_printf("%i " #structName " instances written to file.", i);											\
				return i;																									\
			}																												\
		}																													\
		debug_printf("%i " #structName " instances written to file, %i written with default values.", i, nSkipped);			\
		return i;																											\
	};


	//----------------------------------------------------------------------
	#define __GDEFINE_FILEREAD_FUNCTION_NO_MEMBER_POINTERS( structName )									\
	uint32_t god::fileReadData( structName** out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )	\
	{																										\
		GPROFILE_FUNCTION();																				\
		if( 0 == in_fp )																					\
		{																									\
			error_printf("fileReadData(): Invalid parameters!\n"											\
				#structName "**		: 0x%p\n"																\
				"uint32_t			: %i\n"																	\
				"FILE*				: 0x%p"																	\
				, out_DefinitionList																		\
				, (int) nDefinitionCount																	\
				, in_fp																						\
				);																							\
			return 0;	/* no source file to read from */													\
		}																									\
																											\
		uint32_t i, j, nLoaded=0, nSkipped=0;																\
		glist<structName*> lstLoadedData( nDefinitionCount );												\
		gcore_ptr<structName*> newData, oldData;															\
		for( i=0; i< nDefinitionCount; i++ )																\
		{																									\
			newData.create();																				\
			if( !newData )																					\
			{																								\
				error_printf("Cannot create new " #structName " instance! Terminating function...");		\
				return i;																					\
			}																								\
			if( fread( newData.get_data_address(),															\
				structName::__kDataSize, 1, in_fp ) < 1 )													\
			{																								\
				error_printf( "Cannot read " #structName " at position %i:\n"	 							\
					, (int)i																				\
					);																						\
				error_printf("%i " #structName " instances read from file.", i);							\
				return i;																					\
			}																								\
			for( j=0; j<nLoaded; j++ )																		\
			{																								\
				if( 0 == gcompareData(lstLoadedData[j], newData) )											\
				{																							\
 					newData = gacquireData( lstLoadedData[j] );												\
 					nSkipped++;																				\
					break;																					\
				}																							\
			}																								\
			if( j == nLoaded )																				\
			{																								\
				lstLoadedData[nLoaded] = gacquireData( newData );											\
				nLoaded++;																					\
			}																								\
																											\
			if( out_DefinitionList )																		\
			{																								\
				oldData = out_DefinitionList[i];															\
				out_DefinitionList[i] = gacquireData( newData );											\
			}																								\
		}																									\
		debug_printf("%i " #structName " instances read from file, %i referenced.", i, nSkipped);			\
		return i;																							\
	};

#elif defined( _WIN32 ) || defined(WIN32)
	#define __GDEFINE_FILEWRITE_FUNCTION_NO_MEMBER_POINTERS( structName )																		\
	uint32_t god::fileWriteData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp, structName* in_DefaultData )	\
	{																														\
		GPROFILE_FUNCTION();																								\
		if( 0 == out_fp || 0 == in_DefinitionList )																			\
		{																													\
			error_printf("fileWriteData(): Invalid parameters!\n"															\
				#structName "**		: 0x%p\n"																				\
				"uint32_t			: %i\n"																					\
				"FILE*				: 0x%p"																					\
				, in_DefinitionList																							\
				, (int) nDefinitionCount																					\
				, out_fp																									\
				);																											\
			return 0;	/* no source to read from */																		\
		}																													\
																															\
		gcore_ptr<structName*> InstanceToSave, DefaultData = gacquireData( in_DefaultData );								\
		if( 0 == in_DefaultData )																							\
		{																													\
			DefaultData.create();																							\
			if( !DefaultData )																								\
			{																												\
				error_printf("gcreateData() returned NULL! Out of memory? Cannot write data to file.");						\
				return 0;																									\
			};																												\
			memset( DefaultData.get_data_address(), 0, structName::__kDataSize );											\
		}																													\
																															\
		uint32_t i, nSkipped=0;																								\
		for( i=0; i< nDefinitionCount; i++ )																				\
		{																													\
			/* Select instance values or default values if instance is NULL */												\
			if( 0 == in_DefinitionList[i] )																					\
			{																												\
				InstanceToSave = DefaultData;																				\
				nSkipped++;																									\
			}																												\
			else																											\
				InstanceToSave = gacquireData( in_DefinitionList[i] );														\
																															\
			if( fwrite( InstanceToSave.get_data_address(),																	\
				structName::__kDataSize, 1, out_fp ) < 1 )																	\
			{																												\
				error_printf( "Error saving " #structName " instance at location %p:\n" 									\
					"fp->_ptr		: %p\n"																					\
					"fp-> _cnt		: %i\n"																					\
					"fp->_base		: %p\n"																					\
					"fp-> _flag		: %i\n"																					\
					"fp->_file		: %i\n"																					\
					"fp->_charbuf	: %i\n"																					\
					"fp->_bufsiz	: %i\n"																					\
					"fp->_tmpfname	: %p\n"																					\
					"fp->_tmpfname	: %s"																					\
					, in_DefinitionList[i]																					\
					, out_fp->_ptr																							\
					, (int)out_fp-> _cnt																					\
					, out_fp->_base																							\
					, (int)out_fp->_flag																					\
					, (int)out_fp->_file																					\
					, (int)out_fp->_charbuf																					\
					, (int)out_fp->_bufsiz																					\
					, out_fp->_tmpfname																						\
					, ((0 == out_fp->_tmpfname) ? "" : out_fp->_tmpfname)													\
					);																										\
				printInfoString( in_DefinitionList[i] );																	\
				error_printf("%i " #structName " instances written to file.", i);											\
				return i;																									\
			}																												\
		}																													\
		debug_printf("%i " #structName " instances written to file, %i written with default values.", i, nSkipped);			\
		return i;																											\
	};


	//----------------------------------------------------------------------
	#define __GDEFINE_FILEREAD_FUNCTION_NO_MEMBER_POINTERS( structName )									\
	uint32_t god::fileReadData( structName** out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )	\
	{																										\
		GPROFILE_FUNCTION();																				\
		if( 0 == in_fp )																					\
		{																									\
			error_printf("fileReadData(): Invalid parameters!\n"											\
				#structName "**		: 0x%p\n"																\
				"uint32_t			: %i\n"																	\
				"FILE*				: 0x%p"																	\
				, out_DefinitionList																		\
				, (int) nDefinitionCount																	\
				, in_fp																						\
				);																							\
			return 0;	/* no source file to read from */													\
		}																									\
																											\
		uint32_t i, j, nLoaded=0, nSkipped=0;																\
		glist<structName*> lstLoadedData( nDefinitionCount );												\
		gcore_ptr<structName*> newData, oldData;															\
		for( i=0; i< nDefinitionCount; i++ )																\
		{																									\
			newData.create();																				\
			if( !newData )																					\
			{																								\
				error_printf("Cannot create new " #structName " instance! Terminating function...");		\
				return i;																					\
			}																								\
			if( fread( newData.get_data_address(),															\
				structName::__kDataSize, 1, in_fp ) < 1 )													\
			{																								\
				error_printf( "Cannot read " #structName " at position %i:\n"	 							\
					"fp->_ptr		: %p\n"																	\
					"fp-> _cnt		: %i\n"																	\
					"fp->_base		: %p\n"																	\
					"fp-> _flag		: %i\n"																	\
					"fp->_file		: %i\n"																	\
					"fp->_charbuf	: %i\n"																	\
					"fp->_bufsiz	: %i\n"																	\
					"fp->_tmpfname	: %p\n"																	\
					"fp->_tmpfname	: %s"																	\
					, (int)i																				\
					, in_fp->_ptr																			\
					, (int)in_fp->_cnt																		\
					, in_fp->_base																			\
					, (int)in_fp-> _flag																	\
					, (int)in_fp->_file																		\
					, (int)in_fp->_charbuf																	\
					, (int)in_fp->_bufsiz																	\
					, in_fp->_tmpfname																		\
					, ((in_fp->_tmpfname != 0) ? in_fp->_tmpfname : "\"\"")									\
					);																						\
				error_printf("%i " #structName " instances read from file.", i);							\
				return i;																					\
			}																								\
			for( j=0; j<nLoaded; j++ )																		\
			{																								\
				if( 0 == gcompareData(lstLoadedData[j], newData) )											\
				{																							\
 					newData = gacquireData( lstLoadedData[j] );												\
 					nSkipped++;																				\
					break;																					\
				}																							\
			}																								\
			if( j == nLoaded )																				\
			{																								\
				lstLoadedData[nLoaded] = gacquireData( newData );											\
				nLoaded++;																					\
			}																								\
																											\
			if( out_DefinitionList )																		\
			{																								\
				oldData = out_DefinitionList[i];															\
				out_DefinitionList[i] = gacquireData( newData );											\
			}																								\
		}																									\
		debug_printf("%i " #structName " instances read from file, %i referenced.", i, nSkipped);			\
		return i;																							\
	};
#else
#endif // WIN32

//--------------------------------------------- Serialization function macros

#define __GDEFINE_MEMSERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )													\
uint32_t god::memSerializeData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, void* out_pMemoryBuffer )	\
{																															\
	GPROFILE_FUNCTION();																									\
	if( 0 == nDefinitionCount || 0 == in_DefinitionList )																	\
	{																														\
		error_printf("memSerializeData(): Invalid parameters!\n"															\
			#structName "**		: 0x%p\n"																					\
			"uint32_t			: %i\n"																						\
			"void*				: 0x%p"																						\
			, in_DefinitionList																								\
			, (int)nDefinitionCount																							\
			, out_pMemoryBuffer																								\
			);																												\
		return 0;	/* nothing written	*/																					\
	}																														\
																															\
	uint32_t i, nSkipped=0, byteIndex=0;																					\
	uint8_t bSaved;																											\
	for( i=0; i< nDefinitionCount; i++ )																					\
	{																														\
		/* save a byte boolean telling if the data is gonna be saved to the file (0 for a null data) */						\
		bSaved = one_if(in_DefinitionList[i]);																				\
		if( out_pMemoryBuffer )																								\
			((char*)out_pMemoryBuffer)[byteIndex] = bSaved;																	\
		byteIndex++;																										\
																															\
		if( 0 == bSaved )																									\
		{																													\
			nSkipped++;																										\
			continue;																										\
		}																													\
																															\
		if( out_pMemoryBuffer )																								\
			memcpy( &((char*)out_pMemoryBuffer)[byteIndex],																	\
				((char*)&in_DefinitionList[i]->__nRefCount)+structName::__kDataOffset, structName::__kDataSize );			\
		byteIndex += structName::__kDataSize;																				\
	}																														\
																															\
	debug_printf("%i " #structName " instances written to memory stream, %i skipped.", i-nSkipped, nSkipped);				\
	return byteIndex;																										\
};

#define __GDEFINE_MEMDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )														\
uint32_t god::memDeserializeData( structName** out_DefinitionList, uint32_t nDefinitionCount, const void* in_pMemoryBuffer )	\
{																																\
	GPROFILE_FUNCTION();																										\
	if( 0 == in_pMemoryBuffer )																									\
	{																															\
		error_printf("memDeserializeData(): Invalid parameters!"																\
			#structName "**		: 0x%p\n"																						\
			"uint32_t		: %i\n"																								\
			"void*			: 0x%p"																								\
			, out_DefinitionList																								\
			, (int) nDefinitionCount																							\
			, in_pMemoryBuffer																									\
			);																													\
		return 0;	/* no source file to read from	*/																			\
	}																															\
																																\
	uint32_t i, nSkipped=0, byteIndex=0;																						\
	uint8_t bSaved;																												\
	gcore_ptr<structName*> newData, oldData;																					\
																																\
	for( i=0; i< nDefinitionCount; i++ )																						\
	{																															\
		bSaved = ((char*)in_pMemoryBuffer)[byteIndex]; /* read boolean telling if there's data or not */						\
		byteIndex++;																											\
																																\
		if( 0 == bSaved )																										\
		{ /* null definition, continue! */																						\
			nSkipped++;																											\
			if( out_DefinitionList )																							\
				gfreeData( &out_DefinitionList[i] ); /* clear output */															\
			continue; 																											\
		}																														\
																																\
		/* Create new instance to store data */																					\
		newData.create();																										\
		if( !newData )																											\
		{																														\
			error_printf("Cannot create new " #structName " instance! Terminating function...");								\
			return byteIndex;																									\
		}																														\
																																\
		memcpy( newData.get_data_address(),																						\
			&((char*)in_pMemoryBuffer)[byteIndex], structName::__kDataSize );													\
		byteIndex += structName::__kDataSize;																					\
																																\
		if( out_DefinitionList )																								\
		{																														\
			oldData = out_DefinitionList[i];																					\
			out_DefinitionList[i] = gacquireData( newData );																	\
		}																														\
	}																															\
																																\
	debug_printf("%i " #structName " instances read from memory stream, %i skipped.", i-nSkipped, nSkipped);					\
	return byteIndex;																											\
};

#if defined( ANDROID )
	//----------------------------------------------------------------------
	#define __GDEFINE_FILEDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )											\
	uint32_t god::fileDeserializeData( structName** out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )		\
	{																													\
		GPROFILE_FUNCTION();																							\
		if( 0 == in_fp )																								\
		{																												\
			error_printf("fileDeserializeData(): Invalid parameters!\n"													\
				#structName "**		: 0x%p\n"																			\
				"uint32_t			: %i\n"																				\
				"FILE*				: 0x%p"																				\
				, out_DefinitionList																					\
				, (int) nDefinitionCount																				\
				, in_fp																									\
				);																										\
			return 0;	/* no source file to read from */																\
		}																												\
																														\
		uint32_t i, nSkipped=0;																							\
		uint8_t	bSaved;																									\
																														\
		gcore_ptr<structName*> newData, oldData;																		\
		for( i=0; i< nDefinitionCount; i++ )																			\
		{																												\
			if( fread( &bSaved, sizeof( bSaved ), 1, in_fp ) < 1 )														\
			{																											\
				error_printf( "Failed to read metadata from file." );													\
				return i;																								\
			}																											\
			if( 0 == bSaved )																							\
			{/* null buffer, continue! */																				\
				nSkipped++;																								\
				if( out_DefinitionList )																				\
					gfreeData( &out_DefinitionList[i] ); /*clear output*/												\
				continue; 																								\
			}																											\
																														\
			newData.create();																							\
			if( !newData )																								\
			{																											\
				error_printf("Cannot create new " #structName " instance! Terminating function...");					\
				return i;																								\
			}																											\
																														\
			if( fread( ((char*)&newData->__nRefCount)+structName::__kDataOffset,										\
					structName::__kDataSize, 1, in_fp ) < 1 )															\
			{																											\
				error_printf( "Cannot read " #structName " at position %i:\n"	 										\
					, (int)i																							\
					);																									\
				error_printf("%i " #structName " instances read from file.", i-nSkipped);								\
				return i;																								\
			}																											\
			if( out_DefinitionList )																					\
			{																											\
				oldData = out_DefinitionList[i];																		\
				out_DefinitionList[i] = gacquireData( newData );														\
			}																											\
		}																												\
		debug_printf("%i " #structName " instances read from file, %i skipped.", i-nSkipped, nSkipped);					\
		return i;																										\
	};
	
	#define __GDEFINE_FILESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )																\
	uint32_t god::fileSerializeData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp )						\
	{																																		\
		GPROFILE_FUNCTION();																												\
		if( 0 == out_fp || 0 == in_DefinitionList )																							\
		{																																	\
			error_printf("fileSerializeData(): Invalid parameters!\n"																		\
				#structName "**		: 0x%p\n"																								\
				"uint32_t			: %i\n"																									\
				"FILE*				: 0x%p"																									\
				, in_DefinitionList																											\
				, (int) nDefinitionCount																									\
				, out_fp																													\
				);																															\
			return 0;	/* no source to read from */																						\
		}																																	\
																																			\
		uint32_t i, nSkipped=0;																												\
		uint8_t bSaved;																														\
		for( i=0; i< nDefinitionCount; i++ )																								\
		{																																	\
			bSaved = one_if(in_DefinitionList[i]);																							\
			if( fwrite( &bSaved, sizeof( bSaved ), 1, out_fp ) < 1 )																		\
			{																																\
				error_printf("Failed to write metadata to file.");																			\
				return i;																													\
			}																																\
			if( 0 == bSaved )																												\
			{																																\
				nSkipped++;																													\
				continue;																													\
			}																																\
																																			\
			if( fwrite( ((char*)&in_DefinitionList[i]->__nRefCount)+structName::__kDataOffset, structName::__kDataSize, 1, out_fp ) < 1 )	\
			{																																\
				error_printf( "Error saving " #structName " instance at location %p:\n" 													\
					, in_DefinitionList[i]																									\
					);																														\
				printInfoString( in_DefinitionList[i] );																					\
				return i;																													\
			}																																\
		}																																	\
																																			\
		debug_printf("%i " #structName " instances written to file, %i skipped.", i-nSkipped, nSkipped);									\
		return i;																															\
	};
#elif defined( _WIN32 ) || defined( WIN32 )
	//----------------------------------------------------------------------
	#define __GDEFINE_FILEDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )										\
	uint32_t god::fileDeserializeData( structName** out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )	\
	{																												\
		GPROFILE_FUNCTION();																						\
		if( 0 == in_fp )																							\
		{																											\
			error_printf("fileDeserializeData(): Invalid parameters!\n"												\
				#structName "**		: 0x%p\n"																		\
				"uint32_t			: %i\n"																			\
				"FILE*				: 0x%p"																			\
				, out_DefinitionList																				\
				, (int) nDefinitionCount																			\
				, in_fp																								\
				);																									\
			return 0;	/* no source file to read from */															\
		}																											\
																													\
		uint32_t i, nSkipped=0;																						\
		uint8_t	bSaved;																								\
																													\
		gcore_ptr<structName*> newData, oldData;																	\
		for( i=0; i< nDefinitionCount; i++ )																		\
		{																											\
			if( fread( &bSaved, sizeof( bSaved ), 1, in_fp ) < 1 )													\
			{																										\
				error_printf( "Failed to read metadata from file." );												\
				return i;																							\
			}																										\
			if( 0 == bSaved )																						\
			{/* null buffer, continue! */																			\
				nSkipped++;																							\
				gfreeData( &out_DefinitionList[i] ); /*clear output*/												\
				continue; 																							\
			}																										\
																													\
			newData.create();																						\
			if( !newData )																							\
			{																										\
				error_printf("Cannot create new " #structName " instance! Terminating function...");				\
				return i;																							\
			}																										\
																													\
			if( fread( newData.get_data_address(), structName::__kDataSize, 1, in_fp ) < 1 )						\
			{																										\
																													\
				error_printf( "Cannot read " #structName " at position %i:\n"	 									\
					"fp->_ptr		: %p\n"																			\
					"fp-> _cnt		: %i\n"																			\
					"fp->_base		: %p\n"																			\
					"fp-> _flag		: %i\n"																			\
					"fp->_file		: %i\n"																			\
					"fp->_charbuf	: %i\n"																			\
					"fp->_bufsiz	: %i\n"																			\
					"fp->_tmpfname	: %p\n"																			\
					"fp->_tmpfname	: %s"																			\
					, (int)i																						\
					, in_fp->_ptr																					\
					, (int)in_fp->_cnt																				\
					, in_fp->_base																					\
					, (int)in_fp-> _flag																			\
					, (int)in_fp->_file																				\
					, (int)in_fp->_charbuf																			\
					, (int)in_fp->_bufsiz																			\
					, in_fp->_tmpfname																				\
					, ((in_fp->_tmpfname != 0) ? in_fp->_tmpfname : "\"\"")											\
					);																								\
				error_printf("%i " #structName " instances read from file.", i-nSkipped);							\
				return i;																							\
			}																										\
			oldData = out_DefinitionList[i];																		\
			out_DefinitionList[i] = gacquireData( newData );														\
		}																											\
		debug_printf("%i " #structName " instances read from file, %i skipped.", i-nSkipped, nSkipped);				\
		return i;																									\
	};

	#define __GDEFINE_FILESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )											\
	uint32_t god::fileSerializeData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp )	\
	{																													\
		GPROFILE_FUNCTION();																							\
		if( 0 == out_fp || 0 == in_DefinitionList )																		\
		{																												\
			error_printf("fileSerializeData(): Invalid parameters!\n"													\
				#structName "**		: 0x%p\n"																			\
				"uint32_t			: %i\n"																				\
				"FILE*				: 0x%p"																				\
				, in_DefinitionList																						\
				, (int) nDefinitionCount																				\
				, out_fp																								\
				);																										\
			return 0;	/* no source to read from */																	\
		}																												\
																														\
		uint32_t i, nSkipped=0;																							\
		uint8_t bSaved;																									\
		for( i=0; i< nDefinitionCount; i++ )																			\
		{																												\
			bSaved = one_if(in_DefinitionList[i]);																		\
			if( fwrite( &bSaved, sizeof( bSaved ), 1, out_fp ) < 1 )													\
			{																											\
				error_printf("Failed to write metadata to file.");														\
				return i;																								\
			}																											\
			if( 0 == bSaved )																							\
			{																											\
				nSkipped++;																								\
				continue;																								\
			}																											\
																														\
			if( fwrite( ((char*)&in_DefinitionList[i]->__nRefCount)+structName::__kDataOffset,							\
					structName::__kDataSize, 1, out_fp ) < 1 )															\
			{																											\
				error_printf( "Error saving " #structName " instance at location %p:\n" 								\
					"fp->_ptr		: %p\n"																				\
					"fp-> _cnt		: %i\n"																				\
					"fp->_base		: %p\n"																				\
					"fp-> _flag		: %i\n"																				\
					"fp->_file		: %i\n"																				\
					"fp->_charbuf	: %i\n"																				\
					"fp->_bufsiz	: %i\n"																				\
					"fp->_tmpfname	: %p\n"																				\
					"fp->_tmpfname	: %s"																				\
					, in_DefinitionList[i]																				\
					, out_fp->_ptr																						\
					, (int)out_fp-> _cnt																				\
					, out_fp->_base																						\
					, (int)out_fp->_flag																				\
					, (int)out_fp->_file																				\
					, (int)out_fp->_charbuf																				\
					, (int)out_fp->_bufsiz																				\
					, out_fp->_tmpfname																					\
					, ((0 == out_fp->_tmpfname) ? "" : out_fp->_tmpfname)												\
					);																									\
				printInfoString( in_DefinitionList[i] );																\
				return i;																								\
			}																											\
		}																												\
																														\
		debug_printf("%i " #structName " instances written to file, %i skipped.", i-nSkipped, nSkipped);				\
		return i;																										\
	};
#else
	//----------------------------------------------------------------------
	#define __GDEFINE_FILEDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )											\
	uint32_t god::fileDeserializeData( structName** out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )		\
	{																													\
		GPROFILE_FUNCTION();																							\
		if( 0 == in_fp )																								\
		{																												\
			error_printf("fileDeserializeData(): Invalid parameters!\n"													\
				#structName "**		: 0x%p\n"																			\
				"uint32_t			: %i\n"																				\
				"FILE*				: 0x%p"																				\
				, out_DefinitionList																					\
				, (int) nDefinitionCount																				\
				, in_fp																									\
				);																										\
			return 0;	/* no source file to read from */																\
		}																												\
																														\
		uint32_t i, nSkipped=0;																							\
		uint8_t	bSaved;																									\
																														\
		gcore_ptr<structName*> newData, oldData;																		\
		for( i=0; i< nDefinitionCount; i++ )																			\
		{																												\
			if( fread( &bSaved, sizeof( bSaved ), 1, in_fp ) < 1 )														\
			{																											\
				error_printf( "Failed to read metadata from file." );													\
				return i;																								\
			}																											\
			if( 0 == bSaved )																							\
			{/* null buffer, continue! */																				\
				nSkipped++;																								\
				if( out_DefinitionList )																				\
					gfreeData( &out_DefinitionList[i] ); /*clear output*/												\
				continue; 																								\
			}																											\
																														\
			newData.create();																							\
			if( !newData )																								\
			{																											\
				error_printf("Cannot create new " #structName " instance! Terminating function...");					\
				return i;																								\
			}																											\
																														\
			if( fread( ((char*)&newData->__nRefCount)+structName::__kDataOffset,										\
					structName::__kDataSize, 1, in_fp ) < 1 )															\
			{																											\
				error_printf( "Cannot read " #structName " at position %i:\n"	 										\
					, (int)i																							\
					);																									\
				error_printf("%i " #structName " instances read from file.", i-nSkipped);								\
				return i;																								\
			}																											\
			if( out_DefinitionList )																					\
			{																											\
				oldData = out_DefinitionList[i];																		\
				out_DefinitionList[i] = gacquireData( newData );														\
			}																											\
		}																												\
		debug_printf("%i " #structName " instances read from file, %i skipped.", i-nSkipped, nSkipped);					\
		return i;																										\
	};
	
	#define __GDEFINE_FILESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName )																\
	uint32_t god::fileSerializeData( structName* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp )						\
	{																																		\
		GPROFILE_FUNCTION();																												\
		if( 0 == out_fp || 0 == in_DefinitionList )																							\
		{																																	\
			error_printf("fileSerializeData(): Invalid parameters!\n"																		\
				#structName "**		: 0x%p\n"																								\
				"uint32_t			: %i\n"																									\
				"FILE*				: 0x%p"																									\
				, in_DefinitionList																											\
				, (int) nDefinitionCount																									\
				, out_fp																													\
				);																															\
			return 0;	/* no source to read from */																						\
		}																																	\
																																			\
		uint32_t i, nSkipped=0;																												\
		uint8_t bSaved;																														\
		for( i=0; i< nDefinitionCount; i++ )																								\
		{																																	\
			bSaved = one_if(in_DefinitionList[i]);																							\
			if( fwrite( &bSaved, sizeof( bSaved ), 1, out_fp ) < 1 )																		\
			{																																\
				error_printf("Failed to write metadata to file.");																			\
				return i;																													\
			}																																\
			if( 0 == bSaved )																												\
			{																																\
				nSkipped++;																													\
				continue;																													\
			}																																\
																																			\
			if( fwrite( ((char*)&in_DefinitionList[i]->__nRefCount)+structName::__kDataOffset, structName::__kDataSize, 1, out_fp ) < 1 )	\
			{																																\
				error_printf( "Error saving " #structName " instance at location %p:\n" 													\
					, in_DefinitionList[i]																									\
					);																														\
				printInfoString( in_DefinitionList[i] );																					\
				return i;																													\
			}																																\
		}																																	\
																																			\
		debug_printf("%i " #structName " instances written to file, %i skipped.", i-nSkipped, nSkipped);									\
		return i;																															\
	};
#endif

#define __GDEFINE_FILEMEM_SERIALIZATION_FUNCTIONS_NO_MEMBER_POINTERS( structName )	\
	__GDEFINE_FILESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName );				\
	__GDEFINE_FILEDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName );			\
	__GDEFINE_MEMSERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName );				\
	__GDEFINE_MEMDESERIALIZE_FUNCTION_NO_MEMBER_POINTERS( structName );

#define __GDEFINE_FILEMEM_STREAMING_FUNCTIONS_NO_MEMBER_POINTERS(structName)	\
	__GDEFINE_MEMWRITE_FUNCTION_NO_MEMBER_POINTERS(structName);					\
	__GDEFINE_MEMREAD_FUNCTION_NO_MEMBER_POINTERS(structName);					\
	__GDEFINE_FILEWRITE_FUNCTION_NO_MEMBER_POINTERS(structName);				\
	__GDEFINE_FILEREAD_FUNCTION_NO_MEMBER_POINTERS(structName);
