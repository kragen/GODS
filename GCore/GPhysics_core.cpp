/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GPhysics_core.h"
#include "GMemory_core.h"
#include "GCore_function_macros.h"


#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"
#include <stdlib.h>

#ifdef _WIN32
#include <strsafe.h>
#include <crtdbg.h>
#endif

using namespace god;

GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( MASS2D, Mass2D );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( FORCE2D, Force2D );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( MASS3D, Mass3D );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( FORCE3D, Force3D );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(FORCE3D),
	"Acceleration.................: [%f, %f, %f]\n"
	"Velocity.....................: [%f, %f, %f]\n"
	"Rotation.....................: [%f, %f, %f]"
	, in_CoreInstance->Acceleration.x, in_CoreInstance->Acceleration.y, in_CoreInstance->Acceleration.z
	, in_CoreInstance->Velocity.x, in_CoreInstance->Velocity.y, in_CoreInstance->Velocity.z
	, in_CoreInstance->Rotation.x, in_CoreInstance->Rotation.y, in_CoreInstance->Rotation.z
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(MASS3D),
	"InverseMass...................: %f\n"
	"LinearDamping.................: %f\n"
	"AngularDamping................: %f\n"
	"InverseAngularTensor..........: \n"
	"[%f, %f, %f]\n"
	"[%f, %f, %f]\n"
	"[%f, %f, %f]"
	, in_CoreInstance->InverseMass
	, in_CoreInstance->LinearDamping
	, in_CoreInstance->AngularDamping
	, in_CoreInstance->InverseAngularMassTensor._11, in_CoreInstance->InverseAngularMassTensor._12, in_CoreInstance->InverseAngularMassTensor._13
	, in_CoreInstance->InverseAngularMassTensor._21, in_CoreInstance->InverseAngularMassTensor._22, in_CoreInstance->InverseAngularMassTensor._23
	, in_CoreInstance->InverseAngularMassTensor._31, in_CoreInstance->InverseAngularMassTensor._32, in_CoreInstance->InverseAngularMassTensor._33
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(FORCE2D),
	"Acceleration..................: [%f, %f]\n"
	"Velocity......................: [%f, %f]\n"
	"Rotation......................: %f"
	, in_CoreInstance->Acceleration.x,	in_CoreInstance->Acceleration.y
	, in_CoreInstance->Velocity.x,		in_CoreInstance->Velocity.y
	, in_CoreInstance->Rotation
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(MASS2D),
	"InverseMass...................: %f\n"
	"LinearDamping.................: %f\n"
	"AngularDamping................: %f\n"
	"InverseAngularTensor..........: \n"
	"[%f, %f]\n"
	"[%f, %f]"
	, in_CoreInstance->InverseMass
	, in_CoreInstance->LinearDamping
	, in_CoreInstance->AngularDamping
	, in_CoreInstance->InverseAngularMassTensor._11, in_CoreInstance->InverseAngularMassTensor._12
	, in_CoreInstance->InverseAngularMassTensor._21, in_CoreInstance->InverseAngularMassTensor._22
	);
