#include "GColor_object.h"

using namespace god;

_GCOLOR24::_GCOLOR24( uint8_t _r, uint8_t _g, uint8_t _b )
{
	b = _b;
	g = _g;
	r = _r;
};

//------------------------------ GCOLOR24
_GCOLOR24::_GCOLOR24( const _GCOLOR& other )
{
	b = (uint8_t)(other.b*255);
	g = (uint8_t)(other.g*255);
	r = (uint8_t)(other.r*255);
};

_GCOLOR24::_GCOLOR24( const _GCOLOR32& other )
{
	b = other.b;
	g = other.g;
	r = other.r;
};

_GCOLOR24::_GCOLOR24( const _GCOLOR24& other )
{
	b = other.b;
	g = other.g;
	r = other.r;
};

_GCOLOR24::_GCOLOR24( const _GCOLOR16& other )
{
	// shift color from original position,	scale to 0.0f, 1.0f		and then scale each color to "unsigned char"
	b = (uint8_t) ((other & 0x001F)			/ ((float)(0x1F))		*	255);	// final 5 bits	( 1F )
	g = (uint8_t) (((other & 0x07E0) >>5)	/ ((float)(0x3F))		*	255);	// next 6 bits ( 3F )
	r = (uint8_t) (((other & 0xF800) >>11)	/ ((float)(0x1F))		*	255);	// first 5 bits ( 1F )
};

_GCOLOR24::_GCOLOR24( uint32_t other )
{
	b = (other & 0x000000FF) >> 0	;
	g = (other & 0x0000FF00) >> 8	;
	r = (other & 0x00FF0000) >> 16	;
};

bool _GCOLOR24::operator ==( const _GCOLOR32& color ) const
{ return r == color.r && g == color.g && b == color.b; }

bool _GCOLOR24::operator ==( const _GCOLOR24& color ) const
{ return r == color.r && g == color.g && b == color.b; }

bool _GCOLOR24::operator ==( const _GCOLOR16& color ) const
{ 
	GCOLOR24 other = color;
	return r == other.r && g == other.g && b == other.b; 
}

bool _GCOLOR24::operator ==( const _GCOLOR& color ) const
{ 
	GCOLOR24 other = color;
	return r == other.r && g == other.g && b == other.b; 
}

bool _GCOLOR24::operator !=( const _GCOLOR32& color ) const
{ return r != color.r || g != color.g || b != color.b; }

bool _GCOLOR24::operator !=( const _GCOLOR24& color ) const
{ return r != color.r || g != color.g || b != color.b; }

bool _GCOLOR24::operator !=( const _GCOLOR16& color ) const
{
	GCOLOR24 other = color;
	return r != other.r || g != other.g || b != other.b; 
}

bool _GCOLOR24::operator !=( const _GCOLOR& color ) const
{
	GCOLOR24 other = color;
	return r != other.r || g != other.g || b != other.b; 
}

#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 

// Component multiplication
_GCOLOR32 _GCOLOR24::operator *( const _GCOLOR& color ) const
{ 
	return _GCOLOR32(
		min( 255, (int)(r * color.r) ),
		min( 255, (int)(g * color.g) ),
		min( 255, (int)(b * color.b) ),
		min( 255, (int)(255 * color.a) )
		);
}

_GCOLOR32 _GCOLOR24::operator *( const _GCOLOR32& color ) const
{ 
	static const float oneover255 = (1/255.0f);
	return _GCOLOR32(
		min( 255, r * (int)(color.r*oneover255) ), 
		min( 255, g * (int)(color.g*oneover255) ), 
		min( 255, b * (int)(color.b*oneover255) ), 
		min( 255, 255 * (int)(color.b*oneover255) )
		);
}

_GCOLOR24 _GCOLOR24::operator *( const _GCOLOR24& color ) const
{ 
	static const float oneover255 = (1/255.0f);
	return _GCOLOR24(
		min( 255, r * (int)(color.r*oneover255) ), 
		min( 255, g * (int)(color.g*oneover255) ), 
		min( 255, b * (int)(color.b*oneover255) )
		);
}

//-------------------------- Addition
_GCOLOR _GCOLOR24::operator +( const _GCOLOR& color ) const
{ 
	_GCOLOR24 other = color;
	return _GCOLOR(
		min( 255.0f, r + (int)other.r ), 
		min( 255.0f, g + (int)other.g ), 
		min( 255.0f, b + (int)other.b ), 
		color.a
		);
}

_GCOLOR32 _GCOLOR24::operator +( const _GCOLOR32& color ) const
{ 
	return _GCOLOR32(
		min( 255, r + (int)color.r ), 
		min( 255, g + (int)color.g ), 
		min( 255, b + (int)color.b ), 
		color.a
		);
}

_GCOLOR24 _GCOLOR24::operator +( const _GCOLOR24& color ) const
{ 
	return _GCOLOR24(
		min( 255, r + (int)color.r ), 
		min( 255, g + (int)color.g ), 
		min( 255, b + (int)color.b ) 
		);
}

_GCOLOR24 _GCOLOR24::operator /( double scalar ) const
{ 
	return _GCOLOR24((uint8_t)(r / scalar), (uint8_t)(g / scalar), (uint8_t)(b / scalar)); 
}

_GCOLOR24 _GCOLOR24::operator /( float scalar ) const
{ 
	return _GCOLOR24((uint8_t)(r / scalar), (uint8_t)(g / scalar), (uint8_t)(b / scalar)); 
}

_GCOLOR24 _GCOLOR24::operator *( double scalar ) const
{ 
	return _GCOLOR24(
		(uint8_t)min( 255, (r * scalar) ), 
		(uint8_t)min( 255, (g * scalar) ), 
		(uint8_t)min( 255, (b * scalar) )
		); 
};

_GCOLOR24 _GCOLOR24::operator *( float scalar ) const
{ 
	return _GCOLOR24(
		(uint8_t)min( 255, r * scalar ), 
		(uint8_t)min( 255, g * scalar ), 
		(uint8_t)min( 255, b * scalar )
		); 
};

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR24::operator uint32_t( void ) const
{
	return ( ( (uint32_t) 0xFFL	) << 24	)
		|  ( ( (uint32_t) r		) << 16	)
		|  ( ( (uint32_t) g		) << 8 	)
		|  ( ( (uint32_t) b		) << 0	);
}

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR24::operator _GCOLOR16( void ) const
{
	static const float oneover255 = (1/255.0f);
	return ( ( (uint16_t) (b * oneover255 * 0x001F) ) << 0 )
		|  ( ( (uint16_t) (g * oneover255 * 0x003F) ) << 5 )
		|  ( ( (uint16_t) (r * oneover255 * 0x001F) ) << 11 );
	// --- reverse conversion:
	//// shift color from original position,	scale to 0.0f, 1.0f		and then scale each color to "unsigned char"
	//b = (uint8_t) ((other  & 0x001F)			/ ((float)(0x1F))		*	255);	// final 5 bits	( 1F )
	//g = (uint8_t) (((other & 0x07E0) >>5)		/ ((float)(0x3F))		*	255);	// next 6 bits ( 3F )
	//r = (uint8_t) (((other & 0xF800) >>11)	/ ((float)(0x1F))		*	255);	// first 5 bits ( 1F )
}
