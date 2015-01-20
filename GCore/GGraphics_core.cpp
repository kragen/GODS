
/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GGraphics_core.h"
#include "GMemory_core.h"
#include "GCore_function_macros.h"
#include <iostream>
#include <math.h>
#ifdef _WIN32
#include <strsafe.h>
#endif
#include "glist.h"

#include "GCore_printf.h"
#include "GDebugger_core.h"

using namespace god;

	__GCORE_STRUCT_INIT_STATIC_MEMBERS( VIEWPORT, Viewport )
	__GDEFINE_CREATEDATA_FUNCTION_NO_MEMBER_INIT( __GODS(VIEWPORT), Viewport )
	__GDEFINE_FREEDATA_FUNCTION_NO_MEMBER_CLEANUP( __GODS(VIEWPORT), Viewport )
	__GDEFINE_ACQUIREDATA_FUNCTION(__GODS(VIEWPORT), Viewport )
	__GDEFINE_COPYDATA_FUNCTION_NO_MEMBER_POINTERS(__GODS(VIEWPORT))
	__GDEFINE_CLONEDATA_FUNCTION_NO_MEMBER_POINTERS( __GODS(VIEWPORT) )
	__GDEFINE_COMPAREDATA_FUNCTION( __GODS(VIEWPORT) )
	__GDEFINE_FILEMEM_SERIALIZATION_FUNCTIONS_NO_MEMBER_POINTERS( __GODS(VIEWPORT) )
	__GDEFINE_FILEMEM_STREAMING_FUNCTIONS_NO_MEMBER_POINTERS( __GODS(VIEWPORT) )
	__GDEFINE_FREEARRAY_FUNCTION( __GODS(VIEWPORT) )
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( CAMERA, Camera );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( FOG, Fog );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( SPRITE, Sprite );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( RENDERSTATE, RenderState );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( STENCIL, Stencil );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( DEPTH, Depth );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( BLEND, Blend );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( RASTERIZER, Rasterizer );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( TEXTURE, Texture );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( MIRROR, Mirror );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(MIRROR),
	"MirrorType...................: 0x%X\n"
	"ReflectionPlane..............: n=(x=%f, y=%f, z=%f), d=%f\n"
	"AlphaValue...................: %f\n"
	"ReflectionAlpha..............: %s"
	, (int)in_CoreInstance->MirrorType
	, in_CoreInstance->ReflectionPlane.x , in_CoreInstance->ReflectionPlane.y , in_CoreInstance->ReflectionPlane.z
	, in_CoreInstance->ReflectionPlane.d
	, in_CoreInstance->AlphaValue
	, G_CSTRING_FROM_BOOL( in_CoreInstance->ReflectionAlpha  )
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(RENDERSTATE),
	"fTime........................: %f\n"
	"ApplyLights..................: 0x%X\n"
	"CullMode.....................: 0x%X\n"
	"AlphaTest....................: %s\n"
	"AlphaBlend...................: %s\n"
	"AlphaValue...................: %f"
	, in_CoreInstance->fTime
	, (int)in_CoreInstance->ApplyLights
	, (int)in_CoreInstance->CullMode
	, G_CSTRING_FROM_BOOL( in_CoreInstance->AlphaTest  )
	, G_CSTRING_FROM_BOOL( in_CoreInstance->AlphaBlend )
	, in_CoreInstance->AlphaValue 
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(FOG),
	"ColorFar......................: r=%f, g=%f, b=%f\n"
	"ColorNear.....................: r=%f, g=%f, b=%f\n"
	"NearLimit.....................: %f\n"
	"FarLimit......................: %f"
	, in_CoreInstance->ColorFar.r, in_CoreInstance->ColorFar.g, in_CoreInstance->ColorFar.b
	, in_CoreInstance->ColorNear.r, in_CoreInstance->ColorNear.g, in_CoreInstance->ColorNear.b
	, in_CoreInstance->NearLimit
	, in_CoreInstance->FarLimit
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TEXTURE),
	"ImageUsage...................: 0x%X\n"
	"ImageOffset..................: x=%f, y=%f\n"
	"ImageAlpha...................: %s\n"
	"WrapMode.....................: %i"
	, (int)in_CoreInstance->ImageUsage
	, in_CoreInstance->ImageOffset.x , in_CoreInstance->ImageOffset.y
	, G_CSTRING_FROM_BOOL(in_CoreInstance->ImageAlpha)
	, (int)in_CoreInstance->WrapMode
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(SPRITE),
	"FrameSize....................: x=%i, y=%i\n"
	"StepTime.....................: %f\n"
	"FrameColumns.................: %i\n"
	"FrameOffset..................: %i\n"
	"FrameCount...................: %i\n"
	"Reverse......................: %s\n"
	"Loop.........................: %s"
	, (int)in_CoreInstance->FrameSize[0] , (int)in_CoreInstance->FrameSize[1]
	, in_CoreInstance->StepTime
	, (int)in_CoreInstance->FrameColumns
	, (int)in_CoreInstance->FrameOffset
	, (int)in_CoreInstance->FrameCount
	, G_CSTRING_FROM_BOOL( in_CoreInstance->Reverse )
	, G_CSTRING_FROM_BOOL( in_CoreInstance->Loop ) 
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(CAMERA),
	"Right........................: x=%f, y=%f, z=%f\n"
	"Up...........................: x=%f, y=%f, z=%f\n"
	"Front........................: x=%f, y=%f, z=%f\n"
	"Position.....................: x=%f, y=%f, z=%f\n"
	"Angle........................: %f\n"
	"FarPlane.....................: %f\n"
	"NearPlane....................: %f"
	, in_CoreInstance->Right.x		, in_CoreInstance->Right.y		, in_CoreInstance->Right.z		
	, in_CoreInstance->Up.x			, in_CoreInstance->Up.y			, in_CoreInstance->Up.z			
	, in_CoreInstance->Front.x		, in_CoreInstance->Front.y		, in_CoreInstance->Front.z		
	, in_CoreInstance->Position.x	, in_CoreInstance->Position.y	, in_CoreInstance->Position.z	
	, in_CoreInstance->Angle 
	, in_CoreInstance->FarPlane	
	, in_CoreInstance->NearPlane 
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(VIEWPORT),
	"PositionX....................: %i\n"
	"PositionY....................: %i\n"
	"Width........................: %i\n"
	"Height.......................: %i"
	, (int)in_CoreInstance->PositionX		
	, (int)in_CoreInstance->PositionY		
	, (int)in_CoreInstance->Width	
	, (int)in_CoreInstance->Height
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(STENCIL),
	"Operator.....................: 0x%X\n"
	"FailOperation................: 0x%X\n"
	"DepthFailOperation...........: 0x%X\n"
	"PassOperation................: 0x%X\n"
	"ReadMask.....................: 0x%X\n"
	"WriteMask....................: 0x%X\n"
	"StencilEnable................: %s"
	, (int)in_CoreInstance->Operator			
	, (int)in_CoreInstance->FailOperation		
	, (int)in_CoreInstance->DepthFailOperation	
	, (int)in_CoreInstance->PassOperation		
	, (int)in_CoreInstance->ReadMask			
	, (int)in_CoreInstance->WriteMask			
	, G_CSTRING_FROM_BOOL( in_CoreInstance->StencilEnable )
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(BLEND),
	"AlphaToCoverageEnable........: %s\n"
	"IndependentBlendEnable.......: %s\n"
	"BlendEnable..................: %s\n"
	"SrcBlend.....................: %i\n"
	"DestBlend....................: %i\n"
	"BlendOp......................: %i\n"
	"SrcBlendAlpha................: %i\n"
	"DestBlendAlpha...............: %i\n"
	"BlendOpAlpha.................: %i\n"
	"RenderTargetWriteMask........: 0x%X"
	, G_CSTRING_FROM_BOOL( in_CoreInstance->AlphaToCoverageEnable	)
	, G_CSTRING_FROM_BOOL( in_CoreInstance->IndependentBlendEnable	)
	, G_CSTRING_FROM_BOOL( in_CoreInstance->BlendEnable				)
	, (int)in_CoreInstance->SrcBlend
	, (int)in_CoreInstance->DestBlend
	, (int)in_CoreInstance->BlendOp
	, (int)in_CoreInstance->SrcBlendAlpha
	, (int)in_CoreInstance->DestBlendAlpha
	, (int)in_CoreInstance->BlendOpAlpha
	, (int)in_CoreInstance->RenderTargetWriteMask
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(DEPTH),
	"Operator.....................: %i\n"
	"WriteMask....................: %i\n"
	"WriteEnable..................: %s"
	, (int)in_CoreInstance->Operator		
	, (int)in_CoreInstance->WriteMask		
	, G_CSTRING_FROM_BOOL( in_CoreInstance->WriteEnable )
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(RASTERIZER),
	"FillMode.....................: 0x%X\n"
	"CullMode.....................: 0x%X\n"
	"FrontCounterClockwise........: %s\n"
	"DepthBias....................: %i\n"
	"DepthBiasClamp...............: %f\n"
	"SlopeScaledDepthBias.........: %f\n"
	"DepthClipEnable..............: %s\n"
	"ScissorEnable................: %s\n"
	"MultisampleEnabled...........: %s\n"
	"AntialiasingEnabled..........: %s"
	, (int)in_CoreInstance->FillMode				
	, (int)in_CoreInstance->CullMode				
	, G_CSTRING_FROM_BOOL(in_CoreInstance->FrontCounterClockwise )
	, (int)in_CoreInstance->DepthBias				
	, in_CoreInstance->DepthBiasClamp			
	, in_CoreInstance->SlopeScaledDepthBias	
	, G_CSTRING_FROM_BOOL( in_CoreInstance->DepthClipEnable		)
	, G_CSTRING_FROM_BOOL( in_CoreInstance->ScissorEnabled		)
	, G_CSTRING_FROM_BOOL( in_CoreInstance->MultisampleEnabled	)	
	, G_CSTRING_FROM_BOOL( in_CoreInstance->AntialiasingEnabled	)
	);

