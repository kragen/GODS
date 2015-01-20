#include "GVector_object.h"
#include "GColor_object.h"
#include "gcore_ptr.h"

#ifndef _GCOLOR_CORE_H
#define _GCOLOR_CORE_H
namespace god
{

	// This structure stores a combination of colors to apply to triangles with a a specular value for the specular color. 
	// It's used in lighting calculations and other effects to get the final color of the object
	//typedef struct _GMATERIAL
	//{
	//	GCOLOR	Ambient;
	//	GCOLOR	Diffuse;
	//	GCOLOR	Specular;
	//	GCOLOR	Emissive;
	//	float	fSpecularPower;
	//	_GMATERIAL( void );
	//	_GMATERIAL( const _GMATERIAL& mat );
	//	_GMATERIAL& operator =( const _GMATERIAL& mat );
	//	bool operator ==( const _GMATERIAL& mat );
	//	bool operator !=( const _GMATERIAL& mat );
	//	_GMATERIAL(const GCOLOR& a, const GCOLOR& d, const GCOLOR& s, const GCOLOR& e, float power);
	//} GMATERIAL, *PGMATERIAL;
	//
	//// Define some material colors 
	//static const GMATERIAL GMATERIAL_WHITE( GGRAY, GWHITE, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_RED( GDARKRED, GRED, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_GREEN( GDARKGREEN, GGREEN, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_BLUE( GDARKBLUE, GBLUE, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_MAGENTA( GDARKMAGENTA, GMAGENTA, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_YELLOW( GDARKYELLOW, GYELLOW, GWHITE, GBLACK, 10.0f );
	//static const GMATERIAL GMATERIAL_CYAN( GDARKCYAN, GCYAN, GWHITE, GBLACK, 10.0f );

	GDECLARE_CORE_STRUCT( MATERIAL, Material,
		float		fTime;
		GCOLOR		Diffuse;
		GCOLOR		Ambient;
		GCOLOR		Specular;
		GCOLOR		Emissive;
		float		SpecularPower;
	);
	static void onCreateMaterialDefault( ggns_Material* stuff )
	{
		(*stuff)->fTime			= 0.0f;
		(*stuff)->Ambient		= GDARKGRAY*.5f;
		(*stuff)->Diffuse		= GWHITE;
		(*stuff)->Specular		= GGRAY;
		(*stuff)->Emissive		= GBLACK;
		(*stuff)->SpecularPower	= 10.0f;
	};
	// This engine defines 3 types of lights
	// - Directional light is applied to the whole scene. Position is only used to calculate range as it won't be calculated for objects further.
	// - Point light is applied to all objects within the light range but it fades with the distance (an attenuation is added to the color equation).
	// - Spot light is like the point light but it also provides an angle for making the light spread as a cone.
	// 
	typedef uint8_t GLIGHT_TYPE; // bit field
	static const GLIGHT_TYPE	GLIGHTTYPE_NONE			= 0;
	static const GLIGHT_TYPE	GLIGHTTYPE_POINT		= 1;
	static const GLIGHT_TYPE	GLIGHTTYPE_SPOT			= 2;
	static const GLIGHT_TYPE	GLIGHTTYPE_DIRECTIONAL	= 4;

	// Set the last bit to 1 to enable processing of the light
	// 
	typedef uint8_t GLIGHT_STATE;
	static const GLIGHT_STATE	GLIGHTSTATE_ENABLED		= 0x01;
	static const GLIGHT_STATE	GLIGHTSTATE_DISABLED	= 0x00;

	// This structure can store the properties of a light
	// 
	GDECLARE_CORE_STRUCT( LIGHT, Light,
		GCOLOR				Ambient;		// This color is applied no matter what (as an indirect light from reflections not really calculated)
		GCOLOR				Diffuse;		// This color is applied depending on the angle between the light position and the surface normal
		GCOLOR				Specular;		// This color is applied depending on the angle between the light position and the surface normal and a shininess value from the material (specular power)
		float				SpotPower;		// This variable stores the angle of the light
		float				Range;			// The range is used to prevent light calculations for an object if it's too far
		GVector3			Position;		// This variable stores the position of the light
		GVector3			Direction;		// This variable stores the direction of the light
		GVector3			Attenuation;	// This variable stores the attenuation of the light (how it fades with the distance)
		GLIGHT_TYPE			Type;			// This is the light type. There are three types of lights used by this engine
		GLIGHT_STATE		State;			// This is a flag to tell if the light is enabled or not (may more flags be added later)
	);
	static void onCreateLightDefault( ggns_Light* stuff )
	{
		(*stuff)->Ambient		= GDARKGRAY*.05f;
		(*stuff)->Diffuse		= GLIGHTGRAY;
		(*stuff)->Specular		= GWHITE;
		(*stuff)->SpotPower		= 15.0f;
		(*stuff)->Range			= 15.0f;
		(*stuff)->Position		= GVECTOR3_ZERO;
		(*stuff)->Direction		= GVector3( 1.0f, 1.0f, 1.0f ).Normalize();
		(*stuff)->Attenuation	= GVector3( 0.1f, 0.01f, 0.005f );
		(*stuff)->Type			= GLIGHTTYPE_POINT;
		(*stuff)->State			= GLIGHTSTATE_ENABLED;
	};

	error_t addDiffuseVertexColor(  const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorDiffuse, GVECTOR3& inout_vLightColor );
	error_t addAmbientVertexColor(  const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorAmbient, GVECTOR3& inout_vLightColor );
	error_t addSpecularVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorSpecular, GVECTOR3& inout_vLightColor );
	error_t addDiffuseVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorDiffuse, GCOLOR& inout_vLightColor );
	error_t addAmbientVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorAmbient, GCOLOR& inout_vLightColor );
	error_t addSpecularVertexColor( const GVECTOR3& lightNormalW, float fDistance, const GVECTOR3& vPosition, const GVECTOR3& vNormal, 
									const GVECTOR3& vAttenuation, const GCOLOR& colorSpecular, GCOLOR& inout_vLightColor );
}
#endif // _GCOLOR_CORE_H