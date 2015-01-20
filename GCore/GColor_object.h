#ifndef _GCOLOR_OBJECT_H
#define _GCOLOR_OBJECT_H

#ifndef GDISABLE_STDINT
#include <stdint.h>
#else
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#endif

namespace god
{
#pragma pack( push, 1 )
	typedef uint16_t _GCOLOR16, GCOLOR16;

	struct GCOLORI32
	{
		uint8_t b, g, r, a; 
	};

	struct GCOLORI24
	{
		uint8_t b, g, r; 
	};

	typedef struct GCOLORF32
	{
		float r, g, b, a; 
	} GCOLORFLOAT;

	typedef struct GCOLORF64
	{
		double r, g, b, a; 
	} GCOLORDOUBLE;

	struct _GCOLOR;
	struct _GCOLOR32;
	struct _GCOLOR24;

	typedef struct _GCOLOR32 : public GCOLORI32
	{// store the color values as floating point ranged in the values (0,1)
		_GCOLOR32( const _GCOLOR& otherColorFloat );
		_GCOLOR32( const _GCOLOR32& otherColorInt );
		_GCOLOR32( const _GCOLOR24& otherColorInt );
		_GCOLOR32( uint32_t otherColorInt );
		_GCOLOR32( uint8_t r, uint8_t g, uint8_t b, uint8_t a );
		bool		operator ==( const _GCOLOR& )			const;
		bool		operator ==( const _GCOLOR32& )			const;
		bool		operator ==( const _GCOLOR24& )			const;
		bool		operator !=( const _GCOLOR& )			const;
		bool		operator !=( const _GCOLOR32& )			const;
		bool		operator !=( const _GCOLOR24& )			const;
		_GCOLOR32	operator *( const _GCOLOR& color )		const;
		_GCOLOR32	operator *( const _GCOLOR32& color )	const;
		_GCOLOR32	operator *( const _GCOLOR24& color )	const;
		_GCOLOR32	operator +( const _GCOLOR& color )		const;
		_GCOLOR32	operator +( const _GCOLOR32& color )	const;
		_GCOLOR32	operator +( const _GCOLOR24& color )	const;
		_GCOLOR32	operator *( float scalar )				const;
		_GCOLOR32	operator *( double scalar )				const;
		_GCOLOR32	operator /( float scalar )				const;
		_GCOLOR32	operator /( double scalar )				const;
		operator	_GCOLOR( void )							const;
		operator	_GCOLOR24( void )						const;
		operator	uint32_t( void )						const;
	} GCOLOR32;

	typedef struct _GCOLOR24 : public GCOLORI24
	{// store the color values as floating point ranged in the values (0,1)
		_GCOLOR24( const _GCOLOR& otherColorFloat );
		_GCOLOR24( const _GCOLOR32& otherColorInt );
		_GCOLOR24( const _GCOLOR24& otherColorInt );
		_GCOLOR24( const _GCOLOR16& otherColorInt );
		_GCOLOR24( uint32_t otherColorInt );
		_GCOLOR24( uint8_t r, uint8_t g, uint8_t b );
		bool		operator ==( const _GCOLOR& )			const;
		bool		operator ==( const _GCOLOR32& )			const;
		bool		operator ==( const _GCOLOR24& )			const;
		bool		operator ==( const _GCOLOR16& )			const;
		bool		operator !=( const _GCOLOR& )			const;
		bool		operator !=( const _GCOLOR32& )			const;
		bool		operator !=( const _GCOLOR24& )			const;
		bool		operator !=( const _GCOLOR16& )			const;
		_GCOLOR32	operator *( const _GCOLOR& color )		const;
		_GCOLOR32	operator *( const _GCOLOR32& color )	const;
		_GCOLOR24	operator *( const _GCOLOR24& color )	const;
		_GCOLOR		operator +( const _GCOLOR& color )		const;
		_GCOLOR32	operator +( const _GCOLOR32& color )	const;
		_GCOLOR24	operator +( const _GCOLOR24& color )	const;
		_GCOLOR24	operator *( float scalar )				const;
		_GCOLOR24	operator *( double scalar )				const;
		_GCOLOR24	operator /( float scalar )				const;
		_GCOLOR24	operator /( double scalar )				const;
		operator	_GCOLOR( void )							const;
		operator	_GCOLOR16( void )						const;
		operator	uint32_t( void )						const;
	} GCOLOR24;

