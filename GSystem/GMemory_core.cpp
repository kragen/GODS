
/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GMemory_core.h"
#include "GCore_function_macros.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
//#include <windows.h>
#include <strsafe.h>
#endif

#include "GCore_printf.h"
#include "GDebugger_core.h"

#ifdef ANDROID
#define wsprintf( ... )
#endif

using namespace god;

#ifndef max 
#define max( a, b ) (((a)>(b))?(a):(b))
#endif


__GDEFINE_DUMMY_READWRITE_FUNCTIONS( __GODS(BUFFER) );

void god::gcloneData( GODS(BUFFER)* ppdstData, const __GODS(BUFFER)* psrcData )
{
	GPROFILE_FUNCTION();
	if( 0 == psrcData )
	{
		gfreeData( ppdstData );
		return;
	}
	GOPTR(BUFFER) newData, oldData;
	bool bIsText = psrcData->Usage == GUSAGE_TEXT;
	if( 0 > gcreateBuffer( psrcData->DataFormat, psrcData->Usage, psrcData->nElementCount, &newData ) )
	{
		newData.create();
		if( !newData )
		{
			error_printf( "gcreateData() FAILED! Out of memory?" );
			return;
		};
		newData->DataFormat			= psrcData->DataFormat;
		newData->Usage				= psrcData->Usage;
		newData->nElementCount		= psrcData->nElementCount;
		newData->nRowElementCount	= psrcData->nRowElementCount;
		newData->nSizeInBytes		= GTYPEID_TOTALBYTES(psrcData->DataFormat)*(newData->nElementCount+one_if(bIsText));
		newData->pByteArray			= malloc(newData->nSizeInBytes);
		if( 0 == newData->pByteArray )
		{
			error_printf( "malloc() FAILED! Out of memory?" );
			return;
		};
	};
	if( newData )
	{
		newData->nRowElementCount	= psrcData->nRowElementCount;
		memcpy( newData->pByteArray, psrcData->pByteArray, GTYPEID_TOTALBYTES(newData->DataFormat)*(newData->nElementCount) );
	}
	else
	{
		error_printf( "malloc() FAILED! Out of memory?" );
	}
	oldData = *ppdstData;
	(*ppdstData) = newData.acquire();
}

error_t god::gjoinBuffers( const GODS(BUFFER) in_BufferA, const GODS(BUFFER) in_BufferB, GODS(BUFFER)* out_JoinedBuffer )
{
	GOPTR(BUFFER) joinedBuffer;
	if( 0 == in_BufferB && 0 == in_BufferA )
	{
		gfreeData( out_JoinedBuffer );
		return 0;
	}
	else if( 0 == in_BufferB && in_BufferA )
	{
		joinedBuffer = *out_JoinedBuffer;
		*out_JoinedBuffer = gacquireData( in_BufferA );
		return 0;
	}
	else if( 0 == in_BufferA && in_BufferB )
	{
		joinedBuffer = *out_JoinedBuffer;
		*out_JoinedBuffer = gacquireData( in_BufferB );
		return 0;
	}

	uint32_t newBufferSize = in_BufferA->nElementCount + in_BufferB->nElementCount;					
	if( 0 > gcreateBuffer( in_BufferA->DataFormat, in_BufferA->Usage, newBufferSize, &joinedBuffer ) )
	{
		error_printf("gcreateBuffer() FAILED! Out of memory?");
		return -1;
	};

	uint32_t typeSize = GTYPEID_TOTALBYTES(in_BufferA->DataFormat);
	uint32_t nCurrentBufferSizeInBytes = in_BufferA->nElementCount*typeSize,
		nOtherBufferSizeInBytes		= in_BufferB->nElementCount*typeSize;
	memcpy(joinedBuffer->pByteArray, in_BufferA->pByteArray, nCurrentBufferSizeInBytes );
	memcpy( &((char*)joinedBuffer->pByteArray)[nCurrentBufferSizeInBytes], in_BufferB->pByteArray, nOtherBufferSizeInBytes );
	
	if( joinedBuffer->Usage == GUSAGE_TEXT )
	{
		uint32_t elemEnd = nCurrentBufferSizeInBytes+nOtherBufferSizeInBytes;
		for( uint32_t i=0; i< typeSize; i++ )
			((char*)joinedBuffer->pByteArray)[elemEnd+i]		= 0;
	}

	if( out_JoinedBuffer )
	{
		GOPTR(BUFFER) oldBuffer = *out_JoinedBuffer;
		*out_JoinedBuffer = joinedBuffer.acquire();
	}
	return 0;
};

