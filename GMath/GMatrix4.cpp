/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include <cstring>
#include <math.h>

using namespace god;

_GMatrix4x4::_GMatrix4x4( void ){};
_GMatrix4x4::_GMatrix4x4( const GMATRIX4X4& m )
{
	_11 = m._11; _12 = m._12; _13 = m._13; _14 = m._14;
	_21 = m._21; _22 = m._22; _23 = m._23; _24 = m._24;
	_31 = m._31; _32 = m._32; _33 = m._33; _34 = m._34;
	_41 = m._41; _42 = m._42; _43 = m._43; _44 = m._44;
};
_GMatrix4x4::_GMatrix4x4(	float __11, float __12, float __13, float __14, 
				float __21, float __22, float __23, float __24, 
				float __31, float __32, float __33, float __34,
				float __41, float __42, float __43, float __44 
			)
{
	_11 = __11; _12 = __12; _13 = __13; _14 = __14;
	_21 = __21; _22 = __22; _23 = __23; _24 = __24;
	_31 = __31; _32 = __32; _33 = __33; _34 = __34;
	_41 = __41; _42 = __42; _43 = __43; _44 = __44;
};

_GMatrix4x4::_GMatrix4x4( float f )
{
	_11 = f; _12 = f; _13 = f; _14 = f;
	_21 = f; _22 = f; _23 = f; _24 = f;
	_31 = f; _32 = f; _33 = f; _34 = f;
	_41 = f; _42 = f; _43 = f; _44 = f;
};


// comparison operators
bool _GMatrix4x4::operator ==( const GMATRIX4X4 &m ) const
{
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		if( data[i] != other[i] )
			return false;
		i++;
	}
	return true;
};
bool _GMatrix4x4::operator !=( const GMATRIX4X4 &m ) const
{
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		if( data[i] != other[i] )
			return true;
		i++;
	}
	return false;
};
// assignment operators
const _GMatrix4x4&	_GMatrix4x4::operator =( const GMATRIX4X4 &m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		data[i] = other[i];
		i++;
	}
	return *this;
};	

// matrix multiplication
const _GMatrix4x4&	_GMatrix4x4::operator +=( const GMATRIX4X4 &m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		data[i] += other[i];
		i++;
	}
	return *this;
};	
const _GMatrix4x4&	_GMatrix4x4::operator -=( const GMATRIX4X4 &m )
{
	int i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		data[i] -= other[i];
		i++;
	}
	return *this;
};

const _GMatrix4x4&	_GMatrix4x4::operator *=( const GMATRIX4X4 &m )
{
	GMATRIX4X4 temp;

	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &temp._11;

	memset( pM, 0, sizeof( GMATRIX4X4 ) );
	int i, j;
	for( i=0; i<4; i++ ) // rows
	{
		for( j=0; j<4; j++ ) // cols
		{
			pM[4*i+j] += pA[4*i]   * pB[j];
			pM[4*i+j] += pA[4*i+1] * pB[4+j];
			pM[4*i+j] += pA[4*i+2] * pB[8+j];
			pM[4*i+j] += pA[4*i+3] * pB[12+j];
		}
	}
	for( i=0; i<16; i++ )
	{
		(&this->_11)[i] = (&temp._11)[i];
	}
	return *this;
};	// matrix multiplication

const _GMatrix4x4&	_GMatrix4x4::operator *=( float scalar )
{
	int i=0;
	float* data	= &this->_11;
	while( i < 16 )
	{
		data[i] *= scalar;
		i++;
	}
	return *this;
};	// scalar multiplication

const _GMatrix4x4&	_GMatrix4x4::operator /=( float scalar )
{
	int i=0;
	float* data	= &this->_11;
	while( i < 16 )
	{
		data[i] /= scalar;
		i++;
	}
	return *this;
};

// 		
_GMatrix4x4	_GMatrix4x4::operator + ( const GMATRIX4X4& m ) const
{
	_GMatrix4x4 result;
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		(&result._11)[i] = data[i]+other[i];
		i++;
	}
	return result;
};

_GMatrix4x4	_GMatrix4x4::operator - ( const GMATRIX4X4& m ) const
{
	_GMatrix4x4 result;
	int i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 16 )
	{
		(&result._11)[i] = data[i]-other[i];
		i++;
	}
	return result;
};	// matrix substraction

_GMatrix4x4	_GMatrix4x4::operator * ( const GMATRIX4X4& m ) const
{
	_GMatrix4x4 mResult;

	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &mResult._11;

	memset( pM, 0, sizeof( _GMatrix4x4 ) );
	int i, j;
	for( i=0; i<4; i++ ) // rows
	{
		for( j=0; j<4; j++ ) // cols
		{
			pM[4*i+j] += pA[4*i]   * pB[j];
			pM[4*i+j] += pA[4*i+1] * pB[4+j];
			pM[4*i+j] += pA[4*i+2] * pB[8+j];
			pM[4*i+j] += pA[4*i+3] * pB[12+j];
		}
	}
	return mResult;
};	// matrix multiplication

