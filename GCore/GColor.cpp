#include "GColor_object.h"

using namespace god;

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#endif

// ------------------------------------------ 
_GCOLOR::_GCOLOR( void ){}
_GCOLOR::_GCOLOR( uint32_t Color )
{
	static float oneOver255 = 1/255.0f;
	r	= ( ((Color & 0x00FF0000L)>>16)*oneOver255 );
	g	= ( ((Color & 0x0000FF00L)>>8)*oneOver255 );
	b	= ( (Color & 0x000000FFL)*oneOver255 );
	a	= ( ((Color & 0xFF000000L)>>24)*oneOver255 );
};

_GCOLOR::_GCOLOR( const _GCOLOR& color )
{
	r=color.r; g=color.g; b=color.b; a=color.a;
};

//_GCOLOR::_GCOLOR( float _r, float _g, float _b )
//:r(_r), g(_g), b(_b), a(1.0f) {};
_GCOLOR::_GCOLOR( float _r, float _g, float _b, float _a )
{
	r=_r; g=_g; b=_b; a=_a;
};

_GCOLOR::_GCOLOR( const float* rgbaColor )
{
	r = rgbaColor[0];
	g = rgbaColor[1];
	b = rgbaColor[2];
	a = rgbaColor[3];
};

bool _GCOLOR::operator ==( const _GCOLOR& color ) const
{	// convert to uint32 value to compare with better precision
	return ((_GCOLOR32)*this) == color;
}

bool _GCOLOR::operator !=( const _GCOLOR& color ) const
{	// convert to uint32 value to compare with better precision
	return ((_GCOLOR32)*this) != color;
}

// Pack to uint32_t ARGB (stored as B G R A in little-endian systems)
_GCOLOR::operator uint32_t() const
{
	return (((uint32_t)((a)*255.0f))<<24) | (((uint32_t)((r)*255.0f))<<16) | 
		(((uint32_t)((g)*255.0f))<<8) | ((uint32_t)((b)*255.0f));
}

// Assignment
_GCOLOR& _GCOLOR::operator =( const _GCOLOR& color )
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}
// Component-wise division
_GCOLOR _GCOLOR::operator /( float scalar ) const
{ return _GCOLOR(min(1.0f,max(0,r / scalar)), min(1.0f,max(0,g / scalar)), min(1.0f,max(0,b / scalar)), a); }
// Scalar muptiplication
_GCOLOR _GCOLOR::operator *( float scalar ) const
{ return _GCOLOR(min(1.0f,max(0,r * scalar)), min(1.0f,max(0,g * scalar)), min(1.0f,max(0,b * scalar)), a); }
// Scalar multiplication
_GCOLOR _GCOLOR::operator *( double scalar ) const
{ return _GCOLOR(min(1.0f,max(0,r * (float)scalar)), min(1.0f,max(0,g * (float)scalar)), min(1.0f,max(0,b * (float)scalar)), a); }
// Scalar division
_GCOLOR _GCOLOR::operator /( double scalar ) const
{ return _GCOLOR(min(1.0f,max(0,r / (float)scalar)), min(1.0f,max(0,g / (float)scalar)), min(1.0f,max(0,b / (float)scalar)), a); }
// Component-wise multiplication
_GCOLOR _GCOLOR::operator *( const _GCOLOR& color ) const
{ return _GCOLOR(min(1.0f,max(0,r * color.r)), min(1.0f,max(0,g * color.g)), min(1.0f,max(0,b * color.b)), min(1.0f,max(0,a * color.a))); }
// Component-wise addition
_GCOLOR _GCOLOR::operator +( const _GCOLOR& color ) const
{ return _GCOLOR(min(1.0f,max(0,r + color.r)), min(1.0f,max(0,g + color.g)), min(1.0f,max(0,b + color.b)), min(1.0f,max(0,a + color.a))); }

_GCOLOR& _GCOLOR::operator *=( float scalar )
{ 
  r *= scalar; g *= scalar; b *= scalar; // And leave a untouched.
	r = min(1.0f,max(0,r));
	g = min(1.0f,max(0,g));
	b = min(1.0f,max(0,b));
	return *this;
}

_GCOLOR& _GCOLOR::operator *=( double scalar )
{ 
	r *= (float)scalar; g *= (float)scalar; b *= (float)scalar; // And leave a untouched.
	r = min(1.0f,max(0,r));
	g = min(1.0f,max(0,g));
	b = min(1.0f,max(0,b));
	return *this;
}

_GCOLOR& _GCOLOR::operator /=( float scalar )
{ 
	r /= scalar; g /= scalar; b /= scalar; // And leave a untouched.
	r = min(1.0f,max(0,r));
	g = min(1.0f,max(0,g));
	b = min(1.0f,max(0,b));
	return *this;
}

_GCOLOR& _GCOLOR::operator /=( double scalar )
{ 
	r /= (float)scalar; g /= (float)scalar; b /= (float)scalar; // And leave a untouched.
	r = min(1.0f,max(0,r));
	g = min(1.0f,max(0,g));
	b = min(1.0f,max(0,b));
	return *this;
}


_GCOLOR& _GCOLOR::operator +=( const _GCOLOR& color )
{ 
	r += color.r; g += color.g; b += color.b; // And leave a untouched.
	r = min(1.0f,max(0,r));
	g = min(1.0f,max(0,g));
	b = min(1.0f,max(0,b));
	return *this;
}


_GCOLOR& _GCOLOR::Mix( const _GCOLOR& _a, const _GCOLOR& _b, float fFactor )
{
#ifndef mix
	#define mix(a, b, factor) ((b)*(factor)+(a)*(1.0f-(factor)))
#endif
	r	= mix( _a.r, _b.r, fFactor );
	g	= mix( _a.g, _b.g, fFactor );
	b	= mix( _a.b, _b.b, fFactor );
	return *this;
}
