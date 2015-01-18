/// GCore_constants.h
///
/// Declares data types and enumerations to be used by the engine core.
///
/// Configuration definitions:
/// GDISABLE_STDINT: When defined, disables load of stdint.h, then typedefs the standard types.
///
/// This file is part of the Game Oriented Development Software Kit
/// 2013 (c) Pablo Ariel Zorrilla Cepeda
///
#ifndef _GCORE_CONSTANTS_H
#define _GCORE_CONSTANTS_H

#if defined( GDISABLE_STDINT ) 
#	if !defined( WIN32 )
#		include <stdint.h>
#	else
	typedef				__int8				int8_t; 
	typedef signed		__int8				sint8_t;
	typedef unsigned	__int8				uint8_t;
	typedef				__int16				int16_t;
	typedef signed		__int16				sint16_t;
	typedef unsigned	__int16				uint16_t;
	typedef				__int32				int32_t;
	typedef signed		__int32				sint32_t;
	typedef unsigned	__int32				uint32_t;
	typedef				__int64				int64_t;
	typedef signed		__int64				sint64_t;
	typedef unsigned	__int64				uint64_t;
	typedef				__int128			int128_t;
	typedef signed		__int128			sint128_t;
	typedef unsigned	__int128			uint128_t;
#	endif
#else
#include <stdint.h>
#endif


#ifndef NULL
#define NULL 0
#endif

namespace god
{
#ifdef _WIN64
	typedef uint64_t refcount_t;
#else
#ifdef _WIN32
	typedef volatile long refcount_t;
#else
	typedef uint32_t refcount_t;
#endif
#endif
	typedef float		float32_t;
	typedef double		float64_t;
	typedef int32_t		error_t;
	typedef const char*	cue_t;	// Special thanks to Kragen Javier Sitaker for this cue_t idea.
	typedef uint64_t	uid_t;
	typedef int32_t		id_t;

	//struct int32_2_t
	//{
	//	union
	//	{
	//		struct { int32_t a, b; };
	//		struct { int32_t x, y; };
	//	};
	//};
	//
	//struct int32_3_t
	//{
	//	union
	//	{
	//		struct { int32_t a, b, c; };
	//		struct { int32_t x, y, z; };
	//	};
	//};
	//
	//struct int32_4_t
	//{
	//	union
	//	{
	//		struct { int32_t a, b, c, d; };
	//		struct { int32_t x, y, z, w; };
	//	};
	//};

#define one_if(your_bool) ((your_bool)?1:0)

	// Data types (may or may not be -completely?- supported by the hardware)
	// 
	// This was made to represent data types for a variety of hardware and OSs.
	// This system would allow us to represent type sizes up to 16 elements of 256 bits (512 bytes) 
	// as signed/unsigned integer, signed/unsigned float, signed/unsigned normalized integer,
	// signed/unsigned normalized float, all of them in both little or big endianness
	//
	// Bits are arranged such that:
	// first bit: 0 = Signed/ 1 = Unsigned
	// second bit: 0 = Unorm/ 1 = Norm
	// third bit: 0 = int/ 1 = float
	// fourth bit: 0 = little endian / 1 = big endian
	// fifth to eighth bits: element count-1
	// last eight bits: element size-1
	// 
	typedef uint16_t GDATA_TYPE;

#define GTYPEID_MAKE( IsSigned, IsNorm, IsFloat, IsBigEndian, ElementCount, SizeInBits )	\
  (	((IsSigned) ? 0 : 0x8000) | ((IsNorm) ? 0x4000 : 0 ) | ((IsFloat) ? 0x2000 : 0 )		\
  | ((IsBigEndian) ? 0x1000 : 0) | (((ElementCount-1)&0xF) <<8) | ((SizeInBits-1)&0xFF) )

#define GTYPEID_POINTER_MAKE( TypeName )											\
  (	((false) ? 0 : 0x8000) | ((false) ? 0x4000 : 0 ) | ((false) ? 0x2000 : 0 )		\
  | ((false) ? 0x1000 : 0) | (((1-1)&0xF) <<8) | (((sizeof(TypeName*)*8)-1)&0xFF) )	

#define GTYPEID_DATA_MAKE( TypeName )												\
  (	((false) ? 0 : 0x8000) | ((false) ? 0x4000 : 0 ) | ((false) ? 0x2000 : 0 )		\
  | ((false) ? 0x1000 : 0) | (((1-1)&0xF) <<8) | (((sizeof(TypeName)*8)-1)&0xFF) )

// returns true if signed
#define	GTYPEID_ISSIGNED( DataTypeID ) \
	(0 == ( 0x8000 & DataTypeID ))

// returns true if the value represents a range between 0 and 1
#define	GTYPEID_ISNORMALIZED( DataTypeID ) \
	(0 != ( 0x4000 & DataTypeID ))

// returns true if the element bits represent a floating point number
#define	GTYPEID_ISFLOAT( DataTypeID ) \
	(0 != ( 0x2000 & DataTypeID ))

// returns true if the element bits represent a floating point number
#define	GTYPEID_ISBIGENDIAN( DataTypeID ) \
	(0 != ( 0x1000 & DataTypeID ))

// returns the element count (up to 16 elements)
#define	GTYPEID_ELEMENTCOUNT( DataTypeID ) \
	( (( 0x0F00 & DataTypeID ) >> 8) +1 )

// returns the element size in bits (up to 256 bits)
#define	GTYPEID_ELEMENTSIZE( DataTypeID ) \
	(( 0x00FF & DataTypeID )+1)

// Return the byte count of the element type (ElementSize/8)
// Note: this funcion rounds any leading bits to next byte
#define	GTYPEID_ELEMENTBYTES( DataTypeID )				\
( (GTYPEID_ELEMENTSIZE( DataTypeID )/8)					\
	+ one_if(GTYPEID_ELEMENTSIZE( DataTypeID ) % 8)	)

// Return the byte count of the data type (ElementCount*(ElementSize/8))
#define	GTYPEID_TOTALBYTES( DataTypeID )									\
	(GTYPEID_ELEMENTBYTES( DataTypeID )*GTYPEID_ELEMENTCOUNT( DataTypeID ))

