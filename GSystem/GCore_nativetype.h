#include "GCore_constants.h"
#include "GMemory_core.h"

#ifndef _GCORE_NATIVETYPE_H
#define _GCORE_NATIVETYPE_H

#define GDECLARE_SETVALUE_FUNCTION( TYPE )	\
	TYPE* setValue( TYPE* Targets, TYPE value, uint32_t count );

namespace god
{

	GDECLARE_SETVALUE_FUNCTION( int8_t )
	GDECLARE_SETVALUE_FUNCTION( int16_t )
	GDECLARE_SETVALUE_FUNCTION( int32_t )
	GDECLARE_SETVALUE_FUNCTION( int64_t )
	GDECLARE_SETVALUE_FUNCTION( uint8_t )
	GDECLARE_SETVALUE_FUNCTION( uint16_t )
	GDECLARE_SETVALUE_FUNCTION( uint32_t )
	GDECLARE_SETVALUE_FUNCTION( uint64_t )
	GDECLARE_SETVALUE_FUNCTION( char )
	GDECLARE_SETVALUE_FUNCTION( unsigned char )
	GDECLARE_SETVALUE_FUNCTION( int )
	GDECLARE_SETVALUE_FUNCTION( unsigned int )
	GDECLARE_SETVALUE_FUNCTION( long )
	GDECLARE_SETVALUE_FUNCTION( unsigned long )
	GDECLARE_SETVALUE_FUNCTION( long long )
	GDECLARE_SETVALUE_FUNCTION( unsigned long long )
	GDECLARE_SETVALUE_FUNCTION( float )
	GDECLARE_SETVALUE_FUNCTION( double )

	template<typename _T> static inline _T* setValue( _T* Targets, _T value, uint32_t count, size_t nMaxCount )
	{
		for( uint32_t i=0; i < count && i < nMaxCount; i++	)									
			Targets[i] = value;
		return Targets;
	};


	template<typename _T, size_t _Size> static inline _T* setValue( _T (&Targets)[_Size], _T value, uint32_t count )
	{																						
		for( uint32_t i=0; i < count && i < _Size; i++	)									
			Targets[i] = value;																
		return Targets;																		
	};

	template<typename _T, size_t _Size> static inline _T* setValue( _T (&Targets)[_Size], _T value )
	{																						
		for( uint32_t i=0; i < _Size; i++	)									
			Targets[i] = value;																

		return Targets;																		
	};

	template <typename _T> static inline uint32_t getValueIndex( _T value, const _T* arrayValues, uint32_t nMaxCount )
	{
		uint32_t i;
		for( i=0; i<nMaxCount; i++ )
			if( arrayValues[i] == value )
				return i;
		return -1;
	}

	template <typename _T, size_t _Size> static inline uint32_t getValueIndex( _T value, const _T (&arrayValues)[_Size] )
	{
		return getValueIndex( value, arrayValues, _Size );
	}

	error_t pyTupleToArray( const char* in_tupleString, uint32_t nCharCount, GODS(BUFFER)* out_ArrayBuffer );
	bool pyTupleElementCount( const char* tupleString, uint32_t nCharCount, uint32_t *result );
};

#endif // _GCORE_NATIVETYPE_H
