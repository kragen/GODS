
#include "GTerrain_core.h"
#include "GTerrain_core_private.h"

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <limits.h>

using namespace god;

// Forward declarations

//-------------------------------------------
int32_t god::buildTerrainGeometry( GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nBaseTileCount,
				GTILE_TEXTURE_DATA const* in_lstTileTextureData, int32_t nTileTextureDataCount, uint32_t nTextureCount,
				float fTileScale, uint32_t nColumnSubgrids, uint32_t nRowSubgrids,
				uint32_t nSubgridColumnTiles, uint32_t nSubgridRowTiles, 
				GVector3* out_pVertices, uint32_t* inout_nVertexCount, void* out_pIndices, bool* inout_b32BitIndices,
				uint32_t* inout_nIndexCount, GVector2* out_pTexCoord, 
				GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
				uint32_t* out_pTriangleToSubsetMap, 
				uint32_t* out_pSubsetToTextureMap, 
				uint32_t* out_pSubsetToFaceGroupMap, 
				uint32_t* out_pSubsetToChunkMap, 
				uint32_t* inout_nSubsetCount, 
				GTILE_MAPPING_DATA* out_lstTileMappingData,
				GBUFFERSLICE_DATA* out_lstBufferSliceDataChunks,
				GBUFFERSLICE_DATA* out_lstBufferSliceDataFaceGroups,
				GBUFFERSLICE_DATA* out_lstBufferSliceDataSubsets,
				uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
				uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount,
				GVector3& vOptionalOffset
			) // node count is nRowSubgrids*nColumnSubgrids
{
	GODS(TILE_GEOMETRY) pTileCurrent = 0, pTileFront = 0, pTileRight = 0;
	GODS(TILE_TEXTURE) TileTextureDataCurrent=0;

	GODS(TILE_MAPPING)* lstTileMappingData=0;
	glist<GODS(TILE_MAPPING)>	tmpTileMappingDataList;
	if( 0 != out_pNormals )
	{
		if( 0 != out_lstTileMappingData )
			lstTileMappingData = out_lstTileMappingData;
		else
		{
			tmpTileMappingDataList.resize( nBaseTileCount );
			lstTileMappingData = tmpTileMappingDataList;
		}
	}

	uint32_t nTotalWidth = nColumnSubgrids*nSubgridColumnTiles;
	uint32_t nTotalDepth = nRowSubgrids*nSubgridRowTiles;

	bool bCountOnly = false;
	if( 0 == out_pVertices )
		bCountOnly = true;
	if( false == bCountOnly )
	{
		if (0 == out_pIndices)
		{
			error_printf("Output index list is null!");
			return -1;
		}
	}
	if (0 == nRowSubgrids || 0 == nColumnSubgrids || 0 == nSubgridColumnTiles || 0 == nSubgridRowTiles)
	{
		error_printf("Zero is not a valid subgrid count!");
		return -1;
	}
	if (0 == nTextureCount || 0 == in_lstTileTextureData || 0 == in_lstTileGeometryData || 0 == nBaseTileCount)
	{
		error_printf("No texture count or no tile texture list!");
		return -1;
	}

	uint32_t nTileOffsetX, nTileOffsetZ;

	uint32_t i=0, j=0, nTextureIndex=0, nChunkColumnIndex=0, rs=0, nGlobalTileIndex=0;
	uint32_t nVertexCount=0, nIndexCount=0, nTriangleCount=0, nSubsetIndex=0;
	if( out_pNormals )
		memset( out_pNormals, 0, sizeof( GVector3 )*(*inout_nVertexCount) );

	uint32_t nChunkIndex = 0;
	error_t result=0;
	GBUFFERSLICE_DATA	newBufferSliceDataChunk=0, oldBufferSliceDataChunk=0;
	for( rs=0; rs<nRowSubgrids; rs++ )
	{
		for( nChunkColumnIndex=0; nChunkColumnIndex<nColumnSubgrids; nChunkColumnIndex++ )
		{
			nChunkIndex = (rs*nColumnSubgrids)+nChunkColumnIndex;
			nTileOffsetX = (nSubgridColumnTiles)*nChunkColumnIndex;
			nTileOffsetZ = (nSubgridRowTiles)*rs;

			if( 0 == newBufferSliceDataChunk )
			{
				gcreateData( &newBufferSliceDataChunk );
				newBufferSliceDataChunk->fTime				= 0;
				newBufferSliceDataChunk->ElementOffset		= nVertexCount;
				newBufferSliceDataChunk->IndexOffset		= nIndexCount;
			}

			if( 0 != (result=buildChunkGeometry( in_lstTileGeometryData, nTotalWidth, nTotalDepth, in_lstTileTextureData, nTileTextureDataCount, bCountOnly, nTileOffsetX, nTileOffsetZ,
				fTileScale, vOptionalOffset, nTextureCount, nSubgridColumnTiles, nSubgridRowTiles, nChunkIndex, &nVertexCount, &nIndexCount, &nTriangleCount, &nSubsetIndex, 
				out_pVertices, out_pIndices, *inout_b32BitIndices, out_pTexCoord, 
				out_pNormals, out_pTangents, out_pBitangents,
				out_pTriangleToSubsetMap, 
				out_pSubsetToTextureMap, 
				out_pSubsetToFaceGroupMap, 
				out_pSubsetToChunkMap,
				out_lstTileMappingData, 
				out_lstBufferSliceDataFaceGroups,
				out_lstBufferSliceDataSubsets, 
				inout_nTopTileCount, inout_nFrontTileCount, inout_nRightTileCount,
				inout_nBottomTileCount, inout_nBackTileCount, inout_nLeftTileCount
				)) )
			{
				error_printf("BuildChunkGeometry() returned 0x%X!", result);
				return -1;
			}
			newBufferSliceDataChunk->ElementCount	= nVertexCount	- newBufferSliceDataChunk->ElementOffset;
			newBufferSliceDataChunk->IndexCount		= nIndexCount	- newBufferSliceDataChunk->IndexOffset;
			if( out_lstBufferSliceDataChunks )
			{
				oldBufferSliceDataChunk = out_lstBufferSliceDataChunks[nChunkIndex];
				out_lstBufferSliceDataChunks[nChunkIndex] = newBufferSliceDataChunk;
				gfreeData( &oldBufferSliceDataChunk );
				newBufferSliceDataChunk = 0;
			}
			else 
			{
				gfreeData( &newBufferSliceDataChunk );
			}

		} // cs=0;
	}// rs=0;
	if( nVertexCount > 0xFFFF )
		*inout_b32BitIndices = true;
	else 
		*inout_b32BitIndices = false;

	if( (false == bCountOnly) && out_pNormals )
		recalculateTerrainNormals( out_pVertices, lstTileMappingData, in_lstTileGeometryData, nTotalWidth, nTotalDepth, fTileScale, out_pNormals );

	*inout_nVertexCount	= nVertexCount;
	*inout_nIndexCount	= nIndexCount;
	if( inout_nSubsetCount )
		*inout_nSubsetCount = nSubsetIndex;

	//if( 0 != out_pNormals && 0 == out_lstTileMappingData )
	//	free( lstTileMappingData );

	return result;
};


error_t god::buildChunkGeometry( GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, const GTILE_TEXTURE_DATA* in_lstTileTextureData, uint32_t nTileTextureDataCount, 
							bool bCountOnly, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, GVector3& vOptionalOffset,
							uint32_t nTextureCount, uint32_t nChunkColumnTileCount, uint32_t nChunkRowTileCount, uint32_t nChunkIndex, 
							uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t* inout_nSubsetCount,
							GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
							GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
							uint32_t* out_pTriangleToSubsetMap, 
							uint32_t* out_pSubsetToTextureMap, 
							uint32_t* out_pSubsetToFaceGroupMap, 
							uint32_t* out_pSubsetToChunkMap, 
							GTILE_MAPPING_DATA* out_lstTileMappingData,
							GBUFFERSLICE_DATA* out_lstBufferSliceDataFaceGroups,
							GBUFFERSLICE_DATA* out_lstBufferSliceDataSubsets,
							uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
							uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
						    )
{
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0,
		nSubsetIndex		= inout_nSubsetCount		?	*inout_nSubsetCount		: 0,
		nTopTileCount		= inout_nTopTileCount		?	*inout_nTopTileCount	: 0, 
		nFrontTileCount		= inout_nFrontTileCount		?	*inout_nFrontTileCount	: 0, 
		nRightTileCount		= inout_nRightTileCount		?	*inout_nRightTileCount	: 0,
		nBottomTileCount	= inout_nBottomTileCount	?	*inout_nBottomTileCount	: 0, 
		nBackTileCount		= inout_nBackTileCount		?	*inout_nBackTileCount	: 0, 
		nLeftTileCount		= inout_nLeftTileCount		?	*inout_nLeftTileCount	: 0;

	uint32_t nGlobalTileIndex	= 0,
		nGlobalTileX = 0,
		nGlobalTileZ = 0,
		nLocalTileX	= 0,
		nLocalTileZ	= 0;

	//uint32_t nSubsetID, nSubsetVertexCount, nSubsetFaceStart, nSubsetFaceCount, nSubsetVertexStart;

	GTILE_GEOMETRY_DATA pTileCurrent=0, pTileFront=0, pTileRight=0;

	GVector3* pvChunkMin=0, *pvChunkMax=0;//,
		//* pvSubsetMin, * pvSubsetMax;
	
	// pvChunkMax = (GVector3*)&out_pNodeData[nChunkIndex].BoundingVolume.vMax;
	// pvChunkMin = (GVector3*)&out_pNodeData[nChunkIndex].BoundingVolume.vMin;
	// pvChunkMin->x = (nTileOffsetX)*fTileScale;
	// pvChunkMin->z = (nTileOffsetZ)*fTileScale;
	// pvChunkMax->x = (nTileOffsetX+nChunkColumnTileCount)*fTileScale;
	// pvChunkMax->z = (nTileOffsetZ+nChunkRowTileCount)*fTileScale;

	uint32_t iFaceGroup;
	GBUFFERSLICE_DATA	newBufferSliceDataFaceGroup=0, oldBufferSliceDataFaceGroup=0;
	for( iFaceGroup=0; iFaceGroup<GFACINGDIRECTION_COUNT; iFaceGroup++ )
	{
		if( 0 == newBufferSliceDataFaceGroup )
		{
			gcreateData( &newBufferSliceDataFaceGroup );
			newBufferSliceDataFaceGroup->fTime				= 0;
			newBufferSliceDataFaceGroup->ElementOffset	= nVertexCount;
			newBufferSliceDataFaceGroup->IndexOffset		= nIndexCount;
		}

		if( buildChunkFaceGroupGeometry( iFaceGroup, in_lstTileGeometryData, nTotalWidth, nTotalDepth, in_lstTileTextureData, nTileTextureDataCount, 
							bCountOnly, nTileOffsetX, nTileOffsetZ, fTileScale, vOptionalOffset,
							nTextureCount, nChunkColumnTileCount, nChunkRowTileCount, nChunkIndex, 
							&nVertexCount, &nIndexCount, &nTriangleCount, &nSubsetIndex,
							out_pVertices, out_pIndices, b32BitIndices, out_pTexCoord, 
							out_pNormals, out_pTangents, out_pBitangents, 
							out_pTriangleToSubsetMap, 
							out_pSubsetToTextureMap, 
							out_pSubsetToFaceGroupMap, 
							out_pSubsetToChunkMap, 
							out_lstTileMappingData,
							out_lstBufferSliceDataSubsets,
							&nTopTileCount, &nFrontTileCount, &nRightTileCount,
							&nBottomTileCount, &nBackTileCount, &nLeftTileCount
						    ) )
		{
			error_printf("buildChunkFaceGroupGeometry() FAILED for iFaceGroup %u!", iFaceGroup);
			return -1;
		}

		newBufferSliceDataFaceGroup->ElementCount	= nVertexCount	- newBufferSliceDataFaceGroup->ElementOffset;
		newBufferSliceDataFaceGroup->IndexCount		= nIndexCount	- newBufferSliceDataFaceGroup->IndexOffset;
		if( out_lstBufferSliceDataFaceGroups )
		{
			uint32_t nGlobalIndex = nChunkIndex*GFACINGDIRECTION_COUNT+iFaceGroup;
			oldBufferSliceDataFaceGroup = out_lstBufferSliceDataFaceGroups[nGlobalIndex];
			out_lstBufferSliceDataFaceGroups[nGlobalIndex] = newBufferSliceDataFaceGroup;
			gfreeData( &oldBufferSliceDataFaceGroup );
			newBufferSliceDataFaceGroup = 0;
		}
		else 
		{
			gfreeData( &newBufferSliceDataFaceGroup );
		}
		//if( out_pSubsetToTextureMap )
		//	out_pSubsetToTextureMap[nSubsetIndex]	= nTextureIndex;
		//if( out_pSubsetToChunkMap )
		//	out_pSubsetToChunkMap[nSubsetIndex]		= nChunkIndex;
		//nSubsetIndex++;
	} // for( nTextureIndex < nTextureCount )
	gfreeData( &newBufferSliceDataFaceGroup );

	// GVector3 vCenter = (*pvChunkMin)+( ((*pvChunkMax)-(*pvChunkMin))*.5f );
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.x = vCenter.x;
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.y = vCenter.y;
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.z = vCenter.z;
	//}

	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_nSubsetCount		)	{ *inout_nSubsetCount		= nSubsetIndex;		}
	if( inout_nTopTileCount		)	{ *inout_nTopTileCount		= nTopTileCount;	}
	if( inout_nFrontTileCount	)	{ *inout_nFrontTileCount	= nFrontTileCount;	}
	if( inout_nRightTileCount	)	{ *inout_nRightTileCount	= nRightTileCount;	}
	if( inout_nBottomTileCount	)	{ *inout_nBottomTileCount	= nBottomTileCount;	}
	if( inout_nBackTileCount	)	{ *inout_nBackTileCount		= nBackTileCount;	}
	if( inout_nLeftTileCount	)	{ *inout_nLeftTileCount		= nLeftTileCount;	}

	return 0;
}

