
#include "GTerrain_core.h"
#include "GTerrain_core_private.h"
#include "GCore_function_macros.h"
#include "GTerrain_function_macros.h"

#include <stdlib.h>
#include <memory.h>

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

using namespace god;

//-------------------------------------------------------------------------
__GDEFINE_GAMELIB_TILE_MAP_ACCESSORS( TILE_GEOMETRY );
__GDEFINE_GAMELIB_TILE_MAP_ACCESSORS( TILE_NAVIGABILITY );
__GDEFINE_GAMELIB_TILE_MAP_ACCESSORS( TILE_MAPPING );

__GDEFINE_GETAREATILES_FUNCTION( GTILE_GEOMETRY_DATA );
__GDEFINE_GETAREATILES_FUNCTION( GTILE_NAVIGABILITY_DATA );
__GDEFINE_GETAREATILES_FUNCTION( GTILE_MAPPING_DATA );

GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILE_GEOMETRY, TileGeometry );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILE_TEXTURE, TileTexture );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILE_NAVIGABILITY, TileNavigability );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILE_BRIGHTNESS, TileBrightness );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILE_MAPPING, TileMapping );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILED_TERRAIN, TiledTerrain );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TERRAIN_WATER, TerrainWater );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TILED_TERRAIN_COUNTERS, TiledTerrainCounters );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILE_BRIGHTNESS), 
	"%s"
	, "Structure too big, it will not be printed" 
);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILE_GEOMETRY), 
	"fHeight[4]....................: [%f,%f,%f,%f]\n"
	"TopTextureDataIndex...........: %i\n"
	"RightTextureDataIndex.........: %i\n"
	"FrontTextureDataIndex.........: %i"
	, in_CoreInstance->fHeight[0]
	, in_CoreInstance->fHeight[1]
	, in_CoreInstance->fHeight[2]
	, in_CoreInstance->fHeight[3]
	, in_CoreInstance->TopTextureDataIndex
	, in_CoreInstance->RightTextureDataIndex
	, in_CoreInstance->FrontTextureDataIndex
);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILE_MAPPING),	
	"nChunkIndex...................: %i\n"
	"SubsetIndexTop................: %i\n"
	"SubsetIndexFront..............: %i\n"
	"SubsetIndexRight..............: %i\n"
	"VerticesTop[4]................: [%i,%i,%i,%i]\n"
	"VerticesFront[4]..............: [%i,%i,%i,%i]\n"
	"VerticesRight[4]..............: [%i,%i,%i,%i]" 
	, in_CoreInstance->nChunkIndex
	, in_CoreInstance->SubsetIndexTop
	, in_CoreInstance->VerticesTop[0]
	, in_CoreInstance->VerticesTop[1]
	, in_CoreInstance->VerticesTop[2]
	, in_CoreInstance->VerticesTop[3]
	, in_CoreInstance->VerticesFront[0]
	, in_CoreInstance->VerticesFront[1]
	, in_CoreInstance->VerticesFront[2]
	, in_CoreInstance->VerticesFront[3]
	, in_CoreInstance->VerticesRight[0]
	, in_CoreInstance->VerticesRight[1]
	, in_CoreInstance->VerticesRight[2]
	, in_CoreInstance->VerticesRight[3]
);
	// U and V TexCoords follow the same order from GAT_TILE_DATA heights
GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILE_TEXTURE),
	"u[4]..........................: [%f, %f, %f, %f]\n"	//< west->east, south->north ordering; 0=left 1=right
	"v[4]..........................: [%f, %f, %f, %f]\n"	//< west->east, south->north ordering; 0=up 1=down
	"nTextureIndex.................: %i\n"	//< -1 for none
	"nLightmapIndex................: %i\n"	//< -1 for none?
	"dwColor.......................: 0x%X"	// a default color to set to the tile for other kind of representations
	,in_CoreInstance->u[0]
	,in_CoreInstance->u[1]
	,in_CoreInstance->u[2]
	,in_CoreInstance->u[3]
	,in_CoreInstance->v[0]
	,in_CoreInstance->v[1]
	,in_CoreInstance->v[2]
	,in_CoreInstance->v[3]
	,(int)in_CoreInstance->nTextureIndex
	,(int)in_CoreInstance->nLightmapIndex
	,in_CoreInstance->dwColor
);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILE_NAVIGABILITY), 
	"fHeight[4]....................: [%f,%f,%f,%f]\n"
	"Type..........................: %i\n"
	"TopBlocked....................: %i\n"
	"FrontBlocked..................: %i\n"
	"RightBlocked..................: %i"
	, in_CoreInstance->fHeight[0]
	, in_CoreInstance->fHeight[1]
	, in_CoreInstance->fHeight[2]
	, in_CoreInstance->fHeight[3]
	, (int)in_CoreInstance->Type
	, (int)in_CoreInstance->FlagsTop
	, (int)in_CoreInstance->FlagsFront
	, (int)in_CoreInstance->FlagsRight
);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILED_TERRAIN_COUNTERS),
	"nBaseTileCount................: %i\n"
	"nTopTileFaceCount.............: %i\n"
	"nFrontTileFaceCount...........: %i\n"
	"nRightTileFaceCount...........: %i\n"
	"nBottomTileFaceCount..........: %i\n"
	"nBackTileFaceCount............: %i\n"
	"nLeftTileFaceCount............: %i\n"
	"nTotalTileFaceCount...........: %i\n"
	"nChunkColumnTileCount.........: %i\n"
	"nChunkRowTileCount............: %i\n"
	"nColumnChunkCount.............: %i\n"
	"nRowChunkCount................: %i"
	,in_CoreInstance->nBaseTileCount
	,in_CoreInstance->nTopTileFaceCount
	,in_CoreInstance->nFrontTileFaceCount
	,in_CoreInstance->nRightTileFaceCount
	,in_CoreInstance->nBottomTileFaceCount
	,in_CoreInstance->nBackTileFaceCount
	,in_CoreInstance->nLeftTileFaceCount
	,in_CoreInstance->nTotalTileFaceCount
	,in_CoreInstance->nChunkColumnTileCount
	,in_CoreInstance->nChunkRowTileCount
	,in_CoreInstance->nColumnChunkCount
	,in_CoreInstance->nRowChunkCount
);
	// Equivalent to GND_FILE_CONTENTS
GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TILED_TERRAIN),
	"Width.......................: %i\n"
	"Depth.......................: %i\n"
	"TileScale...................: %f\n"
	//L"NameLength..................: %i\n"
	//L"wszName*.....................: %s"
	,(int)in_CoreInstance->Width
	,(int)in_CoreInstance->Depth
	,in_CoreInstance->TileScale
	//,(int)in_CoreInstance->NameLength
	//, in_CoreInstance->wszName ? in_CoreInstance->wszName : L"\"\"" 
);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TERRAIN_WATER),
	"fSeaLevel.....................: %f\n"
	"nWaterType....................: %i\n"
	"fWaveHeight...................: %f\n"
	"fWaveSpeed....................: %f\n"
	"fWavePitch....................: %f\n"
	"nAnimationSpeed...............: %i"
	,in_CoreInstance->fSeaLevel
	,in_CoreInstance->nWaterType
	,in_CoreInstance->fWaveHeight
	,in_CoreInstance->fWaveSpeed
	,in_CoreInstance->fWavePitch
	,in_CoreInstance->nAnimationSpeed
);

//-------------------------------------------------------------------------
uint32_t god::GetLinearIndex3D( uint32_t x, uint32_t z, uint32_t y, uint32_t nMapWidth, uint32_t nMapDepth )
{
	return (nMapWidth*nMapDepth)*y+(nMapWidth*z)+x;
};
//-------------------------------------------------------------------------
uint32_t god::GetLinearIndex2D( uint32_t x, uint32_t z, uint32_t nMapWidth )
{
	return (nMapWidth*z)+x;
};
//-------------------------------------------------------------------------
uint32_t god::GetCartesianIndex3D( uint32_t in_nAbsoluteIndex, uint32_t* x, uint32_t* z, uint32_t* y, uint32_t nMapWidth, uint32_t nMapDepth )
{
	uint32_t surFace = (nMapWidth*nMapDepth);
	if( 0 == surFace )
	{
		error_printf("Cannot provide an index for tiles of zero-sized maps.");
		return -1;
	}
	*y = in_nAbsoluteIndex / surFace;
	uint32_t _mod = in_nAbsoluteIndex % surFace;
	*z = ( _mod ) / nMapWidth;
	*x = ( _mod ) % nMapWidth;
	return 0;
};

uint32_t god::GetCartesianIndex2D( uint32_t in_nAbsoluteIndex, uint32_t* x, uint32_t* z, uint32_t nMapWidth )
{
	if( 0 == nMapWidth )
	{
		error_printf("Cannot provide an index for tiles of zero-sized maps.");
		return -1;
	}
	*z = in_nAbsoluteIndex / nMapWidth;
	*x = in_nAbsoluteIndex % nMapWidth;
	return 0;
};

uint32_t god::GetFrontTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( x < (uint32_t)(nWidth-1) )
	//if( nCurrentTileIndex < (nWidth*nDepth)-1 )
		return nCurrentTileIndex+1;
	return -1;
};

uint32_t god::GetBackTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth ) // , uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( x > 0 )
	//if( nCurrentTileIndex > 0 )
		return nCurrentTileIndex-1;
	return -1;
};

uint32_t god::GetRightTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( z < (uint32_t)(nDepth-1) )
	//if( nCurrentTileIndex < (nWidth*nDepth)-nWidth )
		return nCurrentTileIndex+nWidth;
	return -1;
};

