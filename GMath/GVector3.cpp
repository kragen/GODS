/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"

#include <math.h>

using namespace god;
_GVector3::_GVector3( void ){};
_GVector3::_GVector3( const GVECTOR3& _v )
{ x = _v.x; y = _v.y; z = _v.z; }
_GVector3::_GVector3( const float* f )		
{
	if (0 == f) { x = y = z = 0; return; }
	x = f[0]; y = f[1]; z = f[2];
}
_GVector3::_GVector3( float _x, float _y, float _z )
{ x = _x; y = _y; z = _z; }

bool _GVector3::operator ==( const GVECTOR3& v ) const
{
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return true; 
	//if( v.x == x && v.y == y && v.z == z )
	//	return true;
	return false;
}

bool _GVector3::operator !=( const GVECTOR3& v ) const
{
	if( fabs( (*this - v).Length() ) < GEPSILON ) // this works better than the raw compare, but it's kinda slow
		return false;
//	if( v.x == x && v.y == y && v.z == z )
//		return false;
	return true;
}

const _GVector3& _GVector3::operator =( const GVECTOR3&_v )
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
	return *this;
}

const _GVector3& _GVector3::operator +=( const GVECTOR3&_v )
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	return *this;
}

const _GVector3& _GVector3::operator -=( const GVECTOR3&_v )
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	return *this;
}

const _GVector3& _GVector3::operator *=( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

const _GVector3& _GVector3::operator /=( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

_GVector3 _GVector3::operator+( const GVECTOR3& _v ) const
{
	return _GVector3( x+_v.x, y+_v.y, z+_v.z );
};

_GVector3 _GVector3::operator-( const GVECTOR3& _v ) const
{
	return _GVector3( x-_v.x, y-_v.y, z-_v.z );
};

float _GVector3::operator*( const GVECTOR3& _v ) const
{
	return x*_v.x + y*_v.y + z*_v.z;
}; 

_GVector3 _GVector3::operator*( float scalar ) const
{
	return GVector3( x * scalar, y * scalar, z * scalar );
};

_GVector3 _GVector3::operator/( float scalar ) const
{
	return GVector3( x / scalar, y / scalar, z / scalar );
};

_GVector3 _GVector3::operator-( void ) const
{
	return _GVector3( -x, -y, -z );
};

void _GVector3::Set( float _x, float _y, float _z )
{
	x = _x; y = _y; z = _z;
}

float _GVector3::Element( uint8_t n ) const
{
	//( n > 2 ) ? throw("error, retrieving unexisting element on GVector3") : false;
	switch( n )
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return 0;
}; // return n element from vector

const _GVector3& _GVector3::SetElement( uint8_t n, float fValue )
{
	//( n > 2 ) ? throw("error, setting unexisting element on GVector3") : false;
	switch( n )
	{
	case 0:
		x = fValue;
	case 1:
		y = fValue;
	case 2:
		z = fValue;
	}
	return *this;
};

void _GVector3::AddScaledVector( const GVECTOR3& vector, float scale ) 
{
	x += vector.x*scale;
	y += vector.y*scale;
	z += vector.z*scale;
};

float _GVector3::Dot( const GVECTOR3& _v ) const
{
	return x*_v.x + y*_v.y + z*_v.z;
};

float _GVector3::Length( void ) const
{
	float sql = x*x+y*y+z*z;
	if( sql )
		return sqrtf( sql );
	return 0;
};

float _GVector3::SqLength( void ) const
{
	return x*x+y*y+z*z;
};

const _GVector3& _GVector3::Normalize( void )
{
	float l = x*x+y*y+z*z;
	if( l == 0 )
		return *this;
	l = sqrtf( l );
	x = x / l;
	y = y / l;
	z = z / l;
	return *this;
};

const _GVector3& _GVector3::Difference( const GVECTOR3& orig, const GVECTOR3& dest )
{
	x = dest.x-orig.x;
	y = dest.y-orig.y;
	z = dest.z-orig.z;
	return *this;
};

_GVector3 _GVector3::Difference( const GVECTOR3& dest ) const
{
	return _GVector3(dest)-*this;
};

float _GVector3::Distance( const GVECTOR3& dest ) const
{
	return ( _GVector3(dest)-*this ).Length();
};

_GVector3 _GVector3::Cross( const GVECTOR3& vector ) const 
{ 
	return _GVector3(	y * vector.z - z * vector.y, 
						z * vector.x - x * vector.z, 
						x * vector.y - y * vector.x
					); 
};

const _GVector3& _GVector3::Cross( const GVECTOR3& vector1, const GVECTOR3& vector2 )
{ 
	x = vector1.y * vector2.z - vector1.z * vector2.y;
	y = vector1.z * vector2.x - vector1.x * vector2.z;
	z = vector1.x * vector2.y - vector1.y * vector2.x;
	return *this;
};

float _GVector3::AngleWith( const GVECTOR3& v ) const
{
   return (float)acos( ((*this) * v) / (this->Length()*GVector3(v).Length()) );
}

const _GVector3& _GVector3::operator*=(const GMATRIX3X3& m)
{
	float _x, _y, _z;

	_x = x*m._11 + y*m._21 + z*m._31;
	_y = x*m._12 + y*m._22 + z*m._32;
	_z = x*m._13 + y*m._23 + z*m._33;

	this->x = _x;
	this->y = _y;
	this->z = _z;
	return *this;
}

_GVector3 _GVector3::operator*(const GMATRIX3X3& m) const
{
	_GVector3 vResult;

	vResult.x = x*m._11 + y*m._21 + z*m._31;
	vResult.y = x*m._12 + y*m._22 + z*m._32;
	vResult.z = x*m._13 + y*m._23 + z*m._33;
	
	return vResult;
}

_GVector3& _GVector3::RotateX( float _theta_in_rads ) 
{
	float theta = _theta_in_rads;
	float cs = cosf(theta);
	float sn = sinf(theta);

	float pz = y * cs - z * sn; 
	float py = y * sn + z * cs;

	z = pz;
	y = py;
	return *this;
}

_GVector3& _GVector3::RotateY( float _theta_in_rads ) 
{
	float theta = _theta_in_rads;
	float cs = cosf(theta);
	float sn = sinf(theta);

	float px = x * cs - z * sn; 
	float pz = x * sn + z * cs;

	x = px;
	z = pz;
	return *this;
}

_GVector3& _GVector3::RotateZ( float _theta_in_rads ) 
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