_GMatrix4x4	_GMatrix4x4::operator * ( float scalar ) const
{
	_GMatrix4x4 result;
	int i=0;
	const float* data	= &this->_11;
	while( i < 16 )
	{
		(&result._11)[i] = data[i]*scalar;
		i++;
	}
	return result;
};	// scalar multiplication

_GMatrix4x4	_GMatrix4x4::operator / ( float scalar) const
{
	_GMatrix4x4 result;
	int i=0;
	const float* data	= &this->_11;
	while( i < 16 )
	{
		(&result._11)[i] = data[i]/scalar;
		i++;
	}
	return result;
};	// scalar division
//GVector3	operator * ( const GVector3 &vc ) const;// vector multiplication // this is undefined under our convention V*M

// transpose m, save result in this instantce
void _GMatrix4x4::Identity( void )
{
	_11 = 1;	_12 =		_13 =	_14 = 
	_21 = 0;	_22 = 1;	_23 =	_24 = 
	_31 =		_32 = 0;	_33 = 1;_34 =
	_41 =		_42 =		_43 = 0;_44 = 1.0f;

}

// transpose m, save result in this instantce
void _GMatrix4x4::SetTranspose( const GMATRIX4X4& m )
{
	_11 = m._11; _12 = m._21; _13 = m._31; _14 = m._41;
	_21 = m._12; _22 = m._22; _23 = m._32; _24 = m._42;
	_31 = m._13; _32 = m._23; _33 = m._33; _34 = m._43;
	_41 = m._14; _42 = m._24; _43 = m._34; _44 = m._44;
};

_GMatrix4x4 _GMatrix4x4::GetTranspose( void ) const
{ // I think this should work XD
	GMATRIX4X4 m;
	GMatrixTranspose( &m, this );
	return m;
};	

// transpose this, save result in this instantce
void _GMatrix4x4::Transpose( void )
{// diagonal stays the same, thus 12 numbers remaining to transpose
	float c1, c2, c3, c4, c5, c6; 
	// store a backup of some numbers
	c1 = _23;
	c2 = _21;
	c3 = _31;
	c4 = _41;
	c5 = _43;
	c6 = _42;

	_23 = _32; // _32 is free now	__ set _23 (1st)
	_21 = _12; // _12 is free now	__ set _21 (2nd)
	_31 = _13; // _13 is free		__ set _31 (3rd)
	_41 = _14; // _14 is free		__ set _41 (4th)
	_43 = _34;
	_42 = _24;

	_12 = c2; // store _21 into _12, c2 is free now;			__ set _12 (4th)
	_13 = c3; // store _31 into _13, c3 becomes available		__ set _13 (5th)
	_32 = c1; // restore the backup in the _32 now invalid		__ set _32 (6th)
	_14 = c4; // restore the backup in the _14 now invalid		__ set _14 (7th)
	_34 = c5;
	_24 = c6;
};	