uint32_t god::GetLeftTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( z > 0 )
	//if( nCurrentTileIndex >= nWidth )
		return nCurrentTileIndex-nWidth;
	return -1;
};

uint32_t god::GetFrontRightTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( (x < (uint32_t)(nWidth-1))
		&& (z < (uint32_t)(nDepth-1)) )
	//if( nCurrentTileIndex < (nWidth*nDepth)-(nWidth+1) )
		return nCurrentTileIndex+(nWidth+1);
	return -1;
};

uint32_t god::GetBackRightTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( x > 0 && z < (uint32_t)(nDepth-1) )
	//if( nCurrentTileIndex < (nWidth*nDepth)-(nWidth-1) )
		return nCurrentTileIndex+(nWidth-1);
	return -1;
};
uint32_t god::GetFrontLeftTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( (x < (uint32_t)(nWidth-1))
		&& z > 0 )
	//if( nCurrentTileIndex >= nWidth-1 )
		return nCurrentTileIndex-(nWidth-1);
	return -1;
};

uint32_t god::GetBackLeftTileIndex2D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth )
{
	uint32_t x=0, z=0;
	GetCartesianIndex2D( nCurrentTileIndex, &x, &z, nWidth );
	if( x > 0 && z > 0 )
	//if( nCurrentTileIndex >= nWidth+1 )
		return nCurrentTileIndex-(nWidth+1);
	return -1;
};

//----------------------------------------------------------------------------------------------------------------------------


uint32_t god::GetFrontTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex < (nWidth*nDepth*nHeight)-1 )
		return nCurrentTileIndex+1;
	//return -1;
};

uint32_t god::GetBackTileIndex3D( int32_t nCurrentTileIndex ) //, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex > 0 )
		return nCurrentTileIndex-1;
	//return -1;
};

uint32_t god::GetRightTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex < (nWidth*nDepth*nHeight)-nWidth )
		return nCurrentTileIndex+nWidth;
	//return -1;
};

uint32_t god::GetLeftTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex >= nWidth )
		return nCurrentTileIndex-nWidth;
	//return -1;
};

uint32_t god::GetTopTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex < (nWidth*nDepth*nHeight - nWidth*nDepth) )
		return nCurrentTileIndex + nWidth*nDepth;
	//return -1;
};

uint32_t god::GetBottomTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth ) // , uint16_t nHeight )
{
	//if( nCurrentTileIndex >= (nWidth*nDepth) )
		return nCurrentTileIndex-nWidth*nDepth;
	//return -1;
}

uint32_t god::GetFrontRightTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex < (nWidth*nDepth*nHeight)-(nWidth+1) )
		return nCurrentTileIndex+(nWidth+1);
	//return -1;
};

uint32_t god::GetBackRightTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex < (nWidth*nDepth*nHeight)-(nWidth-1) )
		return nCurrentTileIndex+(nWidth-1);
	//return -1;
};

uint32_t god::GetFrontLeftTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex >= nWidth-1 )
		return nCurrentTileIndex-(nWidth-1);
	//return -1;
};

uint32_t god::GetBackLeftTileIndex3D( int32_t nCurrentTileIndex, uint16_t nWidth ) //, uint16_t nDepth, uint16_t nHeight )
{
	//if( nCurrentTileIndex >= nWidth+1 )
		return nCurrentTileIndex-(nWidth+1);
	//return -1;
};

//----------------------------------------------------------------------------------------------------------------------------
void god::_AssignTileIndices( uint32_t nIndexOffset, bool bInvertedTile, uint32_t nVertexOffset, void* pIndexBuffer, bool b32Bit )
{
	GPROFILE_FUNCTION();
	//static uint32_t indices[6]			= {0,1,2,1,3,2};
	//static uint32_t indicesInverted[6]	= {0,1,3,0,3,2};
	static uint32_t indices[6]			= {0,2,1,1,2,3};
	static uint32_t indicesInverted[6]	= {0,3,1,0,2,3};
 	if( b32Bit )
	{
		if( bInvertedTile )
			for( uint32_t i=0; i<6; i++ )
				((uint32_t*)pIndexBuffer)[nIndexOffset+i] = nVertexOffset+indicesInverted[i];
		else
			for( uint32_t i=0; i<6; i++ )
				((uint32_t*)pIndexBuffer)[nIndexOffset+i] = nVertexOffset+indices[i];
	}
	else
	{
		if( bInvertedTile )
			for( uint32_t i=0; i<6; i++ )
				((uint16_t*)pIndexBuffer)[nIndexOffset+i] = nVertexOffset+indicesInverted[i];
		else
			for( uint32_t i=0; i<6; i++ )
				((uint16_t*)pIndexBuffer)[nIndexOffset+i] = nVertexOffset+indices[i];
	}	
};