error_t god::gsplitBuffer( const GODS(BUFFER) in_BufferToSplit, uint32_t nIndex, GODS(BUFFER)* out_SplitLeft, GODS(BUFFER)* out_SplitRight )
{
	if( 0 == in_BufferToSplit )
	{
		error_printf("Cannot split a null buffer!");
		return -1;
	}
	uint32_t leftCount = nIndex,
		rightCount = in_BufferToSplit->nElementCount-nIndex,
		typeSize = GTYPEID_TOTALBYTES(in_BufferToSplit->DataFormat);

	GOPTR(BUFFER) LeftBuffer, RightBuffer;
	bool isText = (LeftBuffer->Usage == GUSAGE_TEXT);
	if( 0 > gcreateBuffer( in_BufferToSplit->DataFormat, in_BufferToSplit->Usage, leftCount, &LeftBuffer ) )
	{
		LeftBuffer.create();
		if( 0 == LeftBuffer )
		{
			error_printf("gcreateData() FAILED! Out of memory?");
			return -1;
		};
		LeftBuffer->DataFormat			= in_BufferToSplit->DataFormat;
		LeftBuffer->nElementCount		= leftCount;
		LeftBuffer->nRowElementCount	= in_BufferToSplit->nRowElementCount/in_BufferToSplit->nElementCount*LeftBuffer->nElementCount;
		LeftBuffer->Usage				= in_BufferToSplit->Usage;
		LeftBuffer->nSizeInBytes		= typeSize*(LeftBuffer->nElementCount+one_if(isText));
		LeftBuffer->pByteArray			= malloc(LeftBuffer->nSizeInBytes);
		if( 0 == LeftBuffer->pByteArray )
		{
			error_printf("malloc() FAILED! Out of memory?");
			return -1;
		}
		RightBuffer.create();
		if( 0 == RightBuffer )
		{
			error_printf("gcreateData() FAILED! Out of memory?");
			return -1;
		};
		RightBuffer->DataFormat			= in_BufferToSplit->DataFormat;
		RightBuffer->nElementCount		= rightCount;
		RightBuffer->nRowElementCount	= in_BufferToSplit->nRowElementCount/in_BufferToSplit->nElementCount*RightBuffer->nElementCount;
		RightBuffer->Usage				= in_BufferToSplit->Usage;
		RightBuffer->nSizeInBytes		= typeSize*(RightBuffer->nElementCount+one_if(isText));
		RightBuffer->pByteArray			= malloc(RightBuffer->nSizeInBytes);
		if( 0 == RightBuffer->pByteArray )
		{
			error_printf("malloc() FAILED! Out of memory?");
			return -1;
		}
	}
	else if( 0 > gcreateBuffer( in_BufferToSplit->DataFormat, in_BufferToSplit->Usage, rightCount, &RightBuffer ) )
	{
		error_printf("gcreateBuffer() FAILED! Out of memory?");
		return -1;
	};

	memcpy( LeftBuffer->pByteArray, in_BufferToSplit->pByteArray, typeSize*leftCount  );
	memcpy( RightBuffer->pByteArray, in_BufferToSplit->pByteArray, typeSize*rightCount );
	if( isText )
	{
		uint32_t elemEnd = typeSize*leftCount;
		for( uint32_t i=0; i< typeSize; i++ )
			((char*)LeftBuffer->pByteArray)[elemEnd+i]		= 0;
		elemEnd = typeSize*rightCount;
		for( uint32_t i=0; i< typeSize; i++ )
			((char*)RightBuffer->pByteArray)[elemEnd+i]	= 0;
	}


	GOPTR(BUFFER) oldBuffer = *out_SplitLeft;
	*out_SplitLeft = LeftBuffer.acquire();

	oldBuffer = *out_SplitRight;
	*out_SplitRight = RightBuffer.acquire();
	return 0;
};


