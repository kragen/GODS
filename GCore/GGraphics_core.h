/// File:GGraphics_core.h
///
/// This file is part of the Game Object Data System
/// 2011 (c) Pablo Ariel Zorrilla Cepeda
#include "GColor_core.h"
#include "GMath_base.h"

#ifndef _GGRAPHICS_CORE_H
#define _GGRAPHICS_CORE_H

namespace god
{
#pragma pack( push, 1 )
	static const uint8_t GMAX_TEXREF = 8; // maybe later versions will accept INT_MAX textures

	typedef uint8_t GCOLOR_CHANNEL;
	static const GCOLOR_CHANNEL GCCHANNEL_RED		= 0x01;
	static const GCOLOR_CHANNEL GCCHANNEL_GREEN		= 0x02;
	static const GCOLOR_CHANNEL GCCHANNEL_BLUE		= 0x04;
	static const GCOLOR_CHANNEL GCCHANNEL_ALPHA		= 0x08;
	static const GCOLOR_CHANNEL GCCHANNEL_DEPTH		= 0x10;
	static const GCOLOR_CHANNEL GCCHANNEL_STENCIL	= 0x20;

	// These are different ways of using an image (texture) at render time (when drawing a particular object node).
	typedef uint8_t GIMAGE_USAGE;
	static const GIMAGE_USAGE	GIMAGEUSAGE_COLOR			= 0; // The colors of the image are mapped as a common image
	static const GIMAGE_USAGE	GIMAGEUSAGE_BLEND_COLOR		= 1; // A 2D color image to be placed depending on the map value of GIMAGEUSAGE_BLEND_MAP
	static const GIMAGE_USAGE	GIMAGEUSAGE_BLEND_MAP		= 2; // Each color of this texture are used as a filter for each texture set as GIMAGEUSAGE_BLEND_COLOR
	static const GIMAGE_USAGE	GIMAGEUSAGE_BUMP			= 3; // The image is used as a heightmap to be used for light calculations
	static const GIMAGE_USAGE	GIMAGEUSAGE_DISPLACEMENT	= 4; // The image is used as a displacement map 
	static const GIMAGE_USAGE	GIMAGEUSAGE_HEIGHT			= 5; // The image is used as a heightmap to be used for vertex calculations
	static const GIMAGE_USAGE	GIMAGEUSAGE_ALPHA			= 6; // The image is used as a heightmap to be used for vertex calculations
	static const GIMAGE_USAGE	GIMAGEUSAGE_NORMAL			= 7; // The image is used as a heightmap to be used for vertex calculations

	// These are different ways of rendering a reflection
	typedef uint8_t GMIRROR_TYPE;
	static const GMIRROR_TYPE	GMIRRORTYPE_NONE	= 0; // The surface doesn't reflect the surrounding environment in any way
	static const GMIRROR_TYPE	GMIRRORTYPE_PLANAR	= 1; // the reflection is in 1 direction only over the reflective surface (like in a mirror)
	static const GMIRROR_TYPE	GMIRRORTYPE_MAPPING	= 2; // the reflection direction changes depending on the geometry of the reflective surface (like in a metallic ball)

	// These are different ways of rendering the geometry buffers
	typedef uint8_t GPRIMITIVE_TYPE;
	static const GPRIMITIVE_TYPE GPRIMITIVE_POINT			= 1;
	static const GPRIMITIVE_TYPE GPRIMITIVE_LINE			= 2;
	static const GPRIMITIVE_TYPE GPRIMITIVE_TRIANGLE		= 3;
	static const GPRIMITIVE_TYPE GPRIMITIVE_LINESTRIP		= 4; 
	static const GPRIMITIVE_TYPE GPRIMITIVE_TRIANGLESTRIP	= 5; 

	// Max viewports allowed by the renderers
	static const uint8_t GMAX_VIEWPORTS = 8;

