/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include <cstring>
#include <math.h>

using namespace god;

_GMatrix3x3::_GMatrix3x3( void ){};
_GMatrix3x3::_GMatrix3x3( const GMATRIX3X3& m )
{
	_11 = m._11; _12 = m._12; _13 = m._13;
	_21 = m._21; _22 = m._22; _23 = m._23;
	_31 = m._31; _32 = m._32; _33 = m._33;
};

_GMatrix3x3::_GMatrix3x3(	float __11, float __12, float __13, 
				float __21, float __22, float __23, 
				float __31, float __32, float __33 )
{
	_11 = __11; _12 = __12; _13 = __13;
	_21 = __21; _22 = __22; _23 = __23;
	_31 = __31; _32 = __32; _33 = __33;
};
// comparison operators
bool _GMatrix3x3::operator ==( const GMATRIX3X3 &m ) const
{
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 9 )
	{
		if( data[i] != other[i] )
			return false;
		i++;
	}
	return true;
};
bool _GMatrix3x3::operator !=( const GMATRIX3X3 &m ) const
{
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 9 )
	{
		if( data[i] != other[i] )
			return true;
		i++;
	}
	return false;
};
// assignment operators
const _GMatrix3x3&	_GMatrix3x3::operator =( const GMATRIX3X3& m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 9 )
	{
		data[i] = other[i];
		i++;
	}
	return *this;
};	

// matrix multiplication
const _GMatrix3x3&	_GMatrix3x3::operator +=( const GMATRIX3X3 &m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 9 )
	{
		data[i] += other[i];
		i++;
	}
	return *this;
};	
const _GMatrix3x3&	_GMatrix3x3::operator -=( const GMATRIX3X3 &m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 9 )
	{
		data[i] -= other[i];
		i++;
	}
	return *this;
};

const _GMatrix3x3&	_GMatrix3x3::operator *=( const GMATRIX3X3 &m )
{
	_GMatrix3x3 temp;


	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &temp._11;

	memset( pM, 0, sizeof( _GMatrix3x3 ) );
	int i, j;
	for( i=0; i<3; i++ ) // rows
	{
		for( j=0; j<3; j++ ) // cols
		{
			pM[3*i+j] += pA[3*i]   * pB[j];
			pM[3*i+j] += pA[3*i+1] * pB[3+j];
			pM[3*i+j] += pA[3*i+2] * pB[6+j];
		}
	}
	float* data	= &this->_11;
	for( i=0; i<9; i++ )
	{
		data[i] = pM[i];
	}
	return *this;
};	// matrix multiplication

const _GMatrix3x3&	_GMatrix3x3::operator *=( float scalar )
{
	int i=0;
	float* data	= &this->_11;
	while( i < 9 )
	{
		data[i] *= scalar;
		i++;
	}
	return *this;
};	// scalar multiplication

const _GMatrix3x3&	_GMatrix3x3::operator /=( float scalar )
{
	int i=0;
	float* data	= &this->_11;
	while( i < 9 )
	{
		data[i] /= scalar;
		i++;
	}
	return *this;
};

// 		
_GMatrix3x3	_GMatrix3x3::operator + ( const GMATRIX3X3 &m ) const
{
	_GMatrix3x3 mResult;
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	float* result	= &mResult._11;
	while( i < 9 )
	{
		result[i] = data[i]+other[i];
		i++;
	}
	return mResult;
};

_GMatrix3x3	_GMatrix3x3::operator - ( const GMATRIX3X3 &m ) const
{
	_GMatrix3x3 mResult;
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	float* result	= &mResult._11;
	while( i < 9 )
	{
		result[i] = data[i]-other[i];
		i++;
	}
	return mResult;
};	// matrix substraction

_GMatrix3x3	_GMatrix3x3::operator * ( const GMATRIX3X3 &m ) const
{
	_GMatrix3x3 mResult;
	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &mResult._11;

	memset( pM, 0, sizeof( _GMatrix3x3 ) );

	for( int i=0; i<3; i++ ) // rows
	{
		for( int j=0; j<3; j++ ) // cols
		{
			pM[3*i+j] += pA[3*i]   * pB[j];
			pM[3*i+j] += pA[3*i+1] * pB[3+j];
			pM[3*i+j] += pA[3*i+2] * pB[6+j];
		}
	}
	return mResult;
};	// matrix multiplication

