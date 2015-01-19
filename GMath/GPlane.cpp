#include "GVector_object.h"
#include <math.h>

using namespace god;

bool _GPlane::operator==( const GPLANE& other ) const
{
	return d	== other.d
		&& x	== other.x
		&& y	== other.y
		&& z	== other.z;
}

bool _GPlane::operator!=( const GPLANE& other ) const
{
	return d	!= other.d
		|| x	!= other.x
		|| y	!= other.y
		|| z	!= other.z;
}
// F U N C T I O N S ///////////////////////////////////////////////
void _GPlane::Set(const GVECTOR3& vcN, const GVECTOR3 &vcP) 
{
	d		= -( GVector3( vcN ) * vcP );
	x     = vcN.x;
	y     = vcN.y;
	z     = vcN.z;
}
//----------------------------------------------------------------//

void _GPlane::Set(const GVECTOR3 &vcN, float fD) 
{
	x	= vcN.x;
	y	= vcN.y;
	z	= vcN.z;
	d	= fD;
}
//----------------------------------------------------------------//

void _GPlane::Set(const GVECTOR3 &v0, 
				  const GVECTOR3 &v1, 
				  const GVECTOR3 &v2) 
{
	GVector3 vEdge1 = GVector3(v1) - v0;
	GVector3 vEdge2 = GVector3(v2) - v0;
	GVector3 v = GVector3( x, y, z ).Cross(vEdge1, vEdge2);
	v.Normalize();
	float d = - (v * v0);
	Set( v, d );
}
//----------------------------------------------------------------//

// Calculate distance to point. Plane normal must be normalized.
float _GPlane::Distance(const GVECTOR3 &vPoint) const
{
	return ( fabs((GVector3( x, y, z )*vPoint) - d) );
}
//----------------------------------------------------------------//


// Classify point to plane.
GVISIBILITY_VALUE _GPlane::Classify(const GVECTOR3& vPoint) const
{
	float f = (GVector3( x, y, z )*vPoint) + d;

	if (f > GEPSILON) return GFRONT;
	if (f < GEPSILON) return GBACK;
	//if (f >  0.00001f) return GFRONT;
	//if (f < -0.00001f) return GBACK;
	return GPLANAR;
}
//----------------------------------------------------------------//


// clips a ray into two segments if it intersects the plane
bool _GPlane::Clip( const GRAY* _pRay, float fL, GRAY* pF, GRAY* pB )
{
	GVector3 vHit(0.0f,0.0f,0.0f);

	const GRay *pRay = (GRay*)_pRay;

	// ray intersects plane at all?
	if ( !pRay->Intersects( *this, false, fL, 0, &vHit) ) 
		return false;

	int n = Classify( _pRay->Origin );

	// ray comes fron planes backside
	if ( n == GBACK ) 
	{
		if (pB) 
		{
			pB->Origin		= pRay->Origin;
			pB->Direction	= pRay->Direction;
		}
		if (pF) 
		{
			pF->Origin = vHit;
			pF->Direction = pRay->Direction;
		}
	}
	// ray comes from planes front side
	else if ( n == GFRONT ) 
	{
		if (pF) 
		{
			pF->Origin		= pRay->Origin;
			pF->Direction	= pRay->Direction;
		}
		if (pB) 
		{
			pB->Origin = vHit;
			pB->Direction = pRay->Direction;
		}
	}

	return true;
} // Clip [ray]
//----------------------------------------------------------------//


// Intersection of two planes. If third parameter is given the line
// of intersection will be calculated. (www.magic-software.com)
bool _GPlane::Intersects( const GPLANE& plane, GRAY *pIntersection ) const
{
	GVector3 vThis( x, y, z ),
		vOther( plane.x, plane.y, plane.z );

	GVector3 vCross;
	float fSqrLength;

	// if cross product of normals is 0 then planes are parallel
	vCross.Cross( vThis, vOther );
	fSqrLength = vCross.SqLength();

	if (fSqrLength < 1e-08f) 
		return false;

	// find line of intersection
	if (pIntersection) 
	{
		float fN00 = vThis.SqLength();
		float fN01 = vThis * vOther;
		float fN11 = vOther.SqLength();
		float fDet = fN00*fN11 - fN01*fN01;

		if (fabs(fDet) < 1e-08f) 
			return false;

		float fInvDet = 1.0f/fDet;
		float fC0 = (fN11*this->d - fN01*plane.d) * fInvDet;
		float fC1 = (fN00*plane.d - fN01*this->d) * fInvDet;

		(*pIntersection).Direction  = vCross;
		(*pIntersection).Origin		= vThis*fC0 + vOther*fC1;
	}

	return true;
} // Intersects(Plane)
//----------------------------------------------------------------//


// Intersection of a plane with a triangle. If all vertices of the
// triangle are on the same side of the plane, no intersection occured. 
bool _GPlane::Intersects(const GVECTOR3 &v0, const GVECTOR3 &v1, const GVECTOR3 &v2) const 
{
	int n = this->Classify(v0);

	if ( (n == this->Classify(v1)) && (n == this->Classify(v2)) )
		return false;

	return true;
} // Intersects(Tri)
//----------------------------------------------------------------//

GMatrix4x4 _GPlane::GetReflectionMatrix( void ) const
{
	GMatrix4x4 m;
	const GVector3 v( x, y, z );
	const GVector3* n = &v;
	m._11 = -2 * n->x * n->x+1; /**/ m._12 = -2 * n->y * n->x;		/**/ m._13 = -2 * n->z * n->x;		/**/ m._14 = 0;
	m._21 = -2 * n->x * n->y;	/**/ m._22 = -2 * n->y * n->y+1;	/**/ m._23 = -2 * n->z * n->y;		/**/ m._24 = 0;
	m._31 = -2 * n->x * n->z;	/**/ m._32 = -2 * n->y * n->z;		/**/ m._33 = -2 * n->z * n->z+1;	/**/ m._34 = 0;
	m._41 = -2 * n->x * d;		/**/ m._42 = -2 * n->y * d;			/**/ m._43 = -2 * n->z * d;			/**/ m._44 = 1;
	return m;
}