void god::_AssignTileVertices(  GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, uint32_t PositionX, uint32_t PositionZ, const float* fHeights, float fScale, 
							  const GVector3& vOptionalOffset, GTILE_GEOMETRY_DATA pTileSide, GVector3* out_pVertexBuffer )
{
	GPROFILE_FUNCTION();
	if( 0 == out_pVertexBuffer )
	{
		error_printf("GVector3* out_pVertexBuffer is NULL!");
		return;
	}
	switch( TileFacingDirection )
	{
	case GFACINGDIRECTION_TOP:
	case GFACINGDIRECTION_BOTTOM:
		out_pVertexBuffer[nVertexOffset+0]	= GVector3( (PositionX)*fScale,		fHeights[0], (PositionZ)*fScale );
		out_pVertexBuffer[nVertexOffset+1]	= GVector3( (PositionX+1)*fScale,	fHeights[1], (PositionZ)*fScale );
		out_pVertexBuffer[nVertexOffset+2]	= GVector3( (PositionX)*fScale,		fHeights[2], (PositionZ+1)*fScale );
		out_pVertexBuffer[nVertexOffset+3]	= GVector3( (PositionX+1)*fScale,	fHeights[3], (PositionZ+1)*fScale );
		break;
	case GFACINGDIRECTION_BACK:
	case GFACINGDIRECTION_FRONT:
		out_pVertexBuffer[nVertexOffset+0] = GVector3( (PositionX+1)*fScale, fHeights[3], (PositionZ+1)*fScale );
		out_pVertexBuffer[nVertexOffset+1] = GVector3( (PositionX+1)*fScale, fHeights[1], (PositionZ)*fScale );
		if( pTileSide )
		{
			out_pVertexBuffer[nVertexOffset+2] = GVector3( (PositionX+1)*fScale, pTileSide->fHeight[2], (PositionZ+1)*fScale );
			out_pVertexBuffer[nVertexOffset+3] = GVector3( (PositionX+1)*fScale, pTileSide->fHeight[0], (PositionZ)*fScale );
		}
		else
		{
			out_pVertexBuffer[nVertexOffset+2] = GVector3( (PositionX+1)*fScale, 0, (PositionZ+1)*fScale );
			out_pVertexBuffer[nVertexOffset+3] = GVector3( (PositionX+1)*fScale, 0, (PositionZ)*fScale );
		}
		break;
	case GFACINGDIRECTION_LEFT:
	case GFACINGDIRECTION_RIGHT:
		out_pVertexBuffer[nVertexOffset+0] = GVector3( (PositionX)*fScale,		fHeights[2], (PositionZ+1)*fScale );
		out_pVertexBuffer[nVertexOffset+1] = GVector3( (PositionX+1)*fScale,	fHeights[3], (PositionZ+1)*fScale );
		if( pTileSide )
		{
			out_pVertexBuffer[nVertexOffset+2] = GVector3( (PositionX)*fScale,		pTileSide->fHeight[0], (PositionZ+1)*fScale );
			out_pVertexBuffer[nVertexOffset+3] = GVector3( (PositionX+1)*fScale,	pTileSide->fHeight[1], (PositionZ+1)*fScale );
		}
		else
		{
			out_pVertexBuffer[nVertexOffset+2] = GVector3( (PositionX)*fScale,		0, (PositionZ+1)*fScale );
			out_pVertexBuffer[nVertexOffset+3] = GVector3( (PositionX+1)*fScale,	0, (PositionZ+1)*fScale );
		}
		break;
	}
	out_pVertexBuffer[nVertexOffset+0]	+= vOptionalOffset;
	out_pVertexBuffer[nVertexOffset+1]	+= vOptionalOffset;
	out_pVertexBuffer[nVertexOffset+2]	+= vOptionalOffset;
	out_pVertexBuffer[nVertexOffset+3]	+= vOptionalOffset;	
}

void god::_AssignTileTexCoord( uint32_t nVertexOffset, const GODS(TILE_TEXTURE) pTileTextureCoords, GVector2* out_pTexCBuffer )
{
	GPROFILE_FUNCTION();
	if( 0 == out_pTexCBuffer )
	{
		error_printf("GVector2* output is NULL!");
		return;
	}
	if( pTileTextureCoords )
	{
		out_pTexCBuffer[nVertexOffset+0]	= GVector2( pTileTextureCoords->u[0], pTileTextureCoords->v[0] );
		out_pTexCBuffer[nVertexOffset+1]	= GVector2( pTileTextureCoords->u[1], pTileTextureCoords->v[1] );
		out_pTexCBuffer[nVertexOffset+2]	= GVector2( pTileTextureCoords->u[2], pTileTextureCoords->v[2] );
		out_pTexCBuffer[nVertexOffset+3]	= GVector2( pTileTextureCoords->u[3], pTileTextureCoords->v[3] );
	}
	else
	{
		out_pTexCBuffer[nVertexOffset+0]	= GVector2( 0.0f, 0.0f );
		out_pTexCBuffer[nVertexOffset+1]	= GVector2( 1.0f, 0.0f );
		out_pTexCBuffer[nVertexOffset+2]	= GVector2( 0.0f, 1.0f );
		out_pTexCBuffer[nVertexOffset+3]	= GVector2( 1.0f, 1.0f );
	}
}