	typedef struct _GCOLOR : GCOLORFLOAT
	{// store the color values as floating point ranged in the values (0,1)
		_GCOLOR( void );
		_GCOLOR( uint16_t Color );
		_GCOLOR( uint32_t Color );
		_GCOLOR( const float* rgbaColor );
		_GCOLOR( const _GCOLOR& color );
		_GCOLOR( float _r, float _g, float _b, float _a=1.0f );

		operator uint32_t( void ) const;
		operator const float*( void ) const { return &r; };

		bool operator ==( const _GCOLOR& color ) const;
		bool operator !=( const _GCOLOR& color ) const;
		_GCOLOR& operator =( const _GCOLOR& color );
		_GCOLOR& operator +=( const _GCOLOR& color );
		_GCOLOR& operator *=( double scalar );
		_GCOLOR& operator *=( float scalar );
		_GCOLOR& operator /=( double scalar );
		_GCOLOR& operator /=( float scalar );
		_GCOLOR operator /( float scalar )			const;
		_GCOLOR operator /( double scalar )			const;
		_GCOLOR operator *( float scalar )			const;
		_GCOLOR operator *( double scalar )			const;
		_GCOLOR operator *( const _GCOLOR& color ) const;
		_GCOLOR operator +( const _GCOLOR& color ) const;
		_GCOLOR& Mix(const _GCOLOR& a, const _GCOLOR& b, float fFactor);
//		_GCOLOR operator -( const _GCOLOR& color ) const;
	} GCOLOR;
#pragma pack( pop )

	// Define some constant colors... just for fun (?)
	static const GCOLOR GBLACK(0.0f, 0.0f, 0.0f, 1.0f);
	static const GCOLOR GWHITE(1.0f, 1.0f, 1.0f, 1.0f);
	static const GCOLOR GRED(1.0f, 0.0f, 0.0f, 1.0f);
	static const GCOLOR GGREEN(0.0f, 1.0f, 0.0f, 1.0f);
	static const GCOLOR GBLUE(0.0f, 0.0f, 1.0f, 1.0f);
	static const GCOLOR GYELLOW(1.0f, 1.0f, 0.0f, 1.0f);
	static const GCOLOR GMAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
	static const GCOLOR GCYAN(0.0f, 1.0f, 1.0f, 1.0f);
	static const GCOLOR GORANGE(1.0f, 0.647f, 0.0f, 1.0f);
	static const GCOLOR GBROWN(0xFF964B00);
	static const GCOLOR GGRAY(0.5f, 0.5f, 0.5f, 1.0f);
	static const GCOLOR GDARKGRAY(0.25f, 0.25f, 0.25f, 1.0f);
	static const GCOLOR GDARKRED(0.5f, 0.0f, 0.0f, 1.0f);
	static const GCOLOR GDARKGREEN(0.0f, 0.5f, 0.0f, 1.0f);
	static const GCOLOR GDARKBLUE(0.0f, 0.0f, 0.5f, 1.0f);
	static const GCOLOR GDARKYELLOW(0.5f, 0.5f, 0.0f, 1.0f);
	static const GCOLOR GDARKMAGENTA(0.5f, 0.0f, 0.5f, 1.0f);
	static const GCOLOR GDARKCYAN(0.0f, 0.5f, 0.5f, 1.0f);
	static const GCOLOR GDARKORANGE(1.0f, 0.5490196078431373f, 0.0f, 1.0f);
	static const GCOLOR GLIGHTGRAY(0.75f, 0.75f, 0.75f, 1.0f);
	static const GCOLOR GLIGHTRED(1.0f, 0.25f, 0.25f, 1.0f);
	static const GCOLOR GLIGHTGREEN(0.25f, 1.0f, 0.25f, 1.0f);
	static const GCOLOR GLIGHTBLUE(0.25f, 0.25f, 1.0f, 1.0f);
	static const GCOLOR GLIGHTYELLOW(1.0f, 1.0f, 0.25f, 1.0f);
	static const GCOLOR GLIGHTMAGENTA(1.0f, 0.25f, 1.0f, 1.0f);
	static const GCOLOR GLIGHTCYAN(0.25f, 1.0f, 1.0f, 1.0f);
	static const GCOLOR GLIGHTORANGE(1.0f, 0.780f, 0.25f, 1.0f);

#define GCOLOR_ARGB(a,r,g,b)			\
    ( (uint32_t)						\
		( (((a)&0xff)<<24)				\
		| (((r)&0xff)<<16)				\
		| (((g)&0xff)<<8)|((b)&0xff)	\
		)								\
	)
}

#endif // _GCOLOR_OBJECT_H