/// File:GVector_object.h
///
/// This file is part of the Game Object Data System
/// Copyright © 2009-2013: Pablo Ariel Zorrilla Cepeda
///
#include "GVector_core.h"

#ifndef _GVECTOR_H 
#define _GVECTOR_H

typedef unsigned char	uint8_t;

namespace god
{
#pragma pack( push, 1 )
	typedef class _GMatrix3x2	GMatrix3x2;
	typedef class _GMatrix2x3	GMatrix2x3;
	static const float GEPSILON		= 0.00001f;

	// class GVector2
	// 
	// Defines methods for GVECTOR2
	//
	typedef class _GVector2 : public GVECTOR2
	{
	public:
		_GVector2( void );
		_GVector2( const GVECTOR2& _v );
		_GVector2( const float* f );
		_GVector2( float _x, float _y );
		// comparison operator
		bool				operator ==( const GVECTOR2& _v ) const;
		bool				operator !=( const GVECTOR2& _v ) const;
		// assignment operators
		const _GVector2&	operator =( const GVECTOR2& _v );
		const _GVector2&	operator +=( const GVECTOR2& _v );
		const _GVector2&	operator -=( const GVECTOR2& _v );
		const _GVector2&	operator *=( const GMATRIX2X2& m ); // scalar multiplication
		const _GVector2&	operator *=( float scalar ); // scalar multiplication
		const _GVector2&	operator /=( float scalar ); // scalar division
		// 
		_GVector2			operator+( const GVECTOR2& _v ) const;
		_GVector2			operator-( const GVECTOR2& _v ) const;
		float				operator*( const GVECTOR2& _v ) const; // scalar product (dot product)
		_GVector2			operator*( const GMATRIX2X2& m ) const;
		_GVector2			operator*( float scalar ) const;
		_GVector2			operator/( float scalar ) const;
		// unary operators
		_GVector2			operator-( void ) const;

		void				Set( float x, float y );
		float				Element( uint8_t e ) const; // return n element from vector
		const _GVector2&	SetElement( uint8_t n, float fValue ); // set the n element to vector
		void				AddScaledVector( const GVECTOR2& vector, float scale );
		float				Dot( const GVECTOR2& _v ) const;
		float				Length( void ) const;
		float				SqLength( void ) const;
		const _GVector2&	Normalize( void );
		const _GVector2&	Difference( const GVECTOR2& orig, const GVECTOR2& dest );
		_GVector2			Difference( const GVECTOR2& dest ) const;
		float				Distance( const GVECTOR2& dest ) const;
		float				AngleWith( const GVECTOR2 &v ) const;			// angle in rad
		// rotate the vector by an angle in radians
		_GVector2&			Rotate( float _theta_in_rads );
		void				Zero( void )
		{ x = y = 0.0f; };
		static const _GVector2	kZero;
		static const _GVector2	kOne;
		static const _GVector2	kFront;
		static const _GVector2	kRight;
	} GVector2;