void god::_AssignTileNormals( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_GEOMETRY_DATA pTileSide, GVector3* out_pNormalBuffer )
{
	GPROFILE_FUNCTION();
	if( 0 == out_pNormalBuffer )
	{
		error_printf("All outputs are NULL!");
		return;
	}
	float otherHeight = (0 != pTileSide) ? (pTileSide->fHeight[0]*fScale) : 0;
	switch( TileFacingDirection )
	{
	case GFACINGDIRECTION_TOP:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_FRONT:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( 1.0f, 0.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_RIGHT:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, 1.0f );
		}
		break;
	case GFACINGDIRECTION_BOTTOM:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_BACK:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( -1.0f, 0.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_LEFT:
		if( out_pNormalBuffer )
		{
			out_pNormalBuffer[nVertexOffset+0] = 
			out_pNormalBuffer[nVertexOffset+1] = 
			out_pNormalBuffer[nVertexOffset+2] = 
			out_pNormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, -1.0f );
		}
		break;
	}
};


void god::_AssignTileBitangents( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_TEXTURE_DATA TileTexCoord, const GTILE_GEOMETRY_DATA pTileSide, GVector3* out_pBinormalBuffer )
{
	GPROFILE_FUNCTION();
	if( 0 == out_pBinormalBuffer )
	{
		error_printf("All outputs are NULL!");
		return;
	}
	float otherHeight = (0 != pTileSide) ? (pTileSide->fHeight[0]*fScale) : 0;
	switch( TileFacingDirection )
	{
	case GFACINGDIRECTION_TOP:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, 1.0f );
		}
		break;
	case GFACINGDIRECTION_FRONT:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, 1.0f );
		}
		break;
	case GFACINGDIRECTION_RIGHT:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 1.0f, 0.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_BOTTOM:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, 1.0f );
		}
		break;
	case GFACINGDIRECTION_BACK:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 0.0f, 0.0f, 1.0f );
		}
		break;
	case GFACINGDIRECTION_LEFT:
		if( out_pBinormalBuffer )
		{
			out_pBinormalBuffer[nVertexOffset+0] = 
			out_pBinormalBuffer[nVertexOffset+1] = 
			out_pBinormalBuffer[nVertexOffset+2] = 
			out_pBinormalBuffer[nVertexOffset+3] = GVector3( 1.0f, 0.0f, 0.0f );
		}
		break;
	}
};