error_t god::buildChunkFaceGroupGeometry(	GTILEFACE_DIRECTION TileDirection, GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, 
											const GTILE_TEXTURE_DATA* in_lstTileTextureData, uint32_t nTileTextureDataCount, 
											bool bCountOnly, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, GVector3& vOptionalOffset,
											uint32_t nTextureCount, uint32_t nChunkColumnTileCount, uint32_t nChunkRowTileCount, uint32_t nChunkIndex, 
											uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t* inout_nSubsetCount,
											GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
											GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
											uint32_t* out_pTriangleToSubsetMap, 
											uint32_t* out_pSubsetToTextureMap, 
											uint32_t* out_pSubsetToFaceGroupMap, 
											uint32_t* out_pSubsetToChunkMap, 
											GTILE_MAPPING_DATA* out_lstTileMappingData,
											GBUFFERSLICE_DATA* out_lstBufferSliceDataSubsets,
											uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
											uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
											)
{
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0,
		nSubsetIndex		= inout_nSubsetCount		?	*inout_nSubsetCount		: 0,
		nTopTileCount		= inout_nTopTileCount		?	*inout_nTopTileCount	: 0, 
		nFrontTileCount		= inout_nFrontTileCount		?	*inout_nFrontTileCount	: 0, 
		nRightTileCount		= inout_nRightTileCount		?	*inout_nRightTileCount	: 0,
		nBottomTileCount	= inout_nBottomTileCount	?	*inout_nBottomTileCount	: 0, 
		nBackTileCount		= inout_nBackTileCount		?	*inout_nBackTileCount	: 0, 
		nLeftTileCount		= inout_nLeftTileCount		?	*inout_nLeftTileCount	: 0;

	uint32_t nGlobalTileIndex	= 0,
		nGlobalTileX = 0,
		nGlobalTileZ = 0,
		nLocalTileX	= 0,
		nLocalTileZ	= 0;

	//uint32_t nSubsetID, nSubsetVertexCount, nSubsetFaceStart, nSubsetFaceCount, nSubsetVertexStart;

	GTILE_GEOMETRY_DATA pTileCurrent=0, pTileFront=0, pTileRight=0;

	GVector3* pvChunkMin=0, *pvChunkMax=0;//,
		//* pvSubsetMin, * pvSubsetMax;
	
	// pvChunkMax = (GVector3*)&out_pNodeData[nChunkIndex].BoundingVolume.vMax;
	// pvChunkMin = (GVector3*)&out_pNodeData[nChunkIndex].BoundingVolume.vMin;
	// pvChunkMin->x = (nTileOffsetX)*fTileScale;
	// pvChunkMin->z = (nTileOffsetZ)*fTileScale;
	// pvChunkMax->x = (nTileOffsetX+nChunkColumnTileCount)*fTileScale;
	// pvChunkMax->z = (nTileOffsetZ+nChunkRowTileCount)*fTileScale;

	uint32_t nTextureIndex;
	GBUFFERSLICE_DATA	newBufferSliceDataSubset=0, oldBufferSliceDataSubset;
	for( nTextureIndex=0; nTextureIndex<nTextureCount; nTextureIndex++ )
	{
		if( 0 == newBufferSliceDataSubset )
		{
			gcreateData( &newBufferSliceDataSubset );
			newBufferSliceDataSubset->fTime				= 0;
			newBufferSliceDataSubset->ElementOffset	= nVertexCount;
			newBufferSliceDataSubset->IndexOffset		= nIndexCount;
		}

		if( buildChunkSubsetGeometry( TileDirection, in_lstTileGeometryData, nTotalWidth, nTotalDepth, nTileOffsetX, nTileOffsetZ, fTileScale, in_lstTileTextureData, nTileTextureDataCount, nTextureIndex, nChunkIndex, 
			nChunkRowTileCount, nChunkColumnTileCount, bCountOnly, vOptionalOffset, &nVertexCount, &nIndexCount, &nTriangleCount, nSubsetIndex, 
			out_pVertices, out_pIndices, b32BitIndices, out_pTexCoord, 
			out_pNormals, out_pTangents, out_pBitangents,
			out_pTriangleToSubsetMap, out_lstTileMappingData, 
			&nTopTileCount, &nFrontTileCount, &nRightTileCount,
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount 
			) )
		{
			error_printf("BuildChunkSubsetGeometry() FAILED for nTextureIndex %u!", nTextureIndex);
			return -1;
		}

		newBufferSliceDataSubset->ElementCount	= nVertexCount	- newBufferSliceDataSubset->ElementOffset;
		newBufferSliceDataSubset->IndexCount	= nIndexCount	- newBufferSliceDataSubset->IndexOffset;
		if( newBufferSliceDataSubset->ElementCount )
		{
			if( out_lstBufferSliceDataSubsets )
			{
				oldBufferSliceDataSubset = out_lstBufferSliceDataSubsets[nSubsetIndex];
				out_lstBufferSliceDataSubsets[nSubsetIndex] = newBufferSliceDataSubset;
				gfreeData( &oldBufferSliceDataSubset );
				newBufferSliceDataSubset = 0;
			}
			else 
			{
				gfreeData( &newBufferSliceDataSubset );
			}
			if( out_pSubsetToTextureMap )
				out_pSubsetToTextureMap[nSubsetIndex]	= nTextureIndex;
			if( out_pSubsetToChunkMap )
				out_pSubsetToChunkMap[nSubsetIndex]		= nChunkIndex;
			if( out_pSubsetToFaceGroupMap )
				out_pSubsetToFaceGroupMap[nSubsetIndex]	= (nChunkIndex*GFACINGDIRECTION_COUNT)+TileDirection;
			nSubsetIndex++;
		}
		//else
	} // for( nTextureIndex < nTextureCount )
	gfreeData( &newBufferSliceDataSubset );

	// GVector3 vCenter = (*pvChunkMin)+( ((*pvChunkMax)-(*pvChunkMin))*.5f );
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.x = vCenter.x;
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.y = vCenter.y;
	//out_pNodeData[nChunkIndex].BoundingVolume.vCenter.z = vCenter.z;
	//}

	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_nSubsetCount		)	{ *inout_nSubsetCount		= nSubsetIndex;		}
	if( inout_nTopTileCount		)	{ *inout_nTopTileCount		= nTopTileCount;	}
	if( inout_nFrontTileCount	)	{ *inout_nFrontTileCount	= nFrontTileCount;	}
	if( inout_nRightTileCount	)	{ *inout_nRightTileCount	= nRightTileCount;	}
	if( inout_nBottomTileCount	)	{ *inout_nBottomTileCount	= nBottomTileCount;	}
	if( inout_nBackTileCount	)	{ *inout_nBackTileCount		= nBackTileCount;	}
	if( inout_nLeftTileCount	)	{ *inout_nLeftTileCount		= nLeftTileCount;	}

	return 0;
}


error_t god::buildChunkSubsetGeometry( GTILEFACE_DIRECTION TileDirection, GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, 
									GTILE_TEXTURE_DATA const* in_lstTileTextureData, uint32_t nTileTextureDataCount, int32_t nTextureIndex, 
									uint32_t nChunkIndex, uint32_t nChunkRowTileCount, uint32_t nChunkColumnTileCount, bool bCountOnly, GVector3 vOptionalOffset,
									uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t nSubsetIndex, //uint32_t* inout_nSubsetCount,
									GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
									GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
									uint32_t* out_pTriangleToSubsetMap, GTILE_MAPPING_DATA* out_lstTileMappingData,
									uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
									uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
								 )
{
	GPROFILE_FUNCTION();

	uint32_t nVertexCount	= inout_nVertexCount	?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount		?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount	?	*inout_nTriangleCount	: 0,
		nTopTileCount		= inout_nTopTileCount	?	*inout_nTopTileCount	: 0, 
		nFrontTileCount		= inout_nFrontTileCount	?	*inout_nFrontTileCount	: 0, 
		nRightTileCount		= inout_nRightTileCount		?	*inout_nRightTileCount	: 0,
		nBottomTileCount	= inout_nBottomTileCount	?	*inout_nBottomTileCount	: 0, 
		nBackTileCount		= inout_nBackTileCount		?	*inout_nBackTileCount	: 0, 
		nLeftTileCount		= inout_nLeftTileCount		?	*inout_nLeftTileCount	: 0;

	uint32_t nGlobalTileIndex	= 0,
		nGlobalTileX = 0,
		nGlobalTileZ = 0,
		nLocalTileX	= 0,
		nLocalTileZ	= 0;

	uint32_t nSubsetVertexCount, nSubsetVertexStart;

	GTILE_GEOMETRY_DATA pTileCurrent=0, pTileFront=0, pTileRight=0;

	nSubsetVertexStart	= nVertexCount;
	for( nLocalTileZ=0; nLocalTileZ<nChunkRowTileCount; nLocalTileZ++ )
	{
		for( nLocalTileX=0; nLocalTileX<nChunkColumnTileCount; nLocalTileX++ )
		{
			// As we build from local chunk coordinates, we need to calculate the global tile indices
			nGlobalTileIndex	= (nTileOffsetZ+nLocalTileZ)*nTotalWidth+nLocalTileX+nTileOffsetX;
			nGlobalTileX		= (nLocalTileX + nTileOffsetX);	// nGlobalTileIndex % nTotalWidth;
			nGlobalTileZ		= (nLocalTileZ + nTileOffsetZ);	// nGlobalTileIndex / nTotalWidth;

			if( out_lstTileMappingData )
			{
				if( 0 == out_lstTileMappingData[nGlobalTileIndex] )
				{
					gcreateData ( &out_lstTileMappingData[nGlobalTileIndex] );
					out_lstTileMappingData[nGlobalTileIndex]->nChunkIndex = nChunkIndex;
				}
			}
			pTileCurrent = in_lstTileGeometryData[nGlobalTileIndex];
			pTileFront = getFrontTile( in_lstTileGeometryData, nGlobalTileIndex, nTotalWidth, nTotalDepth );
			pTileRight = getRightTile( in_lstTileGeometryData, nGlobalTileIndex, nTotalWidth, nTotalDepth );
			if( buildTileFaces( TileDirection, pTileCurrent, pTileFront, pTileRight, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
				nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
				out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, 
				out_pTriangleToSubsetMap, ( out_lstTileMappingData ) ? &out_lstTileMappingData[nGlobalTileIndex] : 0, 
				&nTopTileCount, &nFrontTileCount, &nRightTileCount,
				&nBottomTileCount, &nBackTileCount, &nLeftTileCount) )
			{
				error_printf("BuildTileGeometry() FAILED when requested to build TOP tile geometry.");
				gfreeData( &pTileFront );
				gfreeData( &pTileRight );
				return -1;
			};
			gfreeData( &pTileFront );
			gfreeData( &pTileRight );
		} // j=0;
	} // i=0;
	nSubsetVertexCount = nVertexCount - nSubsetVertexStart;
	if( nSubsetVertexCount > 0 )
		nSubsetIndex++;

	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_nTopTileCount		)	{ *inout_nTopTileCount		= nTopTileCount;	}
	if( inout_nFrontTileCount	)	{ *inout_nFrontTileCount	= nFrontTileCount;	}
	if( inout_nRightTileCount	)	{ *inout_nRightTileCount	= nRightTileCount;	}
	if( inout_nBottomTileCount	)	{ *inout_nBottomTileCount	= nBottomTileCount;	}
	if( inout_nBackTileCount	)	{ *inout_nBackTileCount		= nBackTileCount;	}
	if( inout_nLeftTileCount	)	{ *inout_nLeftTileCount		= nLeftTileCount;	}

	return 0;
}


