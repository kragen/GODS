#include "GColor_core.h"
#include "GCore_function_macros.h"


#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

#include <stdlib.h>
#ifdef _WIN32
#include <crtdbg.h>
#endif

using namespace god;

#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 

__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( LIGHT, Light );
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( MATERIAL, Material );

#ifndef min
#define min( a, b ) ( ((a)>(b)) ? (b) : (a) )
#endif 
#ifndef max
#define max( a, b ) ( ((a)<(b)) ? (b) : (a) )
#endif 

static inline const float __calcattenuation__gcolor_core_cpp( const GVECTOR3& vAttenuation, float fDistance )
{
	return vAttenuation.x + vAttenuation.y*fDistance + vAttenuation.z*fDistance*fDistance; // attenuation
};

///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
error_t god::addDiffuseVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
										const GVECTOR3& vAttenuation, const GCOLOR& cDiffuse, GVECTOR3& inout_vLightColor )
{
	// calculate diffuse from vector
	GCOLOR finalcolor;
	float fAttenuationOrDot = max( GVectorDot( &lightNormalW, &vNormal ), 0.0f );	// Dot
	finalcolor.r	= cDiffuse.r*fAttenuationOrDot;
	finalcolor.g	= cDiffuse.g*fAttenuationOrDot;
	finalcolor.b	= cDiffuse.b*fAttenuationOrDot;
	// attenuate
	fAttenuationOrDot = __calcattenuation__gcolor_core_cpp(vAttenuation, fDistance);	// Attenuation
	if( fAttenuationOrDot )
		finalcolor	/= fAttenuationOrDot; 

	inout_vLightColor.x += finalcolor.r;
	inout_vLightColor.y += finalcolor.g;
	inout_vLightColor.z += finalcolor.b;
	return 0;
};

///------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
error_t god::addDiffuseVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
										const GVECTOR3& vAttenuation, const GCOLOR& cDiffuse, GCOLOR& inout_vLightColor )
{
	// calculate diffuse from vector
	GCOLOR finalcolor;
	float fAttenuationOrDot = max( GVectorDot( &lightNormalW, &vNormal ), 0.0f );	// Dot
	finalcolor.r	= cDiffuse.r*fAttenuationOrDot;
	finalcolor.g	= cDiffuse.g*fAttenuationOrDot;
	finalcolor.b	= cDiffuse.b*fAttenuationOrDot;
	// attenuate
	fAttenuationOrDot = __calcattenuation__gcolor_core_cpp(vAttenuation, fDistance);	// Attenuation
	if( fAttenuationOrDot )
		finalcolor	/= fAttenuationOrDot; 

	inout_vLightColor.r += finalcolor.r;
	inout_vLightColor.g += finalcolor.g;
	inout_vLightColor.b += finalcolor.b;
	return 0;
};

__GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(MATERIAL),
	"fTime.........................: %f\n"
	"Diffuse.......................: r=%f, g=%f, b=%f, a=%f\n"
	"Ambient.......................: r=%f, g=%f, b=%f, a=%f\n"
	"Specular......................: r=%f, g=%f, b=%f, a=%f\n"
	"Emissive......................: r=%f, g=%f, b=%f, a=%f\n"
	"fSpecularPower................: %f"
	, in_CoreInstance->fTime
	, in_CoreInstance->Ambient.r	, in_CoreInstance->Ambient.g	, in_CoreInstance->Ambient.b	, in_CoreInstance->Ambient.a
	, in_CoreInstance->Diffuse.r	, in_CoreInstance->Diffuse.g	, in_CoreInstance->Diffuse.b	, in_CoreInstance->Diffuse.a
	, in_CoreInstance->Specular.r	, in_CoreInstance->Specular.g	, in_CoreInstance->Specular.b	, in_CoreInstance->Specular.a
	, in_CoreInstance->Emissive.r	, in_CoreInstance->Emissive.g	, in_CoreInstance->Emissive.b	, in_CoreInstance->Emissive.a
	, in_CoreInstance->SpecularPower
	);

__GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(LIGHT),
	"Ambient.......................: r=%f, g=%f, b=%f, a=%f\n"
	"Diffuse.......................: r=%f, g=%f, b=%f, a=%f\n"
	"Specular......................: r=%f, g=%f, b=%f, a=%f\n"
	"fSpotPower....................: %f\n"
	"fRange........................: %f\n"
	"vAttenuation..................: x=%f, y=%f, z=%f\n"
	"vPosition.....................: x=%f, y=%f, z=%f\n"
	"vDirection....................: x=%f, y=%f, z=%f\n"
	"nType.........................: 0x%X\n"
	"State.........................: 0x%X"		
	, in_CoreInstance->Ambient .r, in_CoreInstance->Ambient .g, in_CoreInstance->Ambient .b, in_CoreInstance->Ambient .a
	, in_CoreInstance->Diffuse .r, in_CoreInstance->Diffuse .g, in_CoreInstance->Diffuse .b, in_CoreInstance->Diffuse .a
	, in_CoreInstance->Specular.r, in_CoreInstance->Specular.g, in_CoreInstance->Specular.b, in_CoreInstance->Specular.a
	, in_CoreInstance->SpotPower
	, in_CoreInstance->Range
	, in_CoreInstance->Attenuation.x, in_CoreInstance->Attenuation.y, in_CoreInstance->Attenuation.z
	, in_CoreInstance->Position.x	, in_CoreInstance->Position.y	, in_CoreInstance->Position.z
	, in_CoreInstance->Direction.x	, in_CoreInstance->Direction.y	, in_CoreInstance->Direction.z
	, (int)in_CoreInstance->Type
	, (int)in_CoreInstance->State
	);