	// class GVector3
	// 
	// Defines methods for GVECTOR3
	//
	typedef class _GVector3 : public GVECTOR3
	{
	public:
		_GVector3( void );
		_GVector3( const GVECTOR3& _v );
		_GVector3( const float* f );
		_GVector3( float _x, float _y, float _z );
		// comparison operator
		bool				operator ==( const GVECTOR3& _v ) const;
		bool				operator !=( const GVECTOR3& _v ) const;
		// assignment operators
		const _GVector3&	operator =( const GVECTOR3& _v );
		const _GVector3&	operator +=( const GVECTOR3& _v );
		const _GVector3&	operator -=( const GVECTOR3& _v );
		const _GVector3&	operator *=( const GMATRIX3X3& m ); // scalar multiplication
		const _GVector3&	operator *=( float scalar ); // scalar multiplication
		const _GVector3&	operator /=( float scalar ); // scalar division
		// 
		_GVector3			operator+( const GVECTOR3& _v ) const;
		_GVector3			operator-( const GVECTOR3& _v ) const;
		float				operator*( const GVECTOR3& _v ) const; // scalar product (dot product)
		_GVector3			operator*( const GMATRIX3X3& m ) const;
		_GVector3			operator*( float scalar ) const;
		_GVector3			operator/( float scalar ) const;
		// unary operators
		_GVector3			operator-( void ) const;
		//
		void				Set( float x, float y, float z );
		float				Element( uint8_t e ) const; // return n element from vector
		const _GVector3&	SetElement( uint8_t n, float fValue ); // set the n element to vector
		void				AddScaledVector( const GVECTOR3& vector, float scale );
		float				Dot( const GVECTOR3& _v ) const;
		_GVector3			Cross( const GVECTOR3& _v ) const;
		const _GVector3&	Cross( const GVECTOR3& _v1, const GVECTOR3& _v2 );
		float				Length( void ) const;
		float				SqLength( void ) const;
		const _GVector3&	Normalize( void );
		const _GVector3&	Difference( const GVECTOR3& orig, const GVECTOR3& dest );
		_GVector3			Difference( const GVECTOR3& dest ) const;
		float				Distance( const GVECTOR3& dest ) const;
		float				AngleWith( const GVECTOR3& v ) const;			// angle in rad
		_GVector3&			RotateX( float _theta_in_rads );
		_GVector3&			RotateY( float _theta_in_rads );
		_GVector3&			RotateZ( float _theta_in_rads );

		void				Zero( void )
		{ x = y = z = 0.0f; };

		//static const _GVector3	kZero;
		//static const _GVector3	kOne;
		//static const _GVector3	kFront;
		//static const _GVector3	kUp;
		//static const _GVector3	kRight;
	} GVector3;
	

	// class GVector4
	// 
	// Defines methods for GVECTOR4
	//
	typedef class _GVector4 : public GVECTOR4
	{
	public:
		_GVector4( void );
		_GVector4( const GVECTOR4& _v );
		_GVector4( const float* f );
		_GVector4( float _x, float _y, float _z, float _w );
		// comparison operator
		bool				operator ==( const GVECTOR4& _v ) const;
		bool				operator !=( const GVECTOR4& _v ) const;
		// assignment operators
		const _GVector4&	operator =( const GVECTOR4& _v );
		const _GVector4&	operator +=( const GVECTOR4& _v );
		const _GVector4&	operator -=( const GVECTOR4& _v );
		const _GVector4&	operator *=( const GMATRIX4& m ); // scalar multiplication
		const _GVector4&	operator *=( float scalar ); // scalar multiplication
		const _GVector4&	operator /=( float scalar ); // scalar division
		_GVector4			operator+( const GVECTOR4& _v ) const;
		_GVector4			operator-( const GVECTOR4& _v ) const;
		float				operator*( const GVECTOR4& _v ) const; // scalar product (dot product)
		_GVector4			operator*( const GMATRIX4& m ) const;
		_GVector4			operator*( float scalar ) const;
		_GVector4			operator/( float scalar ) const;
		// unary operators
		_GVector4			operator-( void ) const;
		//
		void				Set( float x, float y, float z, float w );
		void				Set( float x, float y, float z );
		float				Element( uint8_t n ) const; // return n element from vector
		const _GVector4&	SetElement( uint8_t n, float fValue ); // set the n element to vector
		void				AddScaledVector( const GVECTOR4& vector, float scale );
		float				Dot( const GVECTOR4& _v ) const;
		float				Length( void ) const;
		float				SqLength( void ) const;
		const _GVector4&	Normalize( void );
		const _GVector4&	Difference( const GVECTOR4& orig, const GVECTOR4& dest );
		_GVector4			Difference( const GVECTOR4& dest ) const;
		float				Distance( const GVECTOR4& dest ) const;
		float				AngleWith( const GVECTOR4& v ) const;			// angle in rad

		void				Zero( void )
		{ x = y = z = 0.0f; w = 1.0f; };
		static const _GVector4	kZero;
		static const _GVector4	kOne;
		static const _GVector4	kFront;
		static const _GVector4	kUp;
		static const _GVector4	kRight;
	} GVector4;