void god::_AssignTileTangents( GTILEFACE_DIRECTION TileFacingDirection, uint32_t nVertexOffset, const float* fHeights, float fScale, const GTILE_TEXTURE_DATA TileTexCoord, const GTILE_GEOMETRY_DATA pTileSide, GVector3* out_pTangentBuffer )
{
	GPROFILE_FUNCTION();
	if( 0 == out_pTangentBuffer )
	{
		error_printf("All outputs are NULL!");
		return;
	}
	float otherHeight = (0 != pTileSide) ? (pTileSide->fHeight[0]*fScale) : 0;
	switch( TileFacingDirection )
	{
	case GFACINGDIRECTION_TOP:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 1.0f, 0.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_FRONT:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_RIGHT:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_BOTTOM:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 1.0f, 0.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_BACK:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	case GFACINGDIRECTION_LEFT:
		if( out_pTangentBuffer )
		{
			out_pTangentBuffer[nVertexOffset+0] = 
			out_pTangentBuffer[nVertexOffset+1] = 
			out_pTangentBuffer[nVertexOffset+2] = 
			out_pTangentBuffer[nVertexOffset+3] = GVector3( 0.0f, 1.0f, 0.0f );
		}
		break;
	}
};
#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif
void god::_SetTerrainBVMinMax( const GVector3* v0, const GVector3* v1, const GVector3* v2, const GVector3* v3, 
				  GVector3* pvMinChunk, GVector3* pvMaxChunk, GVector3* pvMinSubset, GVector3* pvMaxSubset )
{
	GPROFILE_FUNCTION();
	
	if( pvMinSubset )
	{
		pvMinSubset->x = v0 ? min( pvMinSubset->x, v0->x ) : pvMinSubset->x;
		pvMinSubset->x = v1 ? min( pvMinSubset->x, v1->x ) : pvMinSubset->x;
		pvMinSubset->x = v2 ? min( pvMinSubset->x, v2->x ) : pvMinSubset->x;
		pvMinSubset->x = v3 ? min( pvMinSubset->x, v3->x ) : pvMinSubset->x;
		
		pvMinSubset->z = v0 ? min( pvMinSubset->z, v0->z ) : pvMinSubset->z;
		pvMinSubset->z = v1 ? min( pvMinSubset->z, v1->z ) : pvMinSubset->z;
		pvMinSubset->z = v2 ? min( pvMinSubset->z, v2->z ) : pvMinSubset->z;
		pvMinSubset->z = v3 ? min( pvMinSubset->z, v3->z ) : pvMinSubset->z;
		
		pvMinSubset->y = v0 ? min( pvMinSubset->y, v0->y ) : pvMinSubset->y;
		pvMinSubset->y = v1 ? min( pvMinSubset->y, v1->y ) : pvMinSubset->y;
		pvMinSubset->y = v2 ? min( pvMinSubset->y, v2->y ) : pvMinSubset->y;
		pvMinSubset->y = v3 ? min( pvMinSubset->y, v3->y ) : pvMinSubset->y;
	}
	if( pvMaxSubset )
	{
		pvMaxSubset->x = v0 ? max( pvMaxSubset->x, v0->x ) : pvMaxSubset->x; 
		pvMaxSubset->x = v1 ? max( pvMaxSubset->x, v1->x ) : pvMaxSubset->x; 
		pvMaxSubset->x = v2 ? max( pvMaxSubset->x, v2->x ) : pvMaxSubset->x; 
		pvMaxSubset->x = v3 ? max( pvMaxSubset->x, v3->x ) : pvMaxSubset->x; 

		pvMaxSubset->z = v0 ? max( pvMaxSubset->z, v0->z ) : pvMaxSubset->z; 
		pvMaxSubset->z = v1 ? max( pvMaxSubset->z, v1->z ) : pvMaxSubset->z; 
		pvMaxSubset->z = v2 ? max( pvMaxSubset->z, v2->z ) : pvMaxSubset->z; 
		pvMaxSubset->z = v3 ? max( pvMaxSubset->z, v3->z ) : pvMaxSubset->z; 

		pvMaxSubset->y = v0 ? max( pvMaxSubset->y, v0->y ) : pvMaxSubset->y; 
		pvMaxSubset->y = v1 ? max( pvMaxSubset->y, v1->y ) : pvMaxSubset->y; 
		pvMaxSubset->y = v2 ? max( pvMaxSubset->y, v2->y ) : pvMaxSubset->y; 
		pvMaxSubset->y = v3 ? max( pvMaxSubset->y, v3->y ) : pvMaxSubset->y; 
	}

	if( pvMinChunk )
	{
		pvMinChunk->x = v0 ? min( pvMinChunk->x, v0->x ) : pvMinChunk->x;
		pvMinChunk->x = v1 ? min( pvMinChunk->x, v1->x ) : pvMinChunk->x;
		pvMinChunk->x = v2 ? min( pvMinChunk->x, v2->x ) : pvMinChunk->x;
		pvMinChunk->x = v3 ? min( pvMinChunk->x, v3->x ) : pvMinChunk->x;

		pvMinChunk->z = v0 ? min( pvMinChunk->z, v0->z ) : pvMinChunk->z;
		pvMinChunk->z = v1 ? min( pvMinChunk->z, v1->z ) : pvMinChunk->z;
		pvMinChunk->z = v2 ? min( pvMinChunk->z, v2->z ) : pvMinChunk->z;
		pvMinChunk->z = v3 ? min( pvMinChunk->z, v3->z ) : pvMinChunk->z;

		pvMinChunk->y = v0 ? min( pvMinChunk->y, v0->y ) : pvMinChunk->y;
		pvMinChunk->y = v1 ? min( pvMinChunk->y, v1->y ) : pvMinChunk->y;
		pvMinChunk->y = v2 ? min( pvMinChunk->y, v2->y ) : pvMinChunk->y;
		pvMinChunk->y = v3 ? min( pvMinChunk->y, v3->y ) : pvMinChunk->y;
	}
	if( pvMaxChunk )
	{
		pvMaxChunk->x = v0 ? max( pvMaxChunk->x, v0->x ) : pvMaxChunk->x; 
		pvMaxChunk->x = v1 ? max( pvMaxChunk->x, v1->x ) : pvMaxChunk->x; 
		pvMaxChunk->x = v2 ? max( pvMaxChunk->x, v2->x ) : pvMaxChunk->x; 
		pvMaxChunk->x = v3 ? max( pvMaxChunk->x, v3->x ) : pvMaxChunk->x; 

		pvMaxChunk->z = v0 ? max( pvMaxChunk->z, v0->z ) : pvMaxChunk->z; 
		pvMaxChunk->z = v1 ? max( pvMaxChunk->z, v1->z ) : pvMaxChunk->z; 
		pvMaxChunk->z = v2 ? max( pvMaxChunk->z, v2->z ) : pvMaxChunk->z; 
		pvMaxChunk->z = v3 ? max( pvMaxChunk->z, v3->z ) : pvMaxChunk->z; 

		pvMaxChunk->y = v0 ? max( pvMaxChunk->y, v0->y ) : pvMaxChunk->y; 
		pvMaxChunk->y = v1 ? max( pvMaxChunk->y, v1->y ) : pvMaxChunk->y; 
		pvMaxChunk->y = v2 ? max( pvMaxChunk->y, v2->y ) : pvMaxChunk->y; 
		pvMaxChunk->y = v3 ? max( pvMaxChunk->y, v3->y ) : pvMaxChunk->y; 
	}

}

