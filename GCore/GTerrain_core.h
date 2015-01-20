#include "GGeometry_core.h"
#include <stdio.h>

#ifndef _GTERRAIN_CORE_H
#define _GTERRAIN_CORE_H

namespace god
{
#pragma pack( push, 1 )
	// 0 - No walkable - No Snipable
	// 1 - Walkable
	// 2 - Snipable
	// 3 - Walkable - Snipable
	GDECLARE_CORE_STRUCT( TILE_MAPPING, TileMapping,
		int32_t		nChunkIndex;
		int32_t		SubsetIndexTop;
		int32_t		SubsetIndexFront;
		int32_t		SubsetIndexRight;
		int32_t		VerticesTop[4];
		int32_t		VerticesFront[4];
		int32_t		VerticesRight[4];
	 );
	static void onCreateTileMappingDefault( ggns_TileMapping* stuff )
	{
		(*stuff)->nChunkIndex		= 0;
		(*stuff)->SubsetIndexTop	= 
		(*stuff)->SubsetIndexFront	= 
		(*stuff)->SubsetIndexRight	= 0;
		memset( &(*stuff)->VerticesTop[0], -1, sizeof( int32_t )*12 );
	};

	// U and V TexCoords follow the same order from GAT format heights
	GDECLARE_CORE_STRUCT( TILE_TEXTURE, TileTexture,
		float			u[4];				//< west->east, north->south ordering; 0=left 1=right
		float			v[4];				//< west->east, north->south ordering; 0=up 1=down
		int16_t			nTextureIndex;		//< -1 for none
		int16_t			nLightmapIndex;		//< -1 for none?
		uint32_t		dwColor;			// a default color to set to the tile for other kind of representations
	);
	static void onCreateTileTextureDefault( ggns_TileTexture* stuff )
	{
		float u[4] = { 0, 1, 0, 1 };
		float v[4] = { 0, 0, 1, 1 };
		memcpy( (*stuff)->u, u, sizeof( float )*4 );	//< west->east, north->south ordering; 0=left 1=right
		memcpy( (*stuff)->v, v, sizeof( float )*4 );	//< west->east, north->south ordering; 0=up 1=down
		(*stuff)->nTextureIndex;	// -1 for none
		(*stuff)->nLightmapIndex;	// -1 for none?
		(*stuff)->dwColor;			// a default color to set to the tile for other kind of representations
	};

	GDECLARE_CORE_STRUCT( TILE_GEOMETRY, TileGeometry,
		float			fHeight[4];				// west->east, north->south ordering
		int32_t			TopTextureDataIndex;	// <= -1 for none
		int32_t			RightTextureDataIndex;	// <= -1 for none
		int32_t			FrontTextureDataIndex;	// <= -1 for none
		int16_t			Flags;					// GND v <= 1.5 // maybe a color key? a terrain property? We're going to use it to tell if the triangle is inverted.
	);
	static void onCreateTileGeometryDefault( ggns_TileGeometry* stuff )
	{
		memset( &(*stuff)->fHeight[0], 0, sizeof( float )*4 );
		memset( &(*stuff)->TopTextureDataIndex, -1L, sizeof( int32_t )*3 );
		(*stuff)->Flags = 0;
	};

