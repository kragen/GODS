/// File:GMath_core.h
///
/// NOTES: This is the only file in the engine core including headers from another lib (GMath.lib)
///
/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
///
#include "GCore_constants.h"
#include "GCore_macros.h"
#include "GVector_object.h"
#include "gcore_ptr.h"

#ifndef _GGEOMETRY_CORE_H
#define _GGEOMETRY_CORE_H

namespace god
{
#pragma pack( push, 1 )
	//------------------------------------------------------------------------------------------
	// Describes a 2D rectangle
	GDECLARE_CORE_STRUCT( RECTANGLE, Rectangle,
		GVector2		vOffset;
		GVector2		vSize;
	);
	static void onCreateRectangleDefault( ggns_Rectangle* stuff )
	{
		(*stuff)->vOffset	= GVECTOR2_ZERO;
		(*stuff)->vSize		= GVECTOR2_ONE;
	};

	//------------------------------------------------------------------------------------------
	// Describes an axis-aligned cube
	GDECLARE_CORE_STRUCT( RECTANGLEL, RectangleL,
		int32_t			OffsetX;
		int32_t			OffsetY;
		int32_t			SizeX;
		int32_t			SizeY;
	);
	static void onCreateRectangleLDefault( ggns_RectangleL* stuff )
	{
		(*stuff)->OffsetX	= (*stuff)->OffsetY	= 0;
		(*stuff)->SizeX		= (*stuff)->SizeY	= 1;
	};

	//------------------------------------------------------------------------------------------
	// Describes an 
	GDECLARE_CORE_STRUCT( POINTL, PointL,
		int32_t			x;
		int32_t			y;
	);
	static void onCreatePointLDefault( ggns_PointL* stuff )
	{
		(*stuff)->x	= (*stuff)->y = 0;
	};

	//------------------------------------------------------------------------------------------
	// Describes an 
	GDECLARE_CORE_STRUCT( POINTF, PointF,
		float			x;
		float			y;
	);
	static void onCreatePointFDefault( ggns_PointF* stuff )
	{
		(*stuff)->x	= (*stuff)->y = 0;
	};

	//------------------------------------------------------------------------------------------
	// Describes a 2D rectangle
	GDECLARE_CORE_STRUCT(RANGE, Range,
		uint32_t		Offset;
		uint32_t		Count;
	);
	static void onCreateRangeDefault(ggns_Range* stuff)
	{
		(*stuff)->Offset = (*stuff)->Count = 0;
	};

	//------------------------------------------------------------------------------------------
	// Describes an axis-aligned cube
	GDECLARE_CORE_STRUCT( CUBOID, Cuboid,
		GVector3		vCenter;
		GVector3		vHalfSizes;
	);
	static void onCreateCuboidDefault( ggns_Cuboid* stuff )
	{
		(*stuff)->vCenter		= GVECTOR3_ZERO;
		(*stuff)->vHalfSizes	= GVECTOR3_ONE;
	};

	//------------------------------------------------------------------------------------------
	// Describes an axis-aligned cube
	GDECLARE_CORE_STRUCT( PIVOTFRAME, PivotFrame,
		float			fTime;
		GVector3		Scale;
		GQuaternion		Orientation;
		GVector3		Position;
	);
	static void onCreatePivotFrameDefault( ggns_PivotFrame* stuff )
	{
		(*stuff)->fTime			= 0.0f;
		(*stuff)->Scale			= GVECTOR3_ONE;
		(*stuff)->Position		= GVECTOR3_ZERO;
		(*stuff)->Orientation	= GQUATERNION_IDENTITY;
	};

	//------------------------------------------------------------------------------------------
	// Stores a pack of space transform matrices
	GDECLARE_CORE_STRUCT( TRANSFORM, Transform,
		GMatrix4		Matrix;
		GMatrix4		MatrixInverse;
	);
	static void onCreateTransformDefault( ggns_Transform* stuff )
	{
		(*stuff)->Matrix		= 
		(*stuff)->MatrixInverse	= GMATRIX4_IDENTITY;
	};


	typedef unsigned char GBOUNDINGPRIMITIVE_TYPE;
	const GBOUNDINGPRIMITIVE_TYPE GBOUNDINGPRIMITIVE_SPHERE	= 1;
	const GBOUNDINGPRIMITIVE_TYPE GBOUNDINGPRIMITIVE_BOX	= 2;
	const GBOUNDINGPRIMITIVE_TYPE GBOUNDINGPRIMITIVE_PLANE	= 4;
	const GBOUNDINGPRIMITIVE_TYPE GBOUNDINGPRIMITIVE_TORUS	= 8;

	//------------------------------------------------------------------------------------------
	// Stores a description of a bounding geometry for optimizing collision detection and frustum culling
	GDECLARE_CORE_STRUCT( BOUNDINGVOLUME, BoundingVolume,
		GBOUNDINGPRIMITIVE_TYPE	nType;
		GVector3				vCenter;
		float					fRadius;
		GVector3				vMin;
		GVector3				vMax;
	);
	static void onCreateBoundingVolumeDefault( ggns_BoundingVolume* stuff )
	{
		(*stuff)->nType			= GBOUNDINGPRIMITIVE_SPHERE;
		(*stuff)->vCenter		= GVECTOR3_ZERO;
		(*stuff)->fRadius		= 0.5f;
		(*stuff)->vMin.x 		= (*stuff)->vMin.y 		= (*stuff)->vMin.z 		= -0.5f;
		(*stuff)->vMax.x 		= (*stuff)->vMax.y 		= (*stuff)->vMax.z 		= 0.5f;
	};

	//------------------------------------------------------------------------------------------
	// Stores information for referencing data stored on hardware buffers
	GDECLARE_CORE_STRUCT( BUFFERSLICE, BufferSlice,
		float			fTime;
		uint32_t		IndexOffset;
		uint32_t		ElementOffset;
		uint32_t		IndexCount;
		uint32_t		ElementCount;
	);
	static void onCreateBufferSliceDefault( ggns_BufferSlice* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size()-sizeof(uint32_t) );
	};
#pragma pack( pop )
};

#endif // _GGEOMETRY_CORE_H