error_t god::buildTileFaces( GTILEFACE_DIRECTION TileDirection, const GTILE_GEOMETRY_DATA pTileCurrent, const GTILE_GEOMETRY_DATA pTileFront, const GTILE_GEOMETRY_DATA pTileRight, 
							uint32_t nGlobalTileX, uint32_t nGlobalTileZ, float fTileScale, bool bCountOnly, GVector3 vOptionalOffset,
							const GTILE_TEXTURE_DATA* in_lstTileTextureData, uint32_t nTileTextureDataCount, int32_t nTextureIndex, uint32_t nSubsetIndex,
							uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
							GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
							GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
							uint32_t* out_pTriangleToSubsetMap, GTILE_MAPPING_DATA* out_CurrentTileMappingData,
							uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
							uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
						)
{
//	GPROFILE_FUNCTION();
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0,
		nTopTileCount		= inout_nTopTileCount		?	*inout_nTopTileCount	: 0, 
		nFrontTileCount		= inout_nFrontTileCount		?	*inout_nFrontTileCount	: 0, 
		nRightTileCount		= inout_nRightTileCount		?	*inout_nRightTileCount	: 0,
		nBottomTileCount	= inout_nBottomTileCount	?	*inout_nBottomTileCount	: 0, 
		nBackTileCount		= inout_nBackTileCount		?	*inout_nBackTileCount	: 0, 
		nLeftTileCount		= inout_nLeftTileCount		?	*inout_nLeftTileCount	: 0;

	switch( TileDirection )
	{
	case GFACINGDIRECTION_TOP:
		if( buildTileGeometry( GFACINGDIRECTION_TOP, pTileCurrent, 0, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build TOP tile geometry.");
			return -1;
		};
		break;
	case GFACINGDIRECTION_FRONT:
		if( buildTileGeometry( GFACINGDIRECTION_FRONT, pTileCurrent, pTileFront, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build FRONT tile geometry.");
			return -1;
		};
		break;
	case GFACINGDIRECTION_RIGHT:
		if( buildTileGeometry( GFACINGDIRECTION_RIGHT, pTileCurrent, pTileRight, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build RIGHT tile geometry.");
			return -1;
		};
		break;
	case GFACINGDIRECTION_BOTTOM:
		if( buildTileGeometry( GFACINGDIRECTION_BOTTOM, pTileCurrent, 0, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build TOP tile geometry.");
			return -1;
		};
		break;
	case GFACINGDIRECTION_BACK:
		if( buildTileGeometry( GFACINGDIRECTION_BACK, pTileCurrent, pTileFront, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build BACK tile geometry.");
			return -1;
		};
		break;
	case GFACINGDIRECTION_LEFT:
		if( buildTileGeometry( GFACINGDIRECTION_LEFT, pTileCurrent, pTileRight, nGlobalTileX, nGlobalTileZ, fTileScale, bCountOnly, vOptionalOffset, in_lstTileTextureData, nTileTextureDataCount, 
			nTextureIndex, nSubsetIndex, &nVertexCount, &nIndexCount, &nTriangleCount, out_pVertices, out_pIndices, b32BitIndices, 
			out_pTexCoord, out_pNormals, out_pTangents, out_pBitangents, out_pTriangleToSubsetMap, out_CurrentTileMappingData,
			&nTopTileCount, &nFrontTileCount, &nRightTileCount, 
			&nBottomTileCount, &nBackTileCount, &nLeftTileCount
			) )
		{
			error_printf("BuildTileGeometry() FAILED when was requested to build LEFT tile geometry.");
			return -1;
		};
	}



	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_nTopTileCount		)	{ *inout_nTopTileCount		= nTopTileCount;	}
	if( inout_nFrontTileCount	)	{ *inout_nFrontTileCount	= nFrontTileCount;	}
	if( inout_nRightTileCount	)	{ *inout_nRightTileCount	= nRightTileCount;	}
	if( inout_nBottomTileCount	)	{ *inout_nBottomTileCount	= nBottomTileCount;	}
	if( inout_nBackTileCount	)	{ *inout_nBackTileCount		= nBackTileCount;	}
	if( inout_nLeftTileCount	)	{ *inout_nLeftTileCount		= nLeftTileCount;	}

	return 0;
};

error_t god::buildTileGeometry( GTILEFACE_DIRECTION TileDirection, const GTILE_GEOMETRY_DATA pTileCurrent, const GTILE_GEOMETRY_DATA pTileSide, 
								uint32_t nGlobalTileX, uint32_t nGlobalTileZ, float fTileScale, bool bCountOnly, GVector3 vOptionalOffset,
								const GTILE_TEXTURE_DATA* in_lstTileTextureData, uint32_t nTileTextureDataCount, int32_t nTextureIndex, uint32_t nSubsetIndex,
								uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
								GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
								GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
								uint32_t* out_pTriangleToSubsetMap, GTILE_MAPPING_DATA* out_CurrentTileMappingData,
								uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
								uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
								)
{
//	GPROFILE_FUNCTION();
	uint32_t nVertexCount	= inout_nVertexCount		?	*inout_nVertexCount		: 0, 
		nIndexCount			= inout_nIndexCount			?	*inout_nIndexCount		: 0, 
		nTriangleCount		= inout_nTriangleCount		?	*inout_nTriangleCount	: 0,
		nTopTileCount		= inout_nTopTileCount		?	*inout_nTopTileCount	: 0, 
		nFrontTileCount		= inout_nFrontTileCount		?	*inout_nFrontTileCount	: 0, 
		nRightTileCount		= inout_nRightTileCount		?	*inout_nRightTileCount	: 0,
		nBottomTileCount	= inout_nBottomTileCount	?	*inout_nBottomTileCount	: 0, 
		nBackTileCount		= inout_nBackTileCount		?	*inout_nBackTileCount	: 0, 
		nLeftTileCount		= inout_nLeftTileCount		?	*inout_nLeftTileCount	: 0;
	GVector3* out_Vert0 = 0;
	GVector3* out_Vert1 = 0;
	GVector3* out_Vert2 = 0;
	GVector3* out_Vert3 = 0;

	if( 0 == pTileCurrent )
	{
		if( out_CurrentTileMappingData )
		{
			gfreeData( out_CurrentTileMappingData );
		}		
		return 0;
	}

	GODS(TILE_TEXTURE) TileTextureDataCurrent = 0;

	GTILEFACE_DIRECTION TrueDirection = GFACINGDIRECTION_TOP;
	bool bReverseTile = (pTileCurrent->Flags & 0x01) ? true : false;
	float otherHeight = (0 != pTileSide) ? (pTileSide->fHeight[0]*fTileScale) : 0;

	switch( TileDirection )
	{
	case GFACINGDIRECTION_TOP:
		if( ( pTileCurrent->TopTextureDataIndex >= 0 ) 
			&&  ( pTileCurrent->TopTextureDataIndex < (int32_t)nTileTextureDataCount ) 
			&& nTextureIndex == in_lstTileTextureData[pTileCurrent->TopTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = 
					 ( (pTileCurrent->fHeight[0] == pTileCurrent->fHeight[1]) 
					&& (pTileCurrent->fHeight[0] == pTileCurrent->fHeight[2])
					&& (pTileCurrent->fHeight[0] == pTileCurrent->fHeight[3])
					) 
				? GFACINGDIRECTION_TOP : GFACINGDIRECTION_BOTTOM;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->TopTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];
				_AssignTileVertices( GFACINGDIRECTION_TOP, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, 0, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( GFACINGDIRECTION_TOP, nVertexCount, pTileCurrent->fHeight, fTileScale, 0, out_pNormals );
				_AssignTileTangents( GFACINGDIRECTION_TOP, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, 0, out_pTangents );
				_AssignTileBitangents( GFACINGDIRECTION_TOP, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, 0, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesTop[0]	= nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesTop[1]	= nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesTop[2]	= nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesTop[3]	= nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexTop	= nSubsetIndex;
				}
				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]	= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nTopTileCount++;
		}
		else if( -1 == pTileCurrent->TopTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesTop[0] = 
				(*out_CurrentTileMappingData)->VerticesTop[1] = 
				(*out_CurrentTileMappingData)->VerticesTop[2] = 
				(*out_CurrentTileMappingData)->VerticesTop[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexTop	= nSubsetIndex;
			}
		}

		break;
	case GFACINGDIRECTION_FRONT:
		if( ( pTileCurrent->FrontTextureDataIndex >= 0 ) 
			&&  ( pTileCurrent->FrontTextureDataIndex < (int32_t)nTileTextureDataCount ) 
			&& nTextureIndex == in_lstTileTextureData[pTileCurrent->FrontTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = ( (pTileCurrent->fHeight[1]*fTileScale) < otherHeight ) ? GFACINGDIRECTION_BACK : GFACINGDIRECTION_FRONT;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->FrontTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];	
				_AssignTileVertices( GFACINGDIRECTION_FRONT, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, pTileSide, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, pTileSide, out_pNormals );
				_AssignTileTangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pTangents );
				_AssignTileBitangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesFront[0] = nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesFront[1] = nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesFront[2] = nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesFront[3] = nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexFront	= nSubsetIndex;
				}

				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]		= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nFrontTileCount++;
		}
		else if( -1 == pTileCurrent->FrontTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesFront[0] = 
				(*out_CurrentTileMappingData)->VerticesFront[1] = 
				(*out_CurrentTileMappingData)->VerticesFront[2] = 
				(*out_CurrentTileMappingData)->VerticesFront[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexFront	= nSubsetIndex;
			}
		}
						
		break;
	case GFACINGDIRECTION_RIGHT:
		if( ( pTileCurrent->RightTextureDataIndex >= 0 ) 
				&&  ( pTileCurrent->RightTextureDataIndex < (int32_t)nTileTextureDataCount ) 
				&& nTextureIndex == in_lstTileTextureData[pTileCurrent->RightTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = ( (pTileCurrent->fHeight[2]*fTileScale) < otherHeight ) ? GFACINGDIRECTION_LEFT : GFACINGDIRECTION_RIGHT;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->RightTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];
				_AssignTileVertices( GFACINGDIRECTION_RIGHT, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, pTileSide, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, pTileSide, out_pNormals );
				_AssignTileTangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pTangents );
				_AssignTileBitangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesRight[0] = nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesRight[1] = nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesRight[2] = nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesRight[3] = nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexRight		= nSubsetIndex;
				}

				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]	= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nRightTileCount++;
		} // 
		else if( -1 == pTileCurrent->RightTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesRight[0] = 
				(*out_CurrentTileMappingData)->VerticesRight[1] = 
				(*out_CurrentTileMappingData)->VerticesRight[2] = 
				(*out_CurrentTileMappingData)->VerticesRight[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexRight		= nSubsetIndex;
			}
		}
	case GFACINGDIRECTION_BOTTOM:
		if( ( pTileCurrent->TopTextureDataIndex >= 0 ) 
			&&  ( pTileCurrent->TopTextureDataIndex < (int32_t)nTileTextureDataCount ) 
			&& nTextureIndex == in_lstTileTextureData[pTileCurrent->TopTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = ( pTileCurrent->fHeight[0] == pTileCurrent->fHeight[1] 
					&& pTileCurrent->fHeight[0] == pTileCurrent->fHeight[2]
					&& pTileCurrent->fHeight[0] == pTileCurrent->fHeight[3]
					) 
				? GFACINGDIRECTION_TOP : GFACINGDIRECTION_BOTTOM;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->TopTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];
				_AssignTileVertices( GFACINGDIRECTION_BOTTOM, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, 0, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( GFACINGDIRECTION_BOTTOM, nVertexCount, pTileCurrent->fHeight, fTileScale, 0, out_pNormals );
				_AssignTileTangents( GFACINGDIRECTION_BOTTOM, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, 0, out_pTangents );
				_AssignTileBitangents( GFACINGDIRECTION_BOTTOM, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, 0, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesTop[0]	= nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesTop[1]	= nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesTop[2]	= nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesTop[3]	= nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexTop	= nSubsetIndex;
				}
				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]	= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nBottomTileCount++;
		}
		else if( -1 == pTileCurrent->TopTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesTop[0] = 
				(*out_CurrentTileMappingData)->VerticesTop[1] = 
				(*out_CurrentTileMappingData)->VerticesTop[2] = 
				(*out_CurrentTileMappingData)->VerticesTop[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexTop	= nSubsetIndex;
			}
		}

		break;
	case GFACINGDIRECTION_BACK:
		if( ( pTileCurrent->FrontTextureDataIndex >= 0 ) 
			&&  ( pTileCurrent->FrontTextureDataIndex < (int32_t)nTileTextureDataCount ) 
			&& nTextureIndex == in_lstTileTextureData[pTileCurrent->FrontTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = ( (pTileCurrent->fHeight[1]*fTileScale) < otherHeight ) ? GFACINGDIRECTION_BACK : GFACINGDIRECTION_FRONT;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->FrontTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];	
				_AssignTileVertices( TrueDirection, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, pTileSide, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, pTileSide, out_pNormals );
				_AssignTileTangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pTangents );
				_AssignTileBitangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesFront[0] = nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesFront[1] = nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesFront[2] = nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesFront[3] = nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexFront	= nSubsetIndex;
				}

				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]		= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nBackTileCount++;
		}
		else if( -1 == pTileCurrent->FrontTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesFront[0] = 
				(*out_CurrentTileMappingData)->VerticesFront[1] = 
				(*out_CurrentTileMappingData)->VerticesFront[2] = 
				(*out_CurrentTileMappingData)->VerticesFront[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexFront	= nSubsetIndex;
			}
		}
						
		break;
	case GFACINGDIRECTION_LEFT:
		if( ( pTileCurrent->RightTextureDataIndex >= 0 ) 
				&&  ( pTileCurrent->RightTextureDataIndex < (int32_t)nTileTextureDataCount ) 
				&& nTextureIndex == in_lstTileTextureData[pTileCurrent->RightTextureDataIndex]->nTextureIndex )
		{
			TrueDirection = ( (pTileCurrent->fHeight[2]*fTileScale) < otherHeight ) ? GFACINGDIRECTION_LEFT : GFACINGDIRECTION_RIGHT;
			if( TrueDirection != TileDirection )
				break;
			if( false == bCountOnly )
			{
				TileTextureDataCurrent = in_lstTileTextureData[pTileCurrent->RightTextureDataIndex];
				out_Vert0 = &out_pVertices[nVertexCount+0]; out_Vert1 = &out_pVertices[nVertexCount+1];
				out_Vert2 = &out_pVertices[nVertexCount+2]; out_Vert3 = &out_pVertices[nVertexCount+3];
				_AssignTileVertices( TrueDirection, nVertexCount, nGlobalTileX, nGlobalTileZ, pTileCurrent->fHeight, fTileScale, vOptionalOffset, pTileSide, out_pVertices );
				_AssignTileIndices( nIndexCount, bReverseTile, nVertexCount, out_pIndices, b32BitIndices );
				_AssignTileTexCoord( nVertexCount, TileTextureDataCurrent, out_pTexCoord );
				_AssignTileNormals( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, pTileSide, out_pNormals );
				_AssignTileTangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pTangents );
				_AssignTileBitangents( TrueDirection, nVertexCount, pTileCurrent->fHeight, fTileScale, TileTextureDataCurrent, pTileSide, out_pBitangents );
				_SetTerrainBVMinMax( out_Vert0, out_Vert1, out_Vert2, out_Vert3, 0, 0, 0, 0 );

				if( out_CurrentTileMappingData )
				{
					if( 0 == *out_CurrentTileMappingData )
						gcreateData( out_CurrentTileMappingData );
					(*out_CurrentTileMappingData)->VerticesRight[0] = nVertexCount+0;
					(*out_CurrentTileMappingData)->VerticesRight[1] = nVertexCount+1;
					(*out_CurrentTileMappingData)->VerticesRight[2] = nVertexCount+2;
					(*out_CurrentTileMappingData)->VerticesRight[3] = nVertexCount+3;
					(*out_CurrentTileMappingData)->SubsetIndexRight	= nSubsetIndex;
				}

				// set attribute
				if( out_pTriangleToSubsetMap )
				{
					out_pTriangleToSubsetMap[nTriangleCount]	= nSubsetIndex;
					out_pTriangleToSubsetMap[nTriangleCount+1]	= nSubsetIndex;
				}
			}
			nVertexCount	+= 4;
			nIndexCount		+= 6;
			nTriangleCount	+= 2;
			nLeftTileCount++;
		} // 
		else if( -1 == pTileCurrent->RightTextureDataIndex )
		{
			if( out_CurrentTileMappingData )
			{
				if( 0 == *out_CurrentTileMappingData )
					gcreateData( out_CurrentTileMappingData );
				(*out_CurrentTileMappingData)->VerticesRight[0] = 
				(*out_CurrentTileMappingData)->VerticesRight[1] = 
				(*out_CurrentTileMappingData)->VerticesRight[2] = 
				(*out_CurrentTileMappingData)->VerticesRight[3] = -1;
				(*out_CurrentTileMappingData)->SubsetIndexRight	= nSubsetIndex;
			}
		}
	}

	if( inout_nIndexCount		)	{ *inout_nIndexCount		= nIndexCount;		}
	if( inout_nVertexCount		)	{ *inout_nVertexCount	 	= nVertexCount;		}
	if( inout_nTriangleCount	)	{ *inout_nTriangleCount		= nTriangleCount;	}
	if( inout_nTopTileCount		)	{ *inout_nTopTileCount		= nTopTileCount;	}
	if( inout_nFrontTileCount	)	{ *inout_nFrontTileCount	= nFrontTileCount;	}
	if( inout_nRightTileCount	)	{ *inout_nRightTileCount	= nRightTileCount;	}
	if( inout_nBottomTileCount	)	{ *inout_nBottomTileCount	= nBottomTileCount;	}
	if( inout_nBackTileCount	)	{ *inout_nBackTileCount		= nBackTileCount;	}
	if( inout_nLeftTileCount	)	{ *inout_nLeftTileCount		= nLeftTileCount;	}
	return 0;
};

