#include "GVector_core.h"

#include "GDebugger_core.h"
#include "GCore_printf.h"

using namespace god;

GMATRIX2X2*	god::GMatrixAdd			( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight )
{ 
	out_mResult->_11 = in_mLeft->_11 + in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 + in_mRight->_12;
	out_mResult->_21 = in_mLeft->_21 + in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 + in_mRight->_22;
	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixSubstract	( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight ) 
{
	out_mResult->_11 = in_mLeft->_11 - in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 - in_mRight->_12;
	out_mResult->_21 = in_mLeft->_21 - in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 - in_mRight->_22;
	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixMultiply	( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight )
{ 
	out_mResult->_11 
		= in_mLeft->_11*in_mRight->_11
		+ in_mLeft->_12*in_mRight->_21;
	out_mResult->_12 
		= in_mLeft->_11*in_mRight->_12
		+ in_mLeft->_12*in_mRight->_22;

	out_mResult->_21 
		= in_mLeft->_21*in_mRight->_11
		+ in_mLeft->_22*in_mRight->_21;
	out_mResult->_22 
		= in_mLeft->_21*in_mRight->_12
		+ in_mLeft->_22*in_mRight->_22;

	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixInverse		( GMATRIX2X2* out_mResult, float* out_fDeterminant, const GMATRIX2X2* in_Matrix )
{ 
	float fDet = in_Matrix->_11*in_Matrix->_22 
		- in_Matrix->_12*in_Matrix->_21;
	if( 0 == fDet )
	{
		warning_printf( "Division by determinant zero.");
		return out_mResult;
	}

	out_mResult->_11 = in_Matrix->_22/fDet;
	out_mResult->_12 = (-in_Matrix->_12)/fDet;
	out_mResult->_21 = (-in_Matrix->_21)/fDet;
	out_mResult->_22 = in_Matrix->_11/fDet;
	if( out_fDeterminant )
		*out_fDeterminant = fDet;
	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixTranspose		( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_Matrix ) 
{ 
	out_mResult->_11 = in_Matrix->_11;
	out_mResult->_12 = in_Matrix->_21;

	out_mResult->_21 = in_Matrix->_12;
	out_mResult->_22 = in_Matrix->_22;

	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixTransposeInPlace( GMATRIX2X2* inout_mResult ) 
{ 
	float c1;
	// store a backup of some numbers
	c1 = inout_mResult->_21;	inout_mResult->_21 = inout_mResult->_12;
	inout_mResult->_12 = c1;
	return inout_mResult; 
};

GMATRIX2X2*	god::GMatrixMultiply	( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, float in_fScalar ) 
{ 
	out_mResult->_11 = in_mLeft->_11 * in_fScalar;
	out_mResult->_12 = in_mLeft->_12 * in_fScalar;
	out_mResult->_21 = in_mLeft->_21 * in_fScalar;
	out_mResult->_22 = in_mLeft->_22 * in_fScalar;
	return out_mResult; 
};

GVECTOR2*	god::GMatrixTransform	( GVECTOR2* out_mResult, const GVECTOR2* in_vLeft, const GMATRIX2X2* in_mRight ) 
{ 
	out_mResult->x
		= in_vLeft->x*in_mRight->_11
		+ in_vLeft->y*in_mRight->_21;
	out_mResult->y
		= in_vLeft->x*in_mRight->_12
		+ in_vLeft->y*in_mRight->_22;

	return out_mResult; 
};

GVECTOR2*	god::GMatrixTransformInPlace	( GVECTOR2* out_mResult, const GMATRIX2X2* in_mRight ) 
{ 
	float _x = out_mResult->x*in_mRight->_11 + out_mResult->y*in_mRight->_21
		, _y = out_mResult->x*in_mRight->_12 + out_mResult->y*in_mRight->_22;
	out_mResult->x = _x;
	out_mResult->y = _y;
	return out_mResult; 
};

GMATRIX2X2*	god::GMatrixDivide		( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, float in_fScalar )
{ 
	if( in_fScalar )
	{
		out_mResult->_11 = in_mLeft->_11 / in_fScalar;
		out_mResult->_12 = in_mLeft->_12 / in_fScalar;
		out_mResult->_21 = in_mLeft->_21 / in_fScalar;
		out_mResult->_22 = in_mLeft->_22 / in_fScalar;
	}
	else
	{
		warning_printf("Dividing matrix by zero!");
		out_mResult->_12 = out_mResult->_21 = 0;
		out_mResult->_11 = out_mResult->_22 = 1.0f;
	}
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixAdd			( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight )
{ 
	out_mResult->_11 = in_mLeft->_11 + in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 + in_mRight->_12;
	out_mResult->_13 = in_mLeft->_13 + in_mRight->_13;

	out_mResult->_21 = in_mLeft->_21 + in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 + in_mRight->_22;
	out_mResult->_23 = in_mLeft->_23 + in_mRight->_23;

	out_mResult->_31 = in_mLeft->_31 + in_mRight->_31;
	out_mResult->_32 = in_mLeft->_32 + in_mRight->_32;
	out_mResult->_33 = in_mLeft->_33 + in_mRight->_33;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixSubstract	( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight )
{ 
	out_mResult->_11 = in_mLeft->_11 - in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 - in_mRight->_12;
	out_mResult->_13 = in_mLeft->_13 - in_mRight->_13;

	out_mResult->_21 = in_mLeft->_21 - in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 - in_mRight->_22;
	out_mResult->_23 = in_mLeft->_23 - in_mRight->_23;

	out_mResult->_31 = in_mLeft->_31 - in_mRight->_31;
	out_mResult->_32 = in_mLeft->_32 - in_mRight->_32;
	out_mResult->_33 = in_mLeft->_33 - in_mRight->_33;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixMultiply	( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight )
{ 
	// first row
	out_mResult->_11 
		= in_mLeft->_11 * in_mRight->_11
		+ in_mLeft->_12 * in_mRight->_21
		+ in_mLeft->_13 * in_mRight->_31;

	out_mResult->_12 
		= in_mLeft->_11 * in_mRight->_12
		+ in_mLeft->_12 * in_mRight->_22
		+ in_mLeft->_13 * in_mRight->_32;

	out_mResult->_13
		= in_mLeft->_11 * in_mRight->_13
		+ in_mLeft->_12 * in_mRight->_23
		+ in_mLeft->_13 * in_mRight->_33;

	// second row 
	out_mResult->_21 
		= in_mLeft->_21 * in_mRight->_11
		+ in_mLeft->_22 * in_mRight->_21
		+ in_mLeft->_23 * in_mRight->_31;

	out_mResult->_22 
		= in_mLeft->_21 * in_mRight->_12
		+ in_mLeft->_22 * in_mRight->_22
		+ in_mLeft->_23 * in_mRight->_32;

	out_mResult->_23
		= in_mLeft->_21 * in_mRight->_13
		+ in_mLeft->_22 * in_mRight->_23
		+ in_mLeft->_23 * in_mRight->_33;

	// third row 
	out_mResult->_31 
		= in_mLeft->_31 * in_mRight->_11
		+ in_mLeft->_32 * in_mRight->_21
		+ in_mLeft->_33 * in_mRight->_31;

	out_mResult->_32 
		= in_mLeft->_31 * in_mRight->_12
		+ in_mLeft->_32 * in_mRight->_22
		+ in_mLeft->_33 * in_mRight->_32;

	out_mResult->_33
		= in_mLeft->_31 * in_mRight->_13
		+ in_mLeft->_32 * in_mRight->_23
		+ in_mLeft->_33 * in_mRight->_33;


	return out_mResult; 
};

GVECTOR3*	god::GMatrixTransform	( GVECTOR3* out_mResult, const GVECTOR3* in_vLeft, const GMATRIX3X3* in_mRight ) 
{ 
	// first row
	out_mResult->x
		= in_vLeft->x * in_mRight->_11
		+ in_vLeft->y * in_mRight->_21
		+ in_vLeft->z * in_mRight->_31;

	out_mResult->y
		= in_vLeft->x * in_mRight->_12
		+ in_vLeft->y * in_mRight->_22
		+ in_vLeft->z * in_mRight->_32;

	out_mResult->z
		= in_vLeft->x * in_mRight->_13
		+ in_vLeft->y * in_mRight->_23
		+ in_vLeft->z * in_mRight->_33;

	return out_mResult; 
};

GVECTOR3*	god::GMatrixTransformInPlace	( GVECTOR3* out_mResult, const GMATRIX3X3* in_mRight ) 
{ 
	float _x	= out_mResult->x * in_mRight->_11
				+ out_mResult->y * in_mRight->_21
				+ out_mResult->z * in_mRight->_31;

	float _y	= out_mResult->x * in_mRight->_12
				+ out_mResult->y * in_mRight->_22
				+ out_mResult->z * in_mRight->_32;

	float _z	= out_mResult->x * in_mRight->_13
				+ out_mResult->y * in_mRight->_23
				+ out_mResult->z * in_mRight->_33;

	out_mResult->x = _x;
	out_mResult->y = _y;
	out_mResult->z = _z;
	return out_mResult; 
};

GVECTOR4*	god::GMatrixTransform	( GVECTOR4* out_mResult, const GVECTOR4* in_vLeft, const GMATRIX3X3* in_mRight ) 
{ 
	// first row
	out_mResult->x
		= in_vLeft->x * in_mRight->_11
		+ in_vLeft->y * in_mRight->_21
		+ in_vLeft->z * in_mRight->_31;

	out_mResult->y
		= in_vLeft->x * in_mRight->_12
		+ in_vLeft->y * in_mRight->_22
		+ in_vLeft->z * in_mRight->_32;

	out_mResult->z
		= in_vLeft->x * in_mRight->_13
		+ in_vLeft->y * in_mRight->_23
		+ in_vLeft->z * in_mRight->_33;

	out_mResult->w = in_vLeft->w;
	return out_mResult; 
};

GVECTOR4*	god::GMatrixTransformInPlace	( GVECTOR4* out_mResult, const GMATRIX3X3* in_mRight ) 
{ 
	float _x	= out_mResult->x * in_mRight->_11
				+ out_mResult->y * in_mRight->_21
				+ out_mResult->z * in_mRight->_31;

	float _y	= out_mResult->x * in_mRight->_12
				+ out_mResult->y * in_mRight->_22
				+ out_mResult->z * in_mRight->_32;

	float _z	= out_mResult->x * in_mRight->_13
				+ out_mResult->y * in_mRight->_23
				+ out_mResult->z * in_mRight->_33;

	out_mResult->x = _x;
	out_mResult->y = _y;
	out_mResult->z = _z;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixInverse		( GMATRIX3X3* out_mResult, float* out_fDeterminant, const GMATRIX3X3* in_M )
{ 
	float A, B, C, D, E, F, G, H, I;
	float fDet 
		= in_M->_11 * ( in_M->_22 * in_M->_33 - in_M->_23 * in_M->_32) 
		+ in_M->_12 * ( in_M->_23 * in_M->_31 - in_M->_33 * in_M->_21) 
		+ in_M->_13 * ( in_M->_21 * in_M->_32 - in_M->_22 * in_M->_31);

	A = (in_M->_22*in_M->_33 - in_M->_23*in_M->_32);/**/ B = (in_M->_23*in_M->_31 - in_M->_21*in_M->_33);/**/ C = (in_M->_21*in_M->_32 - in_M->_22*in_M->_31);
	D = (in_M->_13*in_M->_32 - in_M->_12*in_M->_33);/**/ E = (in_M->_11*in_M->_33 - in_M->_13*in_M->_31);/**/ F = (in_M->_12*in_M->_31 - in_M->_11*in_M->_32);
	G = (in_M->_12*in_M->_23 - in_M->_13*in_M->_22);/**/ H = (in_M->_13*in_M->_21 - in_M->_11*in_M->_23);/**/ I = (in_M->_11*in_M->_22 - in_M->_12*in_M->_21);
	// 
	out_mResult->_11 = A/fDet ; out_mResult->_12 = D/fDet ; out_mResult->_13 = G/fDet;
	out_mResult->_21 = B/fDet ; out_mResult->_22 = E/fDet ; out_mResult->_23 = H/fDet;
	out_mResult->_31 = C/fDet ; out_mResult->_32 = F/fDet ; out_mResult->_33 = I/fDet;
	if( out_fDeterminant )
		*out_fDeterminant = fDet;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixTranspose	( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_Matrix )
{ 
	out_mResult->_11 = in_Matrix->_11;
	out_mResult->_12 = in_Matrix->_21;
	out_mResult->_13 = in_Matrix->_31;

	out_mResult->_21 = in_Matrix->_12;
	out_mResult->_22 = in_Matrix->_22;
	out_mResult->_23 = in_Matrix->_32;

	out_mResult->_31 = in_Matrix->_13;
	out_mResult->_32 = in_Matrix->_23;
	out_mResult->_33 = in_Matrix->_33;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixTransposeInPlace( GMATRIX3X3* inout_mResult )
{ 
	float c1, c2, c3; 
	// store a backup of some numbers
	c1 = inout_mResult->_23;
	c2 = inout_mResult->_21;
	c3 = inout_mResult->_31;

	inout_mResult->_23 = inout_mResult->_32; // _32 is free now	__ set _23 (1st)
	inout_mResult->_21 = inout_mResult->_12; // _12 is free now	__ set _21 (2nd)
	inout_mResult->_31 = inout_mResult->_13; // _13 is free		__ set _31 (3rd)

	inout_mResult->_12 = c2; // store _21 into _12, c2 is free now;			__ set _12 (4th)
	inout_mResult->_13 = c3; // store _31 into _13, c3 becomes available		__ set _13 (5th)
	inout_mResult->_32 = c1; // restore the backup in the _32 now invalid		__ set _32 (6th)
	return inout_mResult; 
};

GMATRIX3X3*	god::GMatrixMultiply	( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, float in_fScalar )
{ 
	out_mResult->_11 = in_mLeft->_11 * in_fScalar;
	out_mResult->_12 = in_mLeft->_12 * in_fScalar;
	out_mResult->_13 = in_mLeft->_13 * in_fScalar;

	out_mResult->_21 = in_mLeft->_21 * in_fScalar;
	out_mResult->_22 = in_mLeft->_22 * in_fScalar;
	out_mResult->_23 = in_mLeft->_23 * in_fScalar;

	out_mResult->_31 = in_mLeft->_31 * in_fScalar;
	out_mResult->_32 = in_mLeft->_32 * in_fScalar;
	out_mResult->_33 = in_mLeft->_33 * in_fScalar;
	return out_mResult; 
};

GMATRIX3X3*	god::GMatrixDivide		( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, float in_fScalar )
{ 
	if( in_fScalar )
	{
		out_mResult->_11 = in_mLeft->_11 / in_fScalar;
		out_mResult->_12 = in_mLeft->_12 / in_fScalar;
		out_mResult->_13 = in_mLeft->_13 / in_fScalar;

		out_mResult->_21 = in_mLeft->_21 / in_fScalar;
		out_mResult->_22 = in_mLeft->_22 / in_fScalar;
		out_mResult->_23 = in_mLeft->_23 / in_fScalar;

		out_mResult->_31 = in_mLeft->_31 / in_fScalar;
		out_mResult->_32 = in_mLeft->_32 / in_fScalar;
		out_mResult->_33 = in_mLeft->_33 / in_fScalar;
	}
	else
	{
		warning_printf("Dividing matrix by zero! Setting identity...");
		out_mResult->_12 = out_mResult->_13 
			= out_mResult->_21 = out_mResult->_23 
			= out_mResult->_31 = out_mResult->_32
			= 0;
		out_mResult->_11 = out_mResult->_22 = out_mResult->_33 = 1.0f;
	}
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixAdd			( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight )
{ 
	out_mResult->_11 = in_mLeft->_11 + in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 + in_mRight->_12;
	out_mResult->_13 = in_mLeft->_13 + in_mRight->_13;
	out_mResult->_14 = in_mLeft->_14 + in_mRight->_14;

	out_mResult->_21 = in_mLeft->_21 + in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 + in_mRight->_22;
	out_mResult->_23 = in_mLeft->_23 + in_mRight->_23;
	out_mResult->_24 = in_mLeft->_24 + in_mRight->_24;

	out_mResult->_31 = in_mLeft->_31 + in_mRight->_31;
	out_mResult->_32 = in_mLeft->_32 + in_mRight->_32;
	out_mResult->_33 = in_mLeft->_33 + in_mRight->_33;
	out_mResult->_34 = in_mLeft->_34 + in_mRight->_34;

	out_mResult->_41 = in_mLeft->_41 + in_mRight->_41;
	out_mResult->_42 = in_mLeft->_42 + in_mRight->_42;
	out_mResult->_43 = in_mLeft->_43 + in_mRight->_43;
	out_mResult->_44 = in_mLeft->_44 + in_mRight->_44;
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixSubstract	( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight )
{ 
	out_mResult->_11 = in_mLeft->_11 - in_mRight->_11;
	out_mResult->_12 = in_mLeft->_12 - in_mRight->_12;
	out_mResult->_13 = in_mLeft->_13 - in_mRight->_13;
	out_mResult->_14 = in_mLeft->_14 - in_mRight->_14;

	out_mResult->_21 = in_mLeft->_21 - in_mRight->_21;
	out_mResult->_22 = in_mLeft->_22 - in_mRight->_22;
	out_mResult->_23 = in_mLeft->_23 - in_mRight->_23;
	out_mResult->_24 = in_mLeft->_24 - in_mRight->_24;

	out_mResult->_31 = in_mLeft->_31 - in_mRight->_31;
	out_mResult->_32 = in_mLeft->_32 - in_mRight->_32;
	out_mResult->_33 = in_mLeft->_33 - in_mRight->_33;
	out_mResult->_34 = in_mLeft->_34 - in_mRight->_34;

	out_mResult->_41 = in_mLeft->_41 - in_mRight->_41;
	out_mResult->_42 = in_mLeft->_42 - in_mRight->_42;
	out_mResult->_43 = in_mLeft->_43 - in_mRight->_43;
	out_mResult->_44 = in_mLeft->_44 - in_mRight->_44;
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixMultiply	( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight )
{ 
	// first row
	out_mResult->_11 
		= in_mLeft->_11 * in_mRight->_11
		+ in_mLeft->_12 * in_mRight->_21
		+ in_mLeft->_13 * in_mRight->_31
		+ in_mLeft->_14 * in_mRight->_41;

	out_mResult->_12 
		= in_mLeft->_11 * in_mRight->_12
		+ in_mLeft->_12 * in_mRight->_22
		+ in_mLeft->_13 * in_mRight->_32
		+ in_mLeft->_14 * in_mRight->_42;

	out_mResult->_13
		= in_mLeft->_11 * in_mRight->_13
		+ in_mLeft->_12 * in_mRight->_23
		+ in_mLeft->_13 * in_mRight->_33
		+ in_mLeft->_14 * in_mRight->_43;

	out_mResult->_14
		= in_mLeft->_11 * in_mRight->_14
		+ in_mLeft->_12 * in_mRight->_24
		+ in_mLeft->_13 * in_mRight->_34
		+ in_mLeft->_14 * in_mRight->_44;

	// second row
	out_mResult->_21 
		= in_mLeft->_21 * in_mRight->_11
		+ in_mLeft->_22 * in_mRight->_21
		+ in_mLeft->_23 * in_mRight->_31
		+ in_mLeft->_24 * in_mRight->_41;

	out_mResult->_22 
		= in_mLeft->_21 * in_mRight->_12
		+ in_mLeft->_22 * in_mRight->_22
		+ in_mLeft->_23 * in_mRight->_32
		+ in_mLeft->_24 * in_mRight->_42;

	out_mResult->_23
		= in_mLeft->_21 * in_mRight->_13
		+ in_mLeft->_22 * in_mRight->_23
		+ in_mLeft->_23 * in_mRight->_33
		+ in_mLeft->_24 * in_mRight->_43;

	out_mResult->_24
		= in_mLeft->_21 * in_mRight->_14
		+ in_mLeft->_22 * in_mRight->_24
		+ in_mLeft->_23 * in_mRight->_34
		+ in_mLeft->_24 * in_mRight->_44;

	// third row
	out_mResult->_31 
		= in_mLeft->_31 * in_mRight->_11
		+ in_mLeft->_32 * in_mRight->_21
		+ in_mLeft->_33 * in_mRight->_31
		+ in_mLeft->_34 * in_mRight->_41;

	out_mResult->_32 
		= in_mLeft->_31 * in_mRight->_12
		+ in_mLeft->_32 * in_mRight->_22
		+ in_mLeft->_33 * in_mRight->_32
		+ in_mLeft->_34 * in_mRight->_42;

	out_mResult->_33
		= in_mLeft->_31 * in_mRight->_13
		+ in_mLeft->_32 * in_mRight->_23
		+ in_mLeft->_33 * in_mRight->_33
		+ in_mLeft->_34 * in_mRight->_43;

	out_mResult->_34
		= in_mLeft->_31 * in_mRight->_14
		+ in_mLeft->_32 * in_mRight->_24
		+ in_mLeft->_33 * in_mRight->_34
		+ in_mLeft->_34 * in_mRight->_44;

	// fourth row
	out_mResult->_41 
		= in_mLeft->_41 * in_mRight->_11
		+ in_mLeft->_42 * in_mRight->_21
		+ in_mLeft->_43 * in_mRight->_31
		+ in_mLeft->_44 * in_mRight->_41;

	out_mResult->_42 
		= in_mLeft->_41 * in_mRight->_12
		+ in_mLeft->_42 * in_mRight->_22
		+ in_mLeft->_43 * in_mRight->_32
		+ in_mLeft->_44 * in_mRight->_42;

	out_mResult->_43
		= in_mLeft->_41 * in_mRight->_13
		+ in_mLeft->_42 * in_mRight->_23
		+ in_mLeft->_43 * in_mRight->_33
		+ in_mLeft->_44 * in_mRight->_43;

	out_mResult->_44
		= in_mLeft->_41 * in_mRight->_14
		+ in_mLeft->_42 * in_mRight->_24
		+ in_mLeft->_43 * in_mRight->_34
		+ in_mLeft->_44 * in_mRight->_44;
	return out_mResult; 
};

GVECTOR3*	god::GMatrixTransform	( GVECTOR3* out_vResult, const GVECTOR3* in_vLeft, const GMATRIX4X4* in_mRight ) 
{ 
	// first row
	out_vResult->x
		= in_vLeft->x * in_mRight->_11
		+ in_vLeft->y * in_mRight->_21
		+ in_vLeft->z * in_mRight->_31;

	out_vResult->y
		= in_vLeft->x * in_mRight->_12
		+ in_vLeft->y * in_mRight->_22
		+ in_vLeft->z * in_mRight->_32;

	out_vResult->z
		= in_vLeft->x * in_mRight->_13
		+ in_vLeft->y * in_mRight->_23
		+ in_vLeft->z * in_mRight->_33;

	float w 
		= in_vLeft->x*in_mRight->_14 
		+ in_vLeft->y*in_mRight->_24 
		+ in_vLeft->z*in_mRight->_34 
		+ in_mRight->_44;

	if( w )
	{
		out_vResult->x	/= w;
		out_vResult->y	/= w;
		out_vResult->z	/= w;
	}
	return out_vResult; 
};


GVECTOR3*	god::GMatrixTransformInPlace	( GVECTOR3* out_vResult, const GMATRIX4X4* in_mRight ) 
{ 
	// first row
	float _z	= out_vResult->x * in_mRight->_11
				+ out_vResult->y * in_mRight->_21
				+ out_vResult->z * in_mRight->_31;

	float _y	= out_vResult->x * in_mRight->_12
				+ out_vResult->y * in_mRight->_22
				+ out_vResult->z * in_mRight->_32;

	float _x	= out_vResult->x * in_mRight->_13
				+ out_vResult->y * in_mRight->_23
				+ out_vResult->z * in_mRight->_33;

	float _w	= out_vResult->x*in_mRight->_14 
				+ out_vResult->y*in_mRight->_24 
				+ out_vResult->z*in_mRight->_34 
				+ in_mRight->_44;

	if( _w )
	{
		_x	/= _w;
		_y	/= _w;
		_z	/= _w;
	}
	out_vResult->x = _x;
	out_vResult->y = _y;
	out_vResult->z = _z;
	return out_vResult; 
};

GVECTOR4*	god::GMatrixTransform	( GVECTOR4* out_vResult, const GVECTOR4* in_vLeft, const GMATRIX4X4* in_mRight ) 
{ 
	// first row
	out_vResult->x
		= in_vLeft->x * in_mRight->_11
		+ in_vLeft->y * in_mRight->_21
		+ in_vLeft->z * in_mRight->_31
		+ in_vLeft->w * in_mRight->_41;

	out_vResult->y
		= in_vLeft->x * in_mRight->_12
		+ in_vLeft->y * in_mRight->_22
		+ in_vLeft->z * in_mRight->_32
		+ in_vLeft->w * in_mRight->_42;

	out_vResult->z
		= in_vLeft->x * in_mRight->_13
		+ in_vLeft->y * in_mRight->_23
		+ in_vLeft->z * in_mRight->_33
		+ in_vLeft->w * in_mRight->_43;

	out_vResult->w
		= in_vLeft->x*in_mRight->_14 
		+ in_vLeft->y*in_mRight->_24 
		+ in_vLeft->z*in_mRight->_34 
		+ in_vLeft->w*in_mRight->_44;

	if( out_vResult->w )
	{
		out_vResult->x	/= out_vResult->w;
		out_vResult->y	/= out_vResult->w;
		out_vResult->z	/= out_vResult->w;
	}
	out_vResult->w = 1.0f;

	return out_vResult; 
};

GVECTOR4*	god::GMatrixTransformInPlace	( GVECTOR4* out_vResult, const GMATRIX4X4* in_mRight ) 
{ 
	// first row
	float _x
		= out_vResult->x * in_mRight->_11
		+ out_vResult->y * in_mRight->_21
		+ out_vResult->z * in_mRight->_31
		+ out_vResult->w * in_mRight->_41;

	float _y
		= out_vResult->x * in_mRight->_12
		+ out_vResult->y * in_mRight->_22
		+ out_vResult->z * in_mRight->_32
		+ out_vResult->w * in_mRight->_42;

	float _z
		= out_vResult->x * in_mRight->_13
		+ out_vResult->y * in_mRight->_23
		+ out_vResult->z * in_mRight->_33
		+ out_vResult->w * in_mRight->_43;

	float _w
		= out_vResult->x*in_mRight->_14 
		+ out_vResult->y*in_mRight->_24 
		+ out_vResult->z*in_mRight->_34 
		+ out_vResult->w*in_mRight->_44;

	if( _w )
	{
		_x	/= _w;
		_y	/= _w;
		_z	/= _w;
	}

	out_vResult->x = _x;
	out_vResult->y = _y;
	out_vResult->z = _z;
	out_vResult->w = 1.0f;

	return out_vResult; 
};

GMATRIX4X4*	god::GMatrixInverse		( GMATRIX4X4* out_mResult, float* out_fDeterminant, const GMATRIX4X4* in_Matrix )
{ 
	GMATRIX4X4 mTranspose;
	float fCofactors[12];

	// calculate transposed matrix
	GMatrixTranspose( &mTranspose, in_Matrix );

	// Get cofactors
	fCofactors[ 0]  = mTranspose._33 * mTranspose._44;
	fCofactors[ 1]  = mTranspose._34 * mTranspose._43;
	fCofactors[ 2]  = mTranspose._32 * mTranspose._44;
	fCofactors[ 3]  = mTranspose._34 * mTranspose._42;
	fCofactors[ 4]  = mTranspose._32 * mTranspose._43;
	fCofactors[ 5]  = mTranspose._33 * mTranspose._42;
	fCofactors[ 6]  = mTranspose._31 * mTranspose._44;
	fCofactors[ 7]  = mTranspose._34 * mTranspose._41;
	fCofactors[ 8]  = mTranspose._31 * mTranspose._43;
	fCofactors[ 9]  = mTranspose._33 * mTranspose._41;
	fCofactors[10]  = mTranspose._31 * mTranspose._42;
	fCofactors[11]  = mTranspose._32 * mTranspose._41;

	out_mResult->_11  = fCofactors[0]*mTranspose._22 + fCofactors[3]*mTranspose._23 + fCofactors[4] *mTranspose._24;
	out_mResult->_11 -= fCofactors[1]*mTranspose._22 + fCofactors[2]*mTranspose._23 + fCofactors[5] *mTranspose._24;
	out_mResult->_12  = fCofactors[1]*mTranspose._21 + fCofactors[6]*mTranspose._23 + fCofactors[9] *mTranspose._24;
	out_mResult->_12 -= fCofactors[0]*mTranspose._21 + fCofactors[7]*mTranspose._23 + fCofactors[8] *mTranspose._24;
	out_mResult->_13  = fCofactors[2]*mTranspose._21 + fCofactors[7]*mTranspose._22 + fCofactors[10]*mTranspose._24;
	out_mResult->_13 -= fCofactors[3]*mTranspose._21 + fCofactors[6]*mTranspose._22 + fCofactors[11]*mTranspose._24;
	out_mResult->_14  = fCofactors[5]*mTranspose._21 + fCofactors[8]*mTranspose._22 + fCofactors[11]*mTranspose._23;
	out_mResult->_14 -= fCofactors[4]*mTranspose._21 + fCofactors[9]*mTranspose._22 + fCofactors[10]*mTranspose._23;
	out_mResult->_21  = fCofactors[1]*mTranspose._12 + fCofactors[2]*mTranspose._13 + fCofactors[5] *mTranspose._14;
	out_mResult->_21 -= fCofactors[0]*mTranspose._12 + fCofactors[3]*mTranspose._13 + fCofactors[4] *mTranspose._14;
	out_mResult->_22  = fCofactors[0]*mTranspose._11 + fCofactors[7]*mTranspose._13 + fCofactors[8] *mTranspose._14;
	out_mResult->_22 -= fCofactors[1]*mTranspose._11 + fCofactors[6]*mTranspose._13 + fCofactors[9] *mTranspose._14;
	out_mResult->_23  = fCofactors[3]*mTranspose._11 + fCofactors[6]*mTranspose._12 + fCofactors[11]*mTranspose._14;
	out_mResult->_23 -= fCofactors[2]*mTranspose._11 + fCofactors[7]*mTranspose._12 + fCofactors[10]*mTranspose._14;
	out_mResult->_24  = fCofactors[4]*mTranspose._11 + fCofactors[9]*mTranspose._12 + fCofactors[10]*mTranspose._13;
	out_mResult->_24 -= fCofactors[5]*mTranspose._11 + fCofactors[8]*mTranspose._12 + fCofactors[11]*mTranspose._13;

	fCofactors[ 0]  = mTranspose._13 * mTranspose._24;
	fCofactors[ 1]  = mTranspose._14 * mTranspose._23;
	fCofactors[ 2]  = mTranspose._12 * mTranspose._24;
	fCofactors[ 3]  = mTranspose._14 * mTranspose._22;
	fCofactors[ 4]  = mTranspose._12 * mTranspose._23;
	fCofactors[ 5]  = mTranspose._13 * mTranspose._22;
	fCofactors[ 6]  = mTranspose._11 * mTranspose._24;
	fCofactors[ 7]  = mTranspose._14 * mTranspose._21;
	fCofactors[ 8]  = mTranspose._11 * mTranspose._23;
	fCofactors[ 9]  = mTranspose._13 * mTranspose._21;
	fCofactors[10]  = mTranspose._11 * mTranspose._22;
	fCofactors[11]  = mTranspose._12 * mTranspose._21;

	out_mResult->_31  = fCofactors[0] *mTranspose._42 + fCofactors[3] *mTranspose._43 + fCofactors[4] *mTranspose._44;
	out_mResult->_31 -= fCofactors[1] *mTranspose._42 + fCofactors[2] *mTranspose._43 + fCofactors[5] *mTranspose._44;
	out_mResult->_32  = fCofactors[1] *mTranspose._41 + fCofactors[6] *mTranspose._43 + fCofactors[9] *mTranspose._44;
	out_mResult->_32 -= fCofactors[0] *mTranspose._41 + fCofactors[7] *mTranspose._43 + fCofactors[8] *mTranspose._44;
	out_mResult->_33  = fCofactors[2] *mTranspose._41 + fCofactors[7] *mTranspose._42 + fCofactors[10]*mTranspose._44;
	out_mResult->_33 -= fCofactors[3] *mTranspose._41 + fCofactors[6] *mTranspose._42 + fCofactors[11]*mTranspose._44;
	out_mResult->_34  = fCofactors[5] *mTranspose._41 + fCofactors[8] *mTranspose._42 + fCofactors[11]*mTranspose._43;
	out_mResult->_34 -= fCofactors[4] *mTranspose._41 + fCofactors[9] *mTranspose._42 + fCofactors[10]*mTranspose._43;
	out_mResult->_41  = fCofactors[2] *mTranspose._33 + fCofactors[5] *mTranspose._34 + fCofactors[1] *mTranspose._32;
	out_mResult->_41 -= fCofactors[4] *mTranspose._34 + fCofactors[0] *mTranspose._32 + fCofactors[3] *mTranspose._33;
	out_mResult->_42  = fCofactors[8] *mTranspose._34 + fCofactors[0] *mTranspose._31 + fCofactors[7] *mTranspose._33;
	out_mResult->_42 -= fCofactors[6] *mTranspose._33 + fCofactors[9] *mTranspose._34 + fCofactors[1] *mTranspose._31;
	out_mResult->_43  = fCofactors[6] *mTranspose._32 + fCofactors[11]*mTranspose._34 + fCofactors[3] *mTranspose._31;
	out_mResult->_43 -= fCofactors[10]*mTranspose._34 + fCofactors[2] *mTranspose._31 + fCofactors[7] *mTranspose._32;
	out_mResult->_44  = fCofactors[10]*mTranspose._33 + fCofactors[4] *mTranspose._31 + fCofactors[9] *mTranspose._32;
	out_mResult->_44 -= fCofactors[8] *mTranspose._32 + fCofactors[11]*mTranspose._33 + fCofactors[5] *mTranspose._31;

	float fDet = mTranspose._11*out_mResult->_11 + 
		 mTranspose._12*out_mResult->_12 + 
		 mTranspose._13*out_mResult->_13 +
		 mTranspose._14*out_mResult->_14;

	fDet = 1/fDet;

	out_mResult->_11 *= fDet;  
	out_mResult->_12 *= fDet;  
	out_mResult->_13 *= fDet;  
	out_mResult->_14 *= fDet;

	out_mResult->_21 *= fDet;  
	out_mResult->_22 *= fDet;  
	out_mResult->_23 *= fDet;  
	out_mResult->_24 *= fDet;

	out_mResult->_31 *= fDet;  
	out_mResult->_32 *= fDet;  
	out_mResult->_33 *= fDet;  
	out_mResult->_34 *= fDet;

	out_mResult->_41 *= fDet;  
	out_mResult->_42 *= fDet;  
	out_mResult->_43 *= fDet;  
	out_mResult->_44 *= fDet;
	if( out_fDeterminant )
		*out_fDeterminant = fDet;
	return out_mResult; 
};


GMATRIX4X4*	god::GMatrixInverseInPlace( GMATRIX4X4* out_mResult, float* out_fDeterminant )
{ 
	GMATRIX4X4 mTranspose;
	float fCofactors[12];

	// calculate transposed matrix
	GMatrixTranspose( &mTranspose, out_mResult );

	// Get cofactors
	fCofactors[ 0]  = mTranspose._33 * mTranspose._44;
	fCofactors[ 1]  = mTranspose._34 * mTranspose._43;
	fCofactors[ 2]  = mTranspose._32 * mTranspose._44;
	fCofactors[ 3]  = mTranspose._34 * mTranspose._42;
	fCofactors[ 4]  = mTranspose._32 * mTranspose._43;
	fCofactors[ 5]  = mTranspose._33 * mTranspose._42;
	fCofactors[ 6]  = mTranspose._31 * mTranspose._44;
	fCofactors[ 7]  = mTranspose._34 * mTranspose._41;
	fCofactors[ 8]  = mTranspose._31 * mTranspose._43;
	fCofactors[ 9]  = mTranspose._33 * mTranspose._41;
	fCofactors[10]  = mTranspose._31 * mTranspose._42;
	fCofactors[11]  = mTranspose._32 * mTranspose._41;

	out_mResult->_11  = fCofactors[0]*mTranspose._22 + fCofactors[3]*mTranspose._23 + fCofactors[4] *mTranspose._24;
	out_mResult->_11 -= fCofactors[1]*mTranspose._22 + fCofactors[2]*mTranspose._23 + fCofactors[5] *mTranspose._24;
	out_mResult->_12  = fCofactors[1]*mTranspose._21 + fCofactors[6]*mTranspose._23 + fCofactors[9] *mTranspose._24;
	out_mResult->_12 -= fCofactors[0]*mTranspose._21 + fCofactors[7]*mTranspose._23 + fCofactors[8] *mTranspose._24;
	out_mResult->_13  = fCofactors[2]*mTranspose._21 + fCofactors[7]*mTranspose._22 + fCofactors[10]*mTranspose._24;
	out_mResult->_13 -= fCofactors[3]*mTranspose._21 + fCofactors[6]*mTranspose._22 + fCofactors[11]*mTranspose._24;
	out_mResult->_14  = fCofactors[5]*mTranspose._21 + fCofactors[8]*mTranspose._22 + fCofactors[11]*mTranspose._23;
	out_mResult->_14 -= fCofactors[4]*mTranspose._21 + fCofactors[9]*mTranspose._22 + fCofactors[10]*mTranspose._23;
	out_mResult->_21  = fCofactors[1]*mTranspose._12 + fCofactors[2]*mTranspose._13 + fCofactors[5] *mTranspose._14;
	out_mResult->_21 -= fCofactors[0]*mTranspose._12 + fCofactors[3]*mTranspose._13 + fCofactors[4] *mTranspose._14;
	out_mResult->_22  = fCofactors[0]*mTranspose._11 + fCofactors[7]*mTranspose._13 + fCofactors[8] *mTranspose._14;
	out_mResult->_22 -= fCofactors[1]*mTranspose._11 + fCofactors[6]*mTranspose._13 + fCofactors[9] *mTranspose._14;
	out_mResult->_23  = fCofactors[3]*mTranspose._11 + fCofactors[6]*mTranspose._12 + fCofactors[11]*mTranspose._14;
	out_mResult->_23 -= fCofactors[2]*mTranspose._11 + fCofactors[7]*mTranspose._12 + fCofactors[10]*mTranspose._14;
	out_mResult->_24  = fCofactors[4]*mTranspose._11 + fCofactors[9]*mTranspose._12 + fCofactors[10]*mTranspose._13;
	out_mResult->_24 -= fCofactors[5]*mTranspose._11 + fCofactors[8]*mTranspose._12 + fCofactors[11]*mTranspose._13;

	fCofactors[ 0]  = mTranspose._13 * mTranspose._24;
	fCofactors[ 1]  = mTranspose._14 * mTranspose._23;
	fCofactors[ 2]  = mTranspose._12 * mTranspose._24;
	fCofactors[ 3]  = mTranspose._14 * mTranspose._22;
	fCofactors[ 4]  = mTranspose._12 * mTranspose._23;
	fCofactors[ 5]  = mTranspose._13 * mTranspose._22;
	fCofactors[ 6]  = mTranspose._11 * mTranspose._24;
	fCofactors[ 7]  = mTranspose._14 * mTranspose._21;
	fCofactors[ 8]  = mTranspose._11 * mTranspose._23;
	fCofactors[ 9]  = mTranspose._13 * mTranspose._21;
	fCofactors[10]  = mTranspose._11 * mTranspose._22;
	fCofactors[11]  = mTranspose._12 * mTranspose._21;

	out_mResult->_31  = fCofactors[0] *mTranspose._42 + fCofactors[3] *mTranspose._43 + fCofactors[4] *mTranspose._44;
	out_mResult->_31 -= fCofactors[1] *mTranspose._42 + fCofactors[2] *mTranspose._43 + fCofactors[5] *mTranspose._44;
	out_mResult->_32  = fCofactors[1] *mTranspose._41 + fCofactors[6] *mTranspose._43 + fCofactors[9] *mTranspose._44;
	out_mResult->_32 -= fCofactors[0] *mTranspose._41 + fCofactors[7] *mTranspose._43 + fCofactors[8] *mTranspose._44;
	out_mResult->_33  = fCofactors[2] *mTranspose._41 + fCofactors[7] *mTranspose._42 + fCofactors[10]*mTranspose._44;
	out_mResult->_33 -= fCofactors[3] *mTranspose._41 + fCofactors[6] *mTranspose._42 + fCofactors[11]*mTranspose._44;
	out_mResult->_34  = fCofactors[5] *mTranspose._41 + fCofactors[8] *mTranspose._42 + fCofactors[11]*mTranspose._43;
	out_mResult->_34 -= fCofactors[4] *mTranspose._41 + fCofactors[9] *mTranspose._42 + fCofactors[10]*mTranspose._43;
	out_mResult->_41  = fCofactors[2] *mTranspose._33 + fCofactors[5] *mTranspose._34 + fCofactors[1] *mTranspose._32;
	out_mResult->_41 -= fCofactors[4] *mTranspose._34 + fCofactors[0] *mTranspose._32 + fCofactors[3] *mTranspose._33;
	out_mResult->_42  = fCofactors[8] *mTranspose._34 + fCofactors[0] *mTranspose._31 + fCofactors[7] *mTranspose._33;
	out_mResult->_42 -= fCofactors[6] *mTranspose._33 + fCofactors[9] *mTranspose._34 + fCofactors[1] *mTranspose._31;
	out_mResult->_43  = fCofactors[6] *mTranspose._32 + fCofactors[11]*mTranspose._34 + fCofactors[3] *mTranspose._31;
	out_mResult->_43 -= fCofactors[10]*mTranspose._34 + fCofactors[2] *mTranspose._31 + fCofactors[7] *mTranspose._32;
	out_mResult->_44  = fCofactors[10]*mTranspose._33 + fCofactors[4] *mTranspose._31 + fCofactors[9] *mTranspose._32;
	out_mResult->_44 -= fCofactors[8] *mTranspose._32 + fCofactors[11]*mTranspose._33 + fCofactors[5] *mTranspose._31;

	float fDet = mTranspose._11*out_mResult->_11 + 
		 mTranspose._12*out_mResult->_12 + 
		 mTranspose._13*out_mResult->_13 +
		 mTranspose._14*out_mResult->_14;

	fDet = 1/fDet;

	out_mResult->_11 *= fDet;  
	out_mResult->_12 *= fDet;  
	out_mResult->_13 *= fDet;  
	out_mResult->_14 *= fDet;

	out_mResult->_21 *= fDet;  
	out_mResult->_22 *= fDet;  
	out_mResult->_23 *= fDet;  
	out_mResult->_24 *= fDet;

	out_mResult->_31 *= fDet;  
	out_mResult->_32 *= fDet;  
	out_mResult->_33 *= fDet;  
	out_mResult->_34 *= fDet;

	out_mResult->_41 *= fDet;  
	out_mResult->_42 *= fDet;  
	out_mResult->_43 *= fDet;  
	out_mResult->_44 *= fDet;
	if( out_fDeterminant )
		*out_fDeterminant = fDet;
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixTranspose	( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_Matrix )
{ 
	out_mResult->_11 = in_Matrix->_11;
	out_mResult->_12 = in_Matrix->_21;
	out_mResult->_13 = in_Matrix->_31;
	out_mResult->_14 = in_Matrix->_41;

	out_mResult->_21 = in_Matrix->_12;
	out_mResult->_22 = in_Matrix->_22;
	out_mResult->_23 = in_Matrix->_32;
	out_mResult->_24 = in_Matrix->_42;

	out_mResult->_31 = in_Matrix->_13;
	out_mResult->_32 = in_Matrix->_23;
	out_mResult->_33 = in_Matrix->_33;
	out_mResult->_34 = in_Matrix->_43;

	out_mResult->_41 = in_Matrix->_14;
	out_mResult->_42 = in_Matrix->_24;
	out_mResult->_43 = in_Matrix->_34;
	out_mResult->_44 = in_Matrix->_44;
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixTransposeInPlace( GMATRIX4X4* inout_mResult )
{ 
	float c1, c2, c3, c4, c5, c6; 
	// store a backup of some numbers
	c1 = inout_mResult->_23;
	c2 = inout_mResult->_21;
	c3 = inout_mResult->_31;
	c4 = inout_mResult->_41;
	c5 = inout_mResult->_43;
	c6 = inout_mResult->_42;

	inout_mResult->_23 = inout_mResult->_32; // _32 is free now	__ set _23 (1st)
	inout_mResult->_21 = inout_mResult->_12; // _12 is free now	__ set _21 (2nd)
	inout_mResult->_31 = inout_mResult->_13; // _13 is free		__ set _31 (3rd)
	inout_mResult->_41 = inout_mResult->_14; // _14 is free		__ set _41 (4th)
	inout_mResult->_43 = inout_mResult->_34;
	inout_mResult->_42 = inout_mResult->_24;

	inout_mResult->_12 = c2; // store _21 into _12, c2 is free now;			__ set _12 (4th)
	inout_mResult->_13 = c3; // store _31 into _13, c3 becomes available		__ set _13 (5th)
	inout_mResult->_32 = c1; // restore the backup in the _32 now invalid		__ set _32 (6th)
	inout_mResult->_14 = c4; // restore the backup in the _14 now invalid		__ set _14 (7th)
	inout_mResult->_34 = c5;
	inout_mResult->_24 = c6;
	return inout_mResult; 
};


GMATRIX4X4*	god::GMatrixMultiply	( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, float in_fScalar )
{ 
	out_mResult->_11 = in_mLeft->_11 * in_fScalar;
	out_mResult->_12 = in_mLeft->_12 * in_fScalar;
	out_mResult->_13 = in_mLeft->_13 * in_fScalar;
	out_mResult->_14 = in_mLeft->_14 * in_fScalar;

	out_mResult->_21 = in_mLeft->_21 * in_fScalar;
	out_mResult->_22 = in_mLeft->_22 * in_fScalar;
	out_mResult->_23 = in_mLeft->_23 * in_fScalar;
	out_mResult->_24 = in_mLeft->_24 * in_fScalar;

	out_mResult->_31 = in_mLeft->_31 * in_fScalar;
	out_mResult->_32 = in_mLeft->_32 * in_fScalar;
	out_mResult->_33 = in_mLeft->_33 * in_fScalar;
	out_mResult->_34 = in_mLeft->_34 * in_fScalar;

	out_mResult->_41 = in_mLeft->_41 * in_fScalar;
	out_mResult->_42 = in_mLeft->_42 * in_fScalar;
	out_mResult->_43 = in_mLeft->_43 * in_fScalar;
	out_mResult->_44 = in_mLeft->_44 * in_fScalar;
	return out_mResult; 
};

GMATRIX4X4*	god::GMatrixDivide		( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, float in_fScalar )
{ 
	if( in_fScalar )
	{
		out_mResult->_11 = in_mLeft->_11 / in_fScalar;
		out_mResult->_12 = in_mLeft->_12 / in_fScalar;
		out_mResult->_13 = in_mLeft->_13 / in_fScalar;
		out_mResult->_14 = in_mLeft->_14 / in_fScalar;

		out_mResult->_21 = in_mLeft->_21 / in_fScalar;
		out_mResult->_22 = in_mLeft->_22 / in_fScalar;
		out_mResult->_23 = in_mLeft->_23 / in_fScalar;
		out_mResult->_24 = in_mLeft->_24 / in_fScalar;

		out_mResult->_31 = in_mLeft->_31 / in_fScalar;
		out_mResult->_32 = in_mLeft->_32 / in_fScalar;
		out_mResult->_33 = in_mLeft->_33 / in_fScalar;
		out_mResult->_34 = in_mLeft->_34 / in_fScalar;

		out_mResult->_41 = in_mLeft->_41 / in_fScalar;
		out_mResult->_42 = in_mLeft->_42 / in_fScalar;
		out_mResult->_43 = in_mLeft->_43 / in_fScalar;
		out_mResult->_44 = in_mLeft->_44 / in_fScalar;
	}
	else
	{
		warning_printf("Dividing matrix by zero! Setting identity...");
		out_mResult->_12 = out_mResult->_13 = out_mResult->_14 
			= out_mResult->_21 = out_mResult->_23 = out_mResult->_24
			= out_mResult->_31 = out_mResult->_32 = out_mResult->_34
			= out_mResult->_41 = out_mResult->_42 = out_mResult->_43 
			= 0;
		out_mResult->_11 = out_mResult->_22 = out_mResult->_33 = out_mResult->_44 = 1.0f;
	}
	return out_mResult; 
};
