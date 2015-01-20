#include "GTerrain_core.h"

namespace god
{
	void _AssignTileIndices( uint32_t nIndexOffset, bool bInvertedTile, uint32_t nVertexOffset, void* out_pIndexBuffer, bool b32Bit );
	void _AssignTileNormals( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pNormalBuffer );
	void _AssignTileTangents( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_TEXTURE_DATA in_TileTexture, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pTangentBuffer );
	void _AssignTileBitangents( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_TEXTURE_DATA in_TileTexture, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pBitangentBuffer );
	void _AssignTileVertices( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, uint32_t PositionX, uint32_t PositionZ, const float* in_fHeights, float fScale, const GVector3& vOptionalOffset, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pVertexBuffer );
	void _AssignTileTexCoord( uint32_t nVertexOffset, const GTILE_TEXTURE_DATA in_TileTexture, GVector2* out_pTexCoordBuffer );
	void _SetTerrainBVMinMax( const GVector3* v0, const GVector3* v1, const GVector3* v2, const GVector3* v3, GVector3* pvMinChunk, GVector3* pvMaxChunk, GVector3* pvMinSubset, GVector3* pvMaxSubset );
	//void _AssignTileTangents( GFACING_DIRECTION TileFacingDirection, uint32_t nVertexOffset, uint32_t PositionX, uint32_t PositionZ, float fScale, const float* in_fHeights, const GTILE_TEXTURE_DATA pAttr, const GVector3& vOptionalOffset, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pTangentBuffer, GVector3* out_pBinormalBuffer );
};