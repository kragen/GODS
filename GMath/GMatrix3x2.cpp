#include "GVector_object.h"
#include <memory.h>
//#include <math.h>

using namespace god;

_GMatrix2x3::_GMatrix2x3( void ){};
_GMatrix2x3::_GMatrix2x3( const GMATRIX2X3& m )
{
	_11 = m._11; _12 = m._12; _13 = m._13; 
	_21 = m._21; _22 = m._22; _23 = m._23; 
};
_GMatrix2x3::_GMatrix2x3(	float __11, float __12, float __13,
							float __21, float __22, float __23 )
{
	_11 = __11; _12 = __12; _13 = __13; 
	_21 = __21; _22 = __22; _23 = __23; 
};

_GMatrix3x2::_GMatrix3x2( void ){};
_GMatrix3x2::_GMatrix3x2( const GMATRIX3X2& m )
{
	_11 = m._11; _12 = m._12; 
	_21 = m._21; _22 = m._22; 
	_31 = m._31; _32 = m._32; 
};

_GMatrix3x2::_GMatrix3x2(	float __11, float __12, 
							float __21, float __22, 
							float __31, float __32 
							)
{
	_11 = __11; _12 = __12; 
	_21 = __21; _22 = __22; 
	_31 = __31; _32 = __32; 
};

_GMatrix2x3	GMatrix3x2::GetTranspose( void ) const
{
	GMatrix2x3 result;
	result._11 = _11; result._12 = _21; result._13 = _31; 
	result._21 = _12; result._22 = _22; result._23 = _32;
	return result;
}; // transpose m, return result in new copy

GMatrix3x2 _GMatrix2x3::GetTranspose( void ) const
{
	GMatrix3x2 result;
	result._11 = _11; result._12 = _21; result._31 = _13; 
	result._21 = _12; result._22 = _22; result._32 = _23;
	return result;
}; // transpose m, return result in new copy

const _GMatrix3x2&	_GMatrix3x2::operator *=( const GMATRIX2X2 &m )
{
	_GMatrix3x2 temp;
	_GMatrix3x2* out_mResult=&temp;
	_GMatrix3x2* in_mLeft = this;
	const GMATRIX2X2* in_mRight = &m;

	// first row
	out_mResult->_11 
		= in_mLeft->_11 * in_mRight->_11
		+ in_mLeft->_12 * in_mRight->_21;
	out_mResult->_12 
		= in_mLeft->_11 * in_mRight->_12
		+ in_mLeft->_12 * in_mRight->_22;

	// second row 
	out_mResult->_21 
		= in_mLeft->_21 * in_mRight->_11
		+ in_mLeft->_22 * in_mRight->_21;
	out_mResult->_22 
		= in_mLeft->_21 * in_mRight->_12
		+ in_mLeft->_22 * in_mRight->_22;

	// third row 
	out_mResult->_31 
		= in_mLeft->_31 * in_mRight->_11
		+ in_mLeft->_32 * in_mRight->_21;
	out_mResult->_32 
		= in_mLeft->_31 * in_mRight->_12
		+ in_mLeft->_32 * in_mRight->_22;

	return *this;
};	// matrix multiplication