//
//The relevant input data to your problem are the texture coordinates. Tangent and Binormal are vectors locally parallel to the object's surface. 
// And in the case of normal mapping they're describing the local orientation of the normal texture.
// So what you have to do is calculate the direction (in the model's space) in which the texturing vectors point. 
//
// Say you have a triangle ABC, with texture coordinates HKL. This gives us vectors:
//D = B-A
//E = C-A
// 
//F = K-H
//G = L-H
//
//Now we want to express D and E in terms of tangent space T, U, i.e.
//D = F.s * T + F.t * U
//E = G.s * T + G.t * U
//
//This is a system of linear equations with 6 unknowns and 6 equations, it can be written as
//| D.x D.y D.z |   | F.s F.t | | T.x T.y T.z |
//|             | = |         | |             |
//| E.x E.y E.z |   | G.s G.t | | U.x U.y U.z |
//
//Inverting the FG matrix yields
//| T.x T.y T.z |           1         |  G.t  -F.t | | D.x D.y D.z |
//|             | = ----------------- |            | |             |
//| U.x U.y U.z |   F.s G.t - F.t G.s | -G.s   F.s | | E.x E.y E.z |
//
//Together with the vertex normal T and U form a local space basis, called the tangent space, described by the matrix
//| T.x U.x N.x |
//| T.y U.y N.y |
//| T.z U.z N.z |
//
//transforming from tangent space into object space. To do lighting calculations one needs the inverse of this. With a little bit of exercise one finds:
//T' = T - (N·T) N
//U' = U - (N·U) N - (T'·U) T'
//
//normalizing the vectors T' and U', calling them tangent and binormal we obtain the matrix transforming from object into tangent space, where we do the lighting:
//| T'.x T'.y T'.z |
//| U'.x U'.y U'.z |
//| N.x  N.y  N.z  |
//
// we store T' and U' them together with the vertex normal as a part of the model's geometry (as vertex attributes), so that we can use them in the shader for lighting calculations. 
// I repeat: You don't determine tangent and binormal in the shader, you precompute them and store them as part of the model's geometry (just like normals).

