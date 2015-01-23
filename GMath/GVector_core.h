#include "GCore_constants.h"

#ifndef _GVECTOR_CORE_H
#define _GVECTOR_CORE_H

namespace god
{
#pragma pack( push, 1 )
	struct GVECTOR2
	{
		float x;
		float y;
	};

	struct GVECTOR3
	{
		float x, y, z;
	};

	struct GVECTOR4
	{
		float x, y, z, w;
	};

	struct GQUATERNION
	{
		float x, y, z, w;
	};

	struct GPLANE
	{
		float x, y, z, d;
	};

	struct GRAY
	{
		GVECTOR3	Origin, Direction;
	};

	struct GAABB
	{
		GVECTOR3 Min, Max; // box extreme points
		GVECTOR3 Center;     // centerpoint
	};

	struct GOBB
	{
		float fA0, fA1, fA2;	// half axis length
		GVECTOR3 vA0, vA1, vA2;	// box axis
		GVECTOR3 Center;		// centerpoint
	};

	typedef struct GMATRIX2X2
	{
		float	_11, _12;
		float	_21, _22;
	} GMATRIX2;
	typedef struct GMATRIX3X3
	{
		float	_11, _12, _13;
		float	_21, _22, _23;
		float	_31, _32, _33;
	} GMATRIX3;
	typedef struct GMATRIX4X4
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	} GMATRIX4;

	// Other types
	struct GMATRIX2X3
	{
		float	_11, _12, _13; 
		float	_21, _22, _23;
	};
	struct GMATRIX3X2
	{
		float	_11, _12; 
		float	_21, _22;
		float	_31, _32;
	};