_GMatrix3x3	_GMatrix3x3::operator * ( float scalar ) const
{
	_GMatrix3x3 mResult;
	int i=0;
	const float* data	= &this->_11;
	float* result	= &mResult._11;
	while( i < 9 )
	{
		result[i] = data[i]*scalar;
		i++;
	}
	return mResult;
};	// scalar multiplication

_GMatrix3x3	_GMatrix3x3::operator / ( float scalar) const
{
	_GMatrix3x3 mResult;
	int i=0;
	const float* data	= &this->_11;
	float* result	= &mResult._11;
	while( i < 9 )
	{
		result[i] = data[i]/scalar;
		i++;
	}
	return mResult;
};	// scalar division
//GVector3	operator * ( const GVector3 &vc ) const;// vector multiplication // this is undefined under our convention V*M

// transpose m, save result in this instantce
void _GMatrix3x3::Identity( void )
{
	_11 = 1;	_12 =		_13 = 
	_21 = 0;	_22 = 1;	_23 = 
	_31 =		_32 = 0;	_33 = 1;
}

// transpose m, save result in this instantce
void _GMatrix3x3::SetTranspose( const GMATRIX3X3& m )
{
	_11 = m._11; _12 = m._21; _13 = m._31;
	_21 = m._12; _22 = m._22; _23 = m._32;
	_31 = m._13; _32 = m._23; _33 = m._33;

//	_11 = m._11;	_22 = m._22;	_33 = m._33;

};		// transpose m, save result in this instantce

_GMatrix3x3 _GMatrix3x3::GetTranspose( void ) const
{ // I think this should work XD
	_GMatrix3x3 m;
	GMatrixTranspose( &m, this );
	//char i, j;
	//for( i=0; i<3; i++ )
	//	for( j=0; j<3; j++ )
	//		m.f[i][j] = f[j][i];
	return m;
};	

// transpose this, save result in this instantce
void _GMatrix3x3::Transpose( void )
{// diagonal stays the same, thus only 6 numbers remaining to transpose
	float c1, c2, c3; 
	// store a backup of some numbers
	c1 = _23; 
	c2 = _21; 
	c3 = _31;  

	_21 = _12; // _12 is free now	__ set _21 (1st)
	_31 = _13; // _13 is free		__ set _31 (2nd)
	_23 = _32;	// we _23 with _32	__ set _23 (3rd)

	_13 = c3; // store _31 into _13, c3 becomes available		__ set _13 (4th)
	_12 = c2; // store _21 into _12, c2 is free now;			__ set _12 (5th)
	_32 = c1; // store the backup in the _32 now invalid		__ set _32 (6th)
};	

// invert m, save result in this instance
void _GMatrix3x3::SetInverse( const GMATRIX3X3 & m )
{
	float A, B, C, D, E, F, G, H, I;
	float det = m._11*(m._22*m._33-m._23*m._32) 
		+ m._12*(m._23*m._31-m._33*m._21) 
		+ m._13*(m._21*m._32-m._22*m._31);

	A = (m._22*m._33 - m._23*m._32);/**/ B = (m._23*m._31 - m._21*m._33);/**/ C = (m._21*m._32 - m._22*m._31);
	D = (m._13*m._32 - m._12*m._33);/**/ E = (m._11*m._33 - m._13*m._31);/**/ F = (m._12*m._31 - m._11*m._32);
	G = (m._12*m._23 - m._13*m._22);/**/ H = (m._13*m._21 - m._11*m._23);/**/ I = (m._11*m._22 - m._12*m._21);
	// 
	_11 = A/det; _12 = D/det; _13 = G/det;
	_21 = B/det; _22 = E/det; _23 = H/det;
	_31 = C/det; _32 = F/det; _33 = I/det;
};	