	// The way the texture wraps an object
	typedef uint8_t _GWRAP_MODE, GWRAP_MODE;
	static const GWRAP_MODE GWRAPMODE_DEFAULT	= 0;
	static const GWRAP_MODE GWRAPMODE_U			= 1;
	static const GWRAP_MODE	GWRAPMODE_V			= 2;
	static const GWRAP_MODE GWRAPMODE_W			= 4;
	static const GWRAP_MODE GWRAPMODE_3			= 8;

	// The way the triangles are discarded depending on their winding order (clockwise, counter-clockwise, no culling)
	typedef uint8_t _GCULL_MODE, GCULL_MODE;
	static const GCULL_MODE GCULLMODE_CCW		= 0;
	static const GCULL_MODE GCULLMODE_CW		= 1;
	static const GCULL_MODE GCULLMODE_NONE		= 2;

	typedef uint8_t _GFILL_MODE, GFILL_MODE;
	static const GFILL_MODE GFILLMODE_SOLID		= 0;
	static const GFILL_MODE GFILLMODE_WIREFRAME	= 1;
	static const GFILL_MODE GFILLMODE_POINT		= 2;

	// Some projection modes for constructing the projection matrices 
	typedef uint8_t GPROJECTION_TYPE;
	static const GPROJECTION_TYPE GPROJECTIONTYPE_PERSPECTIVE	= 0;	// The perspective mode transforms vertices as in 3D space
	static const GPROJECTION_TYPE GPROJECTIONTYPE_ORTHOGONAL	= 1;	// The orthogonal displays the image as if they were in a plane
	static const GPROJECTION_TYPE GPROJECTIONTYPE_SCREENCOORD	= 2;	// This mode uses 2D top-to-down screen coordinates as frame of reference 


	// 
	typedef struct _GSTENCIL1_DATA
	{
		uint32_t			dwRef;
		uint32_t			dwMask;
		uint32_t			dwWriteMask;
		GLOGICAL_OPERATOR	CmpOperator;
		GOPERATION_TYPE		ZFail;
		GOPERATION_TYPE		Fail;
		GOPERATION_TYPE		Pass;
	} GSTENCIL1_DATA;

	// 
	GDECLARE_CORE_STRUCT( STENCIL, Stencil,
		GLOGICAL_OPERATOR	Operator;
		GOPERATION_TYPE		FailOperation;
		GOPERATION_TYPE		DepthFailOperation;
		GOPERATION_TYPE		PassOperation;
		uint8_t				ReadMask;
		uint8_t				WriteMask;
		bool				StencilEnable;
	);

	// 
	GDECLARE_CORE_STRUCT( DEPTH, Depth,
		GLOGICAL_OPERATOR	Operator;
		uint8_t				WriteMask;
		bool				WriteEnable;
	);

	// 
	GDECLARE_CORE_STRUCT( BLEND, Blend,
		bool					AlphaToCoverageEnable;
		bool					IndependentBlendEnable;
		bool					BlendEnable;
		GINTERPOLATION_VALUE	SrcBlend;
		GINTERPOLATION_VALUE	DestBlend;
		GARITHMETIC_OPERATOR	BlendOp;
		GINTERPOLATION_VALUE	SrcBlendAlpha;
		GINTERPOLATION_VALUE	DestBlendAlpha;
		GARITHMETIC_OPERATOR	BlendOpAlpha;
		bool					RenderTargetWriteMask;
	);