uint32_t god::fileDeserializeData( GODS(BUFFER)* out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )
{
		GPROFILE_FUNCTION();
	if( 0 == in_fp || 0 == out_DefinitionList )
	{
		error_printf("Invalid parameters calling fileLoadBufferData():\n"
			"GODS(BUFFER)*	: 0x%p\n"
			"uint32_t		: %i\n"
			"FILE*			: 0x%p"
			, out_DefinitionList
			, (int) nDefinitionCount
			, in_fp
			);
		return 0;	// no source file to read from
	}

	uint32_t size1, count1, i, nSkipped=0;
	uint8_t	bSaved;

	GOPTR(BUFFER) pNewData, pOldData;
	for( i=0; i< nDefinitionCount; i++ )
	{
		if( fread( &bSaved, sizeof( bSaved ), 1, in_fp ) < 1 )
		{
			error_printf( "Failed to read file trying to load definition #%u.", i );
			return i;
		}
		if( 0 == bSaved )
		{// null buffer, continue!
			nSkipped++;
			gfreeData( &out_DefinitionList[i] ); // clear output
			continue; 
		}

		pNewData.create();
		
		count1	= 1;
		size1	= __GODS(BUFFER)::__kDataSize-sizeof( void* );
		if( fread( pNewData.get_data_address(), size1, 1, in_fp ) < (size_t)1 )
		{
#ifdef _WIN32
			error_printf( "Error loading buffer data at index #%u:\n" 
				"fp->_ptr		: 0x%p\n"			
				"fp-> _cnt		: %i\n"
				"fp->_base		: 0x%p\n"
				"fp-> _flag		: %i\n"
				"fp->_file		: %i\n"
				"fp->_charbuf	: %i\n"
				"fp->_bufsiz	: %i\n"
				"fp->_tmpfname	: 0x%p\n"
				"fp->_tmpfname	: %s"
				, i
				, in_fp->_ptr
				, (int)in_fp-> _cnt
				, in_fp->_base
				, (int)in_fp-> _flag
				, (int)in_fp->_file
				, (int)in_fp->_charbuf
				, (int)in_fp->_bufsiz
				, in_fp->_tmpfname
				, (0 == in_fp->_tmpfname) ? "" : in_fp->_tmpfname
				);
#else
			error_printf( "Error loading buffer data at index #%u:\n", i );
#endif
			return i;
		}		
		if( (count1 = pNewData->nSizeInBytes) != 0 )
		{
			size1	= sizeof( char );
			char* buffer = (char*)malloc(count1*size1);
			if( 0 == buffer )
			{
				error_printf("malloc() FAILED!! Out of memory?");
				return i;
			}
			if( fread( &buffer[0], size1, count1, in_fp ) < (size_t)count1 )
			{
#ifdef _WIN32
				error_printf( "Error (2) loading byte array for buffer data at index #%u:\n" 
					"fp->_ptr		: 0x%p\n"			
					"fp-> _cnt		: %i\n"
					"fp->_base		: 0x%p\n"
					"fp-> _flag		: %i\n"
					"fp->_file		: %i\n"
					"fp->_charbuf	: %i\n"
					"fp->_bufsiz	: %i\n"
					"fp->_tmpfname	: 0x%p\n"
					"fp->_tmpfname	: %s"
					, i
					, in_fp->_ptr
					, (int)in_fp-> _cnt
					, in_fp->_base
					, (int)in_fp-> _flag
					, (int)in_fp->_file
					, (int)in_fp->_charbuf
					, (int)in_fp->_bufsiz
					, in_fp->_tmpfname
					, ( 0 == in_fp->_tmpfname )? "" : in_fp->_tmpfname 
					);
#else
				error_printf( "Error (2) loading byte array for buffer data at index #%u", i );
#endif
				free( buffer );
				return i;
			}
			pNewData->pByteArray = buffer;
		}

		pOldData = out_DefinitionList[i];
		out_DefinitionList[i] = pNewData.acquire();
	}

	debug_printf("%i GODS(BUFFER) instances read from file, %i skipped.", i-nSkipped, nSkipped);		
	return i;
};

