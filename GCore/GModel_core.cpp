/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GModel_core.h"
#include "GMemory_core.h"
#include "GCore_function_macros.h"

#ifdef _WIN32
#include <strsafe.h>
#endif
#include <math.h>

#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

#include <stdlib.h>

using namespace god;

GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( TRIANGLE, Triangle );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TRIANGLE),
	//"VertexIndices[3].............: [%i, %i, %i]\n"
	//"TriangleTextureIndices[3]....: [%i, %i, %i]\n"
	"TextureIndex.................: %i\n"
	"ProgramIndex.................: %i\n"
	"TwoSided.....................: %i\n"
	"SmoothGroup..................: %i\n"
	//"NodeIndex....................: %i"
	//, (int)in_CoreInstance->VertexIndices[0]
	//, (int)in_CoreInstance->VertexIndices[1]
	//, (int)in_CoreInstance->VertexIndices[2]
	//, (int)in_CoreInstance->TexcoordIndices[0]
	//, (int)in_CoreInstance->TexcoordIndices[1]
	//, (int)in_CoreInstance->TexcoordIndices[2]
	, (int)in_CoreInstance->TextureIndex
	, (int)in_CoreInstance->ProgramIndex
	, (int)in_CoreInstance->TwoSided
	, (int)in_CoreInstance->SmoothGroup
	//, (int)in_CoreInstance->NodeIndex
	);

error_t AssignTriangleIndices( bool bTwoSided, uint32_t nVertexCount, uint32_t nIndexCount, bool b32BitIndices, void* out_pIndices )
{
	if( 0 == out_pIndices )
	{
		error_printf("Cannot assign indices to a null address!");
		return -1;
	}
	if( b32BitIndices )
	{	// we invert order because we negated y
		((uint32_t*)out_pIndices)[nIndexCount+0]	= nVertexCount+0;
		((uint32_t*)out_pIndices)[nIndexCount+1]	= nVertexCount+2;
		((uint32_t*)out_pIndices)[nIndexCount+2]	= nVertexCount+1;
		if( bTwoSided )
		{	// indices are useful for 2 sided faces
			((uint32_t*)out_pIndices)[nIndexCount+3]	= nVertexCount+0;
			((uint32_t*)out_pIndices)[nIndexCount+4]	= nVertexCount+1;
			((uint32_t*)out_pIndices)[nIndexCount+5]	= nVertexCount+2;						
		}
	}
	else	// 16 bit indices
	{
		((uint16_t*)out_pIndices)[nIndexCount+0]	= nVertexCount+0;
		((uint16_t*)out_pIndices)[nIndexCount+1]	= nVertexCount+2;
		((uint16_t*)out_pIndices)[nIndexCount+2]	= nVertexCount+1;
		if( bTwoSided )
		{
			((uint16_t*)out_pIndices)[nIndexCount+3]	= nVertexCount+0;
			((uint16_t*)out_pIndices)[nIndexCount+4]	= nVertexCount+1;
			((uint16_t*)out_pIndices)[nIndexCount+5]	= nVertexCount+2;						
		}
	}
	return 0;
}