	GDECLARE_CORE_STRUCT( TILE_NAVIGABILITY, TileNavigability,
		float		fHeight[4]; // west->east, south->north ordering
		uint8_t		Type;
		uint8_t		FlagsTop;
		uint8_t		FlagsRight;
		uint8_t		FlagsFront;
	);
	static void onCreateTileNavigabilityDefault( ggns_TileNavigability* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size() );
	};

	typedef struct _RGB8_COLOR
	{
		unsigned char r, g, b;
	} RGB8_COLOR;

	GDECLARE_CORE_STRUCT( TILE_BRIGHTNESS, TileBrightness,
		uint8_t		Brightness[8][8];
		RGB8_COLOR	ColorRGB[8][8];
	);
	static void onCreateTileBrightnessDefault( ggns_TileBrightness* stuff )
	{
		memset( &(*stuff)->Brightness[0], -1, sizeof( uint8_t )*64 );
		memset( &(*stuff)->ColorRGB[0], 0, sizeof( RGB8_COLOR )*64 );
	};

	GDECLARE_CORE_STRUCT( TILED_TERRAIN, TiledTerrain,
		uint16_t	Width;				// Tiles in the X dimension
		uint16_t	Depth;				// Tiles in the Z dimension
		float		TileScale;			// The size to expand the tiles
		GVector3	Offset;				//
	);
	static void onCreateTiledTerrainDefault( ggns_TiledTerrain* stuff )
	{
		(*stuff)->Width		= 3;		// Tiles in the X dimension
		(*stuff)->Depth		= 3;		// Tiles in the Z dimension
		(*stuff)->TileScale	= 1.0f;		// The size to expand the tiles
		(*stuff)->Offset.Zero();
	};

	GDECLARE_CORE_STRUCT( TERRAIN_WATER, TerrainWater,
		float		fSeaLevel;			//
		int32_t		nWaterType;			//
		float		fWaveHeight;		//
		float		fWaveSpeed;			//
		float		fWavePitch;			//
		int32_t		nAnimationSpeed;	//
	);
	static void onCreateTerrainWaterDefault( ggns_TerrainWater* stuff )
	{
		memset( stuff->get_data_address(), 0, stuff->get_data_size() );
	};

	GDECLARE_CORE_STRUCT( TILED_TERRAIN_COUNTERS, TiledTerrainCounters,
		uint32_t	nBaseTileCount;			// Base tile count is equal to (tile map width*tile map depth)
		uint32_t	nTileColumnCount;		// Stores the amount of tile columns that contain a single map chunk
		uint32_t	nTileRowCount;			// Stores the amount of tile rows that contain a single map chunk
		uint32_t	nTopTileFaceCount;		// Stores the number of tiles facing upwards that contain valid attributes and geometry

		uint32_t	nFrontTileFaceCount;	// Stores the number of tiles facing front that contain valid attributes and geometry
		uint32_t	nRightTileFaceCount;	// Stores the number of tiles facing right that contain valid attributes and geometry
		uint32_t	nBottomTileFaceCount;	// Stores the number of tiles facing upwards that contain valid attributes and geometry
		uint32_t	nBackTileFaceCount;		// Stores the number of tiles facing front that contain valid attributes and geometry

		uint32_t	nLeftTileFaceCount;		// Stores the number of tiles facing right that contain valid attributes and geometry
		uint32_t	nTotalTileFaceCount;	// Stores the total number of valid tiles, which should be equal to the addition of top, front and right tiles.
		uint32_t	nChunkColumnTileCount;	// Stores the amount of tile columns that contain a single map chunk
		uint32_t	nChunkRowTileCount;		// Stores the amount of tile rows that contain a single map chunk

		uint32_t	nChunkTotalTileCount;	// Stores the total number of tiles contained in a single chunk
		uint32_t	nColumnChunkCount;		// Stores the amount of column chunks
		uint32_t	nRowChunkCount;			// Stores the amount of row chunks
		uint32_t	nTotalChunkCount;		// Stores the total chunks contained in a map
	);
	static void onCreateTiledTerrainCountersDefault( ggns_TiledTerrainCounters* stuff )
	{
		memset( stuff->get_data_address(), 0, stuff->get_data_size() );
	};