// invert m, save result in this instance
void _GMatrix4x4::SetInverse( const GMATRIX4X4& m )
{
	_GMatrix4x4 mTranspose;
	float     fTemp[12], fDet;

	// calculate transposed matrix
	mTranspose.SetTranspose(m);

	fTemp[ 0]  = mTranspose._33 * mTranspose._44;
	fTemp[ 1]  = mTranspose._34 * mTranspose._43;
	fTemp[ 2]  = mTranspose._32 * mTranspose._44;
	fTemp[ 3]  = mTranspose._34 * mTranspose._42;
	fTemp[ 4]  = mTranspose._32 * mTranspose._43;
	fTemp[ 5]  = mTranspose._33 * mTranspose._42;
	fTemp[ 6]  = mTranspose._31 * mTranspose._44;
	fTemp[ 7]  = mTranspose._34 * mTranspose._41;
	fTemp[ 8]  = mTranspose._31 * mTranspose._43;
	fTemp[ 9]  = mTranspose._33 * mTranspose._41;
	fTemp[10]  = mTranspose._31 * mTranspose._42;
	fTemp[11]  = mTranspose._32 * mTranspose._41;

	this->_11  = fTemp[0]*mTranspose._22 + fTemp[3]*mTranspose._23 + fTemp[4] *mTranspose._24;
	this->_11 -= fTemp[1]*mTranspose._22 + fTemp[2]*mTranspose._23 + fTemp[5] *mTranspose._24;
	this->_12  = fTemp[1]*mTranspose._21 + fTemp[6]*mTranspose._23 + fTemp[9] *mTranspose._24;
	this->_12 -= fTemp[0]*mTranspose._21 + fTemp[7]*mTranspose._23 + fTemp[8] *mTranspose._24;
	this->_13  = fTemp[2]*mTranspose._21 + fTemp[7]*mTranspose._22 + fTemp[10]*mTranspose._24;
	this->_13 -= fTemp[3]*mTranspose._21 + fTemp[6]*mTranspose._22 + fTemp[11]*mTranspose._24;
	this->_14  = fTemp[5]*mTranspose._21 + fTemp[8]*mTranspose._22 + fTemp[11]*mTranspose._23;
	this->_14 -= fTemp[4]*mTranspose._21 + fTemp[9]*mTranspose._22 + fTemp[10]*mTranspose._23;
	this->_21  = fTemp[1]*mTranspose._12 + fTemp[2]*mTranspose._13 + fTemp[5] *mTranspose._14;
	this->_21 -= fTemp[0]*mTranspose._12 + fTemp[3]*mTranspose._13 + fTemp[4] *mTranspose._14;
	this->_22  = fTemp[0]*mTranspose._11 + fTemp[7]*mTranspose._13 + fTemp[8] *mTranspose._14;
	this->_22 -= fTemp[1]*mTranspose._11 + fTemp[6]*mTranspose._13 + fTemp[9] *mTranspose._14;
	this->_23  = fTemp[3]*mTranspose._11 + fTemp[6]*mTranspose._12 + fTemp[11]*mTranspose._14;
	this->_23 -= fTemp[2]*mTranspose._11 + fTemp[7]*mTranspose._12 + fTemp[10]*mTranspose._14;
	this->_24  = fTemp[4]*mTranspose._11 + fTemp[9]*mTranspose._12 + fTemp[10]*mTranspose._13;
	this->_24 -= fTemp[5]*mTranspose._11 + fTemp[8]*mTranspose._12 + fTemp[11]*mTranspose._13;

	fTemp[ 0]  = mTranspose._13 * mTranspose._24;
	fTemp[ 1]  = mTranspose._14 * mTranspose._23;
	fTemp[ 2]  = mTranspose._12 * mTranspose._24;
	fTemp[ 3]  = mTranspose._14 * mTranspose._22;
	fTemp[ 4]  = mTranspose._12 * mTranspose._23;
	fTemp[ 5]  = mTranspose._13 * mTranspose._22;
	fTemp[ 6]  = mTranspose._11 * mTranspose._24;
	fTemp[ 7]  = mTranspose._14 * mTranspose._21;
	fTemp[ 8]  = mTranspose._11 * mTranspose._23;
	fTemp[ 9]  = mTranspose._13 * mTranspose._21;
	fTemp[10]  = mTranspose._11 * mTranspose._22;
	fTemp[11]  = mTranspose._12 * mTranspose._21;

	this->_31  = fTemp[0] *mTranspose._42 + fTemp[3] *mTranspose._43 + fTemp[4] *mTranspose._44;
	this->_31 -= fTemp[1] *mTranspose._42 + fTemp[2] *mTranspose._43 + fTemp[5] *mTranspose._44;
	this->_32  = fTemp[1] *mTranspose._41 + fTemp[6] *mTranspose._43 + fTemp[9] *mTranspose._44;
	this->_32 -= fTemp[0] *mTranspose._41 + fTemp[7] *mTranspose._43 + fTemp[8] *mTranspose._44;
	this->_33  = fTemp[2] *mTranspose._41 + fTemp[7] *mTranspose._42 + fTemp[10]*mTranspose._44;
	this->_33 -= fTemp[3] *mTranspose._41 + fTemp[6] *mTranspose._42 + fTemp[11]*mTranspose._44;
	this->_34  = fTemp[5] *mTranspose._41 + fTemp[8] *mTranspose._42 + fTemp[11]*mTranspose._43;
	this->_34 -= fTemp[4] *mTranspose._41 + fTemp[9] *mTranspose._42 + fTemp[10]*mTranspose._43;
	this->_41  = fTemp[2] *mTranspose._33 + fTemp[5] *mTranspose._34 + fTemp[1] *mTranspose._32;
	this->_41 -= fTemp[4] *mTranspose._34 + fTemp[0] *mTranspose._32 + fTemp[3] *mTranspose._33;
	this->_42  = fTemp[8] *mTranspose._34 + fTemp[0] *mTranspose._31 + fTemp[7] *mTranspose._33;
	this->_42 -= fTemp[6] *mTranspose._33 + fTemp[9] *mTranspose._34 + fTemp[1] *mTranspose._31;
	this->_43  = fTemp[6] *mTranspose._32 + fTemp[11]*mTranspose._34 + fTemp[3] *mTranspose._31;
	this->_43 -= fTemp[10]*mTranspose._34 + fTemp[2] *mTranspose._31 + fTemp[7] *mTranspose._32;
	this->_44  = fTemp[10]*mTranspose._33 + fTemp[4] *mTranspose._31 + fTemp[9] *mTranspose._32;
	this->_44 -= fTemp[8] *mTranspose._32 + fTemp[11]*mTranspose._33 + fTemp[5] *mTranspose._31;

	fDet = mTranspose._11*this->_11 + 
		 mTranspose._12*this->_12 + 
		 mTranspose._13*this->_13 +
		 mTranspose._14*this->_14;

	fDet = 1/fDet;

	this->_11 *= fDet;  
	this->_12 *= fDet;  
	this->_13 *= fDet;  
	this->_14 *= fDet;

	this->_21 *= fDet;  
	this->_22 *= fDet;  
	this->_23 *= fDet;  
	this->_24 *= fDet;

	this->_31 *= fDet;  
	this->_32 *= fDet;  
	this->_33 *= fDet;  
	this->_34 *= fDet;

	this->_41 *= fDet;  
	this->_42 *= fDet;  
	this->_43 *= fDet;  
	this->_44 *= fDet;
};	