	// 
	GDECLARE_CORE_STRUCT( RASTERIZER, Rasterizer,
		GFILL_MODE			FillMode;
		GCULL_MODE			CullMode;
		bool				FrontCounterClockwise;
		int32_t				DepthBias;
		float				DepthBiasClamp;
		float				SlopeScaledDepthBias;
		bool				DepthClipEnable;
		bool				ScissorEnabled;
		bool				MultisampleEnabled;
		bool				AntialiasingEnabled;
	);
	static void onCreateRasterizerDefault( ggns_Rasterizer* stuff )
	{
		(*stuff)->FillMode				= GFILLMODE_SOLID;
		(*stuff)->CullMode				= GCULLMODE_CCW;
		(*stuff)->FrontCounterClockwise	= false;
		(*stuff)->DepthBias				= 1;
		(*stuff)->DepthBiasClamp		= 1.0f;	
		(*stuff)->SlopeScaledDepthBias	= 1.0f;
		(*stuff)->DepthClipEnable		= true;
		(*stuff)->ScissorEnabled		= false;		
		(*stuff)->MultisampleEnabled	= false;
		(*stuff)->AntialiasingEnabled	= false;
	}
	//---------------
	// This structure is used to define texture sprite animations.
	// 
	// A sprite animation is defined in this engine as a set of consecutive animation frames 
	// stored in a single bitmap.
	GDECLARE_CORE_STRUCT( SPRITE, Sprite,
		uint16_t			FrameSize[2];	// The size of the animation frame (in pixels)
		float				StepTime;		// The amout of time requested for the sprite animation to show each image frame
		uint32_t			FrameColumns;	// The amount of animation columns in the image (if reached then it goes to next row)
		uint32_t			FrameOffset;	// The amount of frames to skip to the first animation frame
		uint8_t 			FrameCount;		// The amount of animation frames 
		bool				Reverse;		// Set to true in order to play the animation backwards
		bool				Loop;			// Set to true in order to play the animation in a loop
	);
	static void onCreateSpriteDefault( ggns_Sprite* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size() );
	}

	// A texture is defined for this engine as the way an image (bitmap) is applied to a polygon
	//
	GDECLARE_CORE_STRUCT( TEXTURE, Texture,
		float			fTime;
		GIMAGE_USAGE	ImageUsage;		// the way each image is applied in the node
		GVector2		ImageOffset;	// the offset for each image applied in the node (in pixels? o.O normalized coords?)
		bool			ImageAlpha;
		GWRAP_MODE		WrapMode;		// the way the texture is mapped to the texture coordinate data
	);
	static void onCreateTextureDefault( ggns_Texture* stuff )
	{
		memset( (*stuff).get_data_address(), 0, (*stuff).get_data_size() );
	}

	GDECLARE_CORE_STRUCT( RENDERSTATE, RenderState,
		float			fTime;
		bool			AlphaTest;		// Set this variable to true in order to enable alpha test
		bool			AlphaBlend;		// Set this variable to true in order to enable fAlpha variable usage (otherwise it's gonna be discarded)
		GCULL_MODE		CullMode;		// The way triangles are discarded depending on their winding order
		GFILL_MODE		FillMode;		// The way triangles are discarded depending on their winding order
		GLIGHT_TYPE		ApplyLights;	// Set the light types that will be processed for this node. If it's set to GLIGHTTYPE_NONE, the node will rendered with raw vertex and/or texture colors
		float			AlphaValue;		// Set this variable in order to change the base alpha value for this frame	
	);
	static void onCreateRenderStateDefault( ggns_RenderState* stuff )
	{
		(*stuff)->fTime			= 0.0f;
		(*stuff)->AlphaTest		= false;
		(*stuff)->AlphaBlend	= false;
		(*stuff)->CullMode		= GCULLMODE_CCW;
		(*stuff)->FillMode		= GFILLMODE_SOLID;
		(*stuff)->ApplyLights	= GLIGHTTYPE_DIRECTIONAL | GLIGHTTYPE_SPOT | GLIGHTTYPE_POINT;
		(*stuff)->AlphaValue	= 1.0f;
	}

	GDECLARE_CORE_STRUCT( MIRROR, Mirror,
		float			fTime;
		GMIRROR_TYPE	MirrorType;			// The type of reflection for a 3d object (reflection in planes are faster to calculate than complex surfaces)
		GPLANE			ReflectionPlane;	// this is used whenever MirrorType variable is set to MIRROR_PLANAR
		float			AlphaValue;			// The alpha value of the reflection (a transition of this value can be used to achieve some effects)
		bool			ReflectionAlpha;	// The alpha value of the reflection (a transition of this value can be used to achieve some effects)
	);
	static void onCreateMirrorDefault( ggns_Mirror* stuff )
	{
		(*stuff)->fTime				= 0.0f;
		(*stuff)->MirrorType		= GMIRRORTYPE_PLANAR;
		(*stuff)->ReflectionPlane	= GPLANEXZ;
		(*stuff)->AlphaValue		= 1.0f;
		(*stuff)->ReflectionAlpha	= false;
	}

	typedef uint8_t GFOG_TYPE;
	static const GFOG_TYPE	GFOGTYPE_NONE	= 0;
	static const GFOG_TYPE	GFOGTYPE_LINEAR = 1;

	GDECLARE_CORE_STRUCT( FOG, Fog,
		float			fTime;
		GFOG_TYPE		FogType;			// The type of fog
		float			FarLimit;
		float			NearLimit;
		GCOLOR			ColorFar;
		GCOLOR			ColorNear;
	);
	static void onCreateFogDefault( ggns_Fog* stuff )
	{
		(*stuff)->fTime		= 0.0f;
		(*stuff)->FogType	= GFOGTYPE_LINEAR;
		(*stuff)->FarLimit	= 2000.f;
		(*stuff)->NearLimit	= 100.0f;
		(*stuff)->ColorFar	= GWHITE;
		(*stuff)->ColorNear	= GWHITE;
	}
	
	// This structure stores the vectors that represent the direction and position of the camera
	GDECLARE_CORE_STRUCT( CAMERA, Camera,	// Basis camera axis, where O(0,0,0) is the center of the screen and -1 and 1 are the limits of the screen, except for the z axis which is 0 to 1: 
		float			fTime;				// The camera identifier				   +y |+1  /+1 +z	
		GVector3		Right;				// default v(x = 1, y = 0, z = 0)             |   /
		GVector3		Up;					// default v(x = 0, y = 1, z = 0)             |  /
		GVector3		Front;				// default v(x = 0, y = 0, z = 1)             | /
		GVector3		Position;			// default v(x = 0, y = 0, z = 0)  (0,0,0) = O|/_______________+1 +x
		float 			Angle;				// The aperture angle of the camera lens  
		float 			FarPlane;			// The maximum depth that a pixel can have in the z axis without being clipped and discarded
		float 			NearPlane;			// The nearest depth that a pixel can have in the z axis without being clipped and discarded
	);
	static void onCreateCameraDefault( ggns_Camera* stuff )
	{
		(*stuff)->fTime			= 0.0f;
		(*stuff)->Front			= GVECTOR3_FRONT;
		(*stuff)->Up			= GVECTOR3_UP;
		(*stuff)->Right			= GVECTOR3_RIGHT;
		(*stuff)->Position		= GVECTOR3_ZERO;
		(*stuff)->Angle			= (float)(GMATH_PI*.25);
		(*stuff)->FarPlane		= 5000.0f;
		(*stuff)->NearPlane		= 0.01f;
	}

	// This structure stores a description of the dimensions and positions for the rendered geometries to be projected on the target image. 
	GDECLARE_CORE_STRUCT( VIEWPORT, Viewport,
		float			fTime;
		uint32_t		PositionX;				// global x position of the render target (window position x)
		uint32_t		PositionY;				// global y position of the render target (window position y)
		uint32_t		Width;			// width in pixels of the target (window target size)
		uint32_t		Height;		// height in pixels of the render target (window target size)
	);
	static void onCreateViewportDefault( ggns_Viewport* stuff )
	{
		memset( (*stuff).get_data_address(), 0, sizeof(uint32_t)*3 );
		(*stuff)->Width		= 256;
		(*stuff)->Height	= 256;
	}

#pragma pack( pop )

	error_t generateVertexLight( const GLIGHT_DATA in_LightData, uint32_t nVertexCount, const GVECTOR3* in_lstVertices, const GVECTOR3* in_lstNormals, 
									GVECTOR3* out_lstLightColor, 
									const GVECTOR3* _vAttenuation );
	error_t generateVertexLight( const GLIGHT_DATA in_LightData, uint32_t nVertexCount, const GVECTOR3* in_lstVertices, const GVECTOR3* in_lstNormals, 
									GCOLOR* out_lstLightColor, 
									const GVECTOR3* _vAttenuation );
} // namespace

#endif // _GGRAPHICS_CORE_H