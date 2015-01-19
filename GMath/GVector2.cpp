/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"

#include <math.h>

using namespace god;
_GVector2::_GVector2( void ){};
_GVector2::_GVector2( const GVECTOR2& _v )
{ x = _v.x; y = _v.y; ; }
_GVector2::_GVector2( const float* f )		
{ x = f[0]; y = f[1]; ; }
_GVector2::_GVector2( float _x, float _y )
{ x = _x; y = _y; ; }


bool _GVector2::operator ==( const GVECTOR2& v ) const
{
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return true; 
	//if( v.x == x && v.y == y )
	//	return true;
	return false;
}

bool _GVector2::operator !=( const GVECTOR2& v ) const
{
//	if( v.x == x && v.y == y )
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return false;
	return true;
}

const _GVector2& _GVector2::operator =( const GVECTOR2& _v )
{
	x = _v.x;
	y = _v.y;
	return *this;
}

const _GVector2& _GVector2::operator +=( const GVECTOR2& _v )
{
	x += _v.x;
	y += _v.y;
	return *this;
}

const _GVector2& _GVector2::operator -=( const GVECTOR2& _v )
{
	x -= _v.x;
	y -= _v.y;
	return *this;
}

const _GVector2& _GVector2::operator *=( float scalar )
{
	x *= scalar;
	y *= scalar;
	return *this;
}

const _GVector2& _GVector2::operator /=( float scalar )
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

_GVector2 _GVector2::operator+( const GVECTOR2& _v ) const
{
	return _GVector2( x+_v.x, y+_v.y );
};

_GVector2 _GVector2::operator-( const GVECTOR2& _v ) const
{
	return _GVector2( x-_v.x, y-_v.y );
};

float _GVector2::operator*( const GVECTOR2& _v ) const
{
	return x*_v.x + y*_v.y;
}; 

_GVector2 _GVector2::operator*( float scalar ) const
{
	return GVector2( x * scalar, y * scalar );
};

_GVector2 _GVector2::operator/( float scalar ) const
{
	return GVector2( x / scalar, y / scalar );
};

_GVector2 _GVector2::operator-( void ) const
{
	return _GVector2( -x, -y );
};

float _GVector2::Element( uint8_t n ) const
{
	//( 0 != n && 1 != n ) ? throw("error, retrieving unexisting element on GVector2") : false;
	switch( n )
	{
	case 0:
		return x;
	case 1:
		return y;
	}
	return 0;
}; // return n element from vector

const _GVector2& _GVector2::SetElement( uint8_t n, float fValue )
{
	//( 0 != n && 1 != n ) ? throw("error, assigning unexisting element on GVector2") : false;
	switch( n )
	{
	case 0:
		x = fValue;
	case 1:
		y = fValue;
	}
	return *this;
};

void _GVector2::AddScaledVector( const GVECTOR2& vector, float scale ) 
{
	x += vector.x*scale;
	y += vector.y*scale;
};

float _GVector2::Dot( const GVECTOR2& _v ) const
{
	return x*_v.x + y*_v.y;
};

float _GVector2::Length( void ) const
{
	return sqrt( x*x+y*y );
};

float _GVector2::SqLength( void ) const
{
	return x*x+y*y;
};

const _GVector2& _GVector2::Normalize( void )
{
	//float l = x*x+y*y;
	//x = x*x / l;
	//y = y*y / l;
	float l = x*x+y*y;
	if( l )
	{
		l = sqrt( l );
		x = x / l;
		y = y / l;
	}
	return *this;
};


const _GVector2& _GVector2::Difference( const GVECTOR2& orig, const GVECTOR2& dest )
{
	x = dest.x-orig.x;
	y = dest.y-orig.y;
	return *this;
};

_GVector2 _GVector2::Difference( const GVECTOR2& dest ) const
{
	return GVector2(dest)-*this;
};

float _GVector2::Distance( const GVECTOR2& dest ) const
{
	return (GVector2(dest)-*this).Length();
};

const _GVector2& _GVector2::operator*=(const GMATRIX2X2& m)
{
	float _x, _y;

	_x = x*m._11 + y*m._21;
	_y = x*m._12 + y*m._22;

	this->x = _x;
	this->y = _y;
	return *this;
}

_GVector2 _GVector2::operator*(const GMATRIX2X2 &m) const
{
	_GVector2 vResult;

	vResult.x = x*m._11 + y*m._21;
	vResult.y = x*m._12 + y*m._22;
	return vResult;
}

_GVector2& _GVector2::Rotate( float _theta_in_rads ) 
{
	float theta = _theta_in_rads;
	float cs = cosf(theta);
	float sn = sinf(theta);

	float px = x * cs - y * sn; 
	float py = x * sn + y * cs;

	x = px;
	y = py;
	return *this;
}

float _GVector2::AngleWith( const GVECTOR2& v ) const
{
   return (float)acos( ((*this) * v) / (this->Length()*GVector2(v).Length()) );
}
