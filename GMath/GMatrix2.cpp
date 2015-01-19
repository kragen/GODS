/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include <cstring>
#include <math.h>

using namespace god;

_GMatrix2x2::_GMatrix2x2( void ){};
_GMatrix2x2::_GMatrix2x2( const GMATRIX2X2& m )
{
	_11 = m._11; _12 = m._12; 
	_21 = m._21; _22 = m._22; 
};
_GMatrix2x2::_GMatrix2x2(	float __11, float __12, 
				float __21, float __22 )
{
	_11 = __11; _12 = __12; 
	_21 = __21; _22 = __22; 
};

// comparison operators
bool _GMatrix2x2::operator ==( const GMATRIX2X2 &m ) const
{
	char i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 4 )
	{
		if( data[i] != other[i] )
			return false;
		i++;
	}
	return true;
};
bool _GMatrix2x2::operator !=( const GMATRIX2X2 &m ) const
{
	char i=0;
	const float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 4 )
	{
		if( data[i] != other[i] )
			return true;
		i++;
	}
	return false;
};
// assignment operators
const _GMatrix2x2&	_GMatrix2x2::operator +=( const GMATRIX2X2 &m )
{
	char i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 4 )
	{
		data[i] += other[i];
		i++;
	}
	return *this;
};	
const _GMatrix2x2&	_GMatrix2x2::operator -=( const GMATRIX2X2 &m )
{
	char i=0;
	float* data	= &this->_11;
	const float* other	= &m._11;
	while( i < 4 )
	{
		data[i] -= other[i];
		i++;
	}
	return *this;
};

const _GMatrix2x2&	_GMatrix2x2::operator *=( const GMATRIX2X2 &m )
{
	_GMatrix2x2 temp;

	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &temp._11;

	memset( pM, 0, sizeof( _GMatrix2x2 ) );
	char i, j;
	for( i=0; i<2; i++ ) // rows
		for( j=0; j<2; j++ ) // cols
		{
			pM[2*i+j] += pA[2*i]   * pB[j];
			pM[2*i+j] += pA[2*i+1] * pB[2+j];
		};

	for( i=0; i<4; i++ )
		(&this->_11)[i] = (&temp._11)[i];
	return *this;
};	// matrix multiplication

//
//const _GMatrix2x2&	_GMatrix2x2::operator *=( const GMATRIX2X2 &m )
//{
//	_GMatrix2x2 temp;
//
//	const float *pA = &this->_11;
//	const float *pB = &m._11;
//	float *pM = &temp._11;
//
//	memset( pM, 0, sizeof( _GMatrix2x2 ) );
//	char i, j;
//	for( i=0; i<2; i++ ) // rows
//		for( j=0; j<3; j++ ) // cols
//		{
//			pM[3*i+j] += pA[3*i]   * pB[j];
//			pM[3*i+j] += pA[3*i+1] * pB[3+j];
//		};
//
//	for( i=0; i<4; i++ )
//		(&this->_11)[i] = (&temp._11)[i];
//	return *this;
//};	// matrix multiplication

const _GMatrix2x2&	_GMatrix2x2::operator *=( float scalar )
{
	char i=0;
	float* data	= &this->_11;
	while( i < 4 )
	{
		data[i] *= scalar;
		i++;
	}
	return *this;
};	// scalar multiplication

const _GMatrix2x2&	_GMatrix2x2::operator /=( float scalar )
{
	char i=0;
	float* data	= &this->_11;
	while( i < 4 )
	{
		data[i] /= scalar;
		i++;
	}
	return *this;
};

// 		
_GMatrix2x2	_GMatrix2x2::operator + ( const GMATRIX2X2 &m ) const
{
	_GMatrix2x2 result;
	char i=0;
	const float* data	= &this->_11;
	while( i < 4 )
	{
		(&result._11)[i] = data[i]+(&m._11)[i];
		i++;
	}
	return result;
};

_GMatrix2x2	_GMatrix2x2::operator - ( const GMATRIX2X2 &m ) const
{
	_GMatrix2x2 result;
	char i=0;
	const float* data	= &this->_11;
	while( i < 4 )
	{
		(&result._11)[i] = data[i]-(&m._11)[i];
		i++;
	}
	return result;
};	// matrix substraction

_GMatrix2x2	_GMatrix2x2::operator * ( const GMATRIX2X2 &m ) const
{
	_GMatrix2x2 mResult;
	const float *pA = &this->_11;
	const float *pB = &m._11;
	float *pM = &mResult._11;
	memset( pM, 0, sizeof( _GMatrix2x2 ) );
	char i, j;
	for( i=0; i<2; i++ ) // rows
		for( j=0; j<2; j++ ) // cols
		{
			pM[2*i+j] += pA[2*i]   * pB[j];
			pM[2*i+j] += pA[2*i+1] * pB[2+j];
		}
	return mResult;
};	// matrix multiplication