error_t god::recalculateTerrainNormals( const GVector3* in_pVertices, GTILE_MAPPING_DATA const * in_lstTileMappingData,
				 GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nWidth, uint32_t nDepth, 
				 float fTileScale, GVector3 *out_pNormals )
{
			GPROFILE_FUNCTION();
	if( 0 == in_pVertices || 0 == in_lstTileGeometryData || 0 == in_lstTileGeometryData || 0 == nWidth || 0 == nDepth || 0 == fTileScale || 0 == out_pNormals )
	{
		error_printf("Invalid parameters! All parameters are mandatory for this function to work!");
		return -1;
	}

	uint32_t i, 
		nTotalTiles = nWidth*nDepth;
	GTILE_MAPPING_DATA pIdxCurrent;
	GTILE_GEOMETRY_DATA pTileCurrent;

	GVector3 BlendNormalFinal;

	error_t result = 0;
	// first pass builds triangle normals independently (no normal smoothing)
	for( i=0; i<nTotalTiles; i++ )
	{
		pIdxCurrent		= in_lstTileMappingData[i];
		pTileCurrent	= in_lstTileGeometryData[i];
		if( 0 == pTileCurrent || -1 == pIdxCurrent->VerticesTop[0] )
			continue;

		if( 0 != (result = recalculateTileNormals( in_pVertices, pTileCurrent, pIdxCurrent, fTileScale, out_pNormals )) )
		{
			error_printf( "RecalculateTileNormals() returned 0x%X.", result );
			return -1;
		}

	}
	return 0;
}

error_t god::recalculateTileNormals( const GVector3* in_pVertices, const GTILE_GEOMETRY_DATA in_TileGeometryDataCurrent, 
									const GTILE_MAPPING_DATA in_TileMappingDataCurrent, float fTileScale, 
									GVector3 *out_pNormals )
{
	GPROFILE_FUNCTION();
	if( 0 == in_pVertices || 0 == in_TileGeometryDataCurrent || 0 == in_TileMappingDataCurrent || 0 == fTileScale || 0 == out_pNormals )
	{
		error_printf("Invalid parameters! All parameters are mandatory for this function to work!");
		return -1;
	}	
	GVector3 BlendNormal0;
	GVector3 BlendNormal1;
	GVector3 BlendNormal2;
	GVector3 BlendNormal3;

	const GTILE_GEOMETRY_DATA	tileGeometryCurrent	= in_TileGeometryDataCurrent;
	const GTILE_MAPPING_DATA	tileMappingCurrent	= in_TileMappingDataCurrent;
	float fScale = 1/fTileScale;
	// ////-------------------------------
	// //BlendNormal0 = ( (pVertices[pIdxCurrent->VerticesTop[2]] - pVertices[pIdxCurrent->VerticesTop[0]]) )
	// //	.Cross( (pVertices[pIdxCurrent->VerticesTop[1]] - pVertices[pIdxCurrent->VerticesTop[0]]) );
	// 
	// //BlendNormal1 = ( (pVertices[pIdxCurrent->VerticesTop[3]] - pVertices[pIdxCurrent->VerticesTop[1]]) )
	// //	.Cross( -(pVertices[pIdxCurrent->VerticesTop[0]] - pVertices[pIdxCurrent->VerticesTop[1]]) );
	// 
	// //BlendNormal2 = ( (pVertices[pIdxCurrent->VerticesTop[3]] - pVertices[pIdxCurrent->VerticesTop[2]]) )
	// //	.Cross( (pVertices[pIdxCurrent->VerticesTop[0]] - pVertices[pIdxCurrent->VerticesTop[2]]) );
	// 
	// //BlendNormal3 = ( (pVertices[pIdxCurrent->VerticesTop[2]] - pVertices[pIdxCurrent->VerticesTop[3]]) )
	// //	.Cross( -(pVertices[pIdxCurrent->VerticesTop[1]] - pVertices[pIdxCurrent->VerticesTop[3]]) );
	GVector3 v0 = GVector3(0.0f, tileGeometryCurrent->fHeight[0]*fScale, 0.0f), 
		v1 = GVector3(1.0f, tileGeometryCurrent->fHeight[1]*fScale, 0.0f), 
		v2 = GVector3(0.0f, tileGeometryCurrent->fHeight[2]*fScale, 1.0f), 
		v3 = GVector3(1.0f, tileGeometryCurrent->fHeight[3]*fScale, 1.0f);
	
	BlendNormal0 = ( v2 - v0 ).Cross( v1 - v0 );
	BlendNormal1 = ( v3 - v1 ).Cross( -(v0 - v1) );
	BlendNormal2 = ( v3 - v2 ).Cross( v0 - v2 );
	BlendNormal3 = ( v2 - v3 ).Cross( -(v1 - v3) );

	out_pNormals[tileMappingCurrent->VerticesTop[0]] = BlendNormal0;
	out_pNormals[tileMappingCurrent->VerticesTop[1]] = BlendNormal1;
	out_pNormals[tileMappingCurrent->VerticesTop[2]] = BlendNormal2;
	out_pNormals[tileMappingCurrent->VerticesTop[3]] = BlendNormal3;
		//(BlendNormal3+BlendNormal1+BlendNormal2+BlendNormal0).Normalize();
	
	//GVector2 H = GVector2( in_TileTextureData->u[0], in_TileTextureData->v[0] ), 
	//		K = GVector2( in_TileTextureData->u[1], in_TileTextureData->v[1] ), 
	//		L = GVector2( in_TileTextureData->u[2], in_TileTextureData->v[2] ),
	//		M = GVector2( in_TileTextureData->u[3], in_TileTextureData->v[3] );
	//	//
	//	//The relevant input data to your problem are the texture coordinates. Tangent and Binormal are vectors locally parallel to the object's surface. 
	//	// And in the case of normal mapping they're describing the local orientation of the normal texture.
	//	// So what you have to do is calculate the direction (in the model's space) in which the texturing vectors point. 
	//	//
	//	// Say you have a triangle ABC, with texture coordinates HKL. This gives us vectors:
	//		GVector3 D, E;
	//		GVector2 F, G;
	//	//D = B-A
	//	//E = C-A
	//		D = v1-v0;
	//		E = v2-v0;
	//	//F = K-H
	//	//G = L-H
	//		F = K-H;
	//		G = L-H;
	//
	//	//Now we want to express D and E in terms of tangent space T, U, i.e.
	//	//D = F.s * T + F.t * U
	//	//E = G.s * T + G.t * U
	//
	//	//This is a system of linear equations with 6 unknowns and 6 equations, it can be written as:
	//	//| D.x D.y D.z |   | F.s F.t | | T.x T.y T.z |
	//	//|             | = |         | |             |
	//	//| E.x E.y E.z |   | G.s G.t | | U.x U.y U.z |

	//	//Inverting the FG matrix yields
	//	//| T.x T.y T.z |           1         |  G.t  -F.t | | D.x D.y D.z |
	//	//|             | = ----------------- |            | |             |
	//	//| U.x U.y U.z |   F.s G.t - F.t G.s | -G.s   F.s | | E.x E.y E.z |
	//	float myd = 1.0f/( (F.x*G.y) - (F.y*G.x) );
	//	// as we use another convention, we have to transpose the DE matrix and change the order of the matrix multiplication
	//	GMatrix2 FGMatrix;
	//	GMatrix2x3 TBMatrix;
	//	FGMatrix._11 = G.x; 	FGMatrix._12 = -G.y;
	//	FGMatrix._21 = -F.x; 	FGMatrix._22 = F.y;
	//	TBMatrix._11 = D.x;	TBMatrix._12 = E.x;			
	//	TBMatrix._21 = D.y;	TBMatrix._22 = E.y;	
	//	TBMatrix._31 = D.z; TBMatrix._32 = E.z;

	//	FGMatrix = FGMatrix*myd;
	//	TBMatrix = TBMatrix*FGMatrix;

	//	//out_pTangentBuffer[nVertexOffset] = GVector3( 

	//	//Together with the vertex normal, T and U form a local space basis called the tangent space, described by the matrix:
	//	//| T.x U.x N.x |
	//	//| T.y U.y N.y |
	//	//| T.z U.z N.z |
	//	//
	//	//transforming from tangent space into object space. To do lighting calculations one needs the inverse of this. With a little bit of exercise one finds:
	//	//T' = T - (N·T) N
	//	//U' = U - (N·U) N - (T'·U) T'
	//	//
	//	//normalizing the vectors T' and U', calling them tangent and binormal we obtain the matrix transforming from object into tangent space, where we do the lighting:
	//	//| T'.x T'.y T'.z |
	//	//| U'.x U'.y U'.z |
	//	//| N.x  N.y  N.z  |
	//	//
	//	// we store T' and U' them together with the vertex normal as a part of the model's geometry (as vertex attributes), so that we can use them in the shader for lighting calculations. 
	//	// I repeat: You don't determine tangent and binormal in the shader, you precompute them and store them as part of the model's geometry (just like normals).


	return 0;
}

