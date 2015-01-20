/// This file is part of the Game Object Data System
/// 2011 (c) Pablo Ariel Zorrilla Cepeda
#include "GVector_object.h"
#include "gcore_ptr.h"

#ifndef _GPHYSICS_CORE_H
#define _GPHYSICS_CORE_H

namespace god
{
#pragma pack( push, 1 )
	// The following structure stores force information for a 3D rigid body
	// NOTES: these data change very often during runtime
	GDECLARE_CORE_STRUCT( FORCE3D, Force3D,
		GVector3		Velocity;
		GVector3		Acceleration;
		GVector3		Rotation;
	);
	static void onCreateForce3DDefault( ggns_Force3D* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size() );
	};

	// The following structure stores mass information for a 3D rigid body
	// NOTES: these data almost never change during runtime
	GDECLARE_CORE_STRUCT( MASS3D, Mass3D,
		float			InverseMass;
		float			LinearDamping;
		float			AngularDamping;
		GMatrix3		InverseAngularMassTensor;
	);
	static void onCreateMass3DDefault( ggns_Mass3D* stuff )
	{
		(*stuff)->InverseMass				= 0.0f;
		(*stuff)->LinearDamping				= 1.0f;
		(*stuff)->AngularDamping			= 1.0f;
		(*stuff)->InverseAngularMassTensor	= GMATRIX3_IDENTITY;
	};

	// The following structure stores force information for a 2D rigid body
	// NOTES: these data change very often during runtime
	GDECLARE_CORE_STRUCT( FORCE2D, Force2D,
		GVector2		Velocity;
		GVector2		Acceleration;
		float			Rotation;
	);
	static void onCreateForce2DDefault( ggns_Force2D* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size() );
	};

	// The following structure stores mass information for a 2D rigid body
	// NOTES: these data almost never change during runtime
	GDECLARE_CORE_STRUCT( MASS2D, Mass2D,
		float			InverseMass;
		float			LinearDamping;
		float			AngularDamping;
		GMatrix2		InverseAngularMassTensor;
	);
	static void onCreateMass2DDefault( ggns_Mass2D* stuff )
	{
		(*stuff)->InverseMass				= 0.0f;
		(*stuff)->LinearDamping				= 1.0f;
		(*stuff)->AngularDamping			= 1.0f;
		(*stuff)->InverseAngularMassTensor	= GMATRIX2_IDENTITY;
	};
#pragma pack( pop )
};

#endif // _GPHYSICS_CORE_H