_GMatrix4x4 _GMatrix4x4::GetInverse( void ) const
{
	_GMatrix4x4 result;
	result.SetInverse( *this );
	return result;
};


void _GMatrix4x4::Invert( void )
{	
	_GMatrix4x4 mTranspose;
	float     fTemp[12], fDet;

	// calculate transposed matrix
	mTranspose.SetTranspose(*this);

	fTemp[ 0]  = mTranspose._33 * mTranspose._44;
	fTemp[ 1]  = mTranspose._34 * mTranspose._43;
	fTemp[ 2]  = mTranspose._32 * mTranspose._44;
	fTemp[ 3]  = mTranspose._34 * mTranspose._42;
	fTemp[ 4]  = mTranspose._32 * mTranspose._43;
	fTemp[ 5]  = mTranspose._33 * mTranspose._42;
	fTemp[ 6]  = mTranspose._31 * mTranspose._44;
	fTemp[ 7]  = mTranspose._34 * mTranspose._41;
	fTemp[ 8]  = mTranspose._31 * mTranspose._43;
	fTemp[ 9]  = mTranspose._33 * mTranspose._41;
	fTemp[10]  = mTranspose._31 * mTranspose._42;
	fTemp[11]  = mTranspose._32 * mTranspose._41;

	this->_11  = fTemp[0]*mTranspose._22 + fTemp[3]*mTranspose._23 + fTemp[4] *mTranspose._24;
	this->_11 -= fTemp[1]*mTranspose._22 + fTemp[2]*mTranspose._23 + fTemp[5] *mTranspose._24;
	this->_12  = fTemp[1]*mTranspose._21 + fTemp[6]*mTranspose._23 + fTemp[9] *mTranspose._24;
	this->_12 -= fTemp[0]*mTranspose._21 + fTemp[7]*mTranspose._23 + fTemp[8] *mTranspose._24;
	this->_13  = fTemp[2]*mTranspose._21 + fTemp[7]*mTranspose._22 + fTemp[10]*mTranspose._24;
	this->_13 -= fTemp[3]*mTranspose._21 + fTemp[6]*mTranspose._22 + fTemp[11]*mTranspose._24;
	this->_14  = fTemp[5]*mTranspose._21 + fTemp[8]*mTranspose._22 + fTemp[11]*mTranspose._23;
	this->_14 -= fTemp[4]*mTranspose._21 + fTemp[9]*mTranspose._22 + fTemp[10]*mTranspose._23;
	this->_21  = fTemp[1]*mTranspose._12 + fTemp[2]*mTranspose._13 + fTemp[5] *mTranspose._14;
	this->_21 -= fTemp[0]*mTranspose._12 + fTemp[3]*mTranspose._13 + fTemp[4] *mTranspose._14;
	this->_22  = fTemp[0]*mTranspose._11 + fTemp[7]*mTranspose._13 + fTemp[8] *mTranspose._14;
	this->_22 -= fTemp[1]*mTranspose._11 + fTemp[6]*mTranspose._13 + fTemp[9] *mTranspose._14;
	this->_23  = fTemp[3]*mTranspose._11 + fTemp[6]*mTranspose._12 + fTemp[11]*mTranspose._14;
	this->_23 -= fTemp[2]*mTranspose._11 + fTemp[7]*mTranspose._12 + fTemp[10]*mTranspose._14;
	this->_24  = fTemp[4]*mTranspose._11 + fTemp[9]*mTranspose._12 + fTemp[10]*mTranspose._13;
	this->_24 -= fTemp[5]*mTranspose._11 + fTemp[8]*mTranspose._12 + fTemp[11]*mTranspose._13;

	fTemp[ 0]  = mTranspose._13 * mTranspose._24;
	fTemp[ 1]  = mTranspose._14 * mTranspose._23;
	fTemp[ 2]  = mTranspose._12 * mTranspose._24;
	fTemp[ 3]  = mTranspose._14 * mTranspose._22;
	fTemp[ 4]  = mTranspose._12 * mTranspose._23;
	fTemp[ 5]  = mTranspose._13 * mTranspose._22;
	fTemp[ 6]  = mTranspose._11 * mTranspose._24;
	fTemp[ 7]  = mTranspose._14 * mTranspose._21;
	fTemp[ 8]  = mTranspose._11 * mTranspose._23;
	fTemp[ 9]  = mTranspose._13 * mTranspose._21;
	fTemp[10]  = mTranspose._11 * mTranspose._22;
	fTemp[11]  = mTranspose._12 * mTranspose._21;

	this->_31  = fTemp[0] *mTranspose._42 + fTemp[3] *mTranspose._43 + fTemp[4] *mTranspose._44;
	this->_31 -= fTemp[1] *mTranspose._42 + fTemp[2] *mTranspose._43 + fTemp[5] *mTranspose._44;
	this->_32  = fTemp[1] *mTranspose._41 + fTemp[6] *mTranspose._43 + fTemp[9] *mTranspose._44;
	this->_32 -= fTemp[0] *mTranspose._41 + fTemp[7] *mTranspose._43 + fTemp[8] *mTranspose._44;
	this->_33  = fTemp[2] *mTranspose._41 + fTemp[7] *mTranspose._42 + fTemp[10]*mTranspose._44;
	this->_33 -= fTemp[3] *mTranspose._41 + fTemp[6] *mTranspose._42 + fTemp[11]*mTranspose._44;
	this->_34  = fTemp[5] *mTranspose._41 + fTemp[8] *mTranspose._42 + fTemp[11]*mTranspose._43;
	this->_34 -= fTemp[4] *mTranspose._41 + fTemp[9] *mTranspose._42 + fTemp[10]*mTranspose._43;
	this->_41  = fTemp[2] *mTranspose._33 + fTemp[5] *mTranspose._34 + fTemp[1] *mTranspose._32;
	this->_41 -= fTemp[4] *mTranspose._34 + fTemp[0] *mTranspose._32 + fTemp[3] *mTranspose._33;
	this->_42  = fTemp[8] *mTranspose._34 + fTemp[0] *mTranspose._31 + fTemp[7] *mTranspose._33;
	this->_42 -= fTemp[6] *mTranspose._33 + fTemp[9] *mTranspose._34 + fTemp[1] *mTranspose._31;
	this->_43  = fTemp[6] *mTranspose._32 + fTemp[11]*mTranspose._34 + fTemp[3] *mTranspose._31;
	this->_43 -= fTemp[10]*mTranspose._34 + fTemp[2] *mTranspose._31 + fTemp[7] *mTranspose._32;
	this->_44  = fTemp[10]*mTranspose._33 + fTemp[4] *mTranspose._31 + fTemp[9] *mTranspose._32;
	this->_44 -= fTemp[8] *mTranspose._32 + fTemp[11]*mTranspose._33 + fTemp[5] *mTranspose._31;

	fDet = mTranspose._11*this->_11 + 
		 mTranspose._12*this->_12 + 
		 mTranspose._13*this->_13 +
		 mTranspose._14*this->_14;

	fDet = 1/fDet;

	this->_11 *= fDet;  
	this->_12 *= fDet;  
	this->_13 *= fDet;  
	this->_14 *= fDet;

	this->_21 *= fDet;  
	this->_22 *= fDet;  
	this->_23 *= fDet;  
	this->_24 *= fDet;

	this->_31 *= fDet;  
	this->_32 *= fDet;  
	this->_33 *= fDet;  
	this->_34 *= fDet;

	this->_41 *= fDet;  
	this->_42 *= fDet;  
	this->_43 *= fDet;  
	this->_44 *= fDet;
};