static const GVector3 gAttenuation012 = GVector3(0.0025f, 0.00085f, 0.00085f);

error_t god::generateVertexLight( const GODS(LIGHT) in_LightData, 
								 uint32_t nVertexCount, const GVECTOR3* in_lstVertices, const GVECTOR3* in_lstNormals, 
								GVECTOR3* out_lstLightColor, const GVECTOR3* _vAttenuation )
{
	GPROFILE_FUNCTION();

	const GCOLOR& diffuse = in_LightData->Diffuse,
		&ambient = in_LightData->Ambient;
	GVector3 lightPosW = in_LightData->Position;

	float fRange = in_LightData->Range;
	fRange *= fRange;	
	for( uint32_t nVertexIndex=0; nVertexIndex<nVertexCount; nVertexIndex++ )
	{
		float fDistance;
		const GVECTOR3& vPosition = in_lstVertices[nVertexIndex];
		GVector3 lightVecW	= (lightPosW - vPosition);
		fDistance = lightVecW.SqLength();
		if( fDistance > (fRange) )
			continue;
		lightVecW.Normalize();
		addDiffuseVertexColor( lightVecW, fDistance ? sqrtf( fDistance ) : fDistance, vPosition, in_lstNormals[nVertexIndex], _vAttenuation ? (*_vAttenuation) : gAttenuation012, diffuse, out_lstLightColor[nVertexIndex] );
	}

	return 0;
};