	typedef class _GQuaternion : public GQUATERNION
	{
	public:
		_GQuaternion( void );
		_GQuaternion( const GQUATERNION& q );
		_GQuaternion( const float* f );
		_GQuaternion( float _x, float _y, float _z, float _w );
		
		// comparison operator
		bool				operator ==( const GQUATERNION& q ) const;
		bool				operator !=( const GQUATERNION& q ) const;

		// Assignment operators
		const _GQuaternion&		operator = ( const GQUATERNION& q );
		const _GQuaternion&		operator += ( const GQUATERNION& q );
		const _GQuaternion&		operator -= ( const GQUATERNION& q );
		const _GQuaternion&		operator *= ( const GQUATERNION& q );
		const _GQuaternion&		operator *= ( float f );
		const _GQuaternion&		operator /= ( float f );
		// 
		_GQuaternion			operator +  ( const GQUATERNION &q ) const;
		_GQuaternion			operator -  ( const GQUATERNION &q ) const;
		_GQuaternion			operator *  ( const GQUATERNION &q ) const;
		_GQuaternion			operator *  ( const GVECTOR4 &v ) const;
		_GQuaternion			operator *  ( const GVECTOR3 &v ) const;
		_GQuaternion			operator *  ( float f ) const;
		_GQuaternion			operator /  ( float f ) const;
		// Unary operators
		_GQuaternion			operator -( void ) const;
		_GQuaternion			operator ~( void ) const { return _GQuaternion(-x, -y, -z, w); }

		// 
		float					Element( uint8_t n ) const; // return n element from quaternion
		const _GQuaternion&		SetElement( uint8_t n, float fValue ); // set the n element to quaternion
		void					AddScaledVector( const GVECTOR4& vec, float scale );
		const _GQuaternion&		AddScaledVector( const GVECTOR3& vec, float scale );
		void					SetRotation( const GQUATERNION &q1, const GQUATERNION &q2 );
		GVector3				RotateVector( const GVECTOR3 &v ) const;
;		float					Dot( const GQUATERNION& q ) const;
		float					Length( void ) const;
		float					SqLength( void ) const;
		// Convert from Euler Angles
		const _GQuaternion&		MakeFromEuler( float pitch, float yaw, float roll );
		const _GQuaternion&		MakeFromEuler( const GVector3& v );
		void					GetEulers( float* pitch, float* yaw, float* roll );
		void					GetMatrix(GMATRIX4X4 *m) const;
		void					GetMatrix(GMATRIX3X3 *m) const;
		//
		const _GQuaternion&		LinearInterpolate( const GQUATERNION &q1, const GQUATERNION &q2, float fTime );
		const _GQuaternion&		SLERP( const GQUATERNION &q1, const GQUATERNION &q2, float fTime );

		const _GQuaternion&		Normalize( void );
		void					Identity( void )
		{ x = y = z = 0.0f; w = 1.0f; }
		static const _GQuaternion	kIdentity;
	} GQuaternion;