_GMatrix3x3 _GMatrix3x3::GetInverse( void ) const
{
	_GMatrix3x3 result;
	result.SetInverse( *this );
	return result;
};

// [ a b c ] = A
// [ d e f ]
// [ g h i ]
// A^-1 = 1/det [A B C](T) = 1/det	[A D G]
//				[D E F]				[B E H]
//				[G H I]				[C F I]
// A = (ei-fh) // B = (fg-di) // C = (dh-eg)
// D = (ch-bi) // E = (ai-cg) // F = (bg-ah)
// G = (bf-ce) // H = (cd-af) // I = (ae-bd)
// det(A) = a(ei-fh)+b(fg-id)+c(dh-eg)
// Notes: copy det function from GetDeterminant() for faster processing
void _GMatrix3x3::Invert( void )
{	
	float A, B, C, D, E, F, G, H, I;
	float det = _11*(_22*_33-_23*_32) + _12*(_23*_31-_33*_21) + _13*(_21*_32-_22*_31);

	A = (_22*_33 - _23*_32);	/**/ B = (_23*_31 - _21*_33);	/**/ C = (_21*_32 - _22*_31);
	D = (_13*_32 - _12*_33);	/**/ E = (_11*_33 - _13*_31);	/**/ F = (_12*_31 - _11*_32);
	G = (_12*_23 - _13*_22);	/**/ H = (_13*_21 - _11*_23);	/**/ I = (_11*_22 - _12*_21);
	// 
	_11 = A/det; _12 = D/det; _13 = G/det;
	_21 = B/det; _22 = E/det; _23 = H/det;
	_31 = C/det; _32 = F/det; _33 = I/det;
};

// I copied this from wikipedia while i learn better ways to calculate the determinant
// [ a b c ] = A
// [ d e f ]
// [ g h i ] 
// det(A) = aei+bfg+cdh-afh-bdi-ceg;
// could also be (and probably involving less operations):
// det(A) = a(ei-fh)+b(fg-id)+c(dh-eg)
float _GMatrix3x3::GetDeterminant( void )
{ 
	return _11*(_22*_33-_23*_32) + _12*(_23*_31-_33*_21) + _13*(_21*_32-_22*_31);
//	return _11*_22*_33 + _12*_23*_31 + _13*_21*_32 
//		- _11*_23*_32 - _12*_21*_33 - _13*_22*_31;
}

const GMatrix3x3& _GMatrix3x3::LinearInterpolate( const GMATRIX3X3& a, const GMATRIX3X3& b, float prop )
{
	float *pT = &this->_11;
	const float *pA = &a._11;
	const float *pB = &b._11;
	for (int i = 0; i < 9; i++) 
	{
		pT[i] = pA[i] * (1.0f-prop) + pB[i] * prop;
	}
	return *this;
}

// Transformation functions
void _GMatrix3x3::Scale( const GVECTOR2& Scale, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = Scale.x;	_12 =			_13 =
		_21 = 0;		_22 = Scale.y;	_23 =
		_31 =			_32 = 0;		_33 = 1.0f;// Scale.z;	
		return;
	}
	_11 *= Scale.x;
	_22 *= Scale.y;
	//_33 *= Scale.z;
}; // Scale the current matrix

void _GMatrix3x3::Scale( float x, float y, float z, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = x;	_12 =		_13 =
		_21 = 0;	_22 = y;	_23 =
		_31 =		_32 = 0;	_33 = z;
		return;
	}
	_11 *= x;
	_22 *= y;
	_33 *= z;
};


// x axis
void _GMatrix3x3::RotationX( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = 1.0f;	_12 =		_13 = 
	_21 = 0.0f;	_22 = fCos; _23 = fSin;
	_31 = 0.0f;	_32 = -fSin;_33 = fCos;
};

// x axis
void _GMatrix3x3::RotationY( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = fCos;	_12 = 0.0f;	_13 = -fSin;
	_21 = 0.0f;	_22 = 1.0f; _23 = 0.0f;
	_31 = fSin;	_32 = 0.0f;	_33 = fCos;
};