error_t god::generateVertexLight( const GODS(LIGHT) in_LightData, uint32_t nVertexCount, const GVECTOR3* in_lstVertices, const GVECTOR3* in_lstNormals, 
								GCOLOR* out_lstLightColor, 
								const GVECTOR3* _vAttenuation )
{
	GPROFILE_FUNCTION();

	const GCOLOR& diffuse = in_LightData->Diffuse,
		&ambient = in_LightData->Ambient;
	GVector3 lightPosW = in_LightData->Position;

	float fRange = in_LightData->Range;
	fRange *= fRange;	
	for( uint32_t nVertexIndex=0; nVertexIndex<nVertexCount; nVertexIndex++ )
	{
		float fDistance;
		const GVECTOR3& vPosition = in_lstVertices[nVertexIndex];
		GVector3 lightVecW	= (lightPosW - vPosition);
		fDistance = lightVecW.SqLength();
		if( fDistance > (fRange) )
			continue;
		lightVecW.Normalize();
		addDiffuseVertexColor( lightVecW, fDistance ? sqrtf( fDistance ) : fDistance, vPosition, in_lstNormals[nVertexIndex], _vAttenuation ? (*_vAttenuation) : gAttenuation012, diffuse, out_lstLightColor[nVertexIndex] );
	}

	return 0;
};

