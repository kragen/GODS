#include "GCore_nativetype.h"
#include "GDebugger_core.h"
#if defined( WIN32 ) || defined( _WIN32 )
//#include <Windows.h>
#endif

#include <stdlib.h>

using namespace god;

#define GDEFINE_SETVALUE_FUNCTION( TYPE )								\
	TYPE* god::setValue( TYPE* Targets, TYPE value, uint32_t count )	\
	{																	\
		if( 0 == Targets )												\
		{																\
			error_printf("Target is NULL!");							\
			return 0;													\
		}																\
		for( uint32_t i=0; i<count; i++	)								\
		{																\
			Targets[i] = value;											\
		}																\
		return Targets;													\
	};




GDEFINE_SETVALUE_FUNCTION( int8_t )
GDEFINE_SETVALUE_FUNCTION( int16_t )
GDEFINE_SETVALUE_FUNCTION( int32_t )
GDEFINE_SETVALUE_FUNCTION( int64_t )
GDEFINE_SETVALUE_FUNCTION( uint8_t )
GDEFINE_SETVALUE_FUNCTION( uint16_t )
GDEFINE_SETVALUE_FUNCTION( uint32_t )
GDEFINE_SETVALUE_FUNCTION( uint64_t )
//GDEFINE_SETVALUE_FUNCTION( signed char )
//GDEFINE_SETVALUE_FUNCTION( unsigned char )
//GDEFINE_SETVALUE_FUNCTION( int )
//GDEFINE_SETVALUE_FUNCTION( unsigned int )
//GDEFINE_SETVALUE_FUNCTION( long )
//GDEFINE_SETVALUE_FUNCTION( unsigned long )
//GDEFINE_SETVALUE_FUNCTION( long long )
//GDEFINE_SETVALUE_FUNCTION( unsigned long long )
GDEFINE_SETVALUE_FUNCTION( float )
GDEFINE_SETVALUE_FUNCTION( double )


error_t	god::pyTupleToArray( const char* in_tupleString, uint32_t nCharCount, GODS(BUFFER)* out_ArrayBuffer )
{
	uint32_t startIndex = 0;
	while( startIndex < nCharCount 
		&& in_tupleString[startIndex]
		&& in_tupleString[startIndex] != '(' 
		) 
		startIndex++;
	if( startIndex == nCharCount || !in_tupleString[startIndex] )
	{
		error_printf("Invalid tuple syntax! Missing opening '('.");
		return -1;
	}
	uint32_t endIndex = startIndex;
	while( endIndex < nCharCount 
		&& in_tupleString[endIndex]
		&& in_tupleString[endIndex] != ')' 
		)
		endIndex++;
	if( endIndex == nCharCount || !in_tupleString[endIndex] )
	{
		error_printf("Invalid tuple syntax! Missing closing ')'.");
		return -1;
	}

	GOPTR(BUFFER) newArrayBuffer;
	uint32_t nPrecalculatedElementCount;
        
	if(!pyTupleElementCount( in_tupleString, nCharCount,
                                 &nPrecalculatedElementCount ))
	{
		error_printf("Invalid tuple size! Malformed input?");
		return -1;
	}
	if( 0 > gcreateBuffer( GDATATYPE_UINT32, GUSAGE_INDEX, nPrecalculatedElementCount, &newArrayBuffer ) )
	{
		error_printf("Failed to create buffer!");
		return -1;
	};

	uint32_t currentIndex = startIndex;
	uint32_t totalElementCount = 0;
	while(currentIndex != endIndex)
	{
		char currentNumber[32] = {0};
		char currentNumberStop = 0;
		while( in_tupleString[currentIndex]
			&& in_tupleString[currentIndex] != ')' 
			&& in_tupleString[currentIndex] != ',' )
		{
			currentNumber[(int)currentNumberStop] = in_tupleString[currentIndex];
			currentIndex++;
			currentNumberStop++;
		}
		if( currentNumberStop )
		{
			((uint32_t*)newArrayBuffer->pByteArray)[totalElementCount] = atoi(currentNumber);
			totalElementCount++;
		}
	};

	if( out_ArrayBuffer )
	{
		GOPTR(BUFFER) oldArrayBuffer = *out_ArrayBuffer;
		*out_ArrayBuffer = gacquireData( newArrayBuffer );
	}

	return 0;
};

bool god::pyTupleElementCount( const char* in_tupleString, uint32_t nCharCount,
                               uint32_t *result )
{
	uint32_t startIndex = 0;
	while( startIndex < nCharCount 
		&& in_tupleString[startIndex]
		&& '(' != in_tupleString[startIndex] 
		) 
		startIndex++;
	if( startIndex == nCharCount || !in_tupleString[startIndex] )
	{
		error_printf("Invalid tuple syntax! Missing opening '('.");
		return 0;
	}
	uint32_t endIndex = startIndex;
	while( endIndex < nCharCount 
		&& in_tupleString[endIndex]
		&& ')' != in_tupleString[endIndex] 
		)
		endIndex++;
	if( endIndex == nCharCount || !in_tupleString[endIndex] )
	{
		error_printf("Invalid tuple syntax! Missing closing ')'.");
		return 0;
	}

	uint32_t currentIndex = startIndex+1;
	uint32_t totalElementCount = 0;
	while(currentIndex != endIndex)
	{
		char currentNumberStop = 0;
		while( in_tupleString[currentIndex]
			&& in_tupleString[currentIndex] != ')' 
			&& in_tupleString[currentIndex] != ',' )
		{
			currentIndex++;
			currentNumberStop++;
		}
		if( currentNumberStop )
			totalElementCount++;
	};
	*result = totalElementCount;
        return 1;
};