// void god::_AssignTileTangents( GFACING_DIRECTION TileFacingDirection, uint32_t nVertexOffset, uint32_t PositionX, uint32_t PositionZ, float fScale, const float* in_fHeights, 
// 	const GTILE_TEXTURE_DATA in_TileTextureData, const GVector3& vOptionalOffset, const GTILE_GEOMETRY_DATA in_pTileSide, GVector3* out_pTangentBuffer, GVector3* out_pBinormalBuffer )
// {
// 	GVector3 v0, v1, v2, v3;
// 	switch( TileFacingDirection )
// 	{
// 	case GFACINGDIRECTION_TOP:
// 		v0	= GVector3( (PositionX)*fScale,		in_fHeights[0], (PositionZ)*fScale );
// 		v1	= GVector3( (PositionX+1)*fScale,	in_fHeights[1], (PositionZ)*fScale );
// 		v2	= GVector3( (PositionX)*fScale,		in_fHeights[2], (PositionZ+1)*fScale );
// 		v3	= GVector3( (PositionX+1)*fScale,	in_fHeights[3], (PositionZ+1)*fScale );
// 		break;
// 	case GFACINGDIRECTION_FRONT:
// 		v0 = GVector3( (PositionX+1)*fScale, in_fHeights[3], (PositionZ+1)*fScale );
// 		v1 = GVector3( (PositionX+1)*fScale, in_fHeights[1], (PositionZ)*fScale );
// 		if( in_pTileSide )
// 		{
// 			v2 = GVector3( (PositionX+1)*fScale, in_pTileSide->fHeight[2], (PositionZ+1)*fScale );
// 			v3 = GVector3( (PositionX+1)*fScale, in_pTileSide->fHeight[0], (PositionZ)*fScale );
// 		}
// 		else
// 		{
// 			v2 = GVector3( (PositionX+1)*fScale, 0, (PositionZ+1)*fScale );
// 			v3 = GVector3( (PositionX+1)*fScale, 0, (PositionZ)*fScale );
// 		}
// 		break;
// 	case GFACINGDIRECTION_RIGHT:
// 		v0 = GVector3( (PositionX)*fScale,		in_fHeights[2], (PositionZ+1)*fScale );
// 		v1 = GVector3( (PositionX+1)*fScale,	in_fHeights[3], (PositionZ+1)*fScale );
// 		if( in_pTileSide )
// 		{
// 			v2 = GVector3( (PositionX)*fScale,		in_pTileSide->fHeight[0], (PositionZ+1)*fScale );
// 			v3 = GVector3( (PositionX+1)*fScale,	in_pTileSide->fHeight[1], (PositionZ+1)*fScale );
// 		}
// 		else
// 		{
// 			v2 = GVector3( (PositionX)*fScale,		0, (PositionZ+1)*fScale );
// 			v3 = GVector3( (PositionX+1)*fScale,	0, (PositionZ+1)*fScale );
// 		}
// 		break;
// 	}
// 
// 	v0	+= vOptionalOffset;
// 	v1	+= vOptionalOffset;
// 	v2	+= vOptionalOffset;
// 	v3	+= vOptionalOffset;	
// 
// 	GVector2 H = GVector2( in_TileTextureData->u[0], in_TileTextureData->v[0] ), 
// 		K = GVector2( in_TileTextureData->u[1], in_TileTextureData->v[1] ), 
// 		L = GVector2( in_TileTextureData->u[2], in_TileTextureData->v[2] ),
// 		M = GVector2( in_TileTextureData->u[3], in_TileTextureData->v[3] );
// //
// //The relevant input data to your problem are the texture coordinates. Tangent and Binormal are vectors locally parallel to the object's surface. 
// // And in the case of normal mapping they're describing the local orientation of the normal texture.
// // So what you have to do is calculate the direction (in the model's space) in which the texturing vectors point. 
// //
// // Say you have a triangle ABC, with texture coordinates HKL. This gives us vectors:
// 	GVector3 D, E;
// 	GVector2 F, G;
// //D = B-A
// //E = C-A
// 	D = v1-v0;
// 	E = v2-v0;
// //F = K-H
// //G = L-H
// 	F = K-H;
// 	G = L-H;
// 	
// //Now we want to express D and E in terms of tangent space T, U, i.e.
// //D = F.s * T + F.t * U
// //E = G.s * T + G.t * U
// 	
// //This is a system of linear equations with 6 unknowns and 6 equations, it can be written as:
// //| D.x D.y D.z |   | F.s F.t | | T.x T.y T.z |
// //|             | = |         | |             |
// //| E.x E.y E.z |   | G.s G.t | | U.x U.y U.z |
// 
// //Inverting the FG matrix yields
// //| T.x T.y T.z |           1         |  G.t  -F.t | | D.x D.y D.z |
// //|             | = ----------------- |            | |             |
// //| U.x U.y U.z |   F.s G.t - F.t G.s | -G.s   F.s | | E.x E.y E.z |
// 	float myd = 1.0f/( (F.x*G.y) - (F.y*G.x) );
// 	// as we use another convention, we have to transpose the DE matrix and change the order of the matrix multiplication
// 	GMatrix2 FGMatrix;
// 	GMatrix2x3 TBMatrix;
// 	FGMatrix._11 = G.x; 	FGMatrix._12 = -G.y;
// 	FGMatrix._21 = -F.x; 	FGMatrix._22 = F.y;
// 	TBMatrix._11 = D.x;	TBMatrix._12 = E.x;			
// 	TBMatrix._21 = D.y;	TBMatrix._22 = E.y;	
// 	TBMatrix._31 = D.z; TBMatrix._32 = E.z;
// 
// 	FGMatrix = FGMatrix*myd;
// 	TBMatrix = TBMatrix*FGMatrix;
// 
// 	//out_pTangentBuffer[nVertexOffset] = GVector3( 
// 
// 	//Together with the vertex normal, T and U form a local space basis called the tangent space, described by the matrix:
// //| T.x U.x N.x |
// //| T.y U.y N.y |
// //| T.z U.z N.z |
// //
// //transforming from tangent space into object space. To do lighting calculations one needs the inverse of this. With a little bit of exercise one finds:
// //T' = T - (N·T) N
// //U' = U - (N·U) N - (T'·U) T'
// //
// //normalizing the vectors T' and U', calling them tangent and binormal we obtain the matrix transforming from object into tangent space, where we do the lighting:
// //| T'.x T'.y T'.z |
// //| U'.x U'.y U'.z |
// //| N.x  N.y  N.z  |
// //
// // we store T' and U' them together with the vertex normal as a part of the model's geometry (as vertex attributes), so that we can use them in the shader for lighting calculations. 
// // I repeat: You don't determine tangent and binormal in the shader, you precompute them and store them as part of the model's geometry (just like normals).
// };