	typedef class _GMatrix2x2 : public GMATRIX2X2
	{
	public:
		_GMatrix2x2( void );
		_GMatrix2x2( const GMATRIX2X2& m );
		_GMatrix2x2(	float __11, float __12, 
						float __21, float __22 );
		// comparison operators
		bool			operator ==( const GMATRIX2X2 &m ) const;
		bool			operator !=( const GMATRIX2X2 &m ) const;
		// assignment operators
		const _GMatrix2x2&	operator =( const GMATRIX2X2& m );	// matrix assignment
		const _GMatrix2x2&	operator +=( const GMATRIX2X2& m );	// matrix addition
		const _GMatrix2x2&	operator -=( const GMATRIX2X2& m );	// matrix substraction
		const _GMatrix2x2&	operator *=( const GMATRIX2X2& m );	// matrix multiplication
		const _GMatrix2x2&	operator *=( const GMATRIX3X2& m );	// matrix multiplication
		const _GMatrix2x2&	operator *=( float scalar );	// scalar multiplication
		const _GMatrix2x2&	operator /=( float scalar );	// scalar division
		// 		
		_GMatrix2x2			operator + ( const GMATRIX2X2& m ) const;	// matrix addition
		_GMatrix2x2			operator - ( const GMATRIX2X2& m ) const;	// matrix substraction
		_GMatrix2x2			operator * ( const GMATRIX2X2& m ) const;	// matrix multiplication
		_GMatrix2x2			operator * ( const GMATRIX3X2& m ) const;	// matrix multiplication
		_GMatrix2x2			operator * ( float scalar ) const;	// scalar multiplication
		_GMatrix2x2			operator / ( float scalar ) const;	// scalar division
		//		GVector2		operator * ( const GVector2 &v ) const; this is undefined under our convention V*M

		// Common matrix functions
		void				Identity( void );
		void				SetTranspose( const GMATRIX2X2& m ); // transpose m, save result in this instantce
		_GMatrix2x2			GetTranspose( void ) const; // transpose m, return result in new copy
		void				Transpose( void );		// transpose this, save result in this instantce
		void				SetInverse( const GMATRIX2X2& m );		// invert m, save result in this instance
		_GMatrix2x2			GetInverse( void ) const; // invert this, return result in new instance
		void				Invert( void ); // invert this, save result in this instance
		float				GetDeterminant( void );
		const _GMatrix2x2&	LinearInterpolate( const GMATRIX2X2& a, const GMATRIX2X2& b, float prop );

		// Transformation functions
		void				Scale( const GVECTOR2& Scale, bool bEraseContent=false ); // Scale the current matrix
		void				Scale( float x, float y, bool bEraseContent=false ); 
		void				RotationZ( float Angle );	// x axis
		GVector2			TransformInverseDirection( const GVECTOR2& _v ) const;
		//static const _GMatrix2x2	kZero;
		//static const _GMatrix2x2	kOne;
		//static const _GMatrix2x2	kIdentity;
	} GMatrix2x2, GMatrix2;