uint32_t god::memDeserializeData( GODS(BUFFER)* out_DefinitionList, uint32_t nDefinitionCount, const void* in_pMemoryBuffer )
{
		GPROFILE_FUNCTION();
	if( 0 == in_pMemoryBuffer || 0 == out_DefinitionList )
	{
		error_printf("Invalid parameters calling memLoadBufferData():\n"
			"GODS(BUFFER)*	: 0x%p\n"
			"uint32_t			: %i\n"
			"void*				: 0x%p"
			, out_DefinitionList
			, (int) nDefinitionCount
			, in_pMemoryBuffer
			);
		return 0;	// no source file to read from
	}


	uint32_t size1, count1, i, byteIndex=0, nSkipped=0;
	uint8_t bSaved;
	GOPTR(BUFFER) pNewData, pOldData;

	for( i=0; i< nDefinitionCount; i++ )
	{
		pNewData	= 0;
		bSaved = ((char*)in_pMemoryBuffer)[byteIndex]; // memcpy( &bSaved, &((char*)in_pMemoryBuffer)[byteIndex], sizeof(bSaved) ); // read boolean telling if there's data or not
		byteIndex++; 

		if( 0 == bSaved )
		{// null buffer, continue!
			nSkipped++;
			gfreeData( &out_DefinitionList[i] ); // clear output
			continue; 
		}

		// Create new alloc to store data
		pNewData.create();
		if( 0 == pNewData )
		{
			error_printf("gcreateData() FAILED!! Out of memory? Cannot load byte array for GBUFFER_DATA.");
			debug_printf("%i _GBUFFER_DATA instances read from memory stream, %i skipped.", i-nSkipped, nSkipped);		
			return byteIndex;
		}
		size1	= __GODS(BUFFER)::__kDataSize-sizeof( void* );
		memcpy( pNewData.get_data_address(), &((char*)in_pMemoryBuffer)[byteIndex], size1 );
		byteIndex += size1;

		if( (count1 = pNewData->nSizeInBytes) != 0 )
		{
			size1	= sizeof( char )*count1;
			char* buffer = (char*)malloc(size1);
			if( 0 == buffer )
			{
				error_printf("malloc() FAILED!! Out of memory? Cannot load byte array for GBUFFER_DATA.");
				debug_printf("%i _GBUFFER_DATA instances read from memory stream, %i skipped.", i-nSkipped, nSkipped);		
				return byteIndex;
			}
			memcpy( &buffer[0], &((char*)in_pMemoryBuffer)[byteIndex], size1);
			pNewData->pByteArray = buffer;
			byteIndex += size1;
		}

		pOldData = out_DefinitionList[i];
		out_DefinitionList[i] = pNewData.acquire();
	}


	debug_printf("%i _GBUFFER_DATA instances read from memory stream, %i skipped.", i-nSkipped, nSkipped);		
	return byteIndex;
};

