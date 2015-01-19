#include "GVector_core.h"

#include "GDebugger_core.h"
#include "GCore_printf.h"

#include <math.h>

using namespace god;

GVECTOR2*	god::GVectorAdd			( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB )
{
	out_vResult->x = in_vA->x + in_vB->x;
	out_vResult->y = in_vA->y + in_vB->y;
	return out_vResult;
};

GVECTOR2*	god::GVectorAddInPlace	( GVECTOR2* out_vResult, const GVECTOR2* in_vB )
{
	out_vResult->x += in_vB->x;
	out_vResult->y += in_vB->y;
	return out_vResult;
};

GVECTOR2*	god::GVectorAddScaled	( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB, float in_fBScale )
{
	out_vResult->x = in_vA->x + in_vB->x*in_fBScale;
	out_vResult->y = in_vA->y + in_vB->y*in_fBScale;
	return out_vResult;
};

GVECTOR2*	god::GVectorAddScaledInPlace( GVECTOR2* out_vResult, const GVECTOR2* in_vB, float in_fBScale )
{
	out_vResult->x	+= in_vB->x*in_fBScale;
	out_vResult->y	+= in_vB->y*in_fBScale;
	return out_vResult;
};

GVECTOR2*	god::GVectorSubstract	( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB )
{ 
	out_vResult->x = in_vA->x - in_vB->x;
	out_vResult->y = in_vA->y - in_vB->y;
	return out_vResult;
};


GVECTOR2*	god::GVectorSubstractInPlace ( GVECTOR2* out_vResult, const GVECTOR2* in_vB )	
{
	out_vResult->x -= in_vB->x;
	out_vResult->y -= in_vB->y;
	return out_vResult;
};

GVECTOR2*	god::GVectorSubstractScaled	( GVECTOR2* out_vResult, const GVECTOR2* in_vA, const GVECTOR2* in_vB, float in_fBScale )
{
	out_vResult->x	= in_vA->x - in_vB->x*in_fBScale;
	out_vResult->y	= in_vA->y - in_vB->y*in_fBScale;
	return out_vResult;
};

GVECTOR2*	god::GVectorSubstractScaledInPlace( GVECTOR2* out_vResult, const GVECTOR2* in_vB, float in_fBScale )
{
	out_vResult->x	-= in_vB->x*in_fBScale;
	out_vResult->y	-= in_vB->y*in_fBScale;
	return out_vResult;
};

GVECTOR2*	god::GVectorMultiply	( GVECTOR2* out_vResult, const GVECTOR2* in_Vector, float in_fScalar )
{ 
	out_vResult->x = in_Vector->x*in_fScalar;
	out_vResult->y = in_Vector->y*in_fScalar;
	return out_vResult;
};

GVECTOR2*	god::GVectorMultiplyInPlace( GVECTOR2* out_vResult, float in_fScalar )
{ 
	out_vResult->x = out_vResult->x*in_fScalar;
	out_vResult->y = out_vResult->y*in_fScalar;
	return out_vResult;
};

GVECTOR2*	god::GVectorDivide		( GVECTOR2* out_vResult, const GVECTOR2* in_Vector, float in_fScalar )
{ 
	if( in_fScalar != 0.0f )
	{
		out_vResult->x = in_Vector->x/in_fScalar;
		out_vResult->y = in_Vector->y/in_fScalar;
	}
	return out_vResult;
};

float		god::GVectorDot			( const GVECTOR2* in_vA, const GVECTOR2* in_vB )
{
	return in_vA->x*in_vB->x+
		in_vA->y*in_vB->y;
};

GVECTOR3*	god::GVectorAdd			( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB )
{
	out_vResult->x = in_vA->x + in_vB->x;
	out_vResult->y = in_vA->y + in_vB->y;
	out_vResult->z = in_vA->z + in_vB->z;
	return out_vResult;
};
GVECTOR3*	god::GVectorAddInPlace	( GVECTOR3* out_vResult, const GVECTOR3* in_vB )
{
	out_vResult->x += in_vB->x;
	out_vResult->y += in_vB->y;
	out_vResult->z += in_vB->z;
	return out_vResult;
};

