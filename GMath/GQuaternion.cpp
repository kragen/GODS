/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include <math.h>

using namespace god;
_GQuaternion::_GQuaternion( void ){};
_GQuaternion::_GQuaternion( const GQUATERNION& q )
{ x = q.x; y = q.y; z = q.z; w = q.w; };
_GQuaternion::_GQuaternion( const float* f )
{ x = f[0]; y = f[1]; z = f[2]; w = f[3]; };
_GQuaternion::_GQuaternion( float _x, float _y, float _z, float _w )
{ x = _x; y = _y; z = _z; w = _w; };
		
bool _GQuaternion::operator ==( const GQUATERNION& v ) const
{
	if( v.x == x && v.y == y && v.z == z && v.w == w )
		return true;
	return false;
}

bool _GQuaternion::operator !=( const GQUATERNION& v ) const
{
	if( v.x == x && v.y == y && v.z == z && v.w == w )
		return false;
	return true;
}

const _GQuaternion& _GQuaternion::operator =( const GQUATERNION& q )
{
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
	return *this;
}

const _GQuaternion& _GQuaternion::operator +=( const GQUATERNION& q )
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

const _GQuaternion& _GQuaternion::operator -=( const GQUATERNION& q )
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

const _GQuaternion& _GQuaternion::operator *=( const GQUATERNION& q )
{
   float _x, _y, _z, _w;

	_w = w*q.w - x*q.x - y*q.y - z*q.z;
	_x = w*q.x + x*q.w + y*q.z - z*q.y;
	_y = w*q.y + y*q.w + z*q.x - x*q.z;
	_z = w*q.z + z*q.w + x*q.y - y*q.x;

	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

const _GQuaternion& _GQuaternion::operator *=( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

const _GQuaternion& _GQuaternion::operator /=( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
};


_GQuaternion _GQuaternion::operator +  ( const GQUATERNION& q ) const
{
	_GQuaternion r;
	r.x = x+q.x;
	r.y = y+q.y;
	r.z = z+q.z;
	r.w = w+q.w;
	return r;
};
_GQuaternion _GQuaternion::operator -  ( const GQUATERNION& q ) const
{
	_GQuaternion r;
	r.x = x-q.x;
	r.y = y-q.y;
	r.z = z-q.z;
	r.w = w-q.w;
	return r;
};
_GQuaternion _GQuaternion::operator *  ( const GQUATERNION& q ) const
{
	_GQuaternion r;
   
   r.w = w*q.w - x*q.x - y*q.y - z*q.z;
   r.x = w*q.x + x*q.w + y*q.z - z*q.y;
   r.y = w*q.y + y*q.w + z*q.x - x*q.z;
   r.z = w*q.z + z*q.w + x*q.y - y*q.x;

   return r;
};
_GQuaternion _GQuaternion::operator *  ( const GVECTOR4& v ) const
{
   return _GQuaternion(  w*v.x + y*v.z - z*v.y,
						w*v.y + z*v.x - x*v.z,
						w*v.z + x*v.y - y*v.x,
					  -(x*v.x + y*v.y + z*v.z) );
};

_GQuaternion _GQuaternion::operator *  ( const GVECTOR3& v ) const
{
	return _GQuaternion(  w*v.x + y*v.z - z*v.y,
					w*v.y + z*v.x - x*v.z,
					w*v.z + x*v.y - y*v.x,
					-(x*v.x + y*v.y + z*v.z) );
}

_GQuaternion _GQuaternion::operator *  ( float f ) const
{
	_GQuaternion r;

	r.x = x*f;
	r.y = y*f;
	r.z = z*f;
	r.w = w*f;

	return r;
};

_GQuaternion _GQuaternion::operator /  ( float f ) const
{
	_GQuaternion r;

	r.x = x/f;
	r.y = y/f;
	r.z = z/f;
	r.w = w/f;

	return r;
};

// Unary operators
_GQuaternion _GQuaternion::operator -( void ) const
{
	_GQuaternion r;

	r.x = -x;
	r.y = -y;
	r.z = -z;
	r.w = -w;

	return r;
};


float _GQuaternion::Element( uint8_t n ) const
{
	//( n > 3 ) ? throw("error, retrieving unexisting element on _GQuaternion") : false;
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
	}
	return 0;
}; // return n element from vector
const _GQuaternion& _GQuaternion::SetElement( uint8_t n, float fValue )
{
	//( n > 3 ) ? throw("error, setting unexisting element on _GQuaternion") : false;
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

//----------------------------------------------------------------//
void _GQuaternion::AddScaledVector( const GVECTOR4& vector, float scale )
{
	GQuaternion q(vector.x * scale, vector.y * scale, vector.z * scale, 0.0f);
	q *= *this;
	w += q.w * 0.5f;
	x += q.x * 0.5f;
	y += q.y * 0.5f;
	z += q.z * 0.5f;
}

//----------------------------------------------------------------//
const _GQuaternion& _GQuaternion::AddScaledVector( const GVECTOR3& vector, float scale )
{
	GQuaternion q(vector.x * scale, vector.y * scale, vector.z * scale, 0.0f);
	q *= *this;
	w += q.w * 0.5f;
	x += q.x * 0.5f;
	y += q.y * 0.5f;
	z += q.z * 0.5f;
	return *this;
}

void _GQuaternion::SetRotation(const GQUATERNION &q1, const GQUATERNION &q2) 
{ 
	_GQuaternion t = GQuaternion(q1)*q2*(~GQuaternion(q1)); 
	x=t.x; y=t.y; z=t.z; w=t.w; 
}
GVector3 _GQuaternion::RotateVector( const GVECTOR3 &v ) const
{ 
	_GQuaternion t(x, y, z, w);
	_GQuaternion r = t*v*(~t);
	return GVector3(r.x, r.y, r.z); 
}

float _GQuaternion::Dot( const GQUATERNION& q ) const 
{ 
	return x * q.x + y * q.y + z * q.z + w * q.w; 
};

float _GQuaternion::Length( void ) const
{
	return sqrt( x*x+y*y+z*z+w*w );
};

float _GQuaternion::SqLength( void ) const
{
	return x*x+y*y+z*z+w*w;
};

const _GQuaternion& _GQuaternion::Normalize( void )
{
	//float l = sqrt(x*x+y*y+z*z+w*w);
	//x	= x / l;
	//y	= y / l;
	//z	= z / l;
	//w	= w / l;
	float m=x*x + y*y + z*z + w*w;
	if( m != 0 )
	{
		m=sqrtf(m);
		x /= m; y /= m; z /= m; w /= m;
	}
	else
		w = 1.0f;
	return *this;
};

// Convert from Euler Angles
const _GQuaternion& _GQuaternion::MakeFromEuler(float fPitch, float fYaw, float fRoll)
{
	 //Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	 //and multiply those together.
	 //the calculation below does the same, just shorter

	//float p = fPitch * GMATH_PI_180 * .5;
	//float y = fYaw * GMATH_PI_180 * .5;
	//float r = fRoll * GMATH_PI_180 * .5;

	//float sinp = sin(p);
	//float siny = sin(y);
	//float sinr = sin(r);
	//float cosp = cos(p);
	//float cosy = cos(y);
	//float cosr = cos(r);

	//this->w = cosr * cosp * cosy + sinr * sinp * siny;
	//this->x = sinr * cosp * cosy - cosr * sinp * siny;
	//this->y = cosr * sinp * cosy + sinr * cosp * siny;
	//this->z = cosr * cosp * siny - sinr * sinp * cosy;

	float cX, cY, cZ, sX, sY, sZ, cYcZ, sYsZ, cYsZ, sYcZ;

	fPitch *= 0.5f;//
	fYaw   *= 0.5f;//
	fRoll  *= 0.5f;//

	cX = cosf(fPitch);
	cY = cosf(fYaw);
	cZ = cosf(fRoll);

	sX = sinf(fPitch);
	sY = sinf(fYaw);
	sZ = sinf(fRoll);

	cYcZ = cY * cZ;
	sYsZ = sY * sZ;
	cYsZ = cY * sZ;
	sYcZ = sY * cZ;

	w = cX * cYcZ + sX * sYsZ;
	x = sX * cYcZ - cX * sYsZ;
	y = cX * sYcZ + sX * cYsZ;
	z = cX * cYsZ - sX * sYcZ;

	return Normalize();
}

// Convert from Euler Angles
const _GQuaternion& _GQuaternion::MakeFromEuler( const GVector3& v )
{
	// float fPitch=v.x, fYaw=v.y, fRoll=v.z;
	return MakeFromEuler(v.x, v.y, v.z);
}

// converts quaternion into euler angles
void _GQuaternion::GetEulers(float *fPitch, float *fYaw, float *fRoll) 
{
	double   r11, r21, r31, r32, r33, r12, r13;
	double   q00, q11, q22, q33;
	double   tmp;

	q00 = w * w;
	q11 = x * x;
	q22 = y * y;
	q33 = z * z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (x*y + w*z);
	r31 = 2 * (x*z - w*y);
	r32 = 2 * (y*z + w*x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (x*y - w*z);
		r13 = 2 * (x*z + w*y);
		static const float GMATH_PI2		= 1.57079632679489F;
		*fPitch = 0.0f;
		*fYaw   = (float)-((GMATH_PI2) * r31/tmp);
		*fRoll  = (float)atan2(-r12, -r31*r13);
	}
	else 
	{
		*fPitch = (float)atan2(r32, r33);
		*fYaw   = (float)asin(-r31);
		*fRoll  = (float)atan2(r21, r11);
	}
} // 
//----------------------------------------------------------------//
//
//// get a rotation matrix out of the quaternion
void _GQuaternion::GetMatrix(GMATRIX4X4 *pMat) const
{
   float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

   // set matrix to identity
   pMat->_41 = pMat->_42 = pMat->_43 = pMat->_14 = pMat->_24 = pMat->_34 = 0;
   pMat->_44 = 1.0f;

   x2 = x + x; 
   y2 = y + y; 
   z2 = z + z;

   xx = x * x2;   
   xy = x * y2;  
   xz = x * z2;

   yy = y * y2;   
   yz = y * z2;   
   zz = z * z2;

   wx = w * x2;   
   wy = w * y2;   
   wz = w * z2;

   //pMat->_12 = xy - wz;
   //pMat->_13 = xz + wy;
   pMat->_11 = 1.0f - (yy + zz);
   pMat->_21 = xy - wz;
   pMat->_31 = xz + wy;

   //pMat->_21 = xy + wz;
   //pMat->_23 = yz - wx;
   pMat->_12 = xy + wz;
   pMat->_22 = 1.0f - (xx + zz);
   pMat->_32 = yz - wx;

   //pMat->_31 = xz - wy;
   //pMat->_32 = yz + wx;
   pMat->_13 = xz - wy;
   pMat->_23 = yz + wx;
   pMat->_33 = 1.0f - (xx + yy);
} // GetMatrix

////----------------------------------------------------------------//
void _GQuaternion::GetMatrix(GMATRIX3X3 *pMat) const
{
   float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

   //// set matrix to identity
   //pMat->_14 = pMat->_24 = pMat->_34 = 0;
   //pMat->_44 = 1.0f;

   x2 = x + x; 
   y2 = y + y; 
   z2 = z + z;

   xx = x * x2;   
   xy = x * y2;  
   xz = x * z2;

   yy = y * y2;   
   yz = y * z2;   
   zz = z * z2;

   wx = w * x2;   
   wy = w * y2;   
   wz = w * z2;

   //pMat->_12 = xy - wz;
   //pMat->_13 = xz + wy;
   pMat->_11 = 1.0f - (yy + zz);
   pMat->_21 = xy - wz;
   pMat->_31 = xz + wy;

   //pMat->_21 = xy + wz;
   //pMat->_23 = yz - wx;
   pMat->_12 = xy + wz;
   pMat->_22 = 1.0f - (xx + zz);
   pMat->_32 = yz - wx;

   //pMat->_31 = xz - wy;
   //pMat->_32 = yz + wx;
   pMat->_13 = xz - wy;
   pMat->_23 = yz + wx;
   pMat->_33 = 1.0f - (xx + yy);
} // GetMatrix

const _GQuaternion& _GQuaternion::LinearInterpolate( const GQUATERNION &p, const GQUATERNION &q, float fTime )
{
	*this = (GQuaternion(q)-p)*fTime+p;
	return *this;
};

const _GQuaternion& _GQuaternion::SLERP( const GQUATERNION &p, const GQUATERNION &q, float fTime )
{
	//const float * q0 = p.data; 
	//const float * q1 = q.data; 
 //
	//Calculate the dot product 
	float fDot = x * q.x + y * q.y + z * q.z + w * q.w;// q0[0] * q1[0] + q0[1] * q1[1] + q0[2] * q1[2] + q0[3] * q1[3]; 

	_GQuaternion q_;
	if(fDot < 0.0f) 
	{ 
		q_ = -GQuaternion(q); 
		fDot = -fDot; 
	} 
 
	if(fDot < 1.00001f && fDot > 0.99999f) 
	{ 
		return LinearInterpolate(p, q_, fTime); 
	} 
 
	//calculate the angle between the quaternions  
	float fTheta = acosf(fDot); 
 
	*this =(GQuaternion(p) * sinf(fTheta * (1 - fTime)) + q_ * sinf(fTheta * fTime))/sinf(fTheta); 
	return *this;
//	return (p * sinf(fTheta * (1 - fTime)) + q_ * sinf(fTheta * fTime))/sinf(fTheta); 
};