float _GMatrix4x4::GetDeterminant( void )
{ 
	_GMatrix4x4 mTranspose;
	float     fTemp[12], fDet;

	mTranspose.SetTranspose( *this );

	fTemp[ 0]  = mTranspose._33 * mTranspose._44;
	fTemp[ 1]  = mTranspose._34 * mTranspose._43;
	fTemp[ 2]  = mTranspose._32 * mTranspose._44;
	fTemp[ 3]  = mTranspose._34 * mTranspose._42;
	fTemp[ 4]  = mTranspose._32 * mTranspose._43;
	fTemp[ 5]  = mTranspose._33 * mTranspose._42;
	fTemp[ 6]  = mTranspose._31 * mTranspose._44;
	fTemp[ 7]  = mTranspose._34 * mTranspose._41;
	fTemp[ 8]  = mTranspose._31 * mTranspose._43;
	fTemp[ 9]  = mTranspose._33 * mTranspose._41;
	fTemp[10]  = mTranspose._31 * mTranspose._42;
	fTemp[11]  = mTranspose._32 * mTranspose._41;

	this->_11  = fTemp[0]*mTranspose._22 + fTemp[3]*mTranspose._23 + fTemp[4] *mTranspose._24;
	this->_11 -= fTemp[1]*mTranspose._22 + fTemp[2]*mTranspose._23 + fTemp[5] *mTranspose._24;
	this->_12  = fTemp[1]*mTranspose._21 + fTemp[6]*mTranspose._23 + fTemp[9] *mTranspose._24;
	this->_12 -= fTemp[0]*mTranspose._21 + fTemp[7]*mTranspose._23 + fTemp[8] *mTranspose._24;
	this->_13  = fTemp[2]*mTranspose._21 + fTemp[7]*mTranspose._22 + fTemp[10]*mTranspose._24;
	this->_13 -= fTemp[3]*mTranspose._21 + fTemp[6]*mTranspose._22 + fTemp[11]*mTranspose._24;
	this->_14  = fTemp[5]*mTranspose._21 + fTemp[8]*mTranspose._22 + fTemp[11]*mTranspose._23;
	this->_14 -= fTemp[4]*mTranspose._21 + fTemp[9]*mTranspose._22 + fTemp[10]*mTranspose._23;

	fDet = mTranspose._11*this->_11 + 
		 mTranspose._12*this->_12 + 
		 mTranspose._13*this->_13 +
		 mTranspose._14*this->_14;
	return fDet;
}