	typedef class _GMatrix3x3 : public GMATRIX3X3
	{
	public:
		_GMatrix3x3( void );
		_GMatrix3x3( const GMATRIX3X3& m );
		_GMatrix3x3( const float* m )
		{
			for( uint32_t i=0; i<9; i++ )
				(&_11)[i] = m[i];
		};
		_GMatrix3x3(	float __11, float __12, float __13, 
						float __21, float __22, float __23, 
						float __31, float __32, float __33 );
		// comparison operators
		bool			operator ==( const GMATRIX3X3 &m ) const;
		bool			operator !=( const GMATRIX3X3 &m ) const;
		// assignment operators
		const _GMatrix3x3&	operator =( const GMATRIX3X3 &m );	// matrix assignment
		const _GMatrix3x3&	operator +=( const GMATRIX3X3 &m );	// matrix addition
		const _GMatrix3x3&	operator -=( const GMATRIX3X3 &m );	// matrix substraction
		const _GMatrix3x3&	operator *=( const GMATRIX3X3 &m );	// matrix multiplication
		const _GMatrix3x3&	operator *=( float scalar );	// scalar multiplication
		const _GMatrix3x3&	operator /=( float scalar );	// scalar division
		// 		
		_GMatrix3x3			operator + ( const GMATRIX3X3 &m ) const;	// matrix addition
		_GMatrix3x3			operator - ( const GMATRIX3X3 &m ) const;	// matrix substraction
		_GMatrix3x3			operator * ( const GMATRIX3X3 &m ) const;	// matrix multiplication
		_GMatrix3x3			operator * ( float scalar ) const;	// scalar multiplication
		_GMatrix3x3			operator / ( float scalar) const;	// scalar division
		//		GVector3		operator * ( const GVector3 &vc )this is undefined under our convention V*M

		// 
		void				Identity( void );
		void				SetTranspose( const GMATRIX3X3& m );		// transpose m, save result in this instantce
		_GMatrix3x3			GetTranspose( void ) const;		// transpose m, save result in this instantce
		void				Transpose( void );		// transpose this, save result in this instantce
		void				SetInverse( const GMATRIX3X3& m );		// invert m, save result in this instance
		_GMatrix3x3			GetInverse( void ) const;
		void				Invert( void );
		float				GetDeterminant( void );
		const _GMatrix3x3&	LinearInterpolate( const GMATRIX3X3& a, const GMATRIX3X3& b, float prop );

		// Transformation functions
		void				Scale( const GVECTOR2& Scale, bool bEraseContent=false ); // Scale the current matrix
		void				Scale( float x, float y, float z, bool bEraseContent=false ); 
		void				RotationX( float Angle );	// x axis
		void				RotationY( float Angle );	// y axis
		void				RotationZ( float Angle );	// z axis
		void				RotationArbitraryAxis(const GVECTOR3& Axis, float a);
		void				SetOrientation( const GQUATERNION& qOrientation );
		void				SetTranslation( const GVECTOR2& vTranslation, bool bEraseContent=false );
		GVector2			GetTranslation( void ) const;
		GVector4			Transform( const GVECTOR4& vector ) const;
		GVector4			TransformInverseDirection( const GVECTOR4& _v ) const;
		GVector3			TransformInverseDirection( const GVECTOR3& _v ) const;
        void				SetCoeffsAngularMass(	float ix, float iy, float iz,
													float ixy=0, float ixz=0, float iyz=0 );
        void				SetBlockAngularMass( const GVECTOR3 &vHalfSizes, float fMass );
		void				InverseRotateVector(float * fpVec) const;

		//static const _GMatrix3x3	kZero;
		//static const _GMatrix3x3	kOne;
		//static const _GMatrix3x3	kIdentity;
	} GMatrix3x3, GMatrix3;