error_t god::blendTerrainNormals( const GVector3* in_pVertices, GTILE_MAPPING_DATA const * in_lstTileMappingData,
				 GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nWidth, uint32_t nDepth, 
				 float fTileScale, GVector3 *out_pNormals )
{
	GPROFILE_FUNCTION();
	if( 0 == in_pVertices || 0 == in_lstTileGeometryData || 0 == in_lstTileGeometryData || 0 == nWidth || 0 == nDepth || 0 == fTileScale || 0 == out_pNormals )
	{
		error_printf("Invalid parameters!");
		return -1;
	}
#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
	uint32_t nChunkColumnTileCount		= nWidth; // 
	uint32_t nChunkRowTileCount			= nDepth; // default size of RO maps is 255 so divisible by both 5 and 15
	uint32_t qTileCount, 
		maxwidth	= min( nWidth-1, 64 ), 
		maxdepth	= min( nDepth-1, 64 );
	for( qTileCount=maxwidth; qTileCount>15; qTileCount-- )
		if( 0 == (nWidth % qTileCount) )
		{
			nChunkColumnTileCount= qTileCount;
			break;
		}
	for( qTileCount=maxdepth; qTileCount>15; qTileCount-- )
		if( 0 == (nDepth % qTileCount) )
		{
			nChunkRowTileCount = qTileCount;
			break;
		}
	uint32_t nColumnChunkCount		= (nWidth) / (nChunkColumnTileCount);
	uint32_t nRowChunkCount			= (nDepth) / (nChunkRowTileCount);

	uint32_t nTileOffsetX, nTileOffsetZ;

	uint32_t i=0, j=0, nTextureIndex=0, nChunkColumnIndex=0, rs=0, nGlobalTileIndex=0;

	error_t result=0;
	for( rs=0; rs<nRowChunkCount; rs++ )
	{
		for( nChunkColumnIndex=0; nChunkColumnIndex<nColumnChunkCount; nChunkColumnIndex++ )
		{
			nTileOffsetX = (nChunkColumnTileCount)*nChunkColumnIndex;
			nTileOffsetZ = (nChunkRowTileCount)*rs;
			if( 0 != (result = blendChunkNormals( in_pVertices, in_lstTileMappingData, in_lstTileGeometryData, nWidth, nDepth, nChunkRowTileCount, nChunkColumnTileCount, nTileOffsetX, nTileOffsetZ, fTileScale, out_pNormals )) )
			{
				error_printf("blendChunkNormals() returned 0x%X!", result);
				return -1;
			}	
		}
	}

	return 0;
}

error_t god::blendChunkNormals( const GVector3* in_pVertices, GTILE_MAPPING_DATA const * in_lstTileMappingData,
				 GTILE_GEOMETRY_DATA const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, 
				 uint32_t nChunkRowTileCount, uint32_t nChunkColumnTileCount, 
				 uint32_t nTileOffsetX, uint32_t nTileOffsetZ, 
				 float fTileScale, GVector3 *out_pNormals )
{
	GPROFILE_FUNCTION();
	uint32_t nLocalTileX, nLocalTileZ;
	uint32_t nGlobalTileIndex, nGlobalTileX, 
		nTotalTiles = nTotalWidth*nTotalDepth;
	GTILE_MAPPING_DATA pIdxCurrent, pIdxFront, pIdxRight, pIdxFrontRight;
	GTILE_GEOMETRY_DATA pTileCurrent, pTileFront=0, pTileRight=0, pTileFrontRight=0;

	error_t result=0;
	for( nLocalTileZ=0; nLocalTileZ<nChunkRowTileCount; nLocalTileZ++ )
	{
		for( nLocalTileX=0; nLocalTileX<nChunkColumnTileCount; nLocalTileX++ )
		{
			// As we build from local chunk coordinates, we need to calculate the global tile indices
			nGlobalTileIndex	= (nTileOffsetZ+nLocalTileZ)*nTotalWidth+nLocalTileX+nTileOffsetX;
			pIdxCurrent		= in_lstTileMappingData[nGlobalTileIndex];
			pTileCurrent	= in_lstTileGeometryData[nGlobalTileIndex];
			if( 0 == pIdxCurrent || -1 == pIdxCurrent->VerticesTop[0] )
				continue;

			if( nGlobalTileIndex < (nTotalTiles-1) )
			{
				pIdxFront	= in_lstTileMappingData[nGlobalTileIndex+1];
				pTileFront	= in_lstTileGeometryData[nGlobalTileIndex+1];
				if( 0 == pIdxFront || -1 == pIdxFront->VerticesTop[0] )
				{ 
					pTileFront = 0; 
					pIdxFront = 0;
				}

				if( nGlobalTileIndex < (nTotalTiles-nTotalWidth) )
				{
					pIdxRight		= in_lstTileMappingData[nGlobalTileIndex+nTotalWidth];
					pTileRight		= in_lstTileGeometryData[nGlobalTileIndex+nTotalWidth];
					if( 0 == pIdxRight || -1 == pIdxRight->VerticesTop[0] )
					{ 
						pTileRight = 0; 
						pIdxRight = 0;
					}

					if( nGlobalTileIndex < (nTotalTiles-nTotalWidth-1) )
					{
						pIdxFrontRight	= in_lstTileMappingData[nGlobalTileIndex+nTotalWidth+1];
						pTileFrontRight	= in_lstTileGeometryData[nGlobalTileIndex+nTotalWidth+1];
						if( 0 == pIdxFrontRight || -1 == pIdxFrontRight->VerticesTop[0] )
						{ 
							pTileFrontRight = 0; 
							pIdxFrontRight	= 0;
						}
					}
					else
					{
						pIdxFrontRight	= 0;
						pTileFrontRight	= 0;
					}
				}
				else
				{
					pIdxRight		= 0;
					pIdxFrontRight	= 0;
					pTileRight		= 0;
					pTileFrontRight	= 0;
				};
			}
			else
			{
				pIdxFront		= 0;
				pIdxRight		= 0;
				pIdxFrontRight	= 0;
				pTileFront		= 0;
				pTileRight		= 0;
				pTileFrontRight	= 0;
			};

			if( 0 != (result = blendTileNormals( pIdxCurrent, pTileCurrent, 
				0, 0, 0, 0, pIdxFront, 0, pIdxRight, pIdxFrontRight,
				0, 0, 0, 0, pTileFront, 0, pTileRight, pTileFrontRight,
				out_pNormals
				) ) )
			{
				error_printf("blendTileNormals() returned 0x%X!", result);
				return -1;
			}

	
		}
	};
	return result;
}

error_t god::blendTileNormals(	const GODS(TILE_MAPPING)	pTileMappingCurrent, 
								const GODS(TILE_GEOMETRY)	pTileGeometryCurrent, 
								const GODS(TILE_MAPPING)	pTileMappingBackLeft, 
								const GODS(TILE_MAPPING)	pTileMappingLeft, 
								const GODS(TILE_MAPPING)	pTileMappingFrontLeft, 
								const GODS(TILE_MAPPING)	pTileMappingBack, 
								const GODS(TILE_MAPPING)	pTileMappingFront, 
								const GODS(TILE_MAPPING)	pTileMappingBackRight, 
								const GODS(TILE_MAPPING)	pTileMappingRight, 
								const GODS(TILE_MAPPING)	pTileMappingFrontRight, 
								const GODS(TILE_GEOMETRY)	pTileGeometryBackLeft, 
								const GODS(TILE_GEOMETRY)	pTileGeometryLeft, 
								const GODS(TILE_GEOMETRY)	pTileGeometryFrontLeft, 
								const GODS(TILE_GEOMETRY)	pTileGeometryBack, 
								const GODS(TILE_GEOMETRY)	pTileGeometryFront, 
								const GODS(TILE_GEOMETRY)	pTileGeometryBackRight, 
								const GODS(TILE_GEOMETRY)	pTileGeometryRight, 
								const GODS(TILE_GEOMETRY)	pTileGeometryFrontRight,
								GVector3* out_pNormals )
{
	GPROFILE_FUNCTION();
	GVector3 preBlendFrontRight; 
	float fHeight[4] = {0};
	if( pTileMappingCurrent && -1 != pTileMappingCurrent->VerticesTop[0] )
	{
		preBlendFrontRight = out_pNormals[pTileMappingCurrent->VerticesTop[3]]*.5f;

//			preBlendFrontRight += out_pNormals[pTileMappingCurrent->VerticesTop[0]]*.25f;
		preBlendFrontRight += out_pNormals[pTileMappingCurrent->VerticesTop[1]]*.25f;
		preBlendFrontRight += out_pNormals[pTileMappingCurrent->VerticesTop[2]]*.25f;
		preBlendFrontRight.Normalize();
		for( uint32_t iCorner=0; iCorner<4; iCorner++ )
			fHeight[iCorner] = pTileGeometryCurrent->fHeight[iCorner];
	}
	else
	{
		preBlendFrontRight.x = preBlendFrontRight.y = preBlendFrontRight.z = 0;
		preBlendFrontRight.y = 1.0f;
	}

	if( pTileMappingFront 
		&& (fabs(fHeight[3] - pTileGeometryFront->fHeight[2]) < 0.0001f) )
	{
		preBlendFrontRight += out_pNormals[pTileMappingFront->VerticesTop[2]]*.25f;

		preBlendFrontRight += out_pNormals[pTileMappingFront->VerticesTop[0]]*.125f;;
//			preBlendFrontRight += out_pNormals[pTileMappingFront->VerticesTop[1]]*.125f;
		preBlendFrontRight += out_pNormals[pTileMappingFront->VerticesTop[3]]*.125f;
	}
	if( pTileMappingRight 
		&& (fabs(fHeight[3] - pTileGeometryRight->fHeight[1]) < 0.0001f) )
	{
		preBlendFrontRight += out_pNormals[pTileMappingRight->VerticesTop[1]]*.25f;

		preBlendFrontRight += out_pNormals[pTileMappingRight->VerticesTop[0]]*.125f;
//			preBlendFrontRight += out_pNormals[pTileMappingRight->VerticesTop[2]]*.25f;
		preBlendFrontRight += out_pNormals[pTileMappingRight->VerticesTop[3]]*.125f;
	}
	if( pTileMappingFrontRight 
		&& (fabs(fHeight[3] - pTileGeometryFrontRight->fHeight[0]) < 0.0001f) )
	{
		preBlendFrontRight += out_pNormals[pTileMappingFrontRight->VerticesTop[0]]*.25f;

		preBlendFrontRight += out_pNormals[pTileMappingFrontRight->VerticesTop[1]]*.125f;
		preBlendFrontRight += out_pNormals[pTileMappingFrontRight->VerticesTop[2]]*.125f;
//			preBlendFrontRight += out_pNormals[pTileMappingFrontRight->VerticesTop[3]]*.125f;
	}

	preBlendFrontRight.Normalize();

	// ---------------------- We average the current tile normal with the front-right tiles 
	if( pTileMappingCurrent )
	{
		out_pNormals[pTileMappingCurrent->VerticesTop[3]]			= (preBlendFrontRight);
		out_pNormals[pTileMappingCurrent->VerticesTop[3]].Normalize();
	}
	if( pTileMappingFront 
		&& (fabs(fHeight[3] - pTileGeometryFront->fHeight[2]) < 0.0001f) )
	{
		out_pNormals[pTileMappingFront->VerticesTop[2]]			= (preBlendFrontRight);
		out_pNormals[pTileMappingFront->VerticesTop[2]].Normalize();
	}
	if( pTileMappingRight 
		&& (fabs(fHeight[3] - pTileGeometryRight->fHeight[1]) < 0.0001f) )
	{
		out_pNormals[pTileMappingRight->VerticesTop[1]]			= (preBlendFrontRight);
		out_pNormals[pTileMappingRight->VerticesTop[1]].Normalize();
	}
	if( pTileMappingFrontRight 
		&& (fabs(fHeight[3] - pTileGeometryFrontRight->fHeight[0]) < 0.0001f) )
	{
		out_pNormals[pTileMappingFrontRight->VerticesTop[0]]	= (preBlendFrontRight);
		out_pNormals[pTileMappingFrontRight->VerticesTop[0]].Normalize();
	}

	return 0;
};	