// Transformation functions
void _GMatrix4x4::Scale( const GVECTOR3& Scale, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = Scale.x;	_12 =			_13 =			_14 = 
		_21 = 0;		_22 = Scale.y;	_23 =			_24 = 
		_31 =			_32 = 0;		_33 = Scale.z;	_34 = 
		_41 =			_42 =			_43 = 0;		_44 = 1.0f;
		return;
	}
	_11 *= Scale.x;
	_22 *= Scale.y;
	_33 *= Scale.z;
}; // Scale the current matrix

void _GMatrix4x4::Scale( float x, float y, float z, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = x;	_12 =		_13 =		_14 = 
		_21 = 0;	_22 = y;	_23 =		_24 = 
		_31 =		_32 = 0;	_33 = z;	_34 = 
		_41 =		_42 =		_43 = 0;	_44 = 1.0f;
		return;
	}
	_11 *= x;
	_22 *= y;
	_33 *= z;
};



// x axis
void _GMatrix4x4::RotationX( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = 1.0f;	_12 =		_13 = 
	_21 = 0.0f;	_22 = fCos; _23 = fSin;
	_31 = 0.0f;	_32 = -fSin;_33 = fCos;
	// 
	_41 = _42 = _43 = _14 = _24 = _34 = 0.0f;
	_44 = 1.0f;
};

// y axis
void _GMatrix4x4::RotationY( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = fCos;	_12 = 0.0f;	_13 = -fSin;
	_21 = 0.0f;	_22 = 1.0f; _23 = 0.0f;
	_31 = fSin;	_32 = 0.0f;	_33 = fCos;
	// 
	_41 = _42 = _43 = _14 = _24 = _34 = 0.0f;
	_44 = 1.0f;
};

// z axis
void _GMatrix4x4::RotationZ( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = fCos;		_12 = fSin;	_13 = 0.0f;
	_21 = -fSin;	_22 = fCos; _23 = 
	_31 =			_32 = 0.0f;	_33 = 1.0f;
	// 
	_41 = _42 = _43 = _14 = _24 = _34 = 0.0f;
	_44 = 1.0f;
};

GVector3 _GMatrix4x4::Transform( const GVECTOR3& v ) const
{
	_GVector3 vResult;
	float w;
	vResult.x = v.x*_11 + v.y*_21 + v.z*_31 + _41;
	vResult.y = v.x*_12 + v.y*_22 + v.z*_32 + _42;
	vResult.z = v.x*_13 + v.y*_23 + v.z*_33 + _43;
	w = v.x*_14 + v.y*_24 + v.z*_34 + _44;

	vResult.x = vResult.x/w;
	vResult.y = vResult.y/w;
	vResult.z = vResult.z/w;
	//vResult.w = 1.0f;
	return vResult;
}
//
// Transform the given direction vector by this matrix.
// 
GVector4 _GMatrix4x4::TransformDirection( const GVECTOR4& vector ) const
{
	return GVector4(
		vector.x * _11 + vector.y * _21 + vector.z * _31,
		vector.x * _12 + vector.y * _22 + vector.z * _32,
		vector.x * _13 + vector.y * _23 + vector.z * _33,
		1.0f
	);
}