	//
	//
	typedef class _GMatrix4x4 : public GMATRIX4X4
	{
	public:
		_GMatrix4x4( void );
		_GMatrix4x4( const GMATRIX4X4& m );
		_GMatrix4x4( float f );
		_GMatrix4x4(	float __11, float __12, float __13, float __14, 
						float __21, float __22, float __23, float __24, 
						float __31, float __32, float __33, float __34,
						float __41, float __42, float __43, float __44 
					);
		// comparison operators
		bool			operator ==( const GMATRIX4X4 &m ) const;
		bool			operator !=( const GMATRIX4X4 &m ) const;
		// assignment operators
		const _GMatrix4x4&	operator =( const GMATRIX4X4 &m );	// matrix assignment
		const _GMatrix4x4&	operator +=( const GMATRIX4X4 &m );	// matrix addition
		const _GMatrix4x4&	operator -=( const GMATRIX4X4 &m );	// matrix substraction
		const _GMatrix4x4&	operator *=( const GMATRIX4X4 &m );	// matrix multiplication
		const _GMatrix4x4&	operator *=( float scalar );	// scalar multiplication
		const _GMatrix4x4&	operator /=( float scalar );	// scalar division
		// 		
		_GMatrix4x4			operator + ( const GMATRIX4X4 &m ) const;	// matrix addition
		_GMatrix4x4			operator - ( const GMATRIX4X4 &m ) const;	// matrix substraction
		_GMatrix4x4			operator * ( const GMATRIX4X4 &m ) const;	// matrix multiplication
		_GMatrix4x4			operator * ( float scalar ) const;	// scalar multiplication
		_GMatrix4x4			operator / ( float scalar) const;	// scalar division

//		GMatrix4x4&			Set( const GMatrix4x3& m );
		// 
		void				Identity( void );
		void				SetTranspose( const GMATRIX4X4& m );		// transpose m, save result in this instantce
		_GMatrix4x4			GetTranspose( void ) const;		// transpose m, save result in this instantce
		void				Transpose( void );		// transpose this, save result in this instantce
		void				SetInverse( const GMATRIX4X4& m );		// invert m, save result in this instance
		_GMatrix4x4			GetInverse( void ) const;
		void				Invert( void );
		float				GetDeterminant( void );

		// Transformation functions
		void				Scale( const GVECTOR3& Scale, bool bEraseContent=false ); // Scale the current matrix
		void				Scale( float x, float y, float z, bool bEraseContent=false ); 
		void				RotationX( float Angle );	// x axis
		void				RotationY( float Angle );	// y axis
		void				RotationZ( float Angle );	// z axis
		void				Rotation( float x, float y, float z ); // all axis
		void				Rotation( const GVECTOR3& euler ); // all axis
		void				RotationArbitraryAxis(const GVECTOR3& _vcAxis, float a);
		void				SetOrientation( const GQUATERNION& qOrientation );
		void				SetTranslation( const GVECTOR3& vTranslation, bool bEraseContent=false );
		GVector3			GetTranslation( void ) const;
		GVector3			Transform( const GVECTOR3& vector ) const;
		GVector4			TransformDirection( const GVECTOR4& vector ) const;
		GVector3			TransformDirection( const GVECTOR3& vector ) const;
		GVector4			TransformInverseDirection( const GVECTOR4& _v ) const;
		GVector3			TransformInverseDirection( const GVECTOR3& _v ) const;
		const _GMatrix4x4&	LinearInterpolate( const GMATRIX4X4& a, const GMATRIX4X4& b, float prop );		
		void				InverseTranslateVector(float * fpVec) const;
		void				InverseRotateVector(float * fpVec) const;
		void				Billboard(	const GVECTOR3& vEye, const GVECTOR3& vDir, 
										const GVECTOR3& vUp = GVector3(0.0f,1.0f,0.0f) );
		// Build view matrix
		void				LookAt(	const GVECTOR3& vPosition, const GVECTOR3& vTarget, 
									const GVECTOR3& vUp = GVector3( 0.0f, 1.0f, 0.0f ) );
		void				View3D( const GVECTOR3& vPosition, const GVECTOR3& vRight, 
									const GVECTOR3& vUp, const GVECTOR3& vFront );
		// Build projection matrix
		// fAngle: vertical field of view angle in radians
		// fAspect: aspect ratio = width / height
		// fNear: near plane distance
		// fFar: far plane distance
		void				FieldOfView( float fAngle, float fAspect, float fNear, float fFar );		

		//static const _GMatrix4x4	kZero;
		//static const _GMatrix4x4	kOne;
		//static const _GMatrix4x4	kIdentity;
	} GMatrix4x4, GMatrix4;

	typedef class _GMatrix3x2 : public GMATRIX3X2
	{
	public:
		_GMatrix3x2( void );
		_GMatrix3x2( const GMATRIX3X2& m );
		_GMatrix3x2(	float __11, float __12, 
						float __21, float __22, 
						float __31, float __32 
						);
		_GMatrix2x3			GetTranspose( void ) const; // transpose m, return result in new copy
		const _GMatrix3x2&	operator *=( const GMATRIX2X2 &m );	// matrix multiplication
		//static const _GMatrix3x2	kIdentity;
	} GMatrix3x2;

