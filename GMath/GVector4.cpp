/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include <math.h>

using namespace god;

_GVector4::_GVector4( void ){};
_GVector4::_GVector4( const GVECTOR4& _v )
{ x = _v.x; y = _v.y; z = _v.z; w = _v.w; }
_GVector4::_GVector4( const float* f )		
{ x = f[0]; y = f[1]; z = f[2]; w = f[3]; }
_GVector4::_GVector4( float _x, float _y, float _z, float _w )
{ x = _x; y = _y; z = _z; w = _w; }

bool _GVector4::operator ==( const GVECTOR4& v ) const
{
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return true;
	//if( v.x == x && v.y == y && v.z == z ) // && v.w == w )
	//	return true;
	return false;
}

bool _GVector4::operator !=( const GVECTOR4& v ) const
{
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return false;
	//if( v.x == x && v.y == y && v.z == z ) //&& v.w == w )
	//	return false;
	return true;
}

const _GVector4& _GVector4::operator =( const GVECTOR4& _v )
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
	w = _v.w;
	return *this;
}

const _GVector4& _GVector4::operator +=( const GVECTOR4 &_v )
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	w = 1.0f; //w += _v.w;
	return *this;
}

const _GVector4& _GVector4::operator -=( const GVECTOR4 &_v )
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	w = 1.0f; //w -= _v.w;
	return *this;
}

const _GVector4& _GVector4::operator *=( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w = 1.0f;
	return *this;
}

const _GVector4& _GVector4::operator /=( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w = 1.0f;
	return *this;
}

_GVector4 _GVector4::operator+( const GVECTOR4& _v ) const
{
	return _GVector4( x+_v.x, y+_v.y, z+_v.z, 1.0f );// , w+_v.w );
};

_GVector4 _GVector4::operator-( const GVECTOR4& _v ) const
{
	return _GVector4( x-_v.x, y-_v.y, z-_v.z, 1.0f ); //, w-_v.w );
};

float _GVector4::operator*( const GVECTOR4& _v ) const
{
	return x*_v.x + y*_v.y + z*_v.z; //  + w*_v.w;
}; 

_GVector4 _GVector4::operator*( float scalar ) const
{
	return GVector4( x * scalar, y * scalar, z * scalar, 1.0f );
};

_GVector4 _GVector4::operator/( float scalar ) const
{
	return GVector4( x / scalar, y / scalar, z / scalar, 1.0f );
};

_GVector4 _GVector4::operator-( void ) const
{
	return _GVector4( -x, -y, -z, w );
};

float _GVector4::Element( uint8_t n ) const
{
	switch( n )
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		return 0; //throw("Error, retrieving unexisting element on GVector4");
	}
	return 0;
}; // return n element from vector
const _GVector4& _GVector4::SetElement( uint8_t n, float fValue )
{
	//( n > 3 ) ? throw("error, setting unexisting element on GVector4") : false;
	switch( n )
	{
	case 0:
		x = fValue;
	case 1:
		y = fValue;
	case 2:
		z = fValue;
	case 3:
		w = fValue;
	}
	return *this;
};

void _GVector4::AddScaledVector( const GVECTOR4& vector, float scale ) 
{
	x += vector.x*scale;
	y += vector.y*scale;
	z += vector.z*scale;
	w = 1.0f;
};

float _GVector4::Dot( const GVECTOR4& v ) const
{
	//return x*_v.x + y*_v.y + z*_v.z + w*_v.w;
	return x * v.x + y * v.y + z * v.z; 
};

float _GVector4::Length( void ) const
{
	float sql = x*x+y*y+z*z;
	if( 0 != sql )
		return sqrt( sql ); // +w*w );
	return 0;
};

float _GVector4::SqLength( void ) const
{
	return x*x+y*y+z*z; // +w*w;
};

const _GVector4& _GVector4::Normalize( void )
{
	float l = x*x+y*y+z*z; 
	if( 0 == l ) 
		return *this;
	l = sqrt( l );
	x	= x / l;
	y	= y / l;
	z	= z / l;
	w	= 1.0f; //w / l;
	return *this;
};

const _GVector4& _GVector4::Difference( const GVECTOR4& orig, const GVECTOR4& dest )
{
	x = dest.x-orig.x;
	y = dest.y-orig.y;
	z = dest.z-orig.z;
	w = 1.0f; //dest.w-orig.w;
	return *this;
};

_GVector4 _GVector4::Difference( const GVECTOR4& dest ) const
{
	return _GVector4(dest)-*this;
};

float _GVector4::Distance( const GVECTOR4& dest ) const
{
	return (_GVector4(dest)-*this).Length();
};

float _GVector4::AngleWith( const GVECTOR4& v ) const
{
   return (float)acos( ((*this) * v) / (this->Length()*_GVector4(v).Length()) );
}

_GVector4 _GVector4::operator*( const GMATRIX4X4& m ) const
{
	_GVector4 vResult;

	vResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

	vResult.x = vResult.x/vResult.w;
	vResult.y = vResult.y/vResult.w;
	vResult.z = vResult.z/vResult.w;
	vResult.w = 1.0f;

	return vResult;
}

const _GVector4& _GVector4::operator*=( const GMATRIX4X4& m )
{
	_GVector4 vResult;

	vResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

	x = vResult.x/vResult.w;
	y = vResult.y/vResult.w;
	z = vResult.z/vResult.w;
	w = 1.0f;

	return *this;
}
//
//GVector4 GVector4::operator*(const GMatrix4x3 &m) const
//{
//	GVector4 vResult;
//
//	vResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
//	vResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
//	vResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
//	// =( vResult.w = x*0+y*0+z*0+1.0f; 
//	vResult.w = 1.0f; 
//	
//	return vResult;
//}