uint32_t god::fileSerializeData( __GODS(BUFFER)* const* in_DefinitionList, uint32_t nDefinitionCount, FILE* out_fp )
{
		GPROFILE_FUNCTION();
	if( 0 == out_fp || 0 == in_DefinitionList )
	{
		error_printf("Invalid parameters calling fileSaveBufferData():\n"
			"_GBUFFER_DATA**	: 0x%p\n"
			"uint32_t			: %i\n"
			"FILE*				: 0x%p"
			, in_DefinitionList
			, (int) nDefinitionCount
			, out_fp
			);
		return 0;	// no source file to read from
	}

	uint32_t size1, count1, i, nSkipped=0;
	uint8_t bSavedBuffer;
	for( i=0; i< nDefinitionCount; i++ )
	{
		bSavedBuffer = one_if(in_DefinitionList[i]);
		size1	= sizeof( bSavedBuffer );
		if( fwrite( &bSavedBuffer, size1, 1, out_fp ) != 1 )
		{
			error_printf("Failed to write metadata to file.");
			debug_printf("%i _GBUFFER_DATA instances read from memory stream, %i skipped.", i-nSkipped, nSkipped);		
			return i;
		}
		if( 0 == bSavedBuffer )
		{
			nSkipped++;
			continue;
		}

		size1	=  __GODS(BUFFER)::__kDataSize-sizeof( void* );
		if( fwrite( ((char*)in_DefinitionList[i])+__GODS(BUFFER)::__kDataOffset, size1, 1, out_fp ) != 1 )
		{
#ifdef _WIN32
			error_printf( "Error saving buffer data at index %u:\n" 
				"fp->_ptr		: 0x%p\n"			
				"fp-> _cnt		: %i\n"
				"fp->_base		: 0x%p\n"
				"fp-> _flag		: %i\n"
				"fp->_file		: %i\n"
				"fp->_charbuf	: %i\n"
				"fp->_bufsiz	: %i\n"
				"fp->_tmpfname	: 0x%p"
				//"fp->_tmpfname	: %s\n"
				, i
				, out_fp->_ptr
				, out_fp-> _cnt
				, out_fp->_base
				, out_fp-> _flag
				, out_fp->_file
				, out_fp->_charbuf
				, out_fp->_bufsiz
				, out_fp->_tmpfname
				//, out_fp->_tmpfname 
				);
#else	
			error_printf( "Error saving buffer data at index %i", (int)i );
#endif
			printInfoString( in_DefinitionList[i] );
			debug_printf("%i _GBUFFER_DATA instances saved to memory stream, %i skipped.", i-nSkipped, nSkipped);		
			return i;
		}
		if( 0 != (count1 = in_DefinitionList[i]->nSizeInBytes) ) // assign size to count1, it is = and not == 
		{
			size1	= sizeof( char );
			if( fwrite( in_DefinitionList[i]->pByteArray, size1, count1, out_fp ) != (size_t)count1 )
			{
#ifdef _WIN32
				error_printf( "Error (2) saving buffer data at index %u:\n" 
					"fp->_ptr		: 0x%p\n"			
					"fp-> _cnt		: %i\n"
					"fp->_base		: 0x%p\n"
					"fp-> _flag		: %i\n"
					"fp->_file		: %i\n"
					"fp->_charbuf	: %i\n"
					"fp->_bufsiz	: %i\n"
					"fp->_tmpfname	: 0x%p\n"
					"fp->_tmpfname	: %s"
					, i
					, out_fp->_ptr
					, out_fp-> _cnt
					, out_fp->_base
					, out_fp-> _flag
					, out_fp->_file
					, out_fp->_charbuf
					, out_fp->_bufsiz
					, out_fp->_tmpfname
					, (0 == out_fp->_tmpfname) ? "" : out_fp->_tmpfname

					);
#else
				error_printf( "Error saving buffer data at index %i", (int)i );
#endif
				printInfoString( in_DefinitionList[i] );
				debug_printf("%i _GBUFFER_DATA instances saved to memory stream, %i skipped.", i-nSkipped, nSkipped);		
				return i;
			}
		}
	}

	debug_printf("%i _GBUFFER_DATA instances saved to file, %i skipped.", i-nSkipped, nSkipped);		
	return i;
};