#pragma pack( pop )

	typedef uint8_t GTILEFACE_DIRECTION;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_TOP	= 0;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_FRONT	= 1;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_RIGHT	= 2;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_BOTTOM	= 3;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_BACK	= 4;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_LEFT	= 5;
	static const GTILEFACE_DIRECTION	GFACINGDIRECTION_COUNT	= 6;

	typedef uint8_t GTILE_BORDER;
	static const GTILE_BORDER	GTILEBORDER_BACK	= 1;
	static const GTILE_BORDER	GTILEBORDER_LEFT	= 2;
	static const GTILE_BORDER	GTILEBORDER_FRONT	= 4;
	static const GTILE_BORDER	GTILEBORDER_RIGHT	= 8;

	error_t	setupTileTemplates( GODS(TILE_GEOMETRY)* out_lstTileTemplates, GODS(TILE_TEXTURE)* out_lstTileTextureTemplates );
	error_t	setupTileTemplates( GOPTR(TILE_GEOMETRY)* out_lstTileTemplates, GOPTR(TILE_TEXTURE)* out_lstTileTextureTemplates );
	error_t	setupTileTemplates_old( GODS(TILE_GEOMETRY)* out_lstTileTemplates, GODS(TILE_TEXTURE)* out_lstTileTextureTemplates );
	error_t	setupTileTemplates_old( GOPTR(TILE_GEOMETRY)* out_lstTileTemplates, GOPTR(TILE_TEXTURE)* out_lstTileTextureTemplates );
	error_t buildTileGeometry(	GTILEFACE_DIRECTION TileDirection, const GODS(TILE_GEOMETRY) pTileCurrent, const GODS(TILE_GEOMETRY) pTileSide, 
								uint32_t nGlobalTileX, uint32_t nGlobalTileZ, float fTileScale, bool bCountOnly, GVector3 vOptionalOffset,
								const GODS(TILE_TEXTURE)* in_lstTileTextureData, uint32_t nTileTextureDataCount, uint32_t nTextureIndex, uint32_t nSubsetIndex,
								uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, 
								GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
								GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
								uint32_t* out_pTriangleToSubsetMap, GODS(TILE_MAPPING)* out_CurrentTileMappingData,
								uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
								uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
							);
	error_t buildTileFaces( GTILEFACE_DIRECTION TileDirection, 
							const GODS(TILE_GEOMETRY) pTileCurrent, const GODS(TILE_GEOMETRY) pTileFront, const GODS(TILE_GEOMETRY) pTileRight, 
							uint32_t nGlobalTileX, uint32_t nGlobalTileZ, float fTileScale, bool bCountOnly, GVector3 vOptionalOffset,
							const GODS(TILE_TEXTURE)* in_lstTileTextureData, uint32_t nTileTextureDataCount, uint32_t nTextureIndex, uint32_t nSubsetIndex,
							uint32_t* out_nVertexCount, uint32_t* out_nIndexCount, uint32_t* out_nTriangleCount, 
							GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
							GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
							uint32_t* out_pTriangleToSubsetMap, GODS(TILE_MAPPING)* out_CurrentTileMappingData,
							uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
							uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
						);
	error_t buildChunkSubsetGeometry(	GTILEFACE_DIRECTION TileDirection, GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, 
										uint32_t nTotalWidth, uint32_t nTotalDepth, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, 
										GODS(TILE_TEXTURE) const* in_lstTileTextureData, uint32_t nTileTextureDataCount, int32_t nTextureIndex, 
										uint32_t nChunkIndex, uint32_t nChunkRowTileCount, uint32_t nChunkColumnTileCount, bool bCountOnly, GVector3 vOptionalOffset,
										uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t nSubsetIndex,
										GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
										GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
										uint32_t* out_pTriangleToSubsetMap, GODS(TILE_MAPPING)* out_lstTileMappingData,
										uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
										uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
									);
	error_t buildChunkFaceGroupGeometry(	GTILEFACE_DIRECTION TileDirection, GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, 
											uint32_t nTotalWidth, uint32_t nTotalDepth, 
											const GODS(TILE_TEXTURE)* in_lstTileTextureData, uint32_t nTileTextureDataCount, 
											bool bCountOnly, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, GVector3& vOptionalOffset,
											uint32_t nTextureCount, uint32_t nChunkColumnTileCount, uint32_t nChunkRowTileCount, uint32_t nChunkIndex, 
											uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t* inout_nSubsetCount,
											GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
											GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
											uint32_t* out_pTriangleToSubsetMap, 
											uint32_t* out_pSubsetToTextureMap, 
											uint32_t* out_pSubsetToFaceGroupMap, 
											uint32_t* out_pSubsetToChunkMap, 
											GODS(TILE_MAPPING)* out_lstTileMappingData,
											GODS(BUFFERSLICE)* out_lstBufferSliceDataSubsets,
											uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
											uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
										);
	error_t buildChunkGeometry( GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, 
								const GODS(TILE_TEXTURE)* in_lstTileTextureData, uint32_t nTileTextureDataCount, 
								bool bCountOnly, uint32_t nTileOffsetX, uint32_t nTileOffsetZ, float fTileScale, GVector3& vOptionalOffset,
								uint32_t nTextureCount, uint32_t nChunkColumnTileCount, uint32_t nChunkRowTileCount, uint32_t nChunkIndex, 
								uint32_t* inout_nVertexCount, uint32_t* inout_nIndexCount, uint32_t* inout_nTriangleCount, uint32_t* inout_nSubsetCount,
								GVector3* out_pVertices, void* out_pIndices, bool b32BitIndices, GVector2* out_pTexCoord, 
								GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
								uint32_t* out_pTriangleToSubsetMap, 
								uint32_t* out_pSubsetToTextureMap, 
								uint32_t* out_pSubsetToFaceGroupMap, 
								uint32_t* out_pSubsetToChunkMap, 
								GODS(TILE_MAPPING)* out_lstTileMappingData,
								GODS(BUFFERSLICE)* out_lstBufferSliceDataFaceGroups,
								GODS(BUFFERSLICE)* out_lstBufferSliceDataSubsets,
								uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
								uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount
							);
	error_t buildTerrainGeometry(	GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, uint32_t nTileCount,
									GODS(TILE_TEXTURE) const* in_lstTileTextureData, int32_t nTileTextureDataCount, uint32_t nTextureCount,
									float fHeightScale, uint32_t nColumnSubgrids, uint32_t nRowSubgrids,
									uint32_t nSubgridColumnTiles, uint32_t nSubgridRowTiles, 
									GVector3* out_pVertices, uint32_t* inout_nVertexCount, void* out_pIndices, bool* inout_b32BitIndices,
									uint32_t* inout_nIndexCount, GVector2* out_pTexCoord, 
									GVector3* out_pNormals, GVector3* out_pTangents, GVector3* out_pBitangents, 
									uint32_t* out_pTriangleToSubsetMap, 
									uint32_t* out_pSubsetToTextureMap, 
									uint32_t* out_pSubsetToFaceGroupMap, 
									uint32_t* out_pSubsetToChunkMap, 
									uint32_t* inout_nSubsetCount, 
									GODS(TILE_MAPPING)* out_lstTileMappingData,
									GODS(BUFFERSLICE)* out_lstBufferSliceDataChunks,
									GODS(BUFFERSLICE)* out_lstBufferSliceDataFaceGroups,
									GODS(BUFFERSLICE)* out_lstBufferSliceDataSubsets,
									uint32_t* inout_nTopTileCount, uint32_t* inout_nFrontTileCount, uint32_t* inout_nRightTileCount,
									uint32_t* inout_nBottomTileCount, uint32_t* inout_nBackTileCount, uint32_t* inout_nLeftTileCount,
									GVector3& vOptionalOffset
								); // node count is nRowSubgrids*nColumnSubgrids

	error_t recalculateTileNormals( const GVector3* in_pVertices, const GODS(TILE_GEOMETRY) in_TileGeometryDataCurrent, 
				const GODS(TILE_MAPPING) in_TileMappingDataCurrent, float fTileScale, 
				GVector3 *out_pNormals );

	error_t recalculateTerrainNormals( const GVector3* in_pVertices, GODS(TILE_MAPPING) const * in_lstTileMappingData,
				 GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, uint32_t nWidth, uint32_t nDepth, 
				 float fTileScale, GVector3 *out_pNormals );

	error_t blendTileNormals(	const GODS(TILE_MAPPING) pTileMappingCurrent, 
								const GODS(TILE_GEOMETRY) pTileGeometryCurrent, 
								const GODS(TILE_MAPPING) pTileMappingBackLeft, 
								const GODS(TILE_MAPPING) pTileMappingLeft, 
								const GODS(TILE_MAPPING) pTileMappingFrontLeft, 
								const GODS(TILE_MAPPING) pTileMappingBack, 
								const GODS(TILE_MAPPING) pTileMappingFront, 
								const GODS(TILE_MAPPING) pTileMappingBackRight, 
								const GODS(TILE_MAPPING) pTileMappingRight, 
								const GODS(TILE_MAPPING) pTileMappingFrontRight, 
								const GODS(TILE_GEOMETRY) pTileGeometryBackLeft, 
								const GODS(TILE_GEOMETRY) pTileGeometryLeft, 
								const GODS(TILE_GEOMETRY) pTileGeometryFrontLeft, 
								const GODS(TILE_GEOMETRY) pTileGeometryBack, 
								const GODS(TILE_GEOMETRY) pTileGeometryFront, 
								const GODS(TILE_GEOMETRY) pTileGeometryBackRight, 
								const GODS(TILE_GEOMETRY) pTileGeometryRight, 
								const GODS(TILE_GEOMETRY) pTileGeometryFrontRight,
								GVector3* out_pNormals 
							);

	error_t blendChunkNormals(	const GVector3* in_pVertices, GODS(TILE_MAPPING) const * in_lstTileMappingData,
								GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, uint32_t nTotalWidth, uint32_t nTotalDepth, 
								uint32_t nChunkRowTileCount, uint32_t nChunkColumnTileCount, 
								uint32_t nTileOffsetX, uint32_t nTileOffsetZ, 
								float fTileScale, GVector3 *out_pNormals 
							);

	error_t blendTerrainNormals( const GVector3* in_pVertices, GODS(TILE_MAPPING) const * in_lstTileMappingData,
				 GODS(TILE_GEOMETRY) const* in_lstTileGeometryData, uint32_t nWidth, uint32_t nDepth, 
				 float fTileScale, GVector3 *out_pNormals );

	// These functions translate linear indices to cartesian coordinates and vice-versa
	uint32_t GetLinearIndex3D( uint32_t x, uint32_t z, uint32_t y, uint32_t nMapWidth, uint32_t nMapDepth );
	uint32_t GetLinearIndex2D( uint32_t x, uint32_t z, uint32_t nMapWidth );
	uint32_t GetCartesianIndex3D( uint32_t in_nAbsoluteIndex, uint32_t* x, uint32_t* z, uint32_t* y, uint32_t nMapWidth, uint32_t nMapDepth );
	uint32_t GetCartesianIndex2D( uint32_t in_nAbsoluteIndex, uint32_t* x, uint32_t* z, uint32_t nMapWidth );

	// These functions return the linear index of an adjacent tile
	uint32_t GetFrontTileIndex2D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBackTileIndex2D			( int32_t nCurrentTileIndex, uint16_t nWidthInTiles );//, uint16_t nMapDepthInTiles );
	uint32_t GetRightTileIndex2D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetLeftTileIndex2D			( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); //, uint16_t nMapDepthInTiles );
	uint32_t GetFrontRightTileIndex2D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetFrontLeftTileIndex2D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); //, uint16_t nMapDepthInTiles );
	uint32_t GetBackRightTileIndex2D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBackLeftTileIndex2D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); //, uint16_t nMapDepthInTiles );

	// These functions return the linear index of an adjacent tile
	uint32_t GetFrontTileIndex3D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBackTileIndex3D			( int32_t nCurrentTileIndex ); //, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetRightTileIndex3D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetLeftTileIndex3D			( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); //, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetTopTileIndex3D			( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBottomTileIndex3D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles ); //, uint16_t nMapDepthInTiles );
	uint32_t GetFrontRightTileIndex3D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetFrontLeftTileIndex3D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); //, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBackRightTileIndex3D	( int32_t nCurrentTileIndex, uint16_t nWidthInTiles, uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );
	uint32_t GetBackLeftTileIndex3D		( int32_t nCurrentTileIndex, uint16_t nWidthInTiles ); // , uint16_t nDepthInTiles, uint16_t nMapDepthInTiles );

	// -------------------------- setTileCornerHeight functions
	error_t setTileCornerHeightBackLeft(	
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingBackLeft, 
										const GODS(TILE_MAPPING)	TileMappingLeft, 
										const GODS(TILE_MAPPING)	TileMappingBack, 
										const float fHeight,
										GVector3* out_pVertices 
									);
	error_t setTileCornerHeightFrontLeft(	
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingLeft, 
										const GODS(TILE_MAPPING)	TileMappingFrontLeft, 
										const GODS(TILE_MAPPING)	TileMappingFront, 
										const float fHeight,
										GVector3* out_pVertices 
									);	
	error_t setTileCornerHeightBackRight(
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingBack, 
										const GODS(TILE_MAPPING)	TileMappingBackRight, 
										const GODS(TILE_MAPPING)	TileMappingRight, 
										const float fHeight,
										GVector3* out_pVertices 
									);
	error_t setTileCornerHeightFrontRight(	
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingFront, 
										const GODS(TILE_MAPPING)	TileMappingRight, 
										const GODS(TILE_MAPPING)	TileMappingFrontRight, 
										const float fHeight,
										GVector3* out_pVertices 
									);
	error_t setTileCornerHeights(		GTILE_BORDER Boundaries, 
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
										GVector3* out_pVertices 
									);
	//------------------------- getTileCornerWalls functions
	error_t getTileCornerWallsBackLeft(	
										const GODS(TILE_MAPPING)	TileMappingBackLeft, 
										const GODS(TILE_MAPPING)	TileMappingLeft, 
										const GODS(TILE_MAPPING)	TileMappingBack, 
										bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
									);
	error_t getTileCornerWallsFrontLeft(	
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingLeft, 
										const GODS(TILE_MAPPING)	TileMappingFrontLeft, 
										bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
									);	
	error_t getTileCornerWallsBackRight(
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingBack, 
										const GODS(TILE_MAPPING)	TileMappingBackRight, 
										bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
									);
	error_t getTileCornerWallsFrontRight(	
										const GODS(TILE_MAPPING)	TileMappingCurrent, 
										const GODS(TILE_MAPPING)	TileMappingFront, 
										const GODS(TILE_MAPPING)	TileMappingRight, 
										//const GODS(TILE_MAPPING)	TileMappingFrontRight, 
										bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
									);
	error_t getTileCornerWalls(	const GODS(TILE_MAPPING)	TileMapping0, 
								const GODS(TILE_MAPPING)	TileMapping1, 
								const GODS(TILE_MAPPING)	TileMapping2, 
								bool* bWallBack, bool* bWallLeft, bool* bWallFront, bool* bWallRight
							);
	error_t buildTileHeightImage( const GODS(TILE_GEOMETRY)* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint8_t* out_lstHeights );
	error_t buildTileHeightImage( const GODS(TILE_GEOMETRY)* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint16_t* out_lstHeights );
	error_t buildTileHeightImage( const GODS(TILE_GEOMETRY)* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, uint32_t* out_lstHeights );