GVECTOR3*	god::GVectorAddScaled	( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB, float in_fBScale )
{
	out_vResult->x = in_vA->x + in_vB->x*in_fBScale;
	out_vResult->y = in_vA->y + in_vB->y*in_fBScale;
	out_vResult->z = in_vA->z + in_vB->z*in_fBScale;
	return out_vResult;
};

GVECTOR3*	god::GVectorAddScaledInPlace( GVECTOR3* out_vResult, const GVECTOR3* in_vB, float in_fBScale )
{
	out_vResult->x	+= in_vB->x*in_fBScale;
	out_vResult->y	+= in_vB->y*in_fBScale;
	out_vResult->z	+= in_vB->z*in_fBScale;
	return out_vResult;
};

GVECTOR3*	god::GVectorSubstract	( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB )
{ 
	out_vResult->x = in_vA->x - in_vB->x;
	out_vResult->y = in_vA->y - in_vB->y;
	out_vResult->z = in_vA->z - in_vB->z;
	return out_vResult;
};

GVECTOR3*	god::GVectorSubstractInPlace ( GVECTOR3* out_vResult, const GVECTOR3* in_vB )	
{
	out_vResult->x -= in_vB->x;
	out_vResult->y -= in_vB->y;
	out_vResult->z -= in_vB->z;
	return out_vResult;
};

GVECTOR3*	god::GVectorSubstractScaled	( GVECTOR3* out_vResult, const GVECTOR3* in_vA, const GVECTOR3* in_vB, float in_fBScale )
{
	out_vResult->x	= in_vA->x - in_vB->x*in_fBScale;
	out_vResult->y	= in_vA->y - in_vB->y*in_fBScale;
	out_vResult->z	= in_vA->z - in_vB->z*in_fBScale;
	return out_vResult;
};

GVECTOR3*	god::GVectorSubstractScaledInPlace( GVECTOR3* out_vResult, const GVECTOR3* in_vB, float in_fBScale )
{
	out_vResult->x	-= in_vB->x*in_fBScale;
	out_vResult->y	-= in_vB->y*in_fBScale;
	out_vResult->z	-= in_vB->z*in_fBScale;
	return out_vResult;
};
GVECTOR3*	god::GVectorMultiply	( GVECTOR3* out_vResult, const GVECTOR3* in_Vector, float in_fScalar )
{ 
	out_vResult->x = in_Vector->x*in_fScalar;
	out_vResult->y = in_Vector->y*in_fScalar;
	out_vResult->z = in_Vector->z*in_fScalar;
	return out_vResult;
};

GVECTOR3*	god::GVectorMultiplyInPlace	( GVECTOR3* out_vResult, float in_fScalar )
{ 
	out_vResult->x = out_vResult->x*in_fScalar;
	out_vResult->y = out_vResult->y*in_fScalar;
	out_vResult->z = out_vResult->z*in_fScalar;
	return out_vResult;
};

GVECTOR3*	god::GVectorDivide		( GVECTOR3* out_vResult, const GVECTOR3* in_Vector, float in_fScalar )
{ 
	if( in_fScalar != 0.0f )
	{
		out_vResult->x = in_Vector->x/in_fScalar;
		out_vResult->y = in_Vector->y/in_fScalar;
		out_vResult->z = in_Vector->z/in_fScalar;
	}
	return out_vResult;
};

GVECTOR2* god::GVectorNormalize			( GVECTOR2* out_vResult, const GVECTOR2* in_vInput )
{
	float l 
		= in_vInput->x*in_vInput->x
		+ in_vInput->y*in_vInput->y; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= in_vInput->x / l;
	out_vResult->y	= in_vInput->y / l;
	return out_vResult;
};


GVECTOR3* god::GVectorNormalize			( GVECTOR3* out_vResult, const GVECTOR3* in_vInput )
{
	float l 
		= in_vInput->x*in_vInput->x
		+ in_vInput->y*in_vInput->y
		+ in_vInput->z*in_vInput->z; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= in_vInput->x / l;
	out_vResult->y	= in_vInput->y / l;
	out_vResult->z	= in_vInput->z / l;
	return out_vResult;
};


GVECTOR4* god::GVectorNormalize			( GVECTOR4* out_vResult, const GVECTOR4* in_vInput )
{
	float l 
		= in_vInput->x*in_vInput->x
		+ in_vInput->y*in_vInput->y
		+ in_vInput->z*in_vInput->z; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= in_vInput->x / l;
	out_vResult->y	= in_vInput->y / l;
	out_vResult->z	= in_vInput->z / l;
	out_vResult->w	= 1.0f; //w / l;
	return out_vResult;
};