error_t god::BuildMeshGeometryFromMeshElements( 
										uint32_t																	in_nFrameCount,
										int32_t																		in_nShadeType,
										uint8_t																		in_cAlpha,				// alpha of the model? (default 0xff)
										uint32_t																	in_nRootNodeIndex,
										//glist<GODS(TEXT_SMALL)>*													in_lstTextureNames,	
										//glist<GODS(TEXT_SMALL)>*													in_lstShaderNames,	
										glist<GODS(TEXT_SMALL)>*													in_lstNodeNames,	
										//gelementary<uint16_t>*														in_lstParentIndices,	
										glist<GODS(BUFFERSLICE)>*													in_lstNodeSlices,
										glist<GODS(BUFFERSLICE)>*													in_lstNodeTexCoordSlices,
										glist<GODS(BUFFERSLICE)>*													in_lstNodeColorSlices,
										glist<GODS(RANGE)>*															in_lstNodeTextureIndexRanges,
										glist<GODS(RANGE)>*															in_lstNodeShaderIndexRanges,
										glist<GODS(PIVOTFRAME)>*													in_lstNodePivots,
										glist<GODS(RANGE)>*															in_lstNodePositionFrameRanges,
										glist<GODS(RANGE)>*															in_lstNodeOrientationFrameRanges,
										glist<GODS(PIVOTFRAME)>*													in_lstVolumeBox,	
										gelementary<uint16_t>*														in_lstTextureIndices,	
										gelementary<uint16_t>*														in_lstShaderIndices,	
										gelementaryex2<uint16_t, GDATATYPE_UINT16, GUSAGE_INDEX>*					in_lstVertexIndices,	
										gelementaryex2<uint16_t, GDATATYPE_UINT16, GUSAGE_INDEX>*					in_lstTexCoordIndices,	
										gelementaryex2<uint16_t, GDATATYPE_UINT16, GUSAGE_INDEX>*					in_lstColorIndices,	
										gelementaryex2<GVector2, GDATATYPE_FLOAT32_2, GUSAGE_TEXCOORD>*				in_lstTexCoord,		
										gelementaryex2<GVector3, GDATATYPE_FLOAT32_3, GUSAGE_POSITION>*				in_lstVertices,		// float[3*nVertexCount] // vertex/point coordinate
										gelementaryex2<uint32_t, GDATATYPE_UINT32, GUSAGE_VERTEXCOLOR>*				in_lstColor,	
										gelementaryex2<GMatrix3, GDATATYPE_FLOAT32_9, GUSAGE_TRANSFORM>*			in_lstNodeTransform,		// float[3*nVertexCount] // vertex/point coordinate
										gelementaryex2<GVector3, GDATATYPE_FLOAT32_3, GUSAGE_POSITION>*				in_lstNodeOffsets,		// float[3*nVertexCount] // vertex/point coordinate
										glist<GODS(TRIANGLE)>*														in_lstTriangleData,
										glist<GODS(PIVOTFRAME)>*													in_lstPositionFrameData,
										glist<GODS(PIVOTFRAME)>*													in_lstOrientationFrameData,
										uint32_t* inout_nVertexCount, GVector3* out_pVertices, GVector2* out_pTexCoord, GVector3* out_pNormals, 
										uint32_t* inout_nIndexCount, bool* b32BitIndices, void* out_pIndices, 
										uint32_t* inout_nSubsetCount, uint32_t* inout_nNodeCount, 
										GODS(BUFFERSLICE)* out_lstNodeSlice,
										GODS(PIVOTFRAME)* out_lstNodePivot,
										GODS(BOUNDINGVOLUME)* out_lstNodeBV,
										GODS(BUFFERSLICE)*	out_lstSubsetSlice,
										GODS(BOUNDINGVOLUME)* out_lstSubsetBV,
										uint32_t* out_lstSubsetToNodeMap,
										uint32_t* out_lstSubsetToTextureMap,
										uint32_t* out_lstSubsetToShaderMap,
										uint32_t* out_lstTriangleToSubsetMap,
										uint32_t dwFlags )
{
	if( 0 == inout_nVertexCount || 0 == inout_nIndexCount )
		return -1;

	bool bCountOnly = false;
	if( 0 == out_pVertices )
		bCountOnly = true;

	uint32_t nNodeCount=in_lstNodeSlices->size();	//nNodeCount;
	if( 0 == nNodeCount )
		return 0;

	uint32_t nTriangleCount=0, nIndexCount=0, nVertexCount=0, nFinalNodeCount=0, nFinalSubsetCount=0;
#ifndef min
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#endif	
	GENIUSS(PIVOTFRAME) newNodePivot(onCreatePivotFrameDefault,0,0), oldPivot;
	GENIUSS(BOUNDINGVOLUME) newNodeBV(onCreateBoundingVolumeDefault,0,0), newSubsetBV(onCreateBoundingVolumeDefault,0,0), oldBV;
	GENIUSS(BUFFERSLICE) newSubsetSlice(onCreateBufferSliceDefault,0,0), newNodeSlice(onCreateBufferSliceDefault,0,0), oldSlice;
	for( uint32_t iNode=0; iNode<nNodeCount; iNode++ )
	{
		newNodeBV.create();
		newNodeBV->nType = GBOUNDINGPRIMITIVE_BOX;
		newNodeSlice.create();
		newNodeSlice->ElementOffset = nVertexCount;
		newNodeSlice->IndexOffset	= nIndexCount;

		GOPTR(BUFFERSLICE) nodeSlice = gacquireData( (*in_lstNodeSlices)[iNode] );
		GOPTR(BUFFERSLICE) texCoordSlice = gacquireData( (*in_lstNodeTexCoordSlices)[iNode] );
		GOPTR(BUFFERSLICE) colorSlice = gacquireData( (*in_lstNodeColorSlices)[iNode] );
		GOPTR(RANGE) texIndexSlice = gacquireData( (*in_lstNodeTextureIndexRanges)[iNode] );
		GOPTR(RANGE) shaderIndexSlice = gacquireData( (*in_lstNodeShaderIndexRanges)[iNode] );
		
		for( uint32_t nShaderCount = shaderIndexSlice->Count, iShader=0; iShader<nShaderCount; iShader++ )
		{
			for( uint32_t nTextureCount = texIndexSlice->Count, iTex=0; iTex<nTextureCount; iTex++ )
			{
				newSubsetSlice.create();
				newSubsetSlice->ElementOffset	= nVertexCount;
				newSubsetSlice->IndexOffset		= nTriangleCount*3;
				newSubsetBV.create();
				newSubsetBV->nType	= GBOUNDINGPRIMITIVE_BOX;
				newSubsetBV->vCenter = newSubsetBV->vMax	= newSubsetBV->vMin		= GVECTOR3_ZERO;
				if( out_lstSubsetToTextureMap )
					out_lstSubsetToTextureMap[nFinalSubsetCount]	= (*in_lstTextureIndices)[texIndexSlice->Offset+iTex];
				if( out_lstSubsetToShaderMap )
					out_lstSubsetToShaderMap[nFinalSubsetCount]		= (*in_lstShaderIndices)[shaderIndexSlice->Offset+iShader];

				GOPTR(TRIANGLE) Triangle;
				uint32_t nTriangleOffset =  nodeSlice->IndexOffset/3;
				for( uint32_t iTri=0, nMaxTriangles = nodeSlice->IndexCount/3; iTri<nMaxTriangles; iTri++ )
				{
					Triangle = gacquireData((*in_lstTriangleData)[nTriangleOffset+iTri]);
					uint32_t indexOffset = nodeSlice->IndexOffset+(iTri*3);
					uint32_t texCindexOffset = texCoordSlice->IndexOffset+iTri*3;
					if( Triangle->TextureIndex != iTex || Triangle->ProgramIndex != iShader )
						continue;

					GVector3 Vert0	= (*in_lstVertices)[nodeSlice->ElementOffset+(*in_lstVertexIndices)[indexOffset+0]], 
						Vert1		= (*in_lstVertices)[nodeSlice->ElementOffset+(*in_lstVertexIndices)[indexOffset+1]], 
						Vert2		= (*in_lstVertices)[nodeSlice->ElementOffset+(*in_lstVertexIndices)[indexOffset+2]];
					GMatrix3 m = (*in_lstNodeTransform)[iNode];
					GVector3 vOffset = (*in_lstNodeOffsets)[iNode];
					if( out_pVertices || out_pNormals || out_lstSubsetBV || out_lstNodeBV )
					{
						Vert0 *= m; Vert1 *= m; Vert2 *= m;
						if( in_lstNodeSlices->size() > 1 )
						{
							Vert0 += vOffset; Vert1 += vOffset; Vert2 += vOffset;
						}
					}
					if( out_pVertices )
					{
						out_pVertices[nVertexCount+0]	= Vert0;
						out_pVertices[nVertexCount+1]	= Vert1;
						out_pVertices[nVertexCount+2]	= Vert2;
					}

					if( out_lstSubsetBV || out_lstNodeBV )
					{
						newSubsetBV->vMax.x = max( Vert0.x, newSubsetBV->vMax.x ); newSubsetBV->vMin.x = min( Vert0.x, newSubsetBV->vMin.x );
						newSubsetBV->vMax.y = max( Vert0.y, newSubsetBV->vMax.y ); newSubsetBV->vMin.y = min( Vert0.y, newSubsetBV->vMin.y );
						newSubsetBV->vMax.z = max( Vert0.z, newSubsetBV->vMax.z ); newSubsetBV->vMin.z = min( Vert0.z, newSubsetBV->vMin.z );
						newSubsetBV->vMax.x = max( Vert1.x, newSubsetBV->vMax.x ); newSubsetBV->vMin.x = min( Vert1.x, newSubsetBV->vMin.x );
						newSubsetBV->vMax.y = max( Vert1.y, newSubsetBV->vMax.y ); newSubsetBV->vMin.y = min( Vert1.y, newSubsetBV->vMin.y );
						newSubsetBV->vMax.z = max( Vert1.z, newSubsetBV->vMax.z ); newSubsetBV->vMin.z = min( Vert1.z, newSubsetBV->vMin.z );
						newSubsetBV->vMax.x = max( Vert2.x, newSubsetBV->vMax.x ); newSubsetBV->vMin.x = min( Vert2.x, newSubsetBV->vMin.x );
						newSubsetBV->vMax.y = max( Vert2.y, newSubsetBV->vMax.y ); newSubsetBV->vMin.y = min( Vert2.y, newSubsetBV->vMin.y );
						newSubsetBV->vMax.z = max( Vert2.z, newSubsetBV->vMax.z ); newSubsetBV->vMin.z = min( Vert2.z, newSubsetBV->vMin.z );
						GVector3 dif = (newSubsetBV->vMax - newSubsetBV->vMin);
						newSubsetBV->vCenter = newSubsetBV->vMin + (dif/2);	// ? I'm not really sure about this right now
						newSubsetBV->fRadius = (newSubsetBV->vMax - newSubsetBV->vCenter).Length();
					}
					if( out_pIndices )
					{
						if( 0 > AssignTriangleIndices( Triangle->TwoSided ? true : false, nVertexCount, nIndexCount, *b32BitIndices, out_pIndices ) )
						{
							error_printf("AssignTriangleIndices() FAILED!");
							return -1;
						}
					}

					if( out_pTexCoord )
					{	// TEXCOORD 
						out_pTexCoord[nVertexCount + 0] = (*in_lstTexCoord)[texCoordSlice->ElementOffset+(*in_lstTexCoordIndices)[texCoordSlice->IndexOffset+iTri*3+0]]; //1.0f-
						out_pTexCoord[nVertexCount + 1] = (*in_lstTexCoord)[texCoordSlice->ElementOffset+(*in_lstTexCoordIndices)[texCoordSlice->IndexOffset+iTri*3+1]]; //
						out_pTexCoord[nVertexCount + 2] = (*in_lstTexCoord)[texCoordSlice->ElementOffset+(*in_lstTexCoordIndices)[texCoordSlice->IndexOffset+iTri*3+2]]; //1.0f-
					}
					if( out_pNormals )
					{ // this may be wrong but it works fine ( I think )
						out_pNormals[nVertexCount+0]	= (Vert2-Vert0).Cross( Vert1-Vert0 );
						out_pNormals[nVertexCount+1]	= (Vert0-Vert1).Cross( Vert2-Vert1 );
						out_pNormals[nVertexCount+2]	= (Vert1-Vert2).Cross( Vert0-Vert2 );
					}
					if( out_lstTriangleToSubsetMap ) 
						out_lstTriangleToSubsetMap[nTriangleCount]	= nFinalSubsetCount;

					nIndexCount += 3; 
					nVertexCount += 3; 
					nTriangleCount++;
					if( 0 != Triangle->TwoSided )
					{
						nIndexCount +=3;
						nTriangleCount++;
					}
				} // for( triangles )
			
				newSubsetSlice->ElementCount = nVertexCount - newSubsetSlice->ElementOffset;
				newSubsetSlice->IndexCount = (nTriangleCount*3) - newSubsetSlice->IndexOffset;
				if( newSubsetSlice->ElementCount > 0 )
				{
					if( out_lstSubsetSlice )
					{
						oldSlice = out_lstSubsetSlice[nFinalSubsetCount];
						out_lstSubsetSlice[nFinalSubsetCount] = newSubsetSlice.acquire();
					}
					if( out_lstSubsetBV )
					{
						oldBV = out_lstSubsetBV[nFinalSubsetCount];
						out_lstSubsetBV[nFinalSubsetCount] = newSubsetBV.acquire();
					}


					if( out_lstNodeBV )
					{
						newNodeBV->vMax.x = max( newSubsetBV->vMax.x, newNodeBV->vMax.x ); newNodeBV->vMin.x = min( newSubsetBV->vMin.x, newNodeBV->vMin.x );
						newNodeBV->vMax.y = max( newSubsetBV->vMax.y, newNodeBV->vMax.y ); newNodeBV->vMin.y = min( newSubsetBV->vMin.y, newNodeBV->vMin.y );
						newNodeBV->vMax.z = max( newSubsetBV->vMax.z, newNodeBV->vMax.z ); newNodeBV->vMin.z = min( newSubsetBV->vMin.z, newNodeBV->vMin.z );
					}
					if( out_lstSubsetToNodeMap )
						out_lstSubsetToNodeMap[nFinalSubsetCount] = iNode;
					//if( out_lstSubsetToTextureMap )
					//	out_lstSubsetToTextureMap[nFinalSubsetCount] = iTex;
					//if( out_lstSubsetToShaderMap )
					//	out_lstSubsetToShaderMap[nFinalSubsetCount] = iShader;

					nFinalSubsetCount++;
				}
				else
				{
					warning_printf("Node attribute vertex count is zero!");
				}
				// we skip empty attributes
			}	// for( texture )
		}	// for( shader )
		if( out_lstNodeSlice )
		{
			newNodeSlice->ElementOffset = nVertexCount-newNodeSlice->ElementOffset;
			newNodeSlice->IndexOffset	= nIndexCount-newNodeSlice->IndexOffset;
			oldSlice = out_lstNodeSlice[iNode];
			out_lstNodeSlice[iNode] = newNodeSlice.acquire();
		}
		if( out_lstNodeBV )
		{
			GVector3 dif = (newNodeBV->vMax - newNodeBV->vMin);
			newNodeBV->vCenter = newNodeBV->vMin + (newNodeBV->vMax - newNodeBV->vMin)/2;	// ? I'm not really sure about this right now
			newNodeBV->fRadius = (newNodeBV->vMax - newNodeBV->vCenter).Length();
			oldBV = out_lstNodeBV[iNode];
			out_lstNodeBV[iNode] = newNodeBV.acquire();
		}
		if( out_lstNodePivot )
		{
			oldPivot = out_lstNodePivot[iNode];
			out_lstNodePivot[iNode] = in_lstNodePivots->acquire(iNode);	// gacquireData( pRSMNode->PivotData );
		}

		nFinalNodeCount++;
	}; // for( nNodeCount )

	if( nVertexCount > 0xFFFF )
		*b32BitIndices = true;
	else 
		*b32BitIndices = false;

	*inout_nIndexCount	= nIndexCount;
	*inout_nVertexCount	= nVertexCount;
	if( inout_nSubsetCount )
		*inout_nSubsetCount = nFinalSubsetCount;
	if( inout_nNodeCount )
		*inout_nNodeCount	= nFinalNodeCount;

	return 0;
};