error_t god::setTileCornerHeightBackLeft(	
									const GODS(TILE_MAPPING)	TileMappingCurrent, 
									const GODS(TILE_MAPPING)	TileMappingBackLeft, 
									const GODS(TILE_MAPPING)	TileMappingLeft, 
									const GODS(TILE_MAPPING)	TileMappingBack, 
									const float fHeight,
									GVector3* out_pVertices 
									)
{
	bool bWallBack	,	// = TileMappingBackLeft	? (TileMappingBackLeft->VerticesRight[3] != -1) : false, 
		bWallLeft	,	// = TileMappingBackLeft	? (TileMappingBackLeft->VerticesFront[2] != -1) : false, 
		bWallFront	,	// = TileMappingLeft		? (TileMappingLeft->VerticesRight[2] != -1) : false, 
		bWallRight	;	// = TileMappingBack		? (TileMappingBack->VerticesFront[3] != -1) : false;

	error_t result=0;
	if( (result=getTileCornerWallsBackLeft( TileMappingBackLeft, TileMappingLeft, TileMappingBack, &bWallBack, &bWallLeft, &bWallFront, &bWallRight )) )
	{
		error_printf("getTileCornerWallsFrontLeft() returned error. This is very unlikely due the nature of the call--it shouldn't happen!.");
		return -1;
	}
	if( bWallBack && bWallLeft && bWallFront && bWallRight )
	{
	}
	else if( bWallBack && bWallFront )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft && TileMappingBackLeft->VerticesRight[3] != -1 )
			out_pVertices[TileMappingBackLeft->VerticesRight[3]].y	= fHeight;
		if( TileMappingLeft && TileMappingLeft->VerticesRight[2]	!= -1 )
			out_pVertices[TileMappingLeft->VerticesRight[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesTop[1]		!= -1 )
			out_pVertices[TileMappingBack->VerticesTop[1]].y	= fHeight;
	}
	else if( bWallLeft && bWallRight )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft	&& TileMappingBackLeft->VerticesFront[2]	!= -1 )
			out_pVertices[TileMappingBackLeft->VerticesFront[2]].y	= fHeight;
		if( TileMappingLeft && TileMappingLeft->VerticesTop[2]	!= -1 )
			out_pVertices[TileMappingLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesFront[3]	!= -1 )
			out_pVertices[TileMappingBack->VerticesFront[3]].y	= fHeight;
	}
	else if( bWallBack && bWallLeft )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft	)
		{
			if( TileMappingBackLeft->VerticesFront[2]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesFront[2]].y	= fHeight;
			if( TileMappingBackLeft->VerticesRight[3]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesRight[3]].y	= fHeight;
		}
		if( TileMappingLeft && TileMappingLeft->VerticesTop[2] != -1 )
			out_pVertices[TileMappingLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesTop[1] != -1 )
			out_pVertices[TileMappingBack->VerticesTop[1]].y	= fHeight;
	}
	else if( bWallBack && bWallRight )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft )
		{
			if( TileMappingBackLeft->VerticesTop[3]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesTop[3]].y	= fHeight;
			if( TileMappingBackLeft->VerticesRight[1]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesRight[1]].y	= fHeight;
		}
		if( TileMappingLeft && TileMappingLeft->VerticesTop[2]	!= -1 )
			out_pVertices[TileMappingLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesFront[3]	!= -1 )
			out_pVertices[TileMappingBack->VerticesFront[3]].y	= fHeight;
	}
	else if( bWallLeft && bWallFront )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft	)
		{
			if( TileMappingBackLeft->VerticesTop[3]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesTop[3]].y	= fHeight;
			if( TileMappingBackLeft->VerticesFront[0]	!= -1 )
				out_pVertices[TileMappingBackLeft->VerticesFront[0]].y	= fHeight;
		}
		if( TileMappingLeft && TileMappingLeft->VerticesRight[2]	!= -1 )
			out_pVertices[TileMappingLeft->VerticesRight[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesTop[1]	!= -1 )
			out_pVertices[TileMappingBack->VerticesTop[1]].y	= fHeight;
	}
	else if( bWallFront && bWallRight )
	{ // BackLeft should not be modified here
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingLeft && TileMappingLeft->VerticesRight[2]		!= -1 )
			out_pVertices[TileMappingLeft->VerticesRight[2]].y		= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesFront[3]		!= -1 )
			out_pVertices[TileMappingBack->VerticesFront[3]].y		= fHeight;
	}
	else
	{	// No walls
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[0] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackLeft && TileMappingBackLeft->VerticesTop[3] != -1 )
			out_pVertices[TileMappingBackLeft->VerticesTop[3]].y		= fHeight;
		if( TileMappingLeft && TileMappingLeft->VerticesTop[2] != -1 )
			out_pVertices[TileMappingLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesTop[1] != -1 )
			out_pVertices[TileMappingBack->VerticesTop[1]].y		= fHeight;
	}

	return 0;
}

error_t god::setTileCornerHeightFrontLeft(	
									const GODS(TILE_MAPPING)	TileMappingCurrent, 
									const GODS(TILE_MAPPING)	TileMappingLeft, 
									const GODS(TILE_MAPPING)	TileMappingFrontLeft, 
									const GODS(TILE_MAPPING)	TileMappingFront, 
									const float fHeight,
									GVector3* out_pVertices 
									)
{
	bool bWallBack	, //= TileMappingLeft		? (TileMappingLeft->VerticesRight[3] != -1) : false, 
		bWallLeft	, //= TileMappingLeft		? (TileMappingLeft->VerticesFront[2] != -1) : false, 
		bWallFront	, //= TileMappingFrontLeft	? (TileMappingFrontLeft->VerticesRight[2] != -1) : false, 
		bWallRight	; //= TileMappingCurrent	? (TileMappingCurrent->VerticesFront[1] != -1) : false;
	
	error_t result=0;
	if( (result=getTileCornerWallsFrontLeft( TileMappingCurrent, TileMappingLeft, TileMappingFrontLeft, &bWallBack, &bWallLeft, &bWallFront, &bWallRight )) )
	{
		error_printf("getTileCornerWallsFrontLeft() returned error. This is very unlikely due the nature of the call, it shouldn't happen!.");
		return -1;
	}

	if( bWallBack && bWallLeft && bWallFront && bWallRight )
	{
	}
	else if( bWallBack && bWallFront )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[1] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[1]].y		= fHeight;
		if( TileMappingLeft	&& TileMappingLeft->VerticesRight[3] != -1 )
			out_pVertices[TileMappingLeft->VerticesRight[3]].y		= fHeight;
		if( TileMappingFrontLeft && TileMappingFrontLeft->VerticesRight[2] != -1 )
			out_pVertices[TileMappingFrontLeft->VerticesRight[2]].y	= fHeight;
		if( TileMappingFront && TileMappingFront->VerticesTop[0] != -1 )
			out_pVertices[TileMappingFront->VerticesTop[0]].y		= fHeight;
	}
	else if( bWallLeft && bWallRight )
	{	// Neither FrontLeft nor Front should be modified for this value.
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[1]].y	= fHeight;
			if( TileMappingCurrent->VerticesFront[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[1]].y	= fHeight;
		}
		if( TileMappingLeft )
		{
			if( TileMappingLeft->VerticesTop[3]				!= -1 )
				out_pVertices[TileMappingLeft->VerticesTop[3]].y		= fHeight;
			if( TileMappingLeft->VerticesFront[0]				!= -1 )
				out_pVertices[TileMappingLeft->VerticesFront[0]].y		= fHeight;
		}
	}
	else if( bWallBack && bWallLeft )
	{
		if( TileMappingCurrent->VerticesTop[1] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[1]].y	= fHeight;
		if( TileMappingLeft )
		{
			if( TileMappingLeft->VerticesFront[2] != -1 )
				out_pVertices[TileMappingLeft->VerticesFront[2]].y	= fHeight;
			if( TileMappingLeft->VerticesRight[3] != -1 )
				out_pVertices[TileMappingLeft->VerticesRight[3]].y	= fHeight;
		}
		if( TileMappingFrontLeft && TileMappingFrontLeft->VerticesTop[2] != -1 )
			out_pVertices[TileMappingFrontLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingFront && TileMappingFront->VerticesTop[0] != -1 )
			out_pVertices[TileMappingFront->VerticesTop[0]].y		= fHeight;
	}
	else if( bWallBack && bWallRight )
	{	// FrontLeft && Front must not be modified here
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[1]].y		= fHeight;
			if( TileMappingCurrent->VerticesFront[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[1]].y		= fHeight;
		}
		if( TileMappingLeft	&& TileMappingLeft->VerticesRight[3] != -1 )
			out_pVertices[TileMappingLeft->VerticesRight[3]].y		= fHeight;
	}
	else if( bWallLeft && bWallFront )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[1]		!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[1]].y		= fHeight;
		if( TileMappingLeft )
		{
			if( TileMappingLeft->VerticesTop[3]				!= -1 )
				out_pVertices[TileMappingLeft->VerticesTop[3]].y		= fHeight;
			if( TileMappingLeft->VerticesFront[0]				!= -1 )
				out_pVertices[TileMappingLeft->VerticesFront[0]].y		= fHeight;
		}
		if( TileMappingFrontLeft && TileMappingFrontLeft->VerticesRight[2]	!= -1 )
			out_pVertices[TileMappingFrontLeft->VerticesRight[2]].y	= fHeight;
		if( TileMappingFront && TileMappingFront->VerticesTop[0]			!= -1 )
			out_pVertices[TileMappingFront->VerticesTop[0]].y		= fHeight;
	}
	else if( bWallFront && bWallRight )
	{	// Front is not modified for this case
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[1]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[1]].y		= fHeight;
			if( TileMappingCurrent->VerticesFront[1]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[1]].y	= fHeight;
		}
		if( TileMappingLeft->VerticesTop[3]	!= -1 )
			out_pVertices[TileMappingLeft->VerticesTop[3]].y		= fHeight;
		if( TileMappingFrontLeft )
		{
			if( TileMappingFrontLeft->VerticesTop[2] != -1 )
				out_pVertices[TileMappingFrontLeft->VerticesTop[2]].y	= fHeight;
			if( TileMappingFrontLeft->VerticesRight[0] != -1 )
				out_pVertices[TileMappingFrontLeft->VerticesRight[0]].y	= fHeight;
		}
	}
	else
	{	// No walls
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[1]		!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[1]].y		= fHeight;
		if( TileMappingLeft && TileMappingLeft->VerticesTop[3]				!= -1 )
			out_pVertices[TileMappingLeft->VerticesTop[3]].y		= fHeight;
		if( TileMappingFrontLeft && TileMappingFrontLeft->VerticesTop[2]	!= -1 )
			out_pVertices[TileMappingFrontLeft->VerticesTop[2]].y	= fHeight;
		if( TileMappingFront && TileMappingFront->VerticesTop[0]			!= -1 )
			out_pVertices[TileMappingFront->VerticesTop[0]].y		= fHeight;
	}

	return 0;
}

error_t god::setTileCornerHeightBackRight(	
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingBack, 
								const GODS(TILE_MAPPING)	TileMappingBackRight, 
								const GODS(TILE_MAPPING)	TileMappingRight, 
								const float fHeight,
								GVector3* out_pVertices 
								)
{	
	bool bWallBack	,	// = TileMappingBack		? (TileMappingBack->VerticesRight[3] != -1) : false, 
		bWallLeft	,	// = TileMappingBack		? (TileMappingBack->VerticesFront[2] != -1) : false, 
		bWallFront	,	// = TileMappingCurrent	? (TileMappingCurrent->VerticesRight[2] != -1) : false, 
		bWallRight	;	// = TileMappingBackRight	? (TileMappingBackRight->VerticesFront[3] != -1) : false;

	error_t result=0;
	if( (result=getTileCornerWallsBackRight( TileMappingCurrent, TileMappingBack, TileMappingBackRight, &bWallBack, &bWallLeft, &bWallFront, &bWallRight )) )
	{
		error_printf("getTileCornerWallsBackRight() returned error. This is very unlikely due the nature of the call, it shouldn't happen!");
		return -1;
	}

	if( bWallBack && bWallLeft && bWallFront && bWallRight )
	{
	}
	else if( bWallLeft && bWallRight )
	{
		if( TileMappingCurrent		&& TileMappingCurrent->VerticesTop[2]		!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
		if( TileMappingBack			&& TileMappingBack->VerticesFront[2]		!= -1 )
			out_pVertices[TileMappingBack->VerticesFront[2]].y		= fHeight;
		if( TileMappingBackRight	&& TileMappingBackRight->VerticesFront[3]	!= -1 )
			out_pVertices[TileMappingBackRight->VerticesFront[3]].y		= fHeight;
		if( TileMappingRight		&& TileMappingRight->VerticesTop[0]			!= -1 )
			out_pVertices[TileMappingRight->VerticesTop[0]].y		= fHeight;
	}	
	else if( bWallBack && bWallFront )
	{
		// Both TileMappingBackRight and TileMappingRight must be kept intact for this case
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[2]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
			if( TileMappingCurrent->VerticesRight[0]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[0]].y		= fHeight;
		}
		if( TileMappingBack )
		{
			if( TileMappingBack->VerticesTop[3]	!= -1 )
				out_pVertices[TileMappingBack->VerticesTop[3]].y		= fHeight;
			if( TileMappingBack->VerticesRight[1]	!= -1 )
				out_pVertices[TileMappingBack->VerticesRight[1]].y		= fHeight;
		}
	}
	else if( bWallBack && bWallLeft )
	{
		if( TileMappingCurrent		&& TileMappingCurrent->VerticesTop[2]	!= -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
		if( TileMappingBack )
		{
			if( TileMappingBack->VerticesFront[2]	!= -1 )
				out_pVertices[TileMappingBack->VerticesFront[2]].y		= fHeight;
			if( TileMappingBack->VerticesRight[3]	!= -1 )
				out_pVertices[TileMappingBack->VerticesRight[3]].y		= fHeight;
		}
		if( TileMappingBackRight	&& TileMappingBackRight->VerticesTop[1]	!= -1 )
			out_pVertices[TileMappingBackRight->VerticesTop[1]].y	= fHeight;
		if( TileMappingRight		&& TileMappingRight->VerticesTop[0]		!= -1 )
			out_pVertices[TileMappingRight->VerticesTop[0]].y		= fHeight;
	}
	else if( bWallBack && bWallRight )
	{
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[2]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
		if( TileMappingBack )
		{
			if( TileMappingBack->VerticesTop[3]	!= -1 )
				out_pVertices[TileMappingBack->VerticesTop[3]].y		= fHeight;
			if( TileMappingBack->VerticesRight[1]	!= -1 )
				out_pVertices[TileMappingBack->VerticesRight[1]].y		= fHeight;
		}
		if( TileMappingRight && TileMappingRight->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingRight->VerticesTop[0]].y		= fHeight;
		if( TileMappingBackRight && TileMappingBackRight->VerticesFront[3]	!= -1 )
			out_pVertices[TileMappingBackRight->VerticesFront[3]].y		= fHeight;
	}
	else if( bWallLeft && bWallFront )
	{	// Both TileBackRight and TileRight are kept intact for this case
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[2]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
			if( TileMappingCurrent->VerticesRight[0]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[0]].y	= fHeight;
		}
		if( TileMappingBack && TileMappingBack->VerticesFront[2]	!= -1 )
			out_pVertices[TileMappingBack->VerticesFront[2]].y		= fHeight;

	}

	else if( bWallFront && bWallRight )
	{
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[2]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
			if( TileMappingCurrent->VerticesRight[0]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[0]].y	= fHeight;
		}
		if( TileMappingBack && TileMappingBack->VerticesTop[3] != -1 )
			out_pVertices[TileMappingBack->VerticesTop[3]].y		= fHeight;
		if( TileMappingBackRight )
		{
			if( TileMappingBackRight->VerticesTop[1] != -1 )
				out_pVertices[TileMappingBackRight->VerticesTop[1]].y	= fHeight;
			if( TileMappingBackRight->VerticesFront[1] != -1 )
				out_pVertices[TileMappingBackRight->VerticesFront[1]].y	= fHeight;
		}
	}
	else
	{	// No walls
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[2] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[2]].y		= fHeight;
		if( TileMappingBack && TileMappingBack->VerticesTop[3] != -1 )
			out_pVertices[TileMappingBack->VerticesTop[3]].y		= fHeight;
		if( TileMappingBackRight && TileMappingBackRight->VerticesTop[1] != -1 )
			out_pVertices[TileMappingBackRight->VerticesTop[1]].y	= fHeight;
		if( TileMappingRight && TileMappingRight->VerticesTop[0] != -1 )
			out_pVertices[TileMappingRight->VerticesTop[0]].y		= fHeight;
	}

	return 0;
}