uint32_t god::memSerializeData( GODS(BUFFER) const* in_DefinitionList, uint32_t nDefinitionCount, void* out_pMemoryBuffer )
{
		GPROFILE_FUNCTION();
	if( 0 == nDefinitionCount || 0 == in_DefinitionList )
	{
		error_printf("Invalid parameters calling memSaveBufferData():\n"
			"GODS(BUFFER)*	: 0x%p\n"
			"uint32_t		: %i\n"
			"void*			: 0x%p"
			, in_DefinitionList
			, (int) nDefinitionCount
			, out_pMemoryBuffer
			);
		return 0;	// no source file to read from
	}

	uint32_t size1, count1, i, nSkipped=0;
	uint8_t bSaved;
	uint32_t byteIndex=0;
	GODS(BUFFER) pDef;
	for( i=0; i< nDefinitionCount; i++ )
	{
		// save a 32 bit boolean telling if the region is gonna be saved to the file (0 for a null region)
		bSaved = one_if(pDef = in_DefinitionList[i]);
		if( out_pMemoryBuffer )
			((char*)out_pMemoryBuffer)[byteIndex] = bSaved; 
		byteIndex++;

		if( 0 == bSaved )
		{
			nSkipped++;
			continue;
		}

		size1	= __GODS(BUFFER)::__kDataSize-sizeof(void*);
		if( out_pMemoryBuffer ) // only write the memory if there is any 
			memcpy( &((char*)out_pMemoryBuffer)[byteIndex], ((char*)in_DefinitionList[i])+__GODS(BUFFER)::__kDataOffset, size1 );
		byteIndex += size1;

		if( 0 != (count1 = in_DefinitionList[i]->nSizeInBytes) )
		{
			size1	= sizeof( char )*count1;
			if( out_pMemoryBuffer )
				memcpy( &((char*)out_pMemoryBuffer)[byteIndex], in_DefinitionList[i]->pByteArray, size1 );
			byteIndex += size1;
		}
	}

	debug_printf("%i GODS(BUFFER) instances saved to memory stream, %i skipped.", i-nSkipped, nSkipped);		
	return byteIndex;
};

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(BUFFER),
	"DataFormat....................: 0x%X\n"
	"Usage.........................: 0x%X\n"
	"nElementCount.................: %u\n"
	"nSizeInBytes..................: %u\n"
	"pByteArray....................: 0x%p"
	, (unsigned int)in_CoreInstance->DataFormat
	, (unsigned int)in_CoreInstance->Usage
	, (unsigned int)in_CoreInstance->nElementCount
	, (unsigned int)in_CoreInstance->nSizeInBytes
	, in_CoreInstance->pByteArray
	);

// the following functions discards comparing the void* address if all other data and void* contents match. 
int32_t god::gcompareData( const __GODS(BUFFER)* BufferA, const __GODS(BUFFER)* BufferB )
{
	GPROFILE_FUNCTION();
	if( BufferA == BufferB )	// If pointers are the same, return equal
		return 0;
	if( !BufferA || !BufferB )	// If only a pointer is null, return not equal
		return -1;
	int _rWhole = memcmp( BufferA, BufferB, sizeof(__GODS(BUFFER)) ),	
		_rNoPtr, _rData;
	if( 0 == _rWhole )		// If the memory contents are the same, return not equal
		return _rWhole;
	char* _startA = ((char*)&BufferA->__nRefCount)+sizeof(refcount_t), // get a pointer to the start of the custom data
		*_startB = ((char*)&BufferB->__nRefCount)+sizeof(refcount_t);	// get a pointer to the start of the custom data
	_rNoPtr = memcmp( _startA, _startB, __GODS(BUFFER)::__kDataSize-sizeof( void* ) );	// compare everything from there except for the void*
	if( _rNoPtr )	// if the data differs, return not equal
		return _rWhole;
	if( 0 != (_rData = memcmp( BufferA->pByteArray, BufferB->pByteArray, BufferA->nElementCount*GTYPEID_TOTALBYTES(BufferA->DataFormat) )) ) // test only for the valid bytes and not the array size
		return _rData;	// if the data contents differ, return equal.
	return 0;
};