#pragma pack( pop )
	static const GVECTOR2		GVECTOR2_FRONT			= { 1.0f, 0.0f };
	static const GVECTOR2		GVECTOR2_RIGHT			= { 0.0f, 1.0f };
	static const GVECTOR2		GVECTOR2_ZERO			= { 0.0f, 0.0f };
	static const GVECTOR2		GVECTOR2_ONE			= { 1.0f, 1.0f };
													 	// x	// y	// z
	static const GVECTOR3		GVECTOR3_FRONT			= { 1.0f, 0.0f, 0.0f };
	static const GVECTOR3		GVECTOR3_UP				= { 0.0f, 1.0f, 0.0f };
	static const GVECTOR3		GVECTOR3_RIGHT			= { 0.0f, 0.0f, 1.0f };
	static const GVECTOR3		GVECTOR3_ZERO			= { 0.0f, 0.0f, 0.0f };
	static const GVECTOR3		GVECTOR3_ONE			= { 1.0f, 1.0f, 1.0f };
														// x	// y	// z	w
	static const GVECTOR4		GVECTOR4_FRONT			= { 1.0f, 0.0f, 0.0f, 0.0f };
	static const GVECTOR4		GVECTOR4_UP				= { 0.0f, 1.0f, 0.0f, 0.0f };
	static const GVECTOR4		GVECTOR4_RIGHT			= { 0.0f, 0.0f, 1.0f, 0.0f };
	static const GVECTOR4		GVECTOR4_ZERO			= { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GVECTOR4		GVECTOR4_ONE			= { 1.0f, 1.0f, 1.0f, 1.0f };

	static const GQUATERNION	GQUATERNION_IDENTITY	= { 0.0f, 0.0f, 0.0f, 1.0f };

	static const GMATRIX2		GMATRIX2_ZERO			= { 0 };
	static const GMATRIX3		GMATRIX3_ZERO			= { 0 };
	static const GMATRIX4		GMATRIX4_ZERO			= { 0 };
	static const GMATRIX2		GMATRIX2_ONE			= { 1.0f, 1.0f, 1.0f, 1.0f };
	static const GMATRIX3		GMATRIX3_ONE			= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	static const GMATRIX4		GMATRIX4_ONE			= { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	static const GMATRIX2		GMATRIX2_IDENTITY		= { 1.0f, 0.0f, 
															0.0f, 1.0f };
	static const GMATRIX3		GMATRIX3_IDENTITY		= { 1.0f, 0.0f, 0.0f, 
															0.0f, 1.0f, 0.0f, 
															0.0f, 0.0f, 1.0f };
	static const GMATRIX4		GMATRIX4_IDENTITY		= { 1.0f, 0.0f, 0.0f, 0.0f, 
															0.0f, 1.0f, 0.0f, 0.0f, 
															0.0f, 0.0f, 1.0f, 0.0f, 
															0.0f, 0.0f, 0.0f, 1.0f };

	GVECTOR2*	GVectorAdd						( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB );
	GVECTOR2*	GVectorAddInPlace				( GVECTOR2* out_vResult, const GVECTOR2* in_vB );
	GVECTOR2*	GVectorAddScaled				( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB, float in_fBScale );
	GVECTOR2*	GVectorAddScaledInPlace			( GVECTOR2* out_vResult, const GVECTOR2* in_vB, float in_fBScale );
	GVECTOR2*	GVectorSubstract				( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB );
	GVECTOR2*	GVectorSubstractInPlace			( GVECTOR2* out_vResult, const GVECTOR2* in_vB );
	GVECTOR2*	GVectorSubstractScaled			( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB, float in_fBScale );
	GVECTOR2*	GVectorSubstractScaledInPlace	( GVECTOR2* out_vResult, const GVECTOR2* in_vB, float in_fBScale );
	GVECTOR2*	GVectorMultiply					( GVECTOR2* out_vResult, const GVECTOR2* in_Vector, float in_fScalar );
	GVECTOR2*	GVectorMultiplyInPlace			( GVECTOR2* out_vResult, float in_fScalar );
	GVECTOR2*	GVectorDivide					( GVECTOR2* out_vResult, const GVECTOR2* in_Vector, float in_fScalar );
	GVECTOR2*	GVectorNormalize				( GVECTOR2* out_vResult, const GVECTOR2* in_Vector );
	GVECTOR2*	GVectorNormalizeInPlace			( GVECTOR2* inout_vResult );
	float		GVectorDot						( const GVECTOR2* in_vA, const GVECTOR2* in_vB );

	GVECTOR3*	GVectorAdd						( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB );
	GVECTOR3*	GVectorAddInPlace				( GVECTOR3* out_vResult, const GVECTOR3* in_vB );
	GVECTOR3*	GVectorAddScaled				( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB, float in_fBScale );
	GVECTOR3*	GVectorAddScaledInPlace			( GVECTOR3* out_vResult, const GVECTOR3* in_vB, float in_fBScale );
	GVECTOR3*	GVectorSubstract				( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB );
	GVECTOR3*	GVectorSubstractInPlace			( GVECTOR3* out_vResult, const GVECTOR3* in_vB );
	GVECTOR3*	GVectorSubstractScaled			( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB, float in_fBScale );
	GVECTOR3*	GVectorSubstractScaledInPlace	( GVECTOR3* out_vResult, const GVECTOR3* in_vB, float in_fBScale );
	GVECTOR3*	GVectorMultiply					( GVECTOR3* out_vResult, const GVECTOR3* in_Vector, float in_fScalar );
	GVECTOR3*	GVectorMultiplyInPlace			( GVECTOR3* out_vResult, float in_fScalar );
	GVECTOR3*	GVectorDivide					( GVECTOR3* out_vResult, const GVECTOR3* in_Vector, float in_fScalar );
	GVECTOR3*	GVectorNormalize				( GVECTOR3* out_vResult, const GVECTOR3* in_Vector );
	GVECTOR3*	GVectorNormalizeInPlace			( GVECTOR3* inout_vResult );
	float		GVectorDot						( const GVECTOR3* in_vA, const GVECTOR3* in_vB );
	GVECTOR3*	GVectorCross					( GVECTOR3* out_vResult, const GVECTOR3* in_vLeft, const GVECTOR3* in_vRight );
	GVECTOR3*	GVectorCrossAndNormalize		( GVECTOR3* out_vResult, const GVECTOR3* in_vLeft, const GVECTOR3* in_vRight );

	GVECTOR4*	GVectorAdd						( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB );
	GVECTOR4*	GVectorAddInPlace				( GVECTOR4* out_vResult, const GVECTOR4* in_vB );
	GVECTOR4*	GVectorAddScaled				( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB, float in_fBScale );
	GVECTOR4*	GVectorAddScaledInPlace			( GVECTOR4* out_vResult, const GVECTOR4* in_vB, float in_fBScale );
	GVECTOR4*	GVectorSubstract				( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB );
	GVECTOR4*	GVectorSubstractInPlace			( GVECTOR4* out_vResult, const GVECTOR4* in_vB );
	GVECTOR4*	GVectorSubstractScaled			( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB, float in_fBScale );
	GVECTOR4*	GVectorSubstractScaledInPlace	( GVECTOR4* out_vResult, const GVECTOR4* in_vB, float in_fBScale );
	GVECTOR4*	GVectorMultiply					( GVECTOR4* out_vResult, const GVECTOR4* in_Vector, float in_fScalar );
	GVECTOR4*	GVectorMultiplyInPlace			( GVECTOR4* out_vResult, float in_fScalar );
	GVECTOR4*	GVectorDivide					( GVECTOR4* out_vResult, const GVECTOR4* in_Vector, float in_fScalar );
	GVECTOR4*	GVectorNormalize				( GVECTOR4* out_vResult, const GVECTOR4* in_Vector );
	GVECTOR4*	GVectorNormalizeInPlace			( GVECTOR4* inout_vResult );
	float		GVectorDot						( const GVECTOR4* in_vA, const GVECTOR4* in_vB );

	GQUATERNION* GQuaternionNormalize			( GQUATERNION* out_vResult, const GQUATERNION* in_vInput );
	GQUATERNION* GQuaternionNormalizeInPlace	( GQUATERNION* inout_vResult );

	GMATRIX2X2*	GMatrixAdd						( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight );
	GMATRIX2X2*	GMatrixSubstract				( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight );
	GMATRIX2X2*	GMatrixMultiply					( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, const GMATRIX2X2* in_mRight );
	GMATRIX2X2*	GMatrixInverse					( GMATRIX2X2* out_mResult, float* out_fDeterminant, const GMATRIX2X2* in_Matrix);
	GMATRIX2X2*	GMatrixTranspose				( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_Matrix);
	GMATRIX2X2*	GMatrixTransposeInPlace			( GMATRIX2X2* inout_mResult );
	GMATRIX2X2*	GMatrixMultiply					( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, float in_fScalar);
	GMATRIX2X2*	GMatrixDivide					( GMATRIX2X2* out_mResult, const GMATRIX2X2* in_mLeft, float in_fScalar);
	GVECTOR2*	GMatrixTransform				( GVECTOR2*	out_vResult, const GVECTOR2* in_vLeft, const GMATRIX2X2* in_mRight );
	GVECTOR2*	GMatrixTransformInPlace			( GVECTOR2*	out_vResult, const GMATRIX2X2* in_mRight );

	GMATRIX3X3*	GMatrixAdd						( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight );
	GMATRIX3X3*	GMatrixSubstract				( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight );
	GMATRIX3X3*	GMatrixMultiply					( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, const GMATRIX3X3* in_mRight );
	GMATRIX3X3*	GMatrixInverse					( GMATRIX3X3* out_mResult, float* out_fDeterminant, const GMATRIX3X3* in_Matrix );
	GMATRIX3X3*	GMatrixTranspose				( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_Matrix );
	GMATRIX3X3*	GMatrixTransposeInPlace			( GMATRIX3X3* inout_mResult );
	GMATRIX3X3*	GMatrixMultiply					( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, float in_fScalar );
	GMATRIX3X3*	GMatrixDivide					( GMATRIX3X3* out_mResult, const GMATRIX3X3* in_mLeft, float in_fScalar );
	GVECTOR3*	GMatrixTransform				( GVECTOR3*	out_vResult, const GVECTOR3* in_vLeft, const GMATRIX3X3* in_mRight );
	GVECTOR4*	GMatrixTransform				( GVECTOR4*	out_vResult, const GVECTOR4* in_vLeft, const GMATRIX3X3* in_mRight );
	GVECTOR3*	GMatrixTransformInPlace			( GVECTOR3*	out_vResult, const GMATRIX3X3* in_mRight );
	GVECTOR4*	GMatrixTransformInPlace			( GVECTOR4*	out_vResult, const GMATRIX3X3* in_mRight );

	GMATRIX4X4*	GMatrixAdd						( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight );
	GMATRIX4X4*	GMatrixSubstract				( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight );
	GMATRIX4X4*	GMatrixMultiply					( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, const GMATRIX4X4* in_mRight );
	GMATRIX4X4*	GMatrixInverse					( GMATRIX4X4* out_mResult, float* out_fDeterminant, const GMATRIX4X4* in_Matrix );
	GMATRIX4X4*	GMatrixInverseInPlace			( GMATRIX4X4* out_mResult, float* out_fDeterminant );
	GMATRIX4X4*	GMatrixTranspose				( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_Matrix );
	GMATRIX4X4*	GMatrixTransposeInPlace			( GMATRIX4X4* inout_mResult );
	GMATRIX4X4*	GMatrixMultiply					( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, float in_fScalar );
	GMATRIX4X4*	GMatrixDivide					( GMATRIX4X4* out_mResult, const GMATRIX4X4* in_mLeft, float in_fScalar );
	GVECTOR3*	GMatrixTransform				( GVECTOR3*	out_vResult, const GVECTOR3* in_vLeft, const GMATRIX4X4* in_mRight );
	GVECTOR4*	GMatrixTransform				( GVECTOR4*	out_vResult, const GVECTOR4* in_vLeft, const GMATRIX4X4* in_mRight );
	GVECTOR3*	GMatrixTransformInPlace			( GVECTOR3*	out_vResult, const GMATRIX4X4* in_mRight );
	GVECTOR4*	GMatrixTransformInPlace			( GVECTOR4*	out_vResult, const GMATRIX4X4* in_mRight );

	bool operator ==( const GVECTOR2& a	, const GVECTOR2& b );
	bool operator ==( const GVECTOR3& a	, const GVECTOR3& b );
	bool operator ==( const GVECTOR4& a	, const GVECTOR4& b );
	bool operator ==( const GPLANE& a	, const GPLANE& b );
	bool operator !=( const GVECTOR2& a	, const GVECTOR2& b );
	bool operator !=( const GVECTOR3& a	, const GVECTOR3& b );
	bool operator !=( const GVECTOR4& a	, const GVECTOR4& b );
	bool operator !=( const GPLANE& a	, const GPLANE& b );

	typedef enum _GAXIS
	{
		GAXIS_X = 0,
		GAXIS_Y = 1,
		GAXIS_Z = 2
	} GAXIS;

	//( float rho, float phi, float theta )
	GVECTOR3 SphericalToCartesian( double x, double y, double radius );

	GVECTOR3* ParabolicMotion		( GVECTOR3* out_Position, const GVECTOR3* in_Velocity, float fSec, float* fGravity, GAXIS nAxis );
	GVECTOR3* ParabolicMotionXUP	( GVECTOR3* out_Position, const GVECTOR3* in_Velocity, float fSec, float* fGravity );
	GVECTOR3* ParabolicMotionYUP	( GVECTOR3* out_Position, const GVECTOR3* in_Velocity, float fSec, float* fGravity );
	GVECTOR3* ParabolicMotionZUP	( GVECTOR3* out_Position, const GVECTOR3* in_Velocity, float fSec, float* fGravity );
} // namespace

#endif //  _GVECTOR_CORE_H
