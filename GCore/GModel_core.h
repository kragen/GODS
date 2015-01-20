#include "gcore_ptr.h"
#include "garray.h"
#include "glist.h"
#include "GText_core.h"
#include "GGeometry_core.h"

#ifndef _GMODEL_CORE_H
#define _GMODEL_CORE_H

namespace god
{
#pragma pack( push, 1 )
	GDECLARE_CORE_STRUCT( TRIANGLE, Triangle,
		uint16_t		TextureIndex;				// texture index
		uint16_t		ProgramIndex;				// shader index
		uint16_t		TwoSided;					// 
		uint16_t		SmoothGroup;				// normal blend
	);
	static void onCreateTriangleDefault( ggns_Triangle* stuff )
	{
		memset( (*stuff).get_data_address(), -1, (*stuff).get_data_size() );
	};

	// I can hardly imagine a RSM mesh with more than 65535 subsets, 
	// so I will limit to 16 bit indices and 64 attributes max
	int32_t BuildMeshGeometryFromMeshElements( 
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
										uint32_t dwFlags 
									);

#pragma pack( pop )

};


#endif // _GMODEL_CORE_H