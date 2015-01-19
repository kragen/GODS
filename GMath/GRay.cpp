#include "GVector_object.h"
#include <math.h>

using namespace god;
// F U N C T I O N S ///////////////////////////////////////////////

// set attributs
void _GRay::Set( const GVECTOR3& vOrigin, const GVECTOR3& vDirection ) 
{
   Origin		= vOrigin;
   Direction	= vDirection;
}
//----------------------------------------------------------------//


// transform ray into matrix space
void _GRay::DeTransform(const GMATRIX4X4& _m) 
{
	GMatrix3x3 mInv;
	GMatrix3x3 m;
	m._11=_m._11; m._12=_m._12; m._13=_m._13;
	m._21=_m._21; m._22=_m._22; m._23=_m._23;
	m._31=_m._31; m._32=_m._32; m._33=_m._33;
	// invert translation
	Origin.x -= _m._41;   
	Origin.y -= _m._42;
	Origin.z -= _m._43;

	// delet it from matrix
//	m._41 = m._42 = m._43 = 0.0f;

	// invert matrix and apply to ray
	mInv.SetInverse(m);
	Origin		= GVector3( Origin ) * mInv;
	Direction	= GVector3( Direction ) * mInv;//* mInv;
}
//----------------------------------------------------------------//


// test for intersection with triangle
bool _GRay::Intersects(const GVECTOR3 &v0, const GVECTOR3 &v1, 
                        const GVECTOR3 &v2, bool bCull, float *pTime) const
{
	GVector3 pvec, tvec; // , qvec;

	GVector3 edge1 = GVector3(v1) - v0;
	GVector3 edge2 = GVector3(v2) - v0;

	pvec.Cross(Direction, edge2);

	// if close to 0 ray is parallel
	float det = edge1.Dot( pvec );
	if ( (bCull) && (det < 0.0001f) )
		return false;
	else if ( (det < 0.0001f) && (det > -0.0001f) )
		return false;

	// negative means beyond plane
	tvec = GVector3(Origin) - v0;
	float u = tvec.Dot( pvec );
	if (u < 0.0f || u > det)
		return false;

	pvec.Cross(tvec, edge1);
	float v = GVector3(Direction).Dot( pvec );
	if( v < 0.0f || u+v > det)
		return false;

	if (pTime) 
	{
		*pTime = edge2.Dot( pvec );
		float fInvDet = 1.0f / det;
		*pTime *= fInvDet;
	}

	return true;
} // Intersects(Tri)
//----------------------------------------------------------------//


// test for intersection with triangle at certain length (line segment),
// same as above but test distance to intersection vs segment length.
bool _GRay::Intersects( const GVECTOR3 &v0, const GVECTOR3 &v1, 
                        const GVECTOR3 &v2, bool bCull, float fLength, float *pTime ) const
{
   GVector3 pvec, tvec; // , qvec

   GVector3 edge1 = GVector3(v1) - v0;
   GVector3 edge2 = GVector3(v2) - v0;

   pvec.Cross(Direction, edge2);

   // if close to 0 ray is parallel
   float det = edge1.Dot( pvec );
   if ( (bCull) && (det < 0.0001f) )
      return false;
   else if ( (det < 0.0001f) && (det > -0.0001f) )
      return false;

   // distance to plane, < 0 means beyond plane
   tvec = GVector3(Origin) - v0;
   float u = tvec.Dot( pvec );
   if( u < 0.0f || u > det )
      return false;

   pvec.Cross(tvec, edge1); // test other plane
   float v = GVector3(Direction).Dot( pvec );
   if (v < 0.0f || u+v > det)
      return false;
   
	if( pTime ) 
	{
		*pTime = edge2.Dot( pvec );
		float fInvDet = 1.0f / det;
		*pTime *= fInvDet;
		// not on segment?
		if (*pTime > fLength) 
			return false; 
	}
	else 
	{
		// collision but not on segment?
		float f = (edge2.Dot( pvec )) * (1.0f / det);
		if (f > fLength) 
			return false;
	}

	return true;
} // Intersects(Tri at length)
//----------------------------------------------------------------//


// Intersection with Plane from origin till infinity. 
bool _GRay::Intersects(const GPLANE& plane, bool bCull, float *t, GVECTOR3* vcHit) const 
{
	GVector3 planeVec( plane.x, plane.y, plane.z );
	float Vd = planeVec * Direction;

	// ray parallel to plane
	if (fabs(Vd) < GEPSILON)
		return false;

	// normal pointing away from ray dir
	// => intersection backface if any
	if (bCull && (Vd > 0.0f))
		return false;

	float Vo = -( (planeVec * Origin) + plane.d);

	float _t = Vo / Vd;

	// intersection behind ray origin
	if (_t < 0.0f)
		return false;

	if (vcHit) 
	{
		(*vcHit) = (GVector3(Direction) * _t)+Origin;
	}

	if (t)
		(*t) = _t;

	return true;
} // Intersects(Plane)
//----------------------------------------------------------------//


