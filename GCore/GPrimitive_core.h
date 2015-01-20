#include "GGeometry_core.h"

#ifndef _GPRIMITIVE_CORE_H
#define _GPRIMITIVE_CORE_H

namespace god
{
#pragma pack( push, 1 )
	typedef uint8_t GPRIMITIVE3D_TYPE;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_BOX			= 1;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_GRID			= 2;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_SPHERE		= 3;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_CYLINDER		= 4;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_HEMISPHERE	= 5;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_CYLINDERSEG	= 6;	// cylinder cut in half
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_BOXGRIDS		= 7;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_SPHEREGRIDS	= 8;
	static const GPRIMITIVE3D_TYPE GPRIMITIVE_TEAPOT		= 9;
	//static const GPRIMITIVE3D_TYPE GPRIMITIVE_TEAPOT		= 10;

	GDECLARE_CORE_STRUCT( PRIMITIVE3D, Primitive3D,
		uint32_t			nWidth;
		uint32_t			nHeight;
		uint32_t			nDepth;
		uint32_t			nStackCount;
		
		uint32_t			nSliceCount;
		uint32_t			nWidthCellCount;
		uint32_t			nHeightCellCount;
		uint32_t			nDepthCellCount;
		GQuaternion			qOrientation;
		GVector3			vCenter;
		GVector3			vScale;
		GPRIMITIVE3D_TYPE	Type;
	);
	static void onCreatePrimitive3DDefault( ggns_Primitive3D* stuff )
	{
		(*stuff)->nWidth				= 1;
		(*stuff)->nHeight				= 1;
		(*stuff)->nDepth				= 1;
		(*stuff)->nStackCount			= 16;
		(*stuff)->nSliceCount			= 16;
		(*stuff)->qOrientation			= GQUATERNION_IDENTITY;
		(*stuff)->vCenter				= GVECTOR3_ZERO;
		(*stuff)->vScale				= GVECTOR3_ONE;
		(*stuff)->Type					= GPRIMITIVE_SPHERE;
	};

#pragma pack( pop )
	error_t buildGrid(	uint32_t nCellColumns, uint32_t nCellRows, 
						float64_t fCellWidth, float64_t fCellDepth, const GVector3& in_vCenter,
						float64_t fTexScaleX, float64_t fTexScaleZ, 
						uint32_t* inout_nVertexCount, GVector3* out_pVertices,
						uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
						bool* b32BitIndices, void* out_pIndices, 
						GBUFFERSLICE_DATA* out_BufferSlice,
						GVector2* out_pTexCoord, GVector3* out_pNormals
					);
	error_t buildDisc( float fRadius, uint32_t nSlices, const GVector3& in_vCenter,
						float64_t fTexScaleX, float64_t fTexScaleZ, 
						uint32_t* inout_nVertexCount, GVector3* out_pVertices,
						uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
						bool* b32BitIndices, void* out_pIndices, 
						GBUFFERSLICE_DATA* out_BufferSlice,
						GVector2* out_pTexCoord, GVector3* out_pNormals
					);
};

#endif // _GPRIMITIVE_CORE_H