_GMatrix2x2	_GMatrix2x2::operator * ( float scalar ) const
{
	_GMatrix2x2 mResult;
	char i=0;
	const float* data	= &this->_11;
	float *pResult		= &mResult._11;
	while( i < 4 )
	{
		pResult[i] = data[i]*scalar;
		i++;
	}
	return mResult;
};	// scalar multiplication

_GMatrix2x2	_GMatrix2x2::operator / ( float scalar) const
{
	_GMatrix2x2 mResult;
	char i=0;
	const float* data	= &this->_11;
	float *pResult		= &mResult._11;
	while( i < 4 )
	{
		pResult[i] = data[i]/scalar;
		i++;
	}
	return mResult;
};	// scalar division
//GVector3	operator * ( const GVector3 &vc ) const;// vector multiplication // this is undefined under our convention V*M

// transpose m, save result in this instantce
void _GMatrix2x2::Identity( void )
{
	_11 = 1;	_12 = 
	_21 = 0;	_22 = 1;
}

// transpose m, save result in this instantce
void _GMatrix2x2::SetTranspose( const GMATRIX2X2 & m )
{
	_11 = m._11; _12 = m._21; 
	_21 = m._12; _22 = m._22; 
};		// transpose m, save result in this instantce

_GMatrix2x2 _GMatrix2x2::GetTranspose( void ) const
{ // I think this should work XD
	_GMatrix2x2 m;
	GMatrixTranspose( &m, this );
	//char i, j;
	//for( i=0; i<2; i++ )
	//	for( j=0; j<2; j++ )
	//		m.f[i][j] = f[j][i];
	return m;
};	

// transpose this, save result in this instantce
void _GMatrix2x2::Transpose( void )
{// diagonal stays the same, thus only 2 numbers remaining to transpose
	float c2 = _21;
	_21 = _12; // _12 is free now		__ set _21 (1st)
	_12 = c2; // store _21 into _12;	__ set _12 (2nd)
};	

void _GMatrix2x2::SetInverse( const GMATRIX2X2& m )
{
	float det = m._11*m._22 - m._12*m._21;
	if( 0 == det )
	{
#if defined(ANDROID)
#else
		throw( L"Division by 0 error.\n");
#endif
	}

	_11 = m._22/det;
	_12 = (-m._12)/det;
	_21 = (-m._21)/det;
	_22 = m._11/det;
};		// invert m, save result in this instance

_GMatrix2x2 _GMatrix2x2::GetInverse( void ) const
{
	_GMatrix2x2 result;
	float det = _11*_22 - _12*_21;
	if( 0 == det )
	{
#if defined(ANDROID)
#else
		throw( L"Division by 0 error.\n");
#endif
	}

	result._11 = _22/det;
	result._12 = (-_12)/det;
	result._21 = (-_21)/det;
	result._22 = _11/det;
	return result;
};

// A =	[a b] |   A^-1 = (1/det(A))[d b]
//		[c d] |                    [c a]
// det = ad-bc;
// Notes: copied det formula from GetDeterminant() to improve speed
void _GMatrix2x2::Invert( void )
{ 	
	float det = _11*_22 - _12*_21;
	if( 0 == det )
	{
#if defined(ANDROID)
#else
		throw( L"Division by 0 error.\n");
#endif
	}

	float f = _22/det;
	_22 = _11/det;
	_11 = f;
	_12 = (-_12)/det;
	_21 = (-_21)/det;
};

// A = [ a b ]
//     [ c d ] 
// det(A)= ad-bc;
float _GMatrix2x2::GetDeterminant( void )
{	
	return _11*_22 - _12*_21;
}


const _GMatrix2x2& _GMatrix2x2::LinearInterpolate( const GMATRIX2X2 & a, const GMATRIX2X2 & b, float prop )
{
	float *pT = &this->_11;
	const float *pA = &a._11;
	const float *pB = &b._11;
	for (char i = 0; i < 4; i++) 
	{
		pT[i] = pA[i] * (1-prop) + pB[i] * prop;
	}
	return *this;
}

// Transformation functions
void _GMatrix2x2::Scale( const GVECTOR2& Scale, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = Scale.x;	_12 =
		_21 = 0;		_22 = Scale.y;
		return;
	}
	_11 *= Scale.x;
	_22 *= Scale.y;
}; // Scale the current matrix

void _GMatrix2x2::Scale( float x, float y, bool bEraseContent )
{
	if( bEraseContent )
	{
		_11 = x;	_12 =		
		_21 = 0;	_22 = y;	
		return;
	}
	_11 *= x;
	_22 *= y;
};

// z axis
void _GMatrix2x2::RotationZ( float Angle )
{	
	float fCos = cosf(Angle);
	float fSin = sinf(Angle);
	_11 = fCos;		_12 = fSin;
	_21 = -fSin;	_22 = fCos; 
};


GVector2 _GMatrix2x2::TransformInverseDirection( const GVECTOR2& _v ) const
{
	GVector2 v( _v.x * _11 + _v.y * _12, 
				_v.x * _21 + _v.y * _22 );
	return v;
} // ApplyInverseRota

