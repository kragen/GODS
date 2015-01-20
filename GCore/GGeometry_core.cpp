/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GGeometry_core.h"
#include "GMemory_core.h"
#include "GCore_function_macros.h"

#ifdef _WIN32
#include <strsafe.h>
#endif

#include <math.h>
#include <stdlib.h>

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

using namespace god;


GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( BUFFERSLICE, BufferSlice );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( RANGE, Range );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( RECTANGLE, Rectangle );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( RECTANGLEL, RectangleL );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( POINTL, PointL );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( POINTF, PointF );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( CUBOID, Cuboid );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( PIVOTFRAME, PivotFrame );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( BOUNDINGVOLUME, BoundingVolume );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( TRANSFORM, Transform );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(RANGE),
	"Offset..................: %i\n"
	"Count...................: %i\n"
	, (int)in_CoreInstance->Offset
	, (int)in_CoreInstance->Count
	);


GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(BUFFERSLICE),
	"IndexOffset..................: %i\n"
	"ElementOffset................: %i\n"
	"IndexCount...................: %i\n"
	"ElementCount.................: %i"
	, (int)in_CoreInstance->IndexOffset
	, (int)in_CoreInstance->ElementOffset
	, (int)in_CoreInstance->IndexCount
	, (int)in_CoreInstance->ElementCount 
	);

//-----------
//------------------------------------------------------------------------
GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TRANSFORM),
	"Matrix........................:\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	"MatrixInverse.................:\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	"(%f, %f, %f, %f)\n"
	, in_CoreInstance->Matrix._11		, in_CoreInstance->Matrix._12		, in_CoreInstance->Matrix._13		, in_CoreInstance->Matrix._14
	, in_CoreInstance->Matrix._21		, in_CoreInstance->Matrix._22		, in_CoreInstance->Matrix._23		, in_CoreInstance->Matrix._24
	, in_CoreInstance->Matrix._31		, in_CoreInstance->Matrix._32		, in_CoreInstance->Matrix._33		, in_CoreInstance->Matrix._34
	, in_CoreInstance->Matrix._41		, in_CoreInstance->Matrix._42		, in_CoreInstance->Matrix._43		, in_CoreInstance->Matrix._44

	, in_CoreInstance->MatrixInverse._11	, in_CoreInstance->MatrixInverse._12	, in_CoreInstance->MatrixInverse._13	, in_CoreInstance->MatrixInverse._14
	, in_CoreInstance->MatrixInverse._21	, in_CoreInstance->MatrixInverse._22	, in_CoreInstance->MatrixInverse._23	, in_CoreInstance->MatrixInverse._24
	, in_CoreInstance->MatrixInverse._31	, in_CoreInstance->MatrixInverse._32	, in_CoreInstance->MatrixInverse._33	, in_CoreInstance->MatrixInverse._34
	, in_CoreInstance->MatrixInverse._41	, in_CoreInstance->MatrixInverse._42	, in_CoreInstance->MatrixInverse._43	, in_CoreInstance->MatrixInverse._44
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(PIVOTFRAME),
	"fTime.........................: %f\n"
	"vScale........................: (x=%f, y=%f, z=%f)\n"
	"qOrientation..................: (x=%f, y=%f, z=%f, w=%f)\n"
	"vPosition.....................: (x=%f, y=%f, z=%f)"
	, in_CoreInstance->fTime
	, in_CoreInstance->Scale.x			, in_CoreInstance->Scale.y			, in_CoreInstance->Scale.z
	, in_CoreInstance->Orientation.x	, in_CoreInstance->Orientation.y	, in_CoreInstance->Orientation.z, in_CoreInstance->Orientation.w 
	, in_CoreInstance->Position.x		, in_CoreInstance->Position.y		, in_CoreInstance->Position.z 
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(BOUNDINGVOLUME),
	"nType.........................: 0x%X\n"
	"vCenter.......................: (x=%f, y=%f, z=%f)\n"
	"fRadius.......................: %f\n"
	"vMin..........................: (%f, %f, %f)\n"
	"vMax..........................: (%f, %f, %f)"
	, (int)in_CoreInstance->nType
	, in_CoreInstance->vCenter.x , in_CoreInstance->vCenter.y , in_CoreInstance->vCenter.z
	, in_CoreInstance->fRadius
	, in_CoreInstance->vMin.x , in_CoreInstance->vMin.y , in_CoreInstance->vMin.z
	, in_CoreInstance->vMax.x , in_CoreInstance->vMax.y , in_CoreInstance->vMax.z				
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(CUBOID),
	"vCenter.......................: (x=%f, y=%f, z=%f)\n"
	"vHalfSizes....................: (x=%f, y=%f, z=%f)"
	, in_CoreInstance->vCenter.x, in_CoreInstance->vCenter.y, in_CoreInstance->vCenter.z
	, in_CoreInstance->vHalfSizes.x, in_CoreInstance->vHalfSizes.y, in_CoreInstance->vHalfSizes.z				
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(RECTANGLE),
	"vOffset.......................: (x=%f, y=%f)\n"
	"vSize.........................: (x=%f, y=%f)",
	in_CoreInstance->vOffset.x,	in_CoreInstance->vOffset.y,
	in_CoreInstance->vSize.x, in_CoreInstance->vSize.y
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(RECTANGLEL),
	"Offset........................: (x=%i, y=%i)\n"
	"Size..........................: (x=%i, y=%i)",
	in_CoreInstance->OffsetX, in_CoreInstance->OffsetY,
	in_CoreInstance->SizeX, in_CoreInstance->SizeY
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(POINTL),
	"Data........................: (x=%i, y=%i)\n",
	in_CoreInstance->x, in_CoreInstance->y
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(POINTF),
	"Data........................: (x=%f, y=%f)\n",
	in_CoreInstance->x, in_CoreInstance->y
	);

#define GRAVITY 9.8f