GQUATERNION* god::GQuaternionNormalize			( GQUATERNION* out_vResult, const GQUATERNION* in_vInput )
{
	float l 
		= in_vInput->w*in_vInput->w
		+ in_vInput->x*in_vInput->x
		+ in_vInput->y*in_vInput->y
		+ in_vInput->z*in_vInput->z; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= in_vInput->x / l;
	out_vResult->y	= in_vInput->y / l;
	out_vResult->z	= in_vInput->z / l;
	out_vResult->w	= in_vInput->w / l; //w / l;
	return out_vResult;
};

GQUATERNION* god::GQuaternionNormalizeInPlace	( GQUATERNION* inout_vResult )
{
	float l 
		= inout_vResult->w*inout_vResult->w
		+ inout_vResult->x*inout_vResult->x
		+ inout_vResult->y*inout_vResult->y
		+ inout_vResult->z*inout_vResult->z; 
	if( 0 == l ) 
		return inout_vResult;
	l = sqrt( l );
	inout_vResult->x	= inout_vResult->x / l;
	inout_vResult->y	= inout_vResult->y / l;
	inout_vResult->z	= inout_vResult->z / l;
	inout_vResult->w	= inout_vResult->w / l; //w / l;
	return inout_vResult;
};


GVECTOR2* god::GVectorNormalizeInPlace( GVECTOR2* out_vResult )
{
	float l 
		= out_vResult->x*out_vResult->x
		+ out_vResult->y*out_vResult->y; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= out_vResult->x / l;
	out_vResult->y	= out_vResult->y / l;
	return out_vResult;
};

GVECTOR3* god::GVectorNormalizeInPlace( GVECTOR3* out_vResult )
{
	float l 
		= out_vResult->x*out_vResult->x
		+ out_vResult->y*out_vResult->y
		+ out_vResult->z*out_vResult->z; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= out_vResult->x / l;
	out_vResult->y	= out_vResult->y / l;
	out_vResult->z	= out_vResult->z / l;
	return out_vResult;
};


GVECTOR4* god::GVectorNormalizeInPlace( GVECTOR4* out_vResult )
{
	float l 
		= out_vResult->x*out_vResult->x
		+ out_vResult->y*out_vResult->y
		+ out_vResult->z*out_vResult->z; 
	if( 0 == l ) 
		return out_vResult;
	l = sqrt( l );
	out_vResult->x	= out_vResult->x / l;
	out_vResult->y	= out_vResult->y / l;
	out_vResult->z	= out_vResult->z / l;
	out_vResult->w	= 1.0f; //w / l;
	return out_vResult;
};


float		god::GVectorDot			( const GVECTOR3* in_vA, const GVECTOR3* in_vB )
{
	return in_vA->x*in_vB->x+
		in_vA->y*in_vB->y+
		in_vA->z*in_vB->z;
};

GVECTOR3*	god::GVectorCross		( GVECTOR3* out_vResult, const GVECTOR3* in_vLeft, const GVECTOR3* in_vRight )
{ 
	out_vResult->x = in_vLeft->y * in_vRight->z - in_vLeft->z * in_vRight->y;
	out_vResult->y = in_vLeft->z * in_vRight->x - in_vLeft->x * in_vRight->z;
	out_vResult->z = in_vLeft->x * in_vRight->y - in_vLeft->y * in_vRight->x;
	return out_vResult; 
};

GVECTOR3*	god::GVectorCrossAndNormalize		( GVECTOR3* out_vResult, const GVECTOR3* in_vLeft, const GVECTOR3* in_vRight )
{ 
	out_vResult->x = in_vLeft->y * in_vRight->z - in_vLeft->z * in_vRight->y;
	out_vResult->y = in_vLeft->z * in_vRight->x - in_vLeft->x * in_vRight->z;
	out_vResult->z = in_vLeft->x * in_vRight->y - in_vLeft->y * in_vRight->x;
	return GVectorNormalizeInPlace( out_vResult ); 
};