// the array is never shrank
int32_t god::gresizePointerArray( uint32_t nElementSizeInBytes, uint32_t nMaxElementCount, uint32_t *inout_nOldElementCount, void** inout_pOldArray, uint32_t nMinElementCount )
{
//	GPROFILE_FUNCTION( 0, "god", "gresizePointerArray" ); enabling this line will cause a call stack overflow because this function is used by the debugger and will call itself recursively
	if( 0 == inout_nOldElementCount || 0 == inout_pOldArray )
	{
		error_printf("gresizePointerArray() FAILED! Invalid parameters:\n" 
			"nElementSize=%u, nMaxElementCount=%u, inout_nOldElementCount=0x%p, inout_pOldArray=0x%p, nMinElementCount=%u. "
			, nElementSizeInBytes
			, nMaxElementCount
			, inout_nOldElementCount
			, inout_pOldArray
			, nMinElementCount
			);
		return -1;
	}

	uint32_t nOldByteCount, nNewByteCount, oldCount = *inout_nOldElementCount;
	nOldByteCount = oldCount*nElementSizeInBytes;
	void* oldList = (*inout_pOldArray), *newList;
	if( 0 == (*inout_pOldArray) )
	{
		nMaxElementCount = max( nMinElementCount, nMaxElementCount );
		nNewByteCount = nElementSizeInBytes*nMaxElementCount;
		newList = malloc( nNewByteCount );
		if( 0 == newList )
		{
			error_printf("malloc()  FAILED! Out of memory?\n" 
				"Cannot initialize array! gresizePointerArray( nElementSizeInBytes=%u, nMaxElementCount=%u, inout_nOldElementCount=0x%p, inout_pOldArray=0x%p, nMinElementCount=%u ) FAILED!"
				, nElementSizeInBytes
				, nMaxElementCount
				, inout_nOldElementCount
				, inout_pOldArray
				, nMinElementCount
				);
			(*inout_nOldElementCount) = 0;
			return -1; // fail! nomem!
		}
		memset( &((char*)newList)[0], 0, nNewByteCount );
		(*inout_pOldArray)	= newList;
		(*inout_nOldElementCount)	= nMaxElementCount;
	}
	else if( nMaxElementCount > oldCount )
	{
		//uint32_t i;
		nMaxElementCount = max( oldCount + (oldCount/2), nMaxElementCount );
		nNewByteCount = nElementSizeInBytes*nMaxElementCount;
		newList = malloc( nNewByteCount );
		if( 0 == newList )
		{ // fail! nomem!
			error_printf("malloc() FAILED! Out of memory?\n" 
				"Cannot resize array! gresizePointerArray( nElementSizeInBytes=%u, nMaxElementCount=%u, inout_nOldElementCount=0x%p, inout_pOldArray=0x%p, nMinElementCount=%u ) FAILED!"
				, nElementSizeInBytes
				, nMaxElementCount
				, inout_nOldElementCount
				, inout_pOldArray
				, nMinElementCount
				);
			return -1;
		}
		memcpy( newList, oldList, nOldByteCount );
		memset( &((char*)newList)[nOldByteCount], 0, nNewByteCount-nOldByteCount );
		(*inout_pOldArray)	= newList;
		(*inout_nOldElementCount)	= nMaxElementCount;
		free( oldList );
	}
	return 0;
};


