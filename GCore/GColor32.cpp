#include "GColor_object.h"
#include <math.h>

using namespace god;

_GCOLOR32::_GCOLOR32( uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a )
{
	b = _b;
	g = _g;
	r = _r;
	a = _a;
};

_GCOLOR32::_GCOLOR32( const _GCOLOR& other )
{
	b = (uint8_t)(other.b*0xFF);
	g = (uint8_t)(other.g*0xFF);
	r = (uint8_t)(other.r*0xFF);
	a = (uint8_t)(other.a*0xFF);
};

_GCOLOR32::_GCOLOR32( const _GCOLOR32& other )
{
	b = other.b;
	g = other.g;
	r = other.r;
	a = other.a;
};

_GCOLOR32::_GCOLOR32( const _GCOLOR24& other )
{
	b = other.b;
	g = other.g;
	r = other.r;
	a = 0xFF;
};

//_GCOLOR32::_GCOLOR32( const _GCOLOR16& other )
//{
//	// shift color from original position,	scale to 0.0f, 1.0f		and then scale each color to "unsigned char"
//	b = (uint8_t) ((other & 0x001F)			/ ((float)(0x1F))		*	255);	// final 5 bits	( 1F )
//	g = (uint8_t) (((other & 0x07E0) >>5)	/ ((float)(0x3F))		*	255);	// next 6 bits ( 3F )
//	r = (uint8_t) (((other & 0xF800) >>11)	/ ((float)(0x1F))		*	255);	// first 5 bits ( 1F )
//	a = 255;
//};

_GCOLOR32::_GCOLOR32( uint32_t other )
{
	// shift color from original position,	scale to 0.0f, 1.0f		and then scale each color to "unsigned char"
	b = (other & 0x000000FF) >> 0	;
	g = (other & 0x0000FF00) >> 8	;
	r = (other & 0x00FF0000) >> 16	;
	a = (other & 0xFF000000) >> 24	;
};

bool _GCOLOR32::operator ==( const _GCOLOR32& color ) const
{ return r == color.r && g == color.g && b == color.b && a == color.a; }

bool _GCOLOR32::operator !=( const _GCOLOR32& color ) const
{ return r != color.r || g != color.g || b != color.b || a != color.a; }

bool _GCOLOR32::operator ==( const _GCOLOR24& color ) const
{ return r == color.r && g == color.g && b == color.b; }

bool _GCOLOR32::operator !=( const _GCOLOR24& color ) const
{ return r != color.r || g != color.g || b != color.b; }

//bool _GCOLOR32::operator !=( const _GCOLOR16& color ) const
//{
//	GCOLOR24 other = color;
//	return r != other.r || g != other.g || b != other.b; 
//}

bool _GCOLOR32::operator ==( const _GCOLOR& color ) const
{ 
	_GCOLOR32 other = color;
	return r == other.r && g == other.g && b == other.b; 
}

bool _GCOLOR32::operator !=( const _GCOLOR& color ) const
{
	_GCOLOR32 other = color;
	return r != other.r || g != other.g || b != other.b; 
}

#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 

_GCOLOR32 _GCOLOR32::operator *( const _GCOLOR& color ) const
{ 
	return _GCOLOR32(
		min( 255, (int)(r * color.r) ),
		min( 255, (int)(g * color.g) ),
		min( 255, (int)(b * color.b) ),
		min( 255, (int)(a * color.a) )
		);
	//return _GCOLOR(
	//	min( 255, r * (int)other.r)*oneover255, ///255.0f, 
	//	min( 255, g * (int)other.g)*oneover255, ///255.0f, 
	//	min( 255, b * (int)other.b)*oneover255, ///255.0f, 
	//	min( 255, a * (int)other.a)*oneover255	///255.0f 
	//	);
}

_GCOLOR32 _GCOLOR32::operator *( const _GCOLOR32& color ) const
{ 
	static const float oneover255 = (1/255.0f);
	return _GCOLOR32(
		min( 255, r * (int)(color.r*oneover255) ), 
		min( 255, g * (int)(color.g*oneover255) ), 
		min( 255, b * (int)(color.b*oneover255) ), 
		min( 255, a * (int)(color.a*oneover255) ) 
		);
}

_GCOLOR32 _GCOLOR32::operator *( const _GCOLOR24& color ) const
{ 
	static const float oneover255 = (1/255.0f);
	return _GCOLOR32(
		min( 255, r * (int)(color.r*oneover255) ), 
		min( 255, g * (int)(color.g*oneover255) ), 
		min( 255, b * (int)(color.b*oneover255) ),
		a
		);
}

//-------------------------- Addition
_GCOLOR32 _GCOLOR32::operator +( const _GCOLOR& color ) const
{ 
	_GCOLOR32 other = color;
	return _GCOLOR(
		min( 255.0f, r + (int)other.r ), 
		min( 255.0f, g + (int)other.g ), 
		min( 255.0f, b + (int)other.b ), 
		min( 255.0f, a + (int)other.a )
		);
}

_GCOLOR32 _GCOLOR32::operator +( const _GCOLOR32& color ) const
{ 
	return _GCOLOR32(
		min( 255, r + (int)color.r ), 
		min( 255, g + (int)color.g ), 
		min( 255, b + (int)color.b ), 
		min( 255, a + (int)color.a )
		);
}

_GCOLOR32 _GCOLOR32::operator +( const _GCOLOR24& color ) const
{ 
	return _GCOLOR32(
		min( 255, r + (int)color.r ), 
		min( 255, g + (int)color.g ), 
		min( 255, b + (int)color.b ),
		a
		);
}

_GCOLOR32 _GCOLOR32::operator /( double scalar ) const
{ 
	return _GCOLOR32((uint8_t)(r / scalar), (uint8_t)(g / scalar), (uint8_t)(b / scalar), a); 
}

_GCOLOR32 _GCOLOR32::operator /( float scalar ) const
{ 
	return _GCOLOR32((uint8_t)(r / scalar), (uint8_t)(g / scalar), (uint8_t)(b / scalar), a); 
}
_GCOLOR32 _GCOLOR32::operator *( double scalar ) const
{ 
	return _GCOLOR32(
		(uint8_t)min( 255, r * scalar ), 
		(uint8_t)min( 255, g * scalar ), 
		(uint8_t)min( 255, b * scalar ), 
		a
		); 
};

_GCOLOR32 _GCOLOR32::operator *( float scalar ) const
{ 
	return _GCOLOR32(
		(uint8_t)min( 255, r * scalar ), 
		(uint8_t)min( 255, g * scalar ), 
		(uint8_t)min( 255, b * scalar ), 
		a
		); 
};

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR32::operator _GCOLOR( void ) const
{
	return _GCOLOR( ( (uint32_t) a	) << 24	)
		|  ( ( (uint32_t) r	) << 16	)
		|  ( ( (uint32_t) g	) << 8 	)
		|  ( ( (uint32_t) b	) << 0	);
}

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR32::operator uint32_t( void ) const
{
	return ( ( (uint32_t) a	) << 24	)
		|  ( ( (uint32_t) r	) << 16	)
		|  ( ( (uint32_t) g	) << 8 	)
		|  ( ( (uint32_t) b	) << 0	);
}

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR32::operator _GCOLOR24( void ) const
{
	return _GCOLOR24( r, g, b );
}