//------ GVECTOR4
GVECTOR4*	god::GVectorAdd			( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB )	
{
	out_vResult->x = in_vA->x + in_vB->x;
	out_vResult->y = in_vA->y + in_vB->y;
	out_vResult->z = in_vA->z + in_vB->z;
	out_vResult->w = 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorAddInPlace			( GVECTOR4* out_vResult, const GVECTOR4* in_vB )	
{
	out_vResult->x += in_vB->x;
	out_vResult->y += in_vB->y;
	out_vResult->z += in_vB->z;
	out_vResult->w = 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorAddScaled	( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB, float in_fBScale )
{
	out_vResult->x	= in_vA->x + in_vB->x*in_fBScale;
	out_vResult->y	= in_vA->y + in_vB->y*in_fBScale;
	out_vResult->z	= in_vA->z + in_vB->z*in_fBScale;
	out_vResult->w	= 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorAddScaledInPlace( GVECTOR4* out_vResult, const GVECTOR4* in_vB, float in_fBScale )
{
	out_vResult->x	+= in_vB->x*in_fBScale;
	out_vResult->y	+= in_vB->y*in_fBScale;
	out_vResult->z	+= in_vB->z*in_fBScale;
	out_vResult->w	= 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorSubstract	( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB )
{ 
	out_vResult->x	= in_vA->x - in_vB->x;
	out_vResult->y	= in_vA->y - in_vB->y;
	out_vResult->z	= in_vA->z - in_vB->z;
	out_vResult->w	= 1.0f;
	return out_vResult;
};


GVECTOR4*	god::GVectorSubstractInPlace ( GVECTOR4* out_vResult, const GVECTOR4* in_vB )	
{
	out_vResult->x -= in_vB->x;
	out_vResult->y -= in_vB->y;
	out_vResult->z -= in_vB->z;
	out_vResult->w = 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorSubstractScaled	( GVECTOR4* out_vResult, const GVECTOR4* in_vA, const GVECTOR4* in_vB, float in_fBScale )
{
	out_vResult->x	= in_vA->x - in_vB->x*in_fBScale;
	out_vResult->y	= in_vA->y - in_vB->y*in_fBScale;
	out_vResult->z	= in_vA->z - in_vB->z*in_fBScale;
	out_vResult->w	= 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorSubstractScaledInPlace( GVECTOR4* out_vResult, const GVECTOR4* in_vB, float in_fBScale )
{
	out_vResult->x	-= in_vB->x*in_fBScale;
	out_vResult->y	-= in_vB->y*in_fBScale;
	out_vResult->z	-= in_vB->z*in_fBScale;
	out_vResult->w	= 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorMultiply	( GVECTOR4* out_vResult, const GVECTOR4* in_Vector, float in_fScalar )
{ 
	out_vResult->x = in_Vector->x*in_fScalar;
	out_vResult->y = in_Vector->y*in_fScalar;
	out_vResult->z = in_Vector->z*in_fScalar;
	out_vResult->w = 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorMultiplyInPlace	( GVECTOR4* out_vResult, float in_fScalar )
{ 
	out_vResult->x = out_vResult->x*in_fScalar;
	out_vResult->y = out_vResult->y*in_fScalar;
	out_vResult->z = out_vResult->z*in_fScalar;
	out_vResult->w = 1.0f;
	return out_vResult;
};

GVECTOR4*	god::GVectorDivide		( GVECTOR4* out_vResult, const GVECTOR4* in_Vector, float in_fScalar )
{ 
	if( in_fScalar != 0.0f )
	{
		out_vResult->x = in_Vector->x/in_fScalar;
		out_vResult->y = in_Vector->y/in_fScalar;
		out_vResult->z = in_Vector->z/in_fScalar;
		out_vResult->w = 1.0f;
	}
	else
	{
		warning_printf("Warning! Dividing vector by zero!");
		out_vResult->x = 0.0f;
		out_vResult->y = 0.0f;
		out_vResult->z = 0.0f;
		out_vResult->w = 1.0f;
	}

	return out_vResult;
};

float		god::GVectorDot			( const GVECTOR4* in_vA, const GVECTOR4* in_vB )
{
	return	in_vA->x*in_vB->x+
			in_vA->y*in_vB->y+
			in_vA->z*in_vB->z;
};
#define GGRAVITY 9.8f

GVECTOR3* god::ParabolicMotionXUP( GVECTOR3* out_Position, const GVECTOR3* vVelocity, float fSec, float* fGravity )
{
	(*out_Position).x = vVelocity->x  * fSec;
	(*out_Position).y = vVelocity->y  * fSec;
	(*out_Position).z = vVelocity->z  * fSec;
	fSec *= fSec;
	if( fGravity )
		(*out_Position).x += ((fSec * .5f) * *fGravity) * -1;
	else
		(*out_Position).x += ((fSec * .5f) * GGRAVITY) * -1;
	return out_Position;
}

GVECTOR3* god::ParabolicMotionZUP( GVECTOR3* out_Position, const GVECTOR3* vVelocity, float fSec, float* fGravity )
{
	(*out_Position).x = vVelocity->x  * fSec;
	(*out_Position).y = vVelocity->y  * fSec;
	(*out_Position).z = vVelocity->z  * fSec;
	fSec *= fSec;
	if( fGravity )
		(*out_Position).z += ((fSec * .5f) * *fGravity) * -1;
	else
		(*out_Position).z += ((fSec * .5f) * GGRAVITY) * -1;
	return out_Position;
}

GVECTOR3* god::ParabolicMotionYUP( GVECTOR3* out_Position, const GVECTOR3* vVelocity, float fSec, float* fGravity )
{
	(*out_Position).x = vVelocity->x  * fSec;
	(*out_Position).y = vVelocity->y  * fSec;
	(*out_Position).z = vVelocity->z  * fSec;
	fSec *= fSec;
	if( fGravity )
		(*out_Position).y += ((fSec * .5f) * *fGravity) * -1;
	else
		(*out_Position).y += ((fSec * .5f) * GGRAVITY) * -1;
	return out_Position;
}

GVECTOR3* god::ParabolicMotion( GVECTOR3* out_Position, const GVECTOR3* vVelocity, float fSec, float* fGravity, GAXIS nAxis )
{
	(*out_Position).x = vVelocity->x  * fSec;
	(*out_Position).y = vVelocity->y  * fSec;
	(*out_Position).z = vVelocity->z  * fSec;
	fSec *= fSec;
	if( fGravity )
		((float*)out_Position)[nAxis] += ((fSec * .5f) * *fGravity) * -1;
	else
		((float*)out_Position)[nAxis] += ((fSec * .5f) * GGRAVITY) * -1;
	return out_Position;
}

GVECTOR3 god::SphericalToCartesian( double x, double y, double radius ) //( float rho, float phi, float theta )
{
	GPROFILE_METHOD( 0, "god", "SphericalToCartesian" );
	god::GVECTOR3 v;
	double rho = radius, phi=x, theta = y;

	v.x		= (float) (rho*sin(theta)*sin(phi));
	v.y		= (float) -(rho*cos(theta));
	v.z		= (float) (rho*sin(theta)*cos(phi));
	//v.y		= rho*sin(theta)*cos(phi);
	//v.z		= rho*cos(theta);
	return v;
}

bool god::operator ==( const GVECTOR2& a, const GVECTOR2& b )
{
	return a.x == b.x 
		&& a.y == b.y;
};

bool god::operator ==( const GVECTOR3& a, const GVECTOR3& b )
{
	return a.x == b.x 
		&& a.y == b.y
		&& a.z == b.z;
};

bool god::operator ==( const GVECTOR4& a, const GVECTOR4& b )
{
	return a.x == b.x 
		&& a.y == b.y
		&& a.z == b.z
		&& a.w == b.w;
};

bool god::operator ==( const god::GPLANE& a, const god::GPLANE& b )
{
	return a.x == b.x 
		&& a.y == b.y
		&& a.z == b.z
		&& a.d == b.d;
};

bool god::operator !=( const GVECTOR2& a, const GVECTOR2& b )
{
	return a.x != b.x 
		|| a.y != b.y;
};

bool god::operator !=( const GVECTOR3& a, const GVECTOR3& b )
{
	return a.x != b.x 
		|| a.y != b.y
		|| a.z != b.z;
};

bool god::operator !=( const GVECTOR4& a, const GVECTOR4& b )
{
	return a.x != b.x 
		|| a.y != b.y
		|| a.z != b.z
		|| a.w != b.w;
};

bool god::operator !=( const god::GPLANE& a, const god::GPLANE& b )
{
	return a.x != b.x 
		|| a.y != b.y
		|| a.z != b.z
		|| a.d != b.d;
};