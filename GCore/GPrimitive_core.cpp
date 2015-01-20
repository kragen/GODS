#include "GPrimitive_core.h"
#include "GCore_function_macros.h"
#include <math.h>

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "GMath_base.h"
#include "glist.h"

#ifdef _WIN32
#include <crtdbg.h>
#endif 

#include <stdlib.h>

using namespace god;

__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( PRIMITIVE3D, Primitive3D );
//__GDEFINE_CORE_WRITER_METHODS( _GODS(PRIMITIVE3D), Primitive3D );

__GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(PRIMITIVE3D), 
	"Type..........................: 0x%X\n"
	"nWidth........................: %u\n"
	"nHeight.......................: %u\n"
	"nDepth........................: %u\n"
	"nStackCount...................: %u\n"
	"nSliceCount...................: %u\n"
	"nWidthCellCount...............: %u\n"
	"nHeightCellCount..............: %u\n"
	"nDepthCellCount...............: %u\n"
	"vCenter.......................: (%f, %f, %f)\n"
	"vScale........................: (%f, %f, %f)\n"
	"qOrientation..................: (%f, %f, %f, %f)"
	, in_CoreInstance->Type
	, in_CoreInstance->nWidth
	, in_CoreInstance->nHeight
	, in_CoreInstance->nDepth
	, in_CoreInstance->nStackCount
	, in_CoreInstance->nSliceCount
	, in_CoreInstance->nWidthCellCount
	, in_CoreInstance->nHeightCellCount
	, in_CoreInstance->nDepthCellCount
	, in_CoreInstance->vCenter.x, in_CoreInstance->vCenter.y, in_CoreInstance->vCenter.z
	, in_CoreInstance->vScale.x, in_CoreInstance->vScale.y, in_CoreInstance->vScale.z
	, in_CoreInstance->qOrientation.x
	, in_CoreInstance->qOrientation.y
	, in_CoreInstance->qOrientation.z
	, in_CoreInstance->qOrientation.w
	);

error_t god::buildGrid(	uint32_t in_nColumnCells, uint32_t in_nRowCells, 
						float64_t fCellWidth, float64_t fCellDepth, const GVector3& in_vCenter,
						float64_t fTexScaleX, float64_t fTexScaleZ, 
						uint32_t* inout_nVertexCount, GVector3* out_pVertices,
						uint32_t* inout_nIndexCount, 
						uint32_t* inout_nTriangleCount, 
						bool* b32BitIndices, void* out_pIndices, 
						GBUFFERSLICE_DATA* out_BufferSlice,
						GVector2* out_pTexCoord,
						GVector3* out_pNormals
					)
{
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0;
	bool b32Bit				= b32BitIndices				?	*b32BitIndices			: false;

	int32_t nColumnCells		= ( 0 == in_nColumnCells ) ? 1 : in_nColumnCells;
	int32_t nRowCells			= ( 0 == in_nRowCells ) ? 1 : in_nRowCells;
	int32_t nColumnVertices		= nColumnCells+1;
	int32_t nRowVertices		= nRowCells+1;
	//bool bCountOnly = (0 == out_pIndices);

	uint32_t nTotalVertices	= nColumnVertices*nRowVertices;
	uint32_t nTotalCells	= nColumnCells*nRowCells*2; 
	uint32_t nTotalIndices	= nTotalCells*3;

	float64_t TexScaleX = 1.0 / (nColumnCells);
	float64_t TexScaleZ = 1.0 / (nRowCells); // we set the texture scale to fill the width of the grid

	if( 0.0 != fTexScaleX )
		TexScaleX	*= fTexScaleX;

	if( 0.0 != fTexScaleZ )
		TexScaleZ	*= fTexScaleZ;

	uint16_t* pIndices16	= 0;
	uint32_t* pIndices32	= 0;
	GVector3* pVertices		= out_pVertices, * pNormals = out_pNormals;
	GVector2* pTexCoord		= out_pTexCoord;

	if( b32Bit == true )
	{
		pIndices32 = (uint32_t*)out_pIndices;
	}
	else if( (nVertexCount+nTotalVertices) > 0xFFFF )
	{
		b32Bit = true;
	}
	else
	{
		pIndices16 = (uint16_t*)out_pIndices;
	}

	uint32_t idx=0, tri=0, vert=0;
	int32_t i, j;
	
	for(i = 0; i < nRowCells; ++i)
	{
		for(j = 0; j < nColumnCells; ++j)
		{	// build two triangles for each cell
			if( pIndices32 )
			{	// nVertexCount is used here as index offset
				pIndices32[nIndexCount]		= (  i   * nColumnVertices + j	)	+ nVertexCount;
				pIndices32[nIndexCount+1]	= (  i   * nColumnVertices + j + 1)	+ nVertexCount;
				pIndices32[nIndexCount+2]	= ((i+1) * nColumnVertices + j	)	+ nVertexCount;
						
				pIndices32[nIndexCount+3]	= ((i+1) * nColumnVertices + j	)	+ nVertexCount;
				pIndices32[nIndexCount+4]	= (  i   * nColumnVertices + j + 1)	+ nVertexCount;
				pIndices32[nIndexCount+5]	= ((i+1) * nColumnVertices + j + 1)	+ nVertexCount;
			}
			else if( pIndices16 )
			{
				pIndices16[nIndexCount]		= (  i   * nColumnVertices + j	)	+ nVertexCount;
				pIndices16[nIndexCount+1]	= (  i   * nColumnVertices + j + 1)	+ nVertexCount;
				pIndices16[nIndexCount+2]	= ((i+1) * nColumnVertices + j	)	+ nVertexCount;
						
				pIndices16[nIndexCount+3]	= ((i+1) * nColumnVertices + j	)	+ nVertexCount;
				pIndices16[nIndexCount+4]	= (  i   * nColumnVertices + j + 1)	+ nVertexCount;
				pIndices16[nIndexCount+5]	= ((i+1) * nColumnVertices + j + 1)	+ nVertexCount;
			}
			// next quad
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
		}
	} // 	
	
	GVector3 vCenter = -in_vCenter;
	for(i = 0; i < nRowVertices; ++i)
	{
		for(j = 0; j < nColumnVertices; ++j)
		{
			if( pVertices )
			{
				pVertices[nVertexCount].x = (j)	* (float)fCellWidth;
				pVertices[nVertexCount].z = (i)	* (float)fCellDepth;
				pVertices[nVertexCount].y =  0.0f;
				pVertices[nVertexCount] += vCenter;
			}
			if( pNormals )
				pNormals[vert] = GVector3( 0.0f, 1.0f, 0.0f );
			if( pTexCoord )
			{
				pTexCoord[nVertexCount].x = (j + nColumnCells)	* (float)TexScaleX;
				pTexCoord[nVertexCount].y = (i + nRowCells)		* (float)TexScaleZ;
			}
			++nVertexCount; // Next vertex
		} // for nSubgridColumnCells
	} // for nSubgridRowCells



	if( inout_nIndexCount		)	{ *inout_nIndexCount	= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount	= nTriangleCount;	}
	if( b32BitIndices			)	{ *b32BitIndices		= b32Bit;			}

	return 0;
}

