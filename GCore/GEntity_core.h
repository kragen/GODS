/// File:GEntity_core.h
///
/// This file is part of the Game Object Data System
/// 2011 (c) Pablo Ariel Zorrilla Cepeda
//
#include "GGeometry_core.h"
#include <stdio.h>

#ifndef _GENTITY_CORE_H
#define _GENTITY_CORE_H

// entities will bind the physics data to the graphics data and vice-versa
namespace god
{
	// Maybe the type GENTITYPHYSICS_MODE deserves some explanation
	// 
	// GPHYSICS_ACTOR: physics are applied for the whole actor as a single piece
	// GPHYSICS_NODE: physics are applied for each actor node as a different object
	// GPHYSICS_RAGDOLL: physics affect each node sepparately but these maintain always the same distance between them (very much like tied with a rope)
	// GPHYSICS_SPRINGDOLL: physics affect each node but these behave as if tied with a spring
	typedef uint8_t GENTITYPHYSICS_MODE;
	static const GENTITYPHYSICS_MODE GPHYSICS_ACTOR			= 1; 
	static const GENTITYPHYSICS_MODE GPHYSICS_NODE			= 2; 
	static const GENTITYPHYSICS_MODE GPHYSICS_RAGDOLL		= 4; 
	static const GENTITYPHYSICS_MODE GPHYSICS_SPRINGDOLL	= 8; 

	// The animation mode is interpreted as follows:
	// GANIMATION_NONE: Just skip any animation calculations.
	// GANIMATION_BONEFRAME: Keyframed animation of bones/nodes (the whole set of vertices share the same transform).
	// GANIMATION_MORPHFRAME: Keyframed morph animation (vertex blend, interpolates between two vertex positions and can differ for each vertex).
	// GANIMATION_SKINNED: Keyframed animation of bones with vertex blend animation.
	typedef uint8_t GENTITYANIMATION_MODE;
	static const GENTITYANIMATION_MODE	GANIMATION_NONE			= 0;
	static const GENTITYANIMATION_MODE	GANIMATION_BONEFRAME	= 1;
	static const GENTITYANIMATION_MODE	GANIMATION_MORPHFRAME	= 2;
	static const GENTITYANIMATION_MODE	GANIMATION_SKINNED		= 4;

	// states allow entities to behave in different ways
	// GESTATE_TIMEDOUT: Timed out is usually answered with some destroy messages to shutdown the entity.
	// GESTATE_PAUSED: Paused is used to stop the entity timer.
	// GESTATE_TIMER: This flag is used to... something involving a timer (can't remember right now)
	typedef uint16_t GENTITY_STATE;
	static const GENTITY_STATE GESTATE_NORMAL	= 0;
	static const GENTITY_STATE GESTATE_TIMEDOUT	= 0x8<<12;
	static const GENTITY_STATE GESTATE_PAUSED	= 0x4<<12;
	static const GENTITY_STATE GESTATE_TIMER	= 0x2<<12;
//	static const GENTITY_STATE GESTATE_UNKNOWN	= 0xFFFF;

	// a combination of flag+type id ? still is too green, needs more testing
	typedef uint16_t GENTITY_FAMILY; // 3 bits for the entity base flag - 13 bits used to store custom type ids
	static const GENTITY_FAMILY GEFAMILY_UNKNOWN	= 0;			//	0000 <<12  ==> 00000000 00000000  
	static const GENTITY_FAMILY GEFAMILY_BASE		= 1;	//	0001 <<12  ==> 00010000 00000000
	static const GENTITY_FAMILY GEFAMILY_WORLD		= 2;	//	0010 <<12  ==> 00100000 00000000
	static const GENTITY_FAMILY GEFAMILY_MODEL		= 3;	//	0001 <<12  ==> 00010000 00000000
	static const GENTITY_FAMILY GEFAMILY_TERRAIN	= 4;	//	0100 <<12  ==> 01000000 00000000
	//static const GENTITY_FAMILY GEFAMILY_EFFECT		= 5;	//	1000 <<12  ==> 10000000	00000000
	typedef uint16_t GENTITY_TYPE; // 3 bits for the entity base flag - 13 bits used to store custom type ids

	static const GENTITY_TYPE	GETYPE_MODEL		= GEFAMILY_BASE	| 1;
	static const GENTITY_TYPE	GETYPE_TERRAIN		= GEFAMILY_BASE	| 2;

#pragma pack( push, 1 )
	GDECLARE_CORE_STRUCT( ANIMATION, Animation,
		float						TimeStart;
		float						TimeEnd;
		float						Speed;
		bool						Loop;
		bool						Reverse;
	);
	static void onCreateAnimationDefault( ggns_Animation* stuff )
	{
		(*stuff)->TimeStart	= 0;
		(*stuff)->TimeEnd	= 3.402823466e+38F;
		(*stuff)->Speed		= 1.0f;
		(*stuff)->Loop		= true;	
		(*stuff)->Reverse	= false;
	}
	GDECLARE_CORE_STRUCT( ENTITY, Entity,
		GENTITY_TYPE			Type;
		//GENTITY_FAMILY			Family;
		GENTITY_STATE			State;
		float32_t				LifeTimeout;	// timeout in milliseconds
		bool					Visible;		// Visible objects send messages for registering to renderer
		bool					Solid;			// Collidable objects send messages for registering to collision detector
		bool					Static;			// Non-static objects send messages for registering to integrator
		GENTITYPHYSICS_MODE		PhysicsMode;
		GENTITYANIMATION_MODE	AnimationMode;
	);
	static void onCreateEntityDefault( ggns_Entity* stuff )
	{
		(*stuff)->Type				= GETYPE_MODEL;
		//(*stuff)->Family			= GEFAMILY_UNKNOWN;
		(*stuff)->State				= GESTATE_NORMAL;
		(*stuff)->LifeTimeout		= 0.0f;	
		(*stuff)->Visible			= true;
		(*stuff)->Solid				= false;
		(*stuff)->Static			= true;		
		(*stuff)->PhysicsMode		= GPHYSICS_NODE;
		(*stuff)->AnimationMode		= GANIMATION_NONE;
	}


#pragma pack( pop )
};


#endif // _GENTITY_CORE_H