// 
// Transform the given direction vector by this matrix.
// 
GVector3 _GMatrix4x4::TransformDirection( const GVECTOR3& vector ) const
{
	return GVector3(
		vector.x * _11 + vector.y * _21 + vector.z * _31,
		vector.x * _12 + vector.y * _22 + vector.z * _32,
		vector.x * _13 + vector.y * _23 + vector.z * _33
	);
}

GVector4 _GMatrix4x4::TransformInverseDirection( const GVECTOR4& _v ) const
{
	GVector4 v( _v.x * _11 + _v.y * _12 + _v.z * _13, 
				_v.x * _21 + _v.y * _22 + _v.z * _23,
				_v.x * _31 + _v.y * _32 + _v.z * _33, 1.0f );
	return v;
} // ApplyInverseRota

GVector3 _GMatrix4x4::TransformInverseDirection( const GVECTOR3& _v ) const
{
	GVector3 v( _v.x * _11 + _v.y * _12 + _v.z * _13, 
				_v.x * _21 + _v.y * _22 + _v.z * _23,
				_v.x * _31 + _v.y * _32 + _v.z * _33 );
	return v;
} // ApplyInverseRota

////---------------------------------------------
//
//
void _GMatrix4x4::SetTranslation( const GVECTOR3& vTranslation, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = 1;	_12 =		_13 =	_14 = 
		_21 = 0;	_22 = 1;	_23 =	_24 = 
		_31 =		_32 = 0;	_33 = 1;_34 = 0;
		/*_41 =		_42 =		_43 */	_44 = 1.0f;
	}
	_41	= vTranslation.x;
	_42	= vTranslation.y;
	_43	= vTranslation.z;
};

GVector3 _GMatrix4x4::GetTranslation( void ) const
{
	return GVector3( _41, _42, _43 );
};


// make a projection matrix 
void _GMatrix4x4::FieldOfView( float fAngle, float fAspect, float zn, float zf )
{
	float fTan = tanf( fAngle/2 );
	_11 = 1 / ( fAspect * fTan );
	_22 = 1 / fTan;
	_33 = zf / ( zf - zn );
	_34 = 1;
	_43 = ( -( zf*zn ) ) / ( zf - zn );

	_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _41 = _42 = _44 = 0.0f;
//	return *this;
} // FoV

// look from given position at given point
void _GMatrix4x4::LookAt(const GVECTOR3& vPosition, const GVECTOR3& vTarget, const GVECTOR3& vUp) 
{
	GVector3 F( GVector3( vTarget ) - vPosition );
	F.Normalize();

	GVector3 R;
	R.Cross( vUp, F );
	R.Normalize();

	GVector3 U;
	U.Cross( F, R );
	U.Normalize();

	// Fill in the view matrix entries.
	//float x = -vPosition.Dot( R );
	//float y = -vPosition.Dot( U );
	//float z = -vPosition.Dot( F );

	_11 = R.x;					_12 = U.x;					_13 = F.x;					_14 = 0.0f;
	_21 = R.y;					_22 = U.y;					_23 = F.y;					_24 = 0.0f;
	_31 = R.z;					_32 = U.z;					_33 = F.z;					_34 = 0.0f;
	_41 = -GVectorDot( &vPosition, &R ); 	
	_42 = -GVectorDot( &vPosition, &U ); 	
	_43 = -GVectorDot( &vPosition, &F );	
	_44 = 1.0f;
} // LookAt

//----------------------------------------------------------------//

void _GMatrix4x4::View3D( const GVECTOR3& vPosition, const GVECTOR3& vRight, const GVECTOR3& vUp, const GVECTOR3& vFront )
{
	_11 = vRight.x;	_12 = vUp.x; _13 = vFront.x; _14 = 0.0f;
	_21 = vRight.y;	_22 = vUp.y; _23 = vFront.y; _24 = 0.0f;
	_31 = vRight.z;	_32 = vUp.z; _33 = vFront.z; _34 = 0.0f;
	_41 = -GVectorDot( &vPosition, &vRight );	
	_42 = -GVectorDot( &vPosition, &vUp );	
	_43 = -GVectorDot( &vPosition, &vFront );	
	_44 = 1.0f;
};