	// Type ID can now be built with the 
	// GTYPEID_MAKE macro, so they would be "helper" constants
	static const GDATA_TYPE	GDATATYPE_UNKNOWN			= GTYPEID_MAKE(true	,true	,true	,false	,0	, 0);
	// NOTICE! Signed types can't be represented by a single bit! Should we use these for something more? Such as representing NAN and infinite?
	// 
	// - You can find GDATATYPE_UINT1 later on this list for representing a single-bit integral value
	//static const GDATA_TYPE	GDATATYPE_INT1				= GTYPEID_MAKE(true	,false	,false	,false	,1	, 1); 
	//static const GDATA_TYPE	GDATATYPE_INT1_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	, 1);
	//---------------------------------------------------------------Signed - Norm -Float-BigEndian-Elements-Size	
	static const GDATA_TYPE	GDATATYPE_INT2				= GTYPEID_MAKE(true	,false	,false	,false	,1	, 2);	// this uncommon type stores a range of integrals from 1 to -1  
	static const GDATA_TYPE	GDATATYPE_INT2_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	, 2);	// 2 dimension points/vectors are defined for each defined type
	static const GDATA_TYPE	GDATATYPE_INT2_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	, 2);	// 3 dimension points/vectors are defined for each defined type
	static const GDATA_TYPE	GDATATYPE_INT2_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	, 2);	// 4 dimension points/vectors are defined for each defined type
	static const GDATA_TYPE	GDATATYPE_INT2_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	, 2);	// 3x3 matrix types are defined for each defined type
	static const GDATA_TYPE	GDATATYPE_INT2_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	, 2);	// 4x4 matrix types are defined for each defined type
	static const GDATA_TYPE	GDATATYPE_INT2_NORM			= GTYPEID_MAKE(true	,true	,false	,false	,1	, 2);	//	
	static const GDATA_TYPE	GDATATYPE_INT2_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	, 2);	// Normalized types are defined for all defined types
	static const GDATA_TYPE	GDATATYPE_INT2_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	, 2);	//
	static const GDATA_TYPE	GDATATYPE_INT2_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	, 2);	//
	static const GDATA_TYPE	GDATATYPE_INT2_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	, 2);	//
	static const GDATA_TYPE	GDATATYPE_INT2_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	, 2);	//
	static const GDATA_TYPE	GDATATYPE_INT4				= GTYPEID_MAKE(true	,false	,false	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM			= GTYPEID_MAKE(true	,true	,false	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_INT4_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_INT8				= GTYPEID_MAKE(true	,false	,false	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM			= GTYPEID_MAKE(true	,true	,false	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_INT8_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_INT16				= GTYPEID_MAKE(true	,false	,false	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM		= GTYPEID_MAKE(true	,true	,false	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_INT16_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_INT32				= GTYPEID_MAKE(true	,false	,false	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM		= GTYPEID_MAKE(true	,true	,false	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_INT32_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_INT64				= GTYPEID_MAKE(true	,false	,false	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_2			= GTYPEID_MAKE(true	,false	,false	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM		= GTYPEID_MAKE(true	,true	,false	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM_2		= GTYPEID_MAKE(true	,true	,false	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM_3		= GTYPEID_MAKE(true	,true	,false	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM_4		= GTYPEID_MAKE(true	,true	,false	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM_9		= GTYPEID_MAKE(true	,true	,false	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_INT64_NORM_16		= GTYPEID_MAKE(true	,true	,false	,false	,16	,64);
	static const GDATA_TYPE	GDATATYPE_UINT1				= GTYPEID_MAKE(false,false	,false	,false	,1	, 1);	// This value represents a single bit boolean type
	static const GDATA_TYPE	GDATATYPE_UINT1_2			= GTYPEID_MAKE(false,false	,false	,false	,2	, 1);
	static const GDATA_TYPE	GDATATYPE_UINT1_3			= GTYPEID_MAKE(false,false	,false	,false	,3	, 1);
	static const GDATA_TYPE	GDATATYPE_UINT1_4			= GTYPEID_MAKE(false,false	,false	,false	,4	, 1);
	static const GDATA_TYPE	GDATATYPE_UINT1_9			= GTYPEID_MAKE(false,false	,false	,false	,9	, 1);
	static const GDATA_TYPE	GDATATYPE_UINT1_16			= GTYPEID_MAKE(false,false	,false	,false	,16	, 1);	// 4x4 boolean matrix or array of 16 boolean
	static const GDATA_TYPE	GDATATYPE_UINT2				= GTYPEID_MAKE(false,false	,false	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_2			= GTYPEID_MAKE(false,false	,false	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_3			= GTYPEID_MAKE(false,false	,false	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_4			= GTYPEID_MAKE(false,false	,false	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_9			= GTYPEID_MAKE(false,false	,false	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_16			= GTYPEID_MAKE(false,false	,false	,false	,16	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT2_NORM_16		= GTYPEID_MAKE(false,true	,false	,false	,16	, 2);
	static const GDATA_TYPE	GDATATYPE_UINT4				= GTYPEID_MAKE(false,false	,false	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_2			= GTYPEID_MAKE(false,false	,false	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_3			= GTYPEID_MAKE(false,false	,false	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_4			= GTYPEID_MAKE(false,false	,false	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_9			= GTYPEID_MAKE(false,false	,false	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_16			= GTYPEID_MAKE(false,false	,false	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT4_NORM_16		= GTYPEID_MAKE(false,true	,false	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_UINT8				= GTYPEID_MAKE(false,false	,false	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_2			= GTYPEID_MAKE(false,false	,false	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_3			= GTYPEID_MAKE(false,false	,false	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_4			= GTYPEID_MAKE(false,false	,false	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_9			= GTYPEID_MAKE(false,false	,false	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_16			= GTYPEID_MAKE(false,false	,false	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT8_NORM_16		= GTYPEID_MAKE(false,true	,false	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_UINT16			= GTYPEID_MAKE(false,false	,false	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_2			= GTYPEID_MAKE(false,false	,false	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_3			= GTYPEID_MAKE(false,false	,false	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_4			= GTYPEID_MAKE(false,false	,false	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_9			= GTYPEID_MAKE(false,false	,false	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_16			= GTYPEID_MAKE(false,false	,false	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_UINT16_NORM_16	= GTYPEID_MAKE(false,true	,false	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_UINT32			= GTYPEID_MAKE(false,false	,false	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_2			= GTYPEID_MAKE(false,false	,false	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_3			= GTYPEID_MAKE(false,false	,false	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_4			= GTYPEID_MAKE(false,false	,false	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_9			= GTYPEID_MAKE(false,false	,false	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_16			= GTYPEID_MAKE(false,false	,false	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_UINT32_NORM_16	= GTYPEID_MAKE(false,true	,false	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_UINT64			= GTYPEID_MAKE(false,false	,false	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_2			= GTYPEID_MAKE(false,false	,false	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_3			= GTYPEID_MAKE(false,false	,false	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_4			= GTYPEID_MAKE(false,false	,false	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_9			= GTYPEID_MAKE(false,false	,false	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_16			= GTYPEID_MAKE(false,false	,false	,false	,16	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM		= GTYPEID_MAKE(false,true	,false	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM_2		= GTYPEID_MAKE(false,true	,false	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM_3		= GTYPEID_MAKE(false,true	,false	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM_4		= GTYPEID_MAKE(false,true	,false	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM_9		= GTYPEID_MAKE(false,true	,false	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_UINT64_NORM_16	= GTYPEID_MAKE(false,true	,false	,false	,16	,64);
	/* these data types are nonsense! Should we use these for something more? Such as representing NAN and infinite?
	static const GDATA_TYPE	GDATATYPE_FLOAT2			= GTYPEID_MAKE(true	,false	,true	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_16			= GTYPEID_MAKE(true	,false	,true	,false	,16	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM_2		= GTYPEID_MAKE(true	,true	,true	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM_3		= GTYPEID_MAKE(true	,true	,true	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM_4		= GTYPEID_MAKE(true	,true	,true	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM_9		= GTYPEID_MAKE(true	,true	,true	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_FLOAT2_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	, 2);
	*/
	//---------------------------------------------------------------Signed - Norm -Float-BigEndian-Elements-Size	
	static const GDATA_TYPE	GDATATYPE_FLOAT4			= GTYPEID_MAKE(true	,false	,true	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_16			= GTYPEID_MAKE(true	,false	,true	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM_2		= GTYPEID_MAKE(true	,true	,true	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM_3		= GTYPEID_MAKE(true	,true	,true	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM_4		= GTYPEID_MAKE(true	,true	,true	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM_9		= GTYPEID_MAKE(true	,true	,true	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT4_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_FLOAT8			= GTYPEID_MAKE(true	,false	,true	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_16			= GTYPEID_MAKE(true	,false	,true	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM_2		= GTYPEID_MAKE(true	,true	,true	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM_3		= GTYPEID_MAKE(true	,true	,true	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM_4		= GTYPEID_MAKE(true	,true	,true	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM_9		= GTYPEID_MAKE(true	,true	,true	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT8_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_FLOAT16			= GTYPEID_MAKE(true	,false	,true	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_16		= GTYPEID_MAKE(true	,false	,true	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM_2	= GTYPEID_MAKE(true	,true	,true	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM_3	= GTYPEID_MAKE(true	,true	,true	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM_4	= GTYPEID_MAKE(true	,true	,true	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM_9	= GTYPEID_MAKE(true	,true	,true	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT16_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_FLOAT32			= GTYPEID_MAKE(true	,false	,true	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_16		= GTYPEID_MAKE(true	,false	,true	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM_2	= GTYPEID_MAKE(true	,true	,true	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM_3	= GTYPEID_MAKE(true	,true	,true	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM_4	= GTYPEID_MAKE(true	,true	,true	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM_9	= GTYPEID_MAKE(true	,true	,true	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT32_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_FLOAT64			= GTYPEID_MAKE(true	,false	,true	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_2			= GTYPEID_MAKE(true	,false	,true	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_3			= GTYPEID_MAKE(true	,false	,true	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_4			= GTYPEID_MAKE(true	,false	,true	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_9			= GTYPEID_MAKE(true	,false	,true	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_16		= GTYPEID_MAKE(true	,false	,true	,false	,16	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM		= GTYPEID_MAKE(true	,true	,true	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM_2	= GTYPEID_MAKE(true	,true	,true	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM_3	= GTYPEID_MAKE(true	,true	,true	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM_4	= GTYPEID_MAKE(true	,true	,true	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM_9	= GTYPEID_MAKE(true	,true	,true	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_FLOAT64_NORM_16	= GTYPEID_MAKE(true	,true	,true	,false	,16	,64);
	/* these data types are nonsense! Should we use these for something more? Such as representing NAN and infinite?
	static const GDATA_TYPE	GDATATYPE_UFLOAT2			= GTYPEID_MAKE(false,false	,true	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_2			= GTYPEID_MAKE(false,false	,true	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_3			= GTYPEID_MAKE(false,false	,true	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_4			= GTYPEID_MAKE(false,false	,true	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_9			= GTYPEID_MAKE(false,false	,true	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_16		= GTYPEID_MAKE(false,false	,true	,false	,16	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	, 2);
	static const GDATA_TYPE	GDATATYPE_UFLOAT2_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	, 2);
	*/
	static const GDATA_TYPE	GDATATYPE_UFLOAT4			= GTYPEID_MAKE(false,false	,true	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_2			= GTYPEID_MAKE(false,false	,true	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_3			= GTYPEID_MAKE(false,false	,true	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_4			= GTYPEID_MAKE(false,false	,true	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_9			= GTYPEID_MAKE(false,false	,true	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_16		= GTYPEID_MAKE(false,false	,true	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT4_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	, 4);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8			= GTYPEID_MAKE(false,false	,true	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_2			= GTYPEID_MAKE(false,false	,true	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_3			= GTYPEID_MAKE(false,false	,true	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_4			= GTYPEID_MAKE(false,false	,true	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_9			= GTYPEID_MAKE(false,false	,true	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_16		= GTYPEID_MAKE(false,false	,true	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT8_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	, 8);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16			= GTYPEID_MAKE(false,false	,true	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_2		= GTYPEID_MAKE(false,false	,true	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_3		= GTYPEID_MAKE(false,false	,true	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_4		= GTYPEID_MAKE(false,false	,true	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_9		= GTYPEID_MAKE(false,false	,true	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_16		= GTYPEID_MAKE(false,false	,true	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT16_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	,16);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32			= GTYPEID_MAKE(false,false	,true	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_2		= GTYPEID_MAKE(false,false	,true	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_3		= GTYPEID_MAKE(false,false	,true	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_4		= GTYPEID_MAKE(false,false	,true	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_9		= GTYPEID_MAKE(false,false	,true	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_16		= GTYPEID_MAKE(false,false	,true	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT32_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	,32);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64			= GTYPEID_MAKE(false,false	,true	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_2		= GTYPEID_MAKE(false,false	,true	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_3		= GTYPEID_MAKE(false,false	,true	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_4		= GTYPEID_MAKE(false,false	,true	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_9		= GTYPEID_MAKE(false,false	,true	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_16		= GTYPEID_MAKE(false,false	,true	,false	,16	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM		= GTYPEID_MAKE(false,true	,true	,false	,1	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM_2	= GTYPEID_MAKE(false,true	,true	,false	,2	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM_3	= GTYPEID_MAKE(false,true	,true	,false	,3	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM_4	= GTYPEID_MAKE(false,true	,true	,false	,4	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM_9	= GTYPEID_MAKE(false,true	,true	,false	,9	,64);
	static const GDATA_TYPE	GDATATYPE_UFLOAT64_NORM_16	= GTYPEID_MAKE(false,true	,true	,false	,16	,64);
	// NOT NUMBERS BUT ERROR VALUES:
	static const GDATA_TYPE		GDATATYPE_NAN			= GTYPEID_MAKE(true	,false	,false	,false	,1	, 1); 	//GDATATYPE_INT1		
	static const GDATA_TYPE		GDATATYPE_INFINITE		= GTYPEID_MAKE(true	,false	,false	,false	,2	, 1); 	//GDATATYPE_INT1_2	
	//static const GDATA_TYPE	GDATATYPE_INT1_3			= GTYPEID_MAKE(true	,false	,false	,false	,3	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_4			= GTYPEID_MAKE(true	,false	,false	,false	,4	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_9			= GTYPEID_MAKE(true	,false	,false	,false	,9	, 1);
	//static const GDATA_TYPE	GDATATYPE_INT1_16			= GTYPEID_MAKE(true	,false	,false	,false	,16	, 1);

	//----------------------------------------------------------------------------------
	// these are a bit field
	typedef uint8_t GRESOURCE_MODE;
	// WARNING! Read function usage documentation before using GRESOURCE_DEFAULT
	static const GRESOURCE_MODE	GRESMODE_DEFAULT			= 0; 
	// Define buffer modes
	static const GRESOURCE_MODE	GRESMODE_READ				= 0x10;	// can read from the buffer
	static const GRESOURCE_MODE	GRESMODE_WRITE				= 0x20;	// can write to the buffer
	static const GRESOURCE_MODE	GRESMODE_APPEND				= 0x40;	// can write to the buffer
	static const GRESOURCE_MODE	GRESMODE_HOST				= 0x01;	// can write to the buffer
	static const GRESOURCE_MODE	GRESMODE_CLIENT				= 0x02;	// can write to the buffer
#define GRESMODE_DEVICE				GRESMODE_CLIENT	// The device can read from the buffer

	static const GRESOURCE_MODE	GRESMODE_HOST_READ			= GRESMODE_READ			| GRESMODE_HOST;	// CPU can read from the buffer
	static const GRESOURCE_MODE	GRESMODE_HOST_WRITE			= GRESMODE_WRITE		| GRESMODE_HOST;	// CPU can write to the buffer
	static const GRESOURCE_MODE	GRESMODE_HOST_APPEND		= GRESMODE_APPEND		| GRESMODE_HOST;	// CPU can append to the buffer
	static const GRESOURCE_MODE	GRESMODE_CLIENT_READ		= GRESMODE_READ			| GRESMODE_DEVICE;	// The device can read from the buffer
	static const GRESOURCE_MODE	GRESMODE_CLIENT_WRITE		= GRESMODE_WRITE		| GRESMODE_DEVICE;	// The device can write to the buffer
	static const GRESOURCE_MODE	GRESMODE_CLIENT_APPEND		= GRESMODE_APPEND		| GRESMODE_DEVICE;	// CPU can append to the buffer
	static const GRESOURCE_MODE	GRESMODE_HOST_READWRITE		= GRESMODE_HOST_READ	| GRESMODE_HOST_WRITE;		// CPU can read from and write to the buffer
	static const GRESOURCE_MODE	GRESMODE_CLIENT_READWRITE	= GRESMODE_CLIENT_READ	| GRESMODE_CLIENT_WRITE;	// The device can read from and write to the buffer

#define GRESMODE_DEVICE_READ		GRESMODE_CLIENT_READ		// The device can read from the buffer
#define GRESMODE_DEVICE_READWRITE	GRESMODE_CLIENT_READWRITE	// The device can read from the buffer
#define GRESMODE_DEVICE_WRITE		GRESMODE_CLIENT_WRITE		// The device can write to the buffer
#define GRESMODE_DEVICE_APPEND		GRESMODE_CLIENT_APPEND		// CPU can append to the buffer

	// Define queue modes
	static const GRESOURCE_MODE	GRESMODE_OUT_OF_ORDER		= 0x10; // The device writes the buffer asynchronically
	static const GRESOURCE_MODE	GRESMODE_IN_ORDER			= 0x20; // The Device writes the buffer synchronically

	//----------------------------------------------------------------------------------
	// This is to let the api know how to deal with the data
	typedef uint32_t GRESOURCE_USAGE;
	
	// 
	typedef uint32_t GRESOURCE_USAGE_SUBSYSTEM;
	static const GRESOURCE_USAGE_SUBSYSTEM		GRESOURCE_USAGE_SUBSYS_NONE		= ((GRESOURCE_USAGE_SUBSYSTEM)0);
	static const GRESOURCE_USAGE_SUBSYSTEM		GRESOURCE_USAGE_SUBSYS_CORE		= ((GRESOURCE_USAGE_SUBSYSTEM)1)<<28;
	static const GRESOURCE_USAGE_SUBSYSTEM		GRESOURCE_USAGE_SUBSYS_VIDEO	= ((GRESOURCE_USAGE_SUBSYSTEM)1)<<29;
	static const GRESOURCE_USAGE_SUBSYSTEM		GRESOURCE_USAGE_SUBSYS_AUDIO	= ((GRESOURCE_USAGE_SUBSYSTEM)1)<<30;
	static const GRESOURCE_USAGE_SUBSYSTEM		GRESOURCE_USAGE_SUBSYS_PHYSICS	= ((GRESOURCE_USAGE_SUBSYSTEM)1)<<31;

	// Base usages
	static const GRESOURCE_USAGE	GUSAGE_BEGIN			= ( 1 << 0 );
	static const GRESOURCE_USAGE	GUSAGE_END				= ( 1 << 1 );

	// 
	static const GRESOURCE_USAGE	GUSAGE_UNKNOWN			= 0x000000000;
	static const GRESOURCE_USAGE	GUSAGE_BINARY			= GRESOURCE_USAGE_SUBSYS_CORE | ( 1 << 0 );		// 0x10000001;
	static const GRESOURCE_USAGE	GUSAGE_TEXT				= GRESOURCE_USAGE_SUBSYS_CORE | ( 1 << 1 );		// 0x10000002;
	static const GRESOURCE_USAGE	GUSAGE_POINTER			= GRESOURCE_USAGE_SUBSYS_CORE | ( 1 << 2 );		// 0x10000004;

	// Video usages
	static const GRESOURCE_USAGE	GUSAGE_INDEX			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 0  );	// 0x20000001;
	static const GRESOURCE_USAGE	GUSAGE_VERTEX			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 1  );	// 0x20000002;
	static const GRESOURCE_USAGE	GUSAGE_NORMAL			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 2  );	// 0x20000004;
	static const GRESOURCE_USAGE	GUSAGE_TEXCOORD			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 3  );	// 0x20000008;
	static const GRESOURCE_USAGE	GUSAGE_VERTEXCOLOR		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 4  );	// 0x20000010;
	static const GRESOURCE_USAGE	GUSAGE_TANGENT			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 5  );	// 0x20000020;
	static const GRESOURCE_USAGE	GUSAGE_BITANGENT		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 6  );	// 0x20000040;
	static const GRESOURCE_USAGE	GUSAGE_BLENDWEIGHT		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 7  );	// 0x20000080;
	static const GRESOURCE_USAGE	GUSAGE_BLENDINDICES		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 8  );	// 0x20000100;
	static const GRESOURCE_USAGE	GUSAGE_TEXEL			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 9  );	// 0x20000200;
	static const GRESOURCE_USAGE	GUSAGE_VOXEL			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 10 );	// 0x20000400;
	static const GRESOURCE_USAGE	GUSAGE_ALPHA			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 11 );	// 0x20000800;
	static const GRESOURCE_USAGE	GUSAGE_HEIGHT			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 12 );	// 0x20001000;
	static const GRESOURCE_USAGE	GUSAGE_TRANSFORM		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 13 );	// 0x20002000;
	static const GRESOURCE_USAGE	GUSAGE_DEPTH			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 14 );	// 0x20004000;
	static const GRESOURCE_USAGE	GUSAGE_STENCIL			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 15 );	// 0x20008000;
	static const GRESOURCE_USAGE	GUSAGE_DEPTHSTENCIL		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 16 );	// 0x20010000;
	static const GRESOURCE_USAGE	GUSAGE_RENDERTARGET		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 17 );	// 0x20020000;	
	static const GRESOURCE_USAGE	GUSAGE_VERTEXSHADER		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 18 );	// 0x20040000;
	static const GRESOURCE_USAGE	GUSAGE_GEOMETRYSHADER	= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 19 );	// 0x20080000;	
	static const GRESOURCE_USAGE	GUSAGE_PIXELSHADER		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 10 );	// 0x20100000;
	static const GRESOURCE_USAGE	GUSAGE_DOMAINSHADER		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 11 );	// 0x20200000;	
	static const GRESOURCE_USAGE	GUSAGE_HULLSHADER		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 22 );	// 0x20400000;
	static const GRESOURCE_USAGE	GUSAGE_COMPUTESHADER	= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 23 );	// 0x20800000;
	static const GRESOURCE_USAGE	GUSAGE_STREAMOUTPUT		= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 24 );	// 0x21000000;
	static const GRESOURCE_USAGE	GUSAGE_UNORDEREDACCESS	= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 25 );	// 0x22000000;
	static const GRESOURCE_USAGE	GUSAGE_DECODER			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 26 );	// 0x24000000;
	static const GRESOURCE_USAGE	GUSAGE_ENCODER			= GRESOURCE_USAGE_SUBSYS_VIDEO | ( 1 << 27 );	// 0x28000000;

	// Audio usages 
	static const GRESOURCE_USAGE	GUSAGE_WAVE				= GRESOURCE_USAGE_SUBSYS_AUDIO | ( 1 << 0 );	// 0x40000001;

	// Physics usages 
	static const GRESOURCE_USAGE	GUSAGE_ANGULARTENSOR	= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 0 );	// 0x80000001;
	static const GRESOURCE_USAGE	GUSAGE_POSITION			= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 1 );	// 0x80000002;
	static const GRESOURCE_USAGE	GUSAGE_VELOCITY			= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 2 );	// 0x80000004;
	static const GRESOURCE_USAGE	GUSAGE_ROTATION			= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 3 );	// 0x80000008;
	static const GRESOURCE_USAGE	GUSAGE_ACCELERATION		= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 4 );	// 0x80000010;
	static const GRESOURCE_USAGE	GUSAGE_SPEED			= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 5 );	// 0x80000020;
	static const GRESOURCE_USAGE	GUSAGE_ORIENTATION		= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 6 );	// 0x80000040;
	static const GRESOURCE_USAGE	GUSAGE_FORCE			= GRESOURCE_USAGE_SUBSYS_PHYSICS | ( 1 << 7 );	// 0x80000080;

	// Operators
	static const GRESOURCE_USAGE	GUSAGE_SYMBOL			= ( 1 << 0 );	// 0x1;

	// Operators
	typedef uint8_t GLOGICAL_OPERATOR;
	static const GLOGICAL_OPERATOR	GOPERATOR_EQUAL			= 0x0;
	static const GLOGICAL_OPERATOR	GOPERATOR_NEVER			= 0x1;
	static const GLOGICAL_OPERATOR	GOPERATOR_ALWAYS		= 0x2;
	static const GLOGICAL_OPERATOR	GOPERATOR_LESS			= 0x3;
	static const GLOGICAL_OPERATOR	GOPERATOR_GREATER		= 0x4;
	static const GLOGICAL_OPERATOR	GOPERATOR_LESSEQUAL		= 0x5;
	static const GLOGICAL_OPERATOR	GOPERATOR_GREATEREQUAL	= 0x6;
	static const GLOGICAL_OPERATOR	GOPERATOR_OR			= 0x7;
	static const GLOGICAL_OPERATOR	GOPERATOR_AND			= 0x8;
	static const GLOGICAL_OPERATOR	GOPERATOR_XOR			= 0x9;
	static const GLOGICAL_OPERATOR	GOPERATOR_BITOR			= 0xA;
	static const GLOGICAL_OPERATOR	GOPERATOR_BITAND		= 0xB;
	static const GLOGICAL_OPERATOR	GOPERATOR_MIN			= 0xC;
	static const GLOGICAL_OPERATOR	GOPERATOR_MAX			= 0xD;

	// Operators
	typedef uint8_t GARITHMETIC_OPERATOR;
	static const GARITHMETIC_OPERATOR	GOPERATOR_SUBSTRACT		= 0x80;
	static const GARITHMETIC_OPERATOR	GOPERATOR_ADD			= 0x81;
	static const GARITHMETIC_OPERATOR	GOPERATOR_MULTIPLY		= 0x82;
	static const GARITHMETIC_OPERATOR	GOPERATOR_DIVIDE		= 0x83;
	static const GARITHMETIC_OPERATOR	GOPERATOR_MODULO		= 0x84;
	static const GARITHMETIC_OPERATOR	GOPERATOR_LSHIFT		= 0x85;
	static const GARITHMETIC_OPERATOR	GOPERATOR_RSHIFT		= 0x86;
	static const GARITHMETIC_OPERATOR	GOPERATOR_ASSIGN		= 0x87;
	static const GARITHMETIC_OPERATOR	GOPERATOR_REVSUBSTRACT	= 0x88;

	// This enumeration defines an interpolation value
	typedef uint8_t GINTERPOLATION_VALUE;
	static const GINTERPOLATION_VALUE GINTERPOLATION_0					= 0x00;
	static const GINTERPOLATION_VALUE GINTERPOLATION_1					= 0x01;
	static const GINTERPOLATION_VALUE GINTERPOLATION_SOURCECOLOR		= 0x02;
	static const GINTERPOLATION_VALUE GINTERPOLATION_INVSOURCECOLOR		= 0x03;
	static const GINTERPOLATION_VALUE GINTERPOLATION_DESTCOLOR			= 0x04;
	static const GINTERPOLATION_VALUE GINTERPOLATION_INVDESTCOLOR		= 0x05;
	static const GINTERPOLATION_VALUE GINTERPOLATION_SOURCEALPHA		= 0x06;
	static const GINTERPOLATION_VALUE GINTERPOLATION_INVSOURCEALPHA		= 0x07;
	static const GINTERPOLATION_VALUE GINTERPOLATION_SOURCEALPHASAT		= 0x08;
	static const GINTERPOLATION_VALUE GINTERPOLATION_BOTHSOURCEALPHA	= 0x09;
	static const GINTERPOLATION_VALUE GINTERPOLATION_BOTHINVSOURCEALPHA	= 0x0A;
	static const GINTERPOLATION_VALUE GINTERPOLATION_DESTALPHA			= 0x0B;
	static const GINTERPOLATION_VALUE GINTERPOLATION_INVDESTALPHA		= 0x0C;

	// This enumeration defines 
	typedef uint8_t GRESOURCE_TYPE;
	static const GRESOURCE_TYPE GRESOURCE_BUFFER	= 1;
	static const GRESOURCE_TYPE GRESOURCE_IMAGE		= 2;
	static const GRESOURCE_TYPE GRESOURCE_FUNCTION	= 3;
	static const GRESOURCE_TYPE GRESOURCE_PROGRAM	= 4;
	// I'm gonna add this one here so this crap doesn't get bigger
	static const GRESOURCE_TYPE GRESOURCE_FONT		= 5;

	// Enumeration that defines a compression type
	typedef uint8_t GCOMPRESSION_TYPE;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_NONE	= 0;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_RLE	= 1;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_ZIP	= 2;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_PNG	= 3;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_JPEG	= 4;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_GIF	= 5;
	static const GCOMPRESSION_TYPE GCOMPRESSTYPE_DX_BC	= 6;

	// Enumeration that defines a compression type
	typedef uint8_t GDRIVERFAMILY_ID;
	static const GDRIVERFAMILY_ID GFAMILYID_NETWORK		= 1;
	static const GDRIVERFAMILY_ID GFAMILYID_VIDEO		= 2;
	static const GDRIVERFAMILY_ID GFAMILYID_AUDIO		= 3;
	static const GDRIVERFAMILY_ID GFAMILYID_HID			= 4;
	static const GDRIVERFAMILY_ID GFAMILYID_PARALLELC	= 5;
	static const GDRIVERFAMILY_ID GFAMILYID_STORAGE		= 6;

	// The following enumeration defines unique values for identifying programming languages
	typedef uint8_t GPROGRAMMINGLANGUAGE_ID, GLANGUAGE_ID;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_UNKNOWN			= 0;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_BINARY			= 1;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_MCU_MPASM		= 2;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_X86			= 3;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_X64			= 4;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_SSE			= 5;	// CPU Extensions
	//static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_SSE5		= 9; now this works with ID + Version
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_3DNOW		= 10;	// CPU Extensions
	//static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_3DNOWPRO	= 11; now this works with ID + Version
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_ASM_D3DSM		= 12;	// D3D Shader Model 
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_HLSL				= 13;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_GLSL				= 14;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_OPENCL			= 15;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_CUDA				= 16;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_C				= 17;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_CPP				= 18;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_LISP				= 19;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_LUA				= 10;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_LUB				= 11;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_HTML				= 22;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_JAVA				= 23;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_PYTHON			= 24;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_RUBY				= 25;
	static const GPROGRAMMINGLANGUAGE_ID GLANGUAGE_PERL				= 26;

	// The following values identify semantic types
	typedef uint8_t GSEMANTIC_CLASS;
	static const GSEMANTIC_CLASS		GSEMANTICCLASS_ELEMENT		= 1;
	static const GSEMANTIC_CLASS		GSEMANTICCLASS_INSTANCE		= 2;

	// Defines an enumeration of possible operations 
	typedef uint8_t _GOPERATION_TYPE, GOPERATION_TYPE;
	static const GOPERATION_TYPE		GOPERATIONTYPE_DEFAULT	= 0;
	static const GOPERATION_TYPE		GOPERATIONTYPE_KEEP		= 1;
	static const GOPERATION_TYPE		GOPERATIONTYPE_ZERO		= 2;
	static const GOPERATION_TYPE		GOPERATIONTYPE_REPLACE	= 3;
	static const GOPERATION_TYPE		GOPERATIONTYPE_INCR_SAT	= 4;
	static const GOPERATION_TYPE		GOPERATIONTYPE_DECR_SAT	= 5;
	static const GOPERATION_TYPE		GOPERATIONTYPE_INVERT	= 6;
	static const GOPERATION_TYPE		GOPERATIONTYPE_INCR		= 7;
	static const GOPERATION_TYPE		GOPERATIONTYPE_DECR		= 8;

	// Defines a basic state for interfaces, where 0 means active and other means unused or invalid
	typedef uint8_t GINTERFACESTATE_TYPE, GINTERFACE_STATE;
	const GINTERFACE_STATE GISTATE_NORMAL = 0;
	const GINTERFACE_STATE GISTATE_UNUSED = 0x80;

	// ----- NOTICE! This id system became obsolete and I'm planning to replace it ASAP
	typedef uint64_t GINTERFACEID_TYPE;
	// --- System/User bit
	static const GINTERFACEID_TYPE GIID_RESERVED			=  0x80000000LL;
	// --- Base system interfaces
	//static const GINTERFACEID_TYPE GIID_DEVICECONTEXT		= (0x10000000LL);
	static const GINTERFACEID_TYPE GIID_DEVICEMANAGER		= (0x08000000LL);
	static const GINTERFACEID_TYPE GIID_RESOURCEMANAGER		= (0x04000000LL);
	static const GINTERFACEID_TYPE GIID_DEVICERESOURCE		= (0x02000000LL);
	static const GINTERFACEID_TYPE GIID_DEVICEHANDLE		= (0x01000000LL);
	// --- System devices
	//static const GINTERFACEID_TYPE GIID_PCMANAGER			= (0x00100000LL) | GIID_DEVICEMANAGER;
	//static const GINTERFACEID_TYPE GIID_INPUTMANAGER		= (0x00200000LL) | GIID_DEVICEMANAGER;
	static const GINTERFACEID_TYPE GIID_KEYBOARDMANAGER		= (0x00300000LL) | GIID_DEVICEMANAGER;
	static const GINTERFACEID_TYPE GIID_MOUSEMANAGER		= (0x00400000LL) | GIID_DEVICEMANAGER;
	//static const GINTERFACEID_TYPE GIID_HIDMANAGER			= (0x00500000LL) | GIID_DEVICEMANAGER;
	static const GINTERFACEID_TYPE GIID_VIDEOMANAGER		= (0x00600000LL) | GIID_DEVICEMANAGER;
	//static const GINTERFACEID_TYPE GIID_AUDIOMANAGER		= (0x00700000LL) | GIID_DEVICEMANAGER;
	//static const GINTERFACEID_TYPE GIID_MEDIAMANAGER		= (0x00800000LL) | GIID_DEVICEMANAGER;
	static const GINTERFACEID_TYPE GIID_NETWORKMANAGER		= (0x00900000LL) | GIID_DEVICEMANAGER;
	static const GINTERFACEID_TYPE GIID_STORAGEMANAGER		= (0x00A00000LL) | GIID_DEVICEMANAGER;
	// --- Device resource interfaces
	static const GINTERFACEID_TYPE GIID_DEVICEBUFFER		= (0x00010000LL) | GIID_DEVICERESOURCE;
	static const GINTERFACEID_TYPE GIID_DEVICEFUNCTION		= (0x00020000LL) | GIID_DEVICERESOURCE;
	static const GINTERFACEID_TYPE GIID_DEVICEPROGRAM		= (0x00030000LL) | GIID_DEVICERESOURCE;
	static const GINTERFACEID_TYPE GIID_DEVICEIMAGE			= (0x00040000LL) | GIID_DEVICERESOURCE;
	//static const GINTERFACEID_TYPE GIID_DEVICEQUEUE			= (0x00001000LL) | GIID_DEVICERESOURCE;

	// --- Graphic resources
	static const GINTERFACEID_TYPE GIID_FONT				= (0x00002000LL) | GIID_DEVICERESOURCE;
	// --- Media resources
	static const GINTERFACEID_TYPE GIID_WAVEBUFFER			= (0x00003000LL) | GIID_DEVICEBUFFER;
	static const GINTERFACEID_TYPE GIID_RESMEDIASYS1		= (0x00004000LL) | GIID_DEVICERESOURCE;
	static const GINTERFACEID_TYPE GIID_RESMEDIASYS2		= (0x00005000LL) | GIID_DEVICERESOURCE;

	// --- Entity
	static const GINTERFACEID_TYPE GIID_ENTITY				= (0x00000200LL);
	static const GINTERFACEID_TYPE GIID_GAME				= (0x00000100LL);
	static const GINTERFACEID_TYPE GIID_FRAMEWORK			= (0x00000080LL);

	// --- Get the implementation pointer with the "custom" type id
	static const GINTERFACEID_TYPE GIID_CUSTOMINTERFACE		= (0x00000040LL);

	// The error system is in ways to be implemented. 
	// NOTE: Exceptions are pretty much forbidden in this engine :) 
	typedef error_t _GERROR_TYPE, GERROR_TYPE;
	// No error
	static const GERROR_TYPE G_OK				= 0;

	// Errors // gonna make a macro to handle this too ,, some day
	static const GERROR_TYPE G_FAIL				= -1;
	static const GERROR_TYPE G_UNSUPPORTED		= -2;
	static const GERROR_TYPE G_NOTFOUND			= -3;
	static const GERROR_TYPE G_NOTAVAILABLE		= -4;
	static const GERROR_TYPE G_NOTINITIALIZED	= -5;
	static const GERROR_TYPE G_READONLY			= -6;
	static const GERROR_TYPE G_WRITEONLY		= -7;
	static const GERROR_TYPE G_NOTREADY			= -8;
	static const GERROR_TYPE G_INVALIDARGUMENT	= -9;
	static const GERROR_TYPE G_INVALIDINDEX		= -10;
	static const GERROR_TYPE G_INVALIDID		= -11;
	static const GERROR_TYPE G_NOMEM			= -12;

	// Warnings
	//static const GERROR_TYPE GWARN_WHATEVER = 
};

#endif // _GCORE_CONSTANTS_H