// x axis
void _GMatrix3x3::RotationZ( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = fCos;		_12 = fSin;	_13 = 0.0f;
	_21 = -fSin;	_22 = fCos; _23 = 
	_31 =			_32 = 0.0f;	_33 = 1.0f;
};

void _GMatrix3x3::RotationArbitraryAxis(const GVECTOR3&_vcAxis, float a) 
{
	GVector3 vcAxis = _vcAxis;
	float fCos = cosf(a);
	float fSin = sinf(a);
	float fSum = 1.0f - fCos;
   
	if( vcAxis.SqLength() != 1.0 )
		vcAxis.Normalize();

	_11 = (vcAxis.x * vcAxis.x) * fSum + fCos;
	_12 = (vcAxis.x * vcAxis.y) * fSum - (vcAxis.z * fSin);
	_13 = (vcAxis.x * vcAxis.z) * fSum + (vcAxis.y * fSin);

	_21 = (vcAxis.y * vcAxis.x) * fSum + (vcAxis.z * fSin);
	_22 = (vcAxis.y * vcAxis.y) * fSum + fCos ;
	_23 = (vcAxis.y * vcAxis.z) * fSum - (vcAxis.x * fSin);

	_31 = (vcAxis.z * vcAxis.x) * fSum - (vcAxis.y * fSin);
	_32 = (vcAxis.z * vcAxis.y) * fSum + (vcAxis.x * fSin);
	_33 = (vcAxis.z * vcAxis.z) * fSum + fCos;
}


void _GMatrix3x3::SetOrientation( const GQUATERNION& qo )
{
	// set matrix to identity
//	_41 = _42 = _43 = _14 = _24 = _34 = 0;
//	_44 = 1.0f;

   float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

   x2 = qo.x + qo.x; 
   y2 = qo.y + qo.y; 
   z2 = qo.z + qo.z;

   xx = qo.x * x2;   
   xy = qo.x * y2;  
   xz = qo.x * z2;

   yy = qo.y * y2;   
   yz = qo.y * z2;   
   zz = qo.z * z2;

   wx = qo.w * x2;   
   wy = qo.w * y2;   
   wz = qo.w * z2;

   //pMat->_12 = xy - wz;
   //pMat->_13 = xz + wy;
   _11 = 1.0f - (yy + zz);
   _21 = xy - wz;
   _31 = xz + wy;

   //pMat->_21 = xy + wz;
   //pMat->_23 = yz - wx;
   _12 = xy + wz;
   _22 = 1.0f - (xx + zz);
   _32 = yz - wx;

   //pMat->_31 = xz - wy;
   //pMat->_32 = yz + wx;
   _13 = xz - wy;
   _23 = yz + wx;
   _33 = 1.0f - (xx + yy);
}

////---------------------------------------------
//
//
void _GMatrix3x3::SetTranslation( const GVECTOR2& vTranslation, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = 1;	_12 =		_13 =	
		_21 = 0;	_22 = 1;	_23 = 0;	
								_33 = 1;	
		/*_41 =		_42 =		_43 */	
	}
	_31	= vTranslation.x;
	_32	= vTranslation.y;
//	_43	= vTranslation.z;
};

GVector2 _GMatrix3x3::GetTranslation( void ) const
{
	return GVector2( _31, _32 );
};


GVector4 _GMatrix3x3::Transform( const GVECTOR4& v ) const
{
	GVector4 vResult(	v.x*_11 + v.y*_21 + v.z*_31,
						v.x*_12 + v.y*_22 + v.z*_32, 
						v.x*_13 + v.y*_23 + v.z*_33, 1.0f );
	return vResult;
}

GVector4 _GMatrix3x3::TransformInverseDirection( const GVECTOR4& _v ) const
{
	GVector4 vResult( _v.x * _11 + _v.y * _12 + _v.z * _13, 
				_v.x * _21 + _v.y * _22 + _v.z * _23,
				_v.x * _31 + _v.y * _32 + _v.z * _33, 1.0f );
	return vResult;
} // ApplyInverseRota