// make a billboard matrix for given position and direction
void _GMatrix4x4::Billboard(const GVECTOR3& vPos, const GVECTOR3& vDir, const GVECTOR3& vWorldUp) 
{
	GVector3 vUp, vRight; 
	float fAngle = GVectorDot( &vWorldUp, &vDir );

	vUp = GVector3( vWorldUp ) - GVector3( vDir )*fAngle;
	vUp.Normalize();

	GVectorCross( &vRight, &vUp, &vDir );

	_11 = vRight.x;	_12 = vRight.y;	_13 = vRight.z;	_14=0.0f; 
	_21 = vUp.x; 	_22 = vUp.y; 	_23 = vUp.z; 	_24=0.0f; 
	_31 = vDir.x;	_32 = vDir.y;	_33 = vDir.z;	_34=0.0f; 
	_41 = vPos.x;	_42 = vPos.y;	_43 = vPos.z;	_44=1.0f;
} // Billboard



void _GMatrix4x4::Rotation( float x, float y, float z )
{
	float sr, sp, sy, cr, cp, cy;

//	Identity();

	sy = sinf( z );
	cy = cosf( z );
	sp = sinf( y );
	cp = cosf( y );
	sr = sinf( x );
	cr = cosf( x );

	_11 = cp*cy;
	_12 = cp*sy;
	_13 = -sp;
	_21 = sr*sp*cy+cr*-sy;
	_22 = sr*sp*sy+cr*cy;
	_23 = sr*cp;
	_31 = (cr*sp*cy+-sr*-sy);
	_32 = (cr*sp*sy+-sr*cy);
	_33 = cr*cp;

	_14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
	_44 = 1.0f;

}

void _GMatrix4x4::Rotation(const GVECTOR3 &vc) 
{
	float sr, sp, sy, cr, cp, cy;

	_14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
	_44 = 1.0f;

	sy = sinf( vc.z );
	cy = cosf( vc.z );
	sp = sinf( vc.y );
	cp = cosf( vc.y );
	sr = sinf( vc.x );
	cr = cosf( vc.x );

	_11 = cp*cy;
	_12 = cp*sy;
	_13 = -sp;
	_21 = sr*sp*cy+cr*-sy;
	_22 = sr*sp*sy+cr*cy;
	_23 = sr*cp;
	_31 = (cr*sp*cy+-sr*-sy);
	_32 = (cr*sp*sy+-sr*cy);
	_33 = cr*cp;


//	return *this;
} // Rota

void _GMatrix4x4::RotationArbitraryAxis(const GVECTOR3& _vcAxis, float a) 
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

	_14 = _24 = _34 = _41 = _42 = _43 = 0.0f;
	_44 = 1.0f;
}

void _GMatrix4x4::SetOrientation( const GQUATERNION& qo )
{
    //_11 = 1-2*orientation.j*orientation.j - 2*orientation.k*orientation.k;
    //_21 = 2*orientation.i*orientation.j - 2*orientation.r*orientation.k;
    //_31 = 2*orientation.i*orientation.k + 2*orientation.r*orientation.j;

    //_12 = 2*orientation.i*orientation.j + 2*orientation.r*orientation.k;
    //_22 = 1-2*orientation.i*orientation.i - 2*orientation.k*orientation.k;
    //_32 = 2*orientation.j*orientation.k - 2*orientation.r*orientation.i;

    //_13 = 2*orientation.i*orientation.k - 2*orientation.r*orientation.j;
    //_23 = 2*orientation.j*orientation.k + 2*orientation.r*orientation.i;
    //_33 = 1-2*orientation.i*orientation.i - 2*orientation.j*orientation.j;

	// set matrix to identity
	_41 = _42 = _43 = _14 = _24 = _34 = 0;
	_44 = 1.0f;

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

const _GMatrix4x4& _GMatrix4x4::LinearInterpolate( const GMATRIX4X4& a, const GMATRIX4X4& b, float prop )
{
	float *pT = &this->_11;
	const float *pA = &a._11;
	const float *pB = &b._11;
	for (int i = 0; i < 16; i++) 
	{
		pT[i] = pA[i] * (1-prop) + pB[i] * prop;
	}
	return *this;
}

void _GMatrix4x4::InverseTranslateVector(float * fpVec) const
{
	//assert(fpVec);
	fpVec[0] = fpVec[0] - this->_41;
	fpVec[1] = fpVec[1] - this->_42;
	fpVec[2] = fpVec[2] - this->_43;
}

//-------------------------------------------------------------
//- InvRotateVec
//- Rotate avector using the inverse of the matrix
//-------------------------------------------------------------
void _GMatrix4x4::InverseRotateVector(float * fpVec) const
{
	float tmp[3];

	tmp[0] = fpVec[0] * this->_11 + fpVec[1] * this->_21 + fpVec[2] * this->_31;
	tmp[1] = fpVec[0] * this->_12 + fpVec[1] * this->_22 + fpVec[2] * this->_32;
	tmp[2] = fpVec[0] * this->_13 + fpVec[1] * this->_23 + fpVec[2] * this->_33;

	memcpy(fpVec, tmp, sizeof(float)*3);
}
