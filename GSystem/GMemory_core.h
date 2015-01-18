/// This file is part of the Game Object Data System
/// 2011 (c) Pablo Ariel Zorrilla Cepeda
#include "gcore_ptr.h"
#include <stdio.h>

#ifndef _GMEMORY_CORE_H
#define _GMEMORY_CORE_H 

namespace god
{
#pragma pack( push, 1 )

	// This structure will be used to communicate memory buffers to the APIs
	GDECLARE_CORE_STRUCT( BUFFER, Buffer,
		GDATA_TYPE		DataFormat;
		GRESOURCE_USAGE	Usage;
		uint32_t		nElementCount;
		uint32_t		nRowElementCount;
		uint32_t		nSizeInBytes;
		void*			pByteArray;
	);
#pragma pack( pop )

		
	uint32_t gfileCompressBufferDataRLE( GODS(BUFFER)* in_lstBufferData, uint32_t in_nInstanceCount, FILE* out_pFile );
	error_t gcreateBuffer( GDATA_TYPE DataFormat, GRESOURCE_USAGE Usage, uint32_t nMinElementCount, GODS(BUFFER)* out_pBuffer );
	error_t gjoinBuffers( const GODS(BUFFER) in_BufferA, const GODS(BUFFER) in_BufferB, GODS(BUFFER)* out_JoinedBuffer );
	error_t gsplitBuffer( const GODS(BUFFER) in_BufferToSplit, uint32_t nIndex, GODS(BUFFER)* out_SplitLeft, GODS(BUFFER)* out_SplitRight );
	error_t gresizePointerArray( uint32_t nElementSizeInBytes, uint32_t nMaxElementCount, uint32_t *inout_nArraySize, void** inout_pArray, uint32_t nMinElementCount=4 );

};

#endif // _GMEMORY_CORE_H 