//	error_t buildTileHeightImage( const GODS(TILE_GEOMETRY)* in_lstGeometryData, float fMin, float fMax, uint32_t nWidth, uint32_t nDepth, GBUFFER_DATA* out_HeightsBuffer );

#define __GDECLARE_GTERRAIN_CORE_GET_TILE_AREA_FUNCTION( TOKEN )													\
	error_t getAreaTiles( GODS(TOKEN)* in_lstTileDataGlobal, uint32_t nSourceTileCount, uint32_t nSourceWidth,		\
		uint32_t nAreaTileOffsetX, uint32_t nAreaTileOffsetZ, uint32_t* nAreaTileCountX, uint32_t* nAreaTileCountZ,	\
		GODS(TOKEN)* out_lstTileDataArea, uint32_t* out_lstAreaToGlobalTileMap );

	__GDECLARE_GTERRAIN_CORE_GET_TILE_AREA_FUNCTION( TILE_GEOMETRY );
	__GDECLARE_GTERRAIN_CORE_GET_TILE_AREA_FUNCTION( TILE_MAPPING );
	__GDECLARE_GTERRAIN_CORE_GET_TILE_AREA_FUNCTION( TILE_NAVIGABILITY );

#define __GDECLARE_GTERRAIN_CORE_TILE_MAP_ACCESSORS( TOKEN )																								\
	GODS(TOKEN) getFrontTile		( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getBackTile			( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getRightTile		( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getLeftTile			( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getFrontRightTile	( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getFrontLeftTile	( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getBackRightTile	( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getBackLeftTile		( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );	\
	GODS(TOKEN) getTile				( GODS(TOKEN) const* in_lstTiles, uint32_t nTileCount, uint32_t nTileIndex );											\
	GODS(TOKEN) getFrontTile		( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getBackTile			( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getRightTile		( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getLeftTile			( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getFrontRightTile	( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getFrontLeftTile	( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getBackRightTile	( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getBackLeftTile		( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nMapWidthInTiles, uint16_t nMapDepthInTiles );			\
	GODS(TOKEN) getTile				( GODS(TOKEN)* in_lstTiles, uint32_t nTileCount, uint32_t nTileIndex );

	__GDECLARE_GTERRAIN_CORE_TILE_MAP_ACCESSORS( TILE_GEOMETRY );
	__GDECLARE_GTERRAIN_CORE_TILE_MAP_ACCESSORS( TILE_NAVIGABILITY );
	__GDECLARE_GTERRAIN_CORE_TILE_MAP_ACCESSORS( TILE_MAPPING );
#undef __GDECLARE_GTERRAIN_CORE_TILE_MAP_ACCESSORS
}; // namespace

#endif // _GTERRAIN_CORE_H