GVector3 _GMatrix3x3::TransformInverseDirection( const GVECTOR3& _v ) const
{
	GVector3 vResult( _v.x * _11 + _v.y * _12 + _v.z * _13, 
				_v.x * _21 + _v.y * _22 + _v.z * _23,
				_v.x * _31 + _v.y * _32 + _v.z * _33 );
	return vResult;
} // ApplyInverseRota



//void _GMatrix3x3::RotaArbi( const GVector3 &_vcAxis, float a )
//{
//	GVector3 vcAxis = _vcAxis;
//	float fCos = real_cos(a);
//	float fSin = real_sin(a);
//	float fSum = 1.0f - fCos;
//
//	if (vcAxis.SqLength() != 1.0)
//	  vcAxis.Normalize();
//
//	_11 = (vcAxis.x * vcAxis.x) * fSum + fCos;
//	_12 = (vcAxis.x * vcAxis.y) * fSum - (vcAxis.z * fSin);
//	_13 = (vcAxis.x * vcAxis.z) * fSum + (vcAxis.y * fSin);
//
//	_21 = (vcAxis.y * vcAxis.x) * fSum + (vcAxis.z * fSin);
//	_22 = (vcAxis.y * vcAxis.y) * fSum + fCos ;
//	_23 = (vcAxis.y * vcAxis.z) * fSum - (vcAxis.x * fSin);
//
//	_31 = (vcAxis.z * vcAxis.x) * fSum - (vcAxis.y * fSin);
//	_32 = (vcAxis.z * vcAxis.y) * fSum + (vcAxis.x * fSin);
//	_33 = (vcAxis.z * vcAxis.z) * fSum + fCos;
//


//// [0 1 2] [0 3 6]
//// [3 4 5] [1 4 7]
//// [6 7 8] [2 5 8]
///// 
// // Sets the value of the matrix from inertia tensor values.
// // 
//void setInertiaTensorCoeffs(real ix, real iy, real iz,
//    real ixy=0, real ixz=0, real iyz=0)
//{
//    data[0] = ix; // _11
//
//	data[1] = -ixy;
//	data[3] = -ixy;
//
//	data[6] = -ixz
//	data[2] = -ixz;
//
//	data[4] = iy; // _22
//
//	data[5] = -iyz;
//	data[7] = -iyz;
//
//	data[8] = iz; // _33
//}


 //
 // Sets the value of the matrix from inertia tensor values.
 //
void _GMatrix3x3::SetCoeffsAngularMass(float ix, float iy, float iz,
    float ixy, float ixz, float iyz)
{
    _11 = ix;
    _21 = _12 = -ixy;
    _31 = _13 = -ixz;
    _22 = iy;
    _32 = _23 = -iyz;
    _33 = iz;
}

// 
// Sets the value of the matrix as an inertia tensor of
// a rectangular block aligned with the body's coordinate 
// system with the given axis half-sizes and mass.
// 
void _GMatrix3x3::SetBlockAngularMass(const GVECTOR3 &halfSizes, float mass)
{
    GVector3 squares = halfSizes;
	squares.x *= halfSizes.x; squares.y *= halfSizes.y; squares.z *= halfSizes.z;
    SetCoeffsAngularMass(0.3f*mass*(squares.y + squares.z),
        0.3f*mass*(squares.x + squares.z),
        0.3f*mass*(squares.x + squares.y));
}

//-------------------------------------------------------------
//- InvRotateVec
//- Rotate avector using the inverse of the matrix
//-------------------------------------------------------------
void _GMatrix3x3::InverseRotateVector(float * fpVec) const
{
	float tmp[3];

	tmp[0] = fpVec[0] * this->_11 + fpVec[1] * this->_21 + fpVec[2] * this->_31;
	tmp[1] = fpVec[0] * this->_12 + fpVec[1] * this->_22 + fpVec[2] * this->_32;
	tmp[2] = fpVec[0] * this->_13 + fpVec[1] * this->_23 + fpVec[2] * this->_33;

	memcpy(fpVec, tmp, sizeof(float)*3);
}