// Intersection with Plane at distance fL. 
bool _GRay::Intersects(const GPLANE &plane, bool bCull, float fL, float *t, GVECTOR3 *vcHit) const
{
	GVector3 planeVec( plane.x, plane.y, plane.z );
	float Vd = planeVec * Direction;

   // ray parallel to plane
   if (fabs(Vd) < GEPSILON)
      return false;

   // normal pointing away from ray dir
   // => intersection backface if any
   if (bCull && (Vd > 0.0f))
      return false;

   float Vo = -( (planeVec * Origin) + plane.d);

   float _t = Vo / Vd;

   // intersection behind ray origin or beyond valid range
   if ( (_t < 0.0f) || (_t > fL) )
      return false;

   if (vcHit) {
      (*vcHit) = (GVector3(Direction) * _t)+Origin;
      }

   if (t)
      (*t) = _t;

   return true;
   } // Intersects(Plane)
//----------------------------------------------------------------//




// FOLLOWING CODE IS Copyright 2001 softSurfer, 2012 Dan Sunday
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.
//
//// Assume that classes are already given for the objects:
////    Point and Vector with
////        coordinates {float x, y, z;}
////        operators for:
////            == to test  equality
////            != to test  inequality
////            (Vector)0 =  (0,0,0)         (null vector)
////            Point   = Point ± Vector
////            Vector =  Point - Point
////            Vector =  Scalar * Vector    (scalar product)
////            Vector =  Vector * Vector    (cross product)
////    Line and Ray and Segment with defining  points {Point P0, P1;}
////        (a Line is infinite, Rays and  Segments start at P0)
////        (a Ray extends beyond P1, but a  Segment ends at P1)
////    Plane with a point and a normal {Point V0; Vector  n;}
////    Triangle with defining vertices {Point V0, V1, V2;}
////    Polyline and Polygon with n vertices {int n;  Point *V;}
////        (a Polygon has V[n]=V[0])
////===================================================================
// 
//
//#define SMALL_NUM   0.00000001 // anything that avoids division overflow
//// dot product (3D) which allows vector operations in arguments
//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
// 
//
//
//// intersect3D_RayTriangle(): find the 3D intersection of a ray with a triangle
////    Input:  a ray R, and a triangle T
////    Output: *I = intersection point (when it exists)
////    Return: -1 = triangle is degenerate (a segment or point)
////             0 =  disjoint (no intersect)
////             1 =  intersect in unique point I1
////             2 =  are in the same plane
//int intersect3D_RayTriangle( GRay& R, GVector3 T[3], GVector3* I )
//{
//    GVector3 u, v, n;			// triangle vectors
//    GVector3 dir, w0, w;		// ray vectors
//    float r, a, b;              // params to calc ray-plane intersect
//
//    // get triangle edge vectors and plane normal
//    u = T[1] - T[0];
//    v = T[2] - T[0];
//    n = u.Cross( v );			// cross product
//    if (n == (GVector3)0)		// triangle is degenerate
//        return -1;				// do not deal with this case
//
//    dir = R.P1 - R.P0;			// ray direction vector
//    w0 = R.P0 - T[0];
//    a = -dot(n,w0);
//    b = dot(n,dir);
//    if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
//        if (a == 0)                 // ray lies in triangle plane
//            return 2;
//        else return 0;              // ray disjoint from plane
//    }
//
//    // get intersect point of ray with triangle plane
//    r = a / b;
//    if (r < 0.0)                    // ray goes away from triangle
//        return 0;                   // => no intersect
//    // for a segment, also test if (r > 1.0) => no intersect
//
//    *I = R.P0 + (dir * r);            // intersect point of ray and plane
//
//    // is I inside T?
//    float    uu, uv, vv, wu, wv, D;
//    uu = dot(u,u);
//    uv = dot(u,v);
//    vv = dot(v,v);
//    w = *I - T[0];
//    wu = dot(w,u);
//    wv = dot(w,v);
//    D = uv * uv - uu * vv;
//
//    // get and test parametric coords
//    float s, t;
//    s = (uv * wv - vv * wu) / D;
//    if (s < 0.0 || s > 1.0)         // I is outside T
//        return 0;
//    t = (uv * wu - uu * wv) / D;
//    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
//        return 0;
//
//    return 1;                       // I is in T
//}
// 
//