error_t god::buildDisc( float fRadius, uint32_t nSlices, const GVector3& in_vCenter,
					float64_t fTexScaleX, float64_t fTexScaleZ, 
					uint32_t* inout_nVertexCount, GVector3* out_pVertices,
					uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
					bool* inout_b32BitIndices, void* out_pIndices, 
					GBUFFERSLICE_DATA* out_BufferSlice,
					GVector2* out_pTexCoord, GVector3* out_pNormals
				)
{
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0;
	bool b32BitIndices		= inout_b32BitIndices		?	*inout_b32BitIndices	: 0;

	uint32_t nTotalVertices = nSlices+1;
	if( false == b32BitIndices 
		&& (nVertexCount+nTotalVertices) > 0xFFFF )
	{
		b32BitIndices = true;
	}

	for( uint32_t iSlice=0; iSlice<nSlices; iSlice++ )
	{
		if( out_pIndices )
		{
			if( b32BitIndices )
			{
				((uint32_t*)out_pIndices)[nIndexCount]		= 0			+ nVertexCount;
				((uint32_t*)out_pIndices)[nIndexCount+1]	= iSlice	+ nVertexCount;
				((uint32_t*)out_pIndices)[nIndexCount+2]	= iSlice	+ 1 + nVertexCount;
			}
			else
			{
				((uint16_t*)out_pIndices)[nIndexCount]		= 0			+ nVertexCount;
				((uint16_t*)out_pIndices)[nIndexCount+1]	= iSlice	+ nVertexCount;
				((uint16_t*)out_pIndices)[nIndexCount+2]	= iSlice	+ 1 + nVertexCount;
			}
		}
		nIndexCount		+= 3;
		nTriangleCount	+= 2;
	};

	//
	float fSliceAngle = ((float)GMATH_2PI)/nSlices;
	float fX, fZ;

	// Assign the first vertex as the center for the cylinder
	if( out_pVertices )
		out_pVertices[0] = in_vCenter;
	nVertexCount++;
	for( uint32_t iSlice=0; iSlice<nSlices; iSlice++ )
	{
		fX = fRadius*cosf( fSliceAngle*iSlice );
		fZ = fRadius*sinf( fSliceAngle*iSlice );
		if( out_pVertices )
		{
			out_pVertices[nVertexCount]	= GVector3( fX, 0, fZ );
			out_pVertices[nVertexCount]	-= in_vCenter;
		}
		if( out_pTexCoord )
		{
			out_pTexCoord[nVertexCount]	= GVector2( fX*.5f, fZ*.5f );
			out_pTexCoord[nVertexCount]	+= GVector2( 0.5f, 0.5f );
		}
		if( out_pNormals )
			out_pNormals[nVertexCount]	= GVector3( 0.0f, 1.0f, 0.0f );
		nVertexCount++;
	};

	if( nVertexCount > USHRT_MAX )
		b32BitIndices = true;
	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_b32BitIndices		)	{ *inout_b32BitIndices		= b32BitIndices;	}
	
	return 0;
};