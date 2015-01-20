
#define __GDEFINE_GAMELIB_TILE_MAP_ACCESSORS( TOKEN )																					\
	GODS(TOKEN) god::getFrontTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )				\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-1 )																							\
			return gacquireData( in_lstTiles[nCurrentTileIndex+1] );																	\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )				\
	{																																	\
		if( nCurrentTileIndex > 0 )																										\
			return gacquireData( in_lstTiles[nCurrentTileIndex-1] );																	\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getRightTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )				\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-nWidth )																					\
			return gacquireData( in_lstTiles[nCurrentTileIndex+nWidth] );																\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getLeftTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )				\
	{																																	\
		if( nCurrentTileIndex > nWidth )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-nWidth] );																\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getFrontRightTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-(nWidth+1) )																				\
			return gacquireData( in_lstTiles[nCurrentTileIndex+(nWidth+1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackRightTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-(nWidth-1) )																				\
			return gacquireData( in_lstTiles[nCurrentTileIndex+(nWidth-1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getFrontLeftTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		if( nCurrentTileIndex > nWidth-1 )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-(nWidth-1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackLeftTile( GODS(TOKEN)* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		if( nCurrentTileIndex > nWidth+1 )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-(nWidth+1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getTile( GODS(TOKEN)* in_lstTiles, uint32_t nTileCount, uint32_t nTileIndex )										\
	{																																	\
		if( nTileIndex < nTileCount )																									\
			return gacquireData( in_lstTiles[nTileIndex] );																				\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getFrontTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )		\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-1 )																							\
			return gacquireData( in_lstTiles[nCurrentTileIndex+1] );																	\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		if( nCurrentTileIndex > 0 )																										\
			return gacquireData( in_lstTiles[nCurrentTileIndex-1] );																	\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getRightTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )		\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-nWidth )																					\
			return gacquireData( in_lstTiles[nCurrentTileIndex+nWidth] );																\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getLeftTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )			\
	{																																	\
		if( nCurrentTileIndex > nWidth )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-nWidth] );																\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getFrontRightTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )	\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-(nWidth+1) )																				\
			return gacquireData( in_lstTiles[nCurrentTileIndex+(nWidth+1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackRightTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )	\
	{																																	\
		int32_t nTotalTiles = nWidth*nDepth;																							\
		if( nCurrentTileIndex < nTotalTiles-(nWidth-1) )																				\
			return gacquireData( in_lstTiles[nCurrentTileIndex+(nWidth-1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getFrontLeftTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )	\
	{																																	\
		if( nCurrentTileIndex > nWidth-1 )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-(nWidth-1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getBackLeftTile( GODS(TOKEN) const* in_lstTiles, int32_t nCurrentTileIndex, uint16_t nWidth, uint16_t nDepth )		\
	{																																	\
		if( nCurrentTileIndex > nWidth+1 )																								\
			return gacquireData( in_lstTiles[nCurrentTileIndex-(nWidth+1)] );															\
		return 0;																														\
	};																																	\
																																		\
	GODS(TOKEN) god::getTile( GODS(TOKEN) const* in_lstTiles, uint32_t nTileCount, uint32_t nTileIndex )								\
	{																																	\
		if( nTileIndex < nTileCount )																									\
			return gacquireData( in_lstTiles[nTileIndex] );																				\
		return 0;																														\
	};

#define __GDEFINE_GETAREATILES_FUNCTION( TYPE )																					\
	error_t god::getAreaTiles( TYPE* in_lstTileDataGlobal, uint32_t nSourceTileCount, uint32_t nSourceWidth,					\
		uint32_t nAreaTileOffsetX, uint32_t nAreaTileOffsetZ, uint32_t* nAreaTileCountX, uint32_t* nAreaTileCountZ,				\
						 TYPE* out_lstTileDataArea, uint32_t* out_lstAreaToGlobalTileMap )										\
	{																															\
		if( 0 == in_lstTileDataGlobal																							\
			|| nSourceWidth > nSourceTileCount																					\
			|| 0 == nAreaTileCountX																								\
			|| 0 == nAreaTileCountZ )																							\
		{																														\
			error_printf("Invalid parameters received at getAreaTiles()!");														\
			return -1;																											\
		}																														\
		uint32_t nGlobalTileIndex,																								\
			nAreaTileIndex = 0,																									\
			nRequestedSizeX = *nAreaTileCountX,																					\
			nRequestedSizeZ = *nAreaTileCountZ,																					\
			nLocalTileX,																										\
			nLocalTileZ;																										\
		gcore_ptr<TYPE>	oldTile;																								\
		for( nAreaTileIndex = nLocalTileZ = 0; nLocalTileZ<nRequestedSizeZ; nLocalTileZ++ )										\
		{																														\
			nGlobalTileIndex	= (nAreaTileOffsetZ+nLocalTileZ)*nSourceWidth+nAreaTileOffsetX;									\
			if( nGlobalTileIndex >= nSourceTileCount )																			\
				break;/* Stop processing if we reached the end of the tile list as there would be no more rows to process */	\
			for( nLocalTileX = 0; nLocalTileX<nRequestedSizeX; nLocalTileX++ )													\
			{																													\
				nGlobalTileIndex	= (nAreaTileOffsetZ+nLocalTileZ)*nSourceWidth+nLocalTileX+nAreaTileOffsetX;					\
				/* Note that missing some tiles doesn't mean it failed, but that the selection area is out of boundaries. */	\
				if( nGlobalTileIndex >= nSourceTileCount )																		\
					break; 		/* We simply skip the remaining columns and move to the next row */								\
				if( out_lstTileDataArea )																						\
				{																												\
					oldTile	= out_lstTileDataArea[nAreaTileIndex];																\
					out_lstTileDataArea[nAreaTileIndex] = gacquireData( in_lstTileDataGlobal[nGlobalTileIndex] );				\
					/*gfreeData( &oldTile	);*/																				\
				}																												\
				if( out_lstAreaToGlobalTileMap )																				\
					out_lstAreaToGlobalTileMap[nAreaTileIndex] = nGlobalTileIndex;												\
				nAreaTileIndex++;																								\
			}																													\
		}																														\
		*nAreaTileCountX	= nLocalTileX;																						\
		*nAreaTileCountZ	= nLocalTileZ;																						\
		return 0;																												\
	}