error_t god::setTileCornerHeightFrontRight(	
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingFront, 
								const GODS(TILE_MAPPING)	TileMappingRight, 
								const GODS(TILE_MAPPING)	TileMappingFrontRight, 
								const float fHeight,
								GVector3* out_pVertices 
								)
{	
	bool bWallBack, bWallLeft, bWallFront, bWallRight;

	error_t result=0;
	if( (result=getTileCornerWallsFrontRight( TileMappingCurrent, TileMappingFront, TileMappingRight, &bWallBack, &bWallLeft, &bWallFront, &bWallRight )) )
	{
		error_printf("getTileCornerWallsFrontRight() returned error. This is very unlikely due the nature of the call, it shouldn't happen!.");
		return -1;
	}

	if( bWallBack && bWallLeft && bWallFront && bWallRight )
	{
	}
	else if( bWallBack && bWallFront )
	{	// Right and FrontRight are kept intact for this one
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[3] != -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
			if( TileMappingCurrent->VerticesRight[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[1]].y	= fHeight;
		}
		if( TileMappingFront )
		{
			if( TileMappingFront->VerticesTop[2]	!= -1 )
				out_pVertices[TileMappingFront->VerticesTop[2]].y	= fHeight;
			if( TileMappingFront->VerticesRight[0]	!= -1 )
				out_pVertices[TileMappingFront->VerticesRight[0]].y	= fHeight;
		}
	}
	else if( bWallLeft && bWallRight )
	{	// TileFront and TileFrontRight should not be modified here.
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[3]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
			if( TileMappingCurrent->VerticesFront[0]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[0]].y	= fHeight;
		}
		if( TileMappingRight )
		{
			if( TileMappingRight->VerticesTop[1]	!= -1 )
				out_pVertices[TileMappingRight->VerticesTop[1]].y	= fHeight;
			if( TileMappingRight->VerticesFront[1]	!= -1 )
				out_pVertices[TileMappingRight->VerticesFront[1]].y	= fHeight;
		}
	}
	else if( bWallBack && bWallLeft )
	{	// only TileCurrent must be modified for this one
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[3] != -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[3]].y	= fHeight;
			if( TileMappingCurrent->VerticesFront[0] != -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[0]].y	= fHeight;
			if( TileMappingCurrent->VerticesRight[1] != -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[1]].y	= fHeight;
		}
	}
	else if( bWallBack && bWallRight )
	{	
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[3]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
			if( TileMappingCurrent->VerticesRight[1]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesRight[1]].y	= fHeight;
		}
		if( TileMappingFront		&& TileMappingFront->VerticesTop[2]		!= -1 )
			out_pVertices[TileMappingFront->VerticesTop[2]].y		= fHeight;
		if( TileMappingRight		&& TileMappingRight->VerticesFront[3]	!= -1 )
			out_pVertices[TileMappingRight->VerticesFront[3]].y		= fHeight;
		if( TileMappingFrontRight	&& TileMappingFrontRight->VerticesTop[0]	!= -1 )
			out_pVertices[TileMappingFrontRight->VerticesTop[0]].y	= fHeight;
	}
	else if( bWallLeft && bWallFront )
	{
		if( TileMappingCurrent )
		{
			if( TileMappingCurrent->VerticesTop[3]		!= -1 )
				out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
			if( TileMappingCurrent->VerticesFront[0]	!= -1 )
				out_pVertices[TileMappingCurrent->VerticesFront[0]].y	= fHeight;
		}
		if( TileMappingFront && TileMappingFront->VerticesRight[2]	!= -1 )
			out_pVertices[TileMappingFront->VerticesRight[2]].y		= fHeight;
		if( TileMappingRight && TileMappingRight->VerticesTop[1] != -1 )
			out_pVertices[TileMappingRight->VerticesTop[1]].y	= fHeight;
		if( TileMappingFrontRight && TileMappingFrontRight->VerticesTop[0] != -1 )
			out_pVertices[TileMappingFrontRight->VerticesTop[0]].y		= fHeight;
	}
	else if( bWallFront && bWallRight )
	{	// TileFrontRight must not be modified in this one
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[3] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
		if( TileMappingFront )
		{
			if( TileMappingFront->VerticesTop[2] != -1 )
				out_pVertices[TileMappingFront->VerticesTop[2]].y		= fHeight;
			if( TileMappingFront->VerticesRight[0] != -1 )
				out_pVertices[TileMappingFront->VerticesRight[0]].y		= fHeight;
		}
		if( TileMappingRight )
		{
			if( TileMappingRight->VerticesTop[1]	!= -1 )
				out_pVertices[TileMappingRight->VerticesTop[1]].y	= fHeight;
			if( TileMappingRight->VerticesFront[1]	!= -1 )
				out_pVertices[TileMappingRight->VerticesFront[1]].y	= fHeight;
		}
	}
	else
	{	// No walls
		if( TileMappingCurrent && TileMappingCurrent->VerticesTop[3] != -1 )
			out_pVertices[TileMappingCurrent->VerticesTop[3]].y		= fHeight;
		if( TileMappingFront && TileMappingFront->VerticesTop[2] != -1 )
			out_pVertices[TileMappingFront->VerticesTop[2]].y		= fHeight;
		if( TileMappingRight && TileMappingRight->VerticesTop[1] != -1 )
			out_pVertices[TileMappingRight->VerticesTop[1]].y	= fHeight;
		if( TileMappingFrontRight && TileMappingFrontRight->VerticesTop[0] != -1 )
			out_pVertices[TileMappingFrontRight->VerticesTop[0]].y		= fHeight;
	}

	return 0;
}


error_t god::setTileCornerHeights(	GTILE_BORDER Boundaries, 
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingBackLeft, 
								const GODS(TILE_MAPPING)	TileMappingLeft, 
								const GODS(TILE_MAPPING)	TileMappingFrontLeft, 
								const GODS(TILE_MAPPING)	TileMappingBack, 
								const GODS(TILE_MAPPING)	TileMappingFront, 
								const GODS(TILE_MAPPING)	TileMappingBackRight, 
								const GODS(TILE_MAPPING)	TileMappingRight, 
								const GODS(TILE_MAPPING)	TileMappingFrontRight, 
								const float fHeights[4],
								GVector3* out_pVertices )
{	
	//  3	2		 3	2
	//	1	0 |---/| 1	0	Tile vertex order
	//		  |  / |
	//		  | /  |
	//	3	2 |/___| 3	2
	//	1	0		 1  0
	error_t result=0;

	if( 0 == (Boundaries & (GTILEBORDER_BACK | GTILEBORDER_FRONT)) )
		Boundaries |= (GTILEBORDER_BACK | GTILEBORDER_FRONT);
	if( 0 == (Boundaries & (GTILEBORDER_LEFT | GTILEBORDER_RIGHT)) )
		Boundaries |= (GTILEBORDER_LEFT | GTILEBORDER_RIGHT);

	if( Boundaries & GTILEBORDER_LEFT )
	{
		if( Boundaries & GTILEBORDER_BACK )
		{
			if( (result = setTileCornerHeightBackLeft( TileMappingCurrent, TileMappingBackLeft, TileMappingLeft, TileMappingBack, fHeights[0], out_pVertices )) )
			{
				error_printf("setTileCornerHeightBackLeft() returned 0x%X!", result);
			};
		}
		if( Boundaries & GTILEBORDER_FRONT )
		{
			if( (result = setTileCornerHeightFrontLeft( TileMappingCurrent, TileMappingLeft, TileMappingFrontLeft, TileMappingFront, fHeights[1], out_pVertices )) )
			{
				error_printf("setTileCornerHeightFrontLeft() returned 0x%X!", result);
			};
		}
	}
	if( Boundaries & GTILEBORDER_RIGHT )
	{
		if( Boundaries & GTILEBORDER_BACK )
		{
			if( (result = setTileCornerHeightBackRight( TileMappingCurrent, TileMappingBack, TileMappingBackRight, TileMappingRight, fHeights[2], out_pVertices )) )
			{
				error_printf("setTileCornerHeightBackRight() returned 0x%X!", result);
			};
		}
		if( Boundaries & GTILEBORDER_FRONT )
		{
			if( (result = setTileCornerHeightFrontRight( TileMappingCurrent, TileMappingFront, TileMappingRight, TileMappingFrontRight, fHeights[3], out_pVertices )) )
			{
				error_printf("setTileCornerHeightFrontRight() returned 0x%X!", result);
			};
		}
	}

	return 0;
};	


error_t god::getTileCornerWallsBackLeft(	
								const GODS(TILE_MAPPING)	TileMappingBackLeft, 
								const GODS(TILE_MAPPING)	TileMappingLeft, 
								const GODS(TILE_MAPPING)	TileMappingBack, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
								)
{
	return getTileCornerWalls(	TileMappingBackLeft	,
								TileMappingLeft		, 
								TileMappingBack		,
								bWallBack, bWallLeft, bWallFront, bWallRight );
};

error_t god::getTileCornerWallsFrontLeft(	
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingLeft, 
								const GODS(TILE_MAPPING)	TileMappingFrontLeft, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
								)
{
	return getTileCornerWalls( 
		TileMappingLeft, 
		TileMappingFrontLeft, 
		TileMappingCurrent, 
		bWallBack, bWallLeft, bWallFront, bWallRight
		);
};

error_t god::getTileCornerWallsBackRight(
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingBack, 
								const GODS(TILE_MAPPING)	TileMappingBackRight, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
								)
{
	return getTileCornerWalls(	TileMappingBack, 
								TileMappingCurrent, 
								TileMappingBackRight, 
								bWallBack, bWallLeft, bWallFront, bWallRight );
};

error_t god::getTileCornerWallsFrontRight(	
								const GODS(TILE_MAPPING)	TileMappingCurrent, 
								const GODS(TILE_MAPPING)	TileMappingFront, 
								const GODS(TILE_MAPPING)	TileMappingRight, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
								)
{
	return getTileCornerWalls(	TileMappingCurrent, 
								TileMappingFront, 
								TileMappingRight, 
								bWallBack, bWallLeft, bWallFront, bWallRight                                                                                                                                                                                );
};

error_t god::getTileCornerWalls(	
								const GODS(TILE_MAPPING)	TileMapping0, 
								const GODS(TILE_MAPPING)	TileMapping1, 
								const GODS(TILE_MAPPING)	TileMapping2, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
								)
{
	if( bWallBack	)	*bWallBack	= TileMapping0	? (TileMapping0->VerticesRight[3]	!= -1) : false;
	if( bWallLeft	)	*bWallLeft	= TileMapping0	? (TileMapping0->VerticesFront[2]	!= -1) : false;
	if( bWallFront	)	*bWallFront	= TileMapping1	? (TileMapping1->VerticesRight[2]	!= -1) : false; 
	if( bWallRight	)	*bWallRight	= TileMapping2	? (TileMapping2->VerticesFront[3]	!= -1) : false;
	return 0;
};