	typedef class _GMatrix2x3 : public GMATRIX2X3
	{
	public:
		_GMatrix2x3( void );
		_GMatrix2x3( const GMATRIX2X3& m );
		_GMatrix2x3(	float __11, float __12, float __13,
						float __21, float __22, float __23 
						);
		_GMatrix3x2			GetTranspose( void ) const; // transpose m, return result in new copy
		//static const _GMatrix2x3	kIdentity;
	} GMatrix2x3;
	
	typedef uint16_t GVISIBILITY_VALUE;
	static const GVISIBILITY_VALUE GVISIBLE		= 0x000;
	static const GVISIBILITY_VALUE GCULLED		= 0x001;
	static const GVISIBILITY_VALUE GCLIPPED		= 0x002;
	static const GVISIBILITY_VALUE GPLANAR		= 0x004;

	typedef GVISIBILITY_VALUE GPOSITION_VALUE;
	static const GPOSITION_VALUE GFRONT		= 0x008;
	static const GPOSITION_VALUE GBACK		= 0x010;
	static const GPOSITION_VALUE GTOP		= 0x020;
	static const GPOSITION_VALUE GBOTTOM	= 0x040;
	static const GPOSITION_VALUE GLEFT		= 0x080;
	static const GPOSITION_VALUE GRIGHT		= 0x100;
	static const GPOSITION_VALUE GXCENTER	= 0x200;
	static const GPOSITION_VALUE GYCENTER	= 0x400;
	static const GPOSITION_VALUE GZCENTER	= 0x800;
//
	typedef class _GPlane : public GPLANE
	{
	public:
		_GPlane( void ){};
		_GPlane( const GPLANE& p )
		{
			x = p.x; y = p.y; z = p.z; d = p.d;
		};
		_GPlane( const GVECTOR3& vNormal, float fDistance )
		{
			x = vNormal.x; y = vNormal.y; z = vNormal.z; d = fDistance;
		};
		bool				operator==( const GPLANE& ) const;
		bool				operator!=( const GPLANE& ) const;
		void				Set(const GVECTOR3 &vcN, const GVECTOR3 &vcP);
		void				Set(const GVECTOR3 &vcN, float fD);
		void				Set(const GVECTOR3 &v0,  const GVECTOR3 &v1, const GVECTOR3 &v2);
		float				Distance( const GVECTOR3 &vcPoint ) const;
		GVISIBILITY_VALUE	Classify( const GVECTOR3 &vcPoint ) const;

		bool 				Clip(const GRAY*, float , GRAY*, GRAY*);
		GMatrix4x4			GetReflectionMatrix( void ) const;
		bool				Intersects( const GVECTOR3 &vc0, const GVECTOR3 &vc1, const GVECTOR3 &vc2 ) const;
		bool				Intersects( const GPLANE &plane, GRAY* pIntersection ) const;

	} GPlane; // class
//
	static const GPlane GPLANEXY( GVector3( 0.0f, 0.0f, 1.0f ), 0.0f );
	static const GPlane GPLANEXZ( GVector3( 0.0f, 1.0f, 0.0f ), 0.0f );
	static const GPlane GPLANEYZ( GVector3( 1.0f, 0.0f, 0.0f ), 0.0f );

	typedef class _GRay : public GRAY
	{
	public:
		void		Set( const GVECTOR3& vOrigin, const GVECTOR3& vDirection );
		void		DeTransform( const GMATRIX4X4& _m ); // move to matrixspace

		bool		Intersects( const GVECTOR3 &v0, const GVECTOR3 &v1, const GVECTOR3 &v2, bool bCull, float *t ) const;
		bool		Intersects( const GVECTOR3 &v0, const GVECTOR3 &v1, const GVECTOR3 &v2, bool bCull, float fLength, float *t ) const;
		bool		Intersects( const GPLANE &Plane, bool bCull, float *t, GVECTOR3 *vHit ) const;
		bool		Intersects( const GPLANE &Plane, bool bCull, float fLength, float *t, GVECTOR3 *vHit ) const;
	} GRay; // class

#pragma pack( pop )
}
#endif // _GVECTOR_H 