uint32_t god::gfileCompressBufferDataRLE( GODS(BUFFER)* out_DefinitionList, uint32_t nDefinitionCount, FILE* in_fp )
{
		GPROFILE_FUNCTION();
	if( 0 == in_fp || 0 == out_DefinitionList )
	{
		error_printf("Invalid parameters calling fileLoadBufferData():\n"
			"GODS(BUFFER)*	: 0x%p\n"
			"uint32_t			: %i\n"
			"FILE*				: 0x%p"
			, out_DefinitionList
			, (int) nDefinitionCount
			, in_fp
			);
		return 0;	// no source file to read from
	}

	uint32_t size1, count1, i, nSkipped=0;
	uint8_t	bSaved;

	GOPTR(BUFFER) pNewData, pOldData=0;
	for( i=0; i< nDefinitionCount; i++ )
	{
		if( fread( &bSaved, sizeof( bSaved ), 1, in_fp ) < 1 )
		{
			error_printf( "Failed to read file trying to load definition #%u.", i );
			return i;
		}
		if( 0 == bSaved )
		{// null buffer, continue!
			nSkipped++;
			gfreeData( &out_DefinitionList[i] ); // clear output
			continue; 
		}
		pNewData.create();

		count1	= 1;
		size1	= __GODS(BUFFER)::__kDataSize-sizeof(void*);
		if( fread( ((char*)&pNewData->__nRefCount)+sizeof( refcount_t ), size1, 1, in_fp ) < (size_t)1 )
		{
#ifdef _WIN32
			error_printf( "Error loading buffer data at index #%u:\n" 
				"fp->_ptr		: 0x%p\n"			
				"fp-> _cnt		: %i\n"
				"fp->_base		: 0x%p\n"
				"fp-> _flag		: %i\n"
				"fp->_file		: %i\n"
				"fp->_charbuf	: %i\n"
				"fp->_bufsiz	: %i\n"
				"fp->_tmpfname	: 0x%p\n"
				"fp->_tmpfname	: %s"
				, i
				, in_fp->_ptr
				, (int)in_fp-> _cnt
				, in_fp->_base
				, (int)in_fp-> _flag
				, (int)in_fp->_file
				, (int)in_fp->_charbuf
				, (int)in_fp->_bufsiz
				, in_fp->_tmpfname
				, (0 == in_fp->_tmpfname) ? "" : in_fp->_tmpfname
				);
#else
			error_printf( "Error loading buffer data at index #%u:\n", i );
#endif
			return i;
		}		
		if( 0 != (count1 = pNewData->nSizeInBytes)  )
		{
			size1	= sizeof( char );
			char* buffer = (char*)malloc(count1*size1);
			if( 0 == buffer )
			{
				error_printf("malloc() FAILED!! Out of memory?");
				free( buffer );
				return i;
			}
			if( fread( &buffer[0], size1, count1, in_fp ) < (size_t)count1 )
			{
#ifdef _WIN32
				error_printf( "Error (2) loading byte array for buffer data at index #%u:\n" 
					"fp->_ptr		: 0x%p\n"			
					"fp-> _cnt		: %i\n"
					"fp->_base		: 0x%p\n"
					"fp-> _flag		: %i\n"
					"fp->_file		: %i\n"
					"fp->_charbuf	: %i\n"
					"fp->_bufsiz	: %i\n"
					"fp->_tmpfname	: 0x%p\n"
					"fp->_tmpfname	: %s"
					, i
					, in_fp->_ptr
					, (int)in_fp-> _cnt
					, in_fp->_base
					, (int)in_fp-> _flag
					, (int)in_fp->_file
					, (int)in_fp->_charbuf
					, (int)in_fp->_bufsiz
					, in_fp->_tmpfname
					, ( 0 == in_fp->_tmpfname )? "" : in_fp->_tmpfname 
					);
#else
				error_printf( "Error (2) loading byte array for buffer data at index #%u", i );
#endif
//				pNewData->pDataBuffer = 0;
				free( buffer );
				return i;
			}
			pNewData->pByteArray = buffer;
		}
		// else
		// { this should be 0 already
		// 	pNewData->pDataBuffer = 0;
		// }

		pOldData = out_DefinitionList[i];
		out_DefinitionList[i] = pNewData.acquire();
	}

	debug_printf("%i _GBUFFER_DATA instances read from file, %i skipped.", i-nSkipped, nSkipped);		
	return i;
};