error_t god::buildTileHeightImage( const GTILE_GEOMETRY_DATA* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint8_t* out_lstHeights )
{
	if( 0 == in_lstGeometryData || 0 == out_lstHeights )
	{
		error_printf("Invalid parameters!");
		return -1;
	}
	uint32_t iLinearTileIndex;
	GODS(TILE_GEOMETRY) CurrentTile=0;
	float32_t fTempColor;
	uint32_t bTempColor;

	float fScalar = fabs( UCHAR_MAX/(fMax-fMin) ),
		fOffsetByCorners = fabs(fMin*4);
	for( uint32_t z=0; z<nDepth; z++ )
	{
		for( uint32_t x=0; x<nWidth; x++ )
		{
			iLinearTileIndex = z*nWidth+x;
			CurrentTile = in_lstGeometryData[iLinearTileIndex];
			if( 0 == CurrentTile )
			{
				out_lstHeights[iLinearTileIndex] = 0;
				continue;
			}
			fTempColor	= (fOffsetByCorners + ( CurrentTile->fHeight[0]+CurrentTile->fHeight[1]+CurrentTile->fHeight[2]+CurrentTile->fHeight[3] )) / 4; // average height 
			bTempColor	= (uint32_t)(fTempColor*fScalar);
			out_lstHeights[iLinearTileIndex] = bTempColor;
		}
	}

	return 0;
}

error_t god::buildTileHeightImage( const GTILE_GEOMETRY_DATA* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint16_t* out_lstHeights )
{
	if( 0 == in_lstGeometryData || 0 == out_lstHeights )
	{
		error_printf("Invalid parameters!");
		return -1;
	}
	uint32_t iLinearTileIndex;
	GODS(TILE_GEOMETRY) CurrentTile;
	float32_t fTempColor;
	uint32_t bTempColor;

	float fScalar = fabs( 0xFFFFUL/(fMax-fMin) ),
		fOffsetByCorners = fabs(fMin*4);
	for( uint32_t z=0; z<nDepth; z++ )
	{
		for( uint32_t x=0; x<nWidth; x++ )
		{
			iLinearTileIndex = z*nWidth+x;
			CurrentTile = in_lstGeometryData[iLinearTileIndex];
			if( 0 == CurrentTile )
			{
				out_lstHeights[iLinearTileIndex] = 0;
				continue;
			}
			fTempColor	= (fOffsetByCorners + ( CurrentTile->fHeight[0]+CurrentTile->fHeight[1]+CurrentTile->fHeight[2]+CurrentTile->fHeight[3] )) / 4; // average height 
			bTempColor	= (uint32_t)(fTempColor*fScalar);
			out_lstHeights[iLinearTileIndex] = bTempColor;
		}
	}

	return 0;
}

error_t god::buildTileHeightImage( const GTILE_GEOMETRY_DATA* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint32_t* out_lstHeights )
{
	if( 0 == in_lstGeometryData || 0 == out_lstHeights )
	{
		error_printf("Invalid parameters!");
		return -1;
	}
	uint32_t iLinearTileIndex;
	GTILE_GEOMETRY_DATA CurrentTile;
	float32_t fTempColor;
	uint32_t bTempColor;

	float fScalar = fabs( 0xFFFFFFFFUL/(fMax-fMin) ),
		fOffsetByCorners = fabs(fMin*4);
	for( uint32_t z=0; z<nDepth; z++ )
	{
		for( uint32_t x=0; x<nWidth; x++ )
		{
			iLinearTileIndex = z*nWidth+x;
			CurrentTile = in_lstGeometryData[iLinearTileIndex];
			if( 0 == CurrentTile )
			{
				out_lstHeights[iLinearTileIndex] = 0;
				continue;
			}
			fTempColor	= (fOffsetByCorners + ( CurrentTile->fHeight[0]+CurrentTile->fHeight[1]+CurrentTile->fHeight[2]+CurrentTile->fHeight[3] )) / 4; // average height 
			bTempColor	= (uint32_t)(fTempColor*fScalar);
			out_lstHeights[iLinearTileIndex] = bTempColor;
		}
	}

	return 0;
}

error_t god::setupTileTemplates_old( GTILE_GEOMETRY_DATA* out_lstTileTemplate, GTILE_TEXTURE_DATA* out_lstTileTextureTemplates )
{	// 
	GENIUSS(TILE_GEOMETRY) newTileGeometry(onCreateTileGeometryDefault,0,0), oldTileGeometry;
	// Tile 0 (top)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	oldTileGeometry = out_lstTileTemplate[0];
	out_lstTileTemplate[0]	= newTileGeometry.acquire();

	// Tile 1 (front)
	newTileGeometry.create();
	newTileGeometry->FrontTextureDataIndex	= 1;
	oldTileGeometry = out_lstTileTemplate[1];
	out_lstTileTemplate[1]	= newTileGeometry.acquire();

	// Tile 2 (right)
	newTileGeometry.create();
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[2];
	out_lstTileTemplate[2]	= newTileGeometry.acquire();

	// Tile 3 (top, front)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->FrontTextureDataIndex	= 1;
	oldTileGeometry = out_lstTileTemplate[3];
	out_lstTileTemplate[3]	= newTileGeometry.acquire();

	// Tile 4 (top, right)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[4];
	out_lstTileTemplate[4]	= newTileGeometry.acquire();

	// Tile 5 (front, right)
	newTileGeometry.create();
	newTileGeometry->FrontTextureDataIndex	= 1;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[5];
	out_lstTileTemplate[5]	= newTileGeometry.acquire();

	// Tile 6 (top, front, right)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->FrontTextureDataIndex	= 1;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[6];
	out_lstTileTemplate[6]	= newTileGeometry.acquire();

	GOPTR(TILE_TEXTURE) newTileTexture, oldTileTexture;
	newTileTexture->nLightmapIndex	= -1;
	newTileTexture->nTextureIndex	= 0;
	newTileTexture->u[0]	= 0.0f; newTileTexture->u[1]	= 1.0f; newTileTexture->u[2]	= 0.0f; newTileTexture->u[3]	= 1.0f;
	newTileTexture->v[0]	= 0.0f; newTileTexture->v[1]	= 0.0f; newTileTexture->v[2]	= 1.0;	newTileTexture->v[3]	= 1.0f;
	newTileTexture->dwColor	= 0;
	oldTileTexture = out_lstTileTextureTemplates[0];
	out_lstTileTextureTemplates[0] = newTileTexture.acquire();

	newTileTexture.clone( newTileTexture );
	newTileTexture->nTextureIndex	= 1;
	oldTileTexture = out_lstTileTextureTemplates[1];
	out_lstTileTextureTemplates[1] = newTileTexture.acquire();

	newTileTexture.clone( newTileTexture );
	newTileTexture->nTextureIndex	= 2;
	oldTileTexture = out_lstTileTextureTemplates[2];
	out_lstTileTextureTemplates[2] = newTileTexture.acquire();

	return 0;
};

error_t god::setupTileTemplates_old( GOPTR(TILE_GEOMETRY)* out_lstTileGeometryTemplates, GOPTR(TILE_TEXTURE)* out_lstTileTextureTemplates )
{	// WARNING!! Unsafe pointer handling in course!
	if( 0 == out_lstTileGeometryTemplates && 0 == out_lstTileTextureTemplates )
	{
		error_printf("Output address is NULL!");
		return -1;
	}
	glist<GODS(TILE_GEOMETRY)> TempGeo( 7 );
	glist<GODS(TILE_TEXTURE)> TempTex( 7 );
	setupTileTemplates_old( TempGeo.get_pointer(), TempTex.get_pointer() );
	if( out_lstTileGeometryTemplates && out_lstTileTextureTemplates)
	{
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
		{
			out_lstTileGeometryTemplates[iTileTpl]	= gacquireData( TempGeo[iTileTpl] );
			out_lstTileTextureTemplates[iTileTpl]	= gacquireData( TempTex[iTileTpl] );
		};
	}
	else if( out_lstTileGeometryTemplates )
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
			out_lstTileGeometryTemplates[iTileTpl]	= gacquireData( TempGeo[iTileTpl] );
	else if( out_lstTileTextureTemplates )
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
			out_lstTileTextureTemplates [iTileTpl]	= gacquireData( TempTex[iTileTpl] );

	return 0;
};



error_t god::setupTileTemplates( GTILE_GEOMETRY_DATA* out_lstTileTemplate, GTILE_TEXTURE_DATA* out_lstTileTextureTemplates )
{	// 
	GENIUSS(TILE_GEOMETRY) newTileGeometry(onCreateTileGeometryDefault,0,0), oldTileGeometry;
	// Tile 0 (top)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	oldTileGeometry = out_lstTileTemplate[0];
	out_lstTileTemplate[0]	= newTileGeometry.acquire();

	// Tile 1 (front)
	newTileGeometry.create();
	newTileGeometry->FrontTextureDataIndex	= 1;
	oldTileGeometry = out_lstTileTemplate[1];
	out_lstTileTemplate[1]	= newTileGeometry.acquire();

	// Tile 2 (top, front)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->FrontTextureDataIndex	= 1;
	oldTileGeometry = out_lstTileTemplate[2];
	out_lstTileTemplate[2]	= newTileGeometry.acquire();

	// Tile 3 (right)
	newTileGeometry.create();
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[3];
	out_lstTileTemplate[3]	= newTileGeometry.acquire();

	// Tile 4 (top, right)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[4];
	out_lstTileTemplate[4]	= newTileGeometry.acquire();

	// Tile 5 (front, right)
	newTileGeometry.create();
	newTileGeometry->FrontTextureDataIndex	= 1;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[5];
	out_lstTileTemplate[5]	= newTileGeometry.acquire();

	// Tile 6 (top, front, right)
	newTileGeometry.create();
	newTileGeometry->TopTextureDataIndex	= 0;
	newTileGeometry->FrontTextureDataIndex	= 1;
	newTileGeometry->RightTextureDataIndex	= 2;
	oldTileGeometry = out_lstTileTemplate[6];
	out_lstTileTemplate[6]	= newTileGeometry.acquire();

	GOPTR(TILE_TEXTURE) newTileTexture, oldTileTexture;
	newTileTexture->nLightmapIndex	= -1;
	newTileTexture->nTextureIndex	= 0;
	newTileTexture->u[0]	= 0.0f; newTileTexture->u[1]	= 1.0f; newTileTexture->u[2]	= 0.0f; newTileTexture->u[3]	= 1.0f;
	newTileTexture->v[0]	= 0.0f; newTileTexture->v[1]	= 0.0f; newTileTexture->v[2]	= 1.0;	newTileTexture->v[3]	= 1.0f;
	newTileTexture->dwColor	= 0;
	oldTileTexture = out_lstTileTextureTemplates[0];
	out_lstTileTextureTemplates[0] = newTileTexture.acquire();

	newTileTexture.clone( newTileTexture );
	newTileTexture->nTextureIndex	= 1;
	oldTileTexture = out_lstTileTextureTemplates[1];
	out_lstTileTextureTemplates[1] = newTileTexture.acquire();

	newTileTexture.clone( newTileTexture );
	newTileTexture->nTextureIndex	= 2;
	oldTileTexture = out_lstTileTextureTemplates[2];
	out_lstTileTextureTemplates[2] = newTileTexture.acquire();

	return 0;
};

error_t god::setupTileTemplates( GOPTR(TILE_GEOMETRY)* out_lstTileGeometryTemplates, GOPTR(TILE_TEXTURE)* out_lstTileTextureTemplates )
{	// WARNING!! Unsafe pointer handling in course!
	if( 0 == out_lstTileGeometryTemplates && 0 == out_lstTileTextureTemplates )
	{
		error_printf("Output address is NULL!");
		return -1;
	}
	glist<GODS(TILE_GEOMETRY)> TempGeo( 7 );
	glist<GODS(TILE_TEXTURE)> TempTex( 7 );
	setupTileTemplates( TempGeo.get_pointer(), TempTex.get_pointer() );
	if( out_lstTileGeometryTemplates && out_lstTileTextureTemplates)
	{
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
		{
			out_lstTileGeometryTemplates[iTileTpl]	= gacquireData( TempGeo[iTileTpl] );
			out_lstTileTextureTemplates[iTileTpl]	= gacquireData( TempTex[iTileTpl] );
		};
	}
	else if( out_lstTileGeometryTemplates )
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
			out_lstTileGeometryTemplates[iTileTpl]	= gacquireData( TempGeo[iTileTpl] );
	else if( out_lstTileTextureTemplates )
		for( uint32_t iTileTpl=0; iTileTpl<7; iTileTpl++ )
			out_lstTileTextureTemplates [iTileTpl]	= gacquireData( TempTex[iTileTpl] );

	return 0;
};

static const uint8_t g_TileSetIndices_2x2[16][4] = 
{
	// Tileset #0
	{	
		0, 0,
		0, 0
	},
	// Tileset #1
	{	
		6, 0,
		0, 0
	},
	// Tileset #2
	{	
		2, 6,
		0, 0
	},
	// Tileset #3
	{	
		4, 0,
		6, 0
	},
	// Tileset #4
	{	
		0, 4,
		2, 6
	},
	// Tileset #5
	{	
		4, 6,
		0, 0
	},
	// Tileset #6
	{	
		2, 0,
		6, 0
	},
	// Tileset #7
	{	
		6, 4,
		2, 6
	},
	// Tileset #8
	{	 
		6, 6,
		6, 0
	},
	// Tileset #9
	{	
		2, 2,
		2, 6
	},
	// Tileset #10
	{	
		4, 4,
		4, 6
	},
	// Tileset #11
	{	
		0, 6,
		6, 0
	},
	// Tileset #12
	{	
		4, 2,
		2, 6
	},
	// Tileset #13  
	{	
		2, 4,
		4, 6
	},
	// Tileset #14
	{	
		6, 2,
		4, 6
	},
	// Tileset #15
	{	
		0, 2,
		4, 6
	}
};
