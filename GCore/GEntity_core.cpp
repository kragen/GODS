/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
#include "GEntity_core.h"
#include "GCore_function_macros.h"
#include <stdlib.h>	// for malloc/free
#include <string.h>	// for memcpy
#ifdef _WIN32
#include <strsafe.h>
#endif


#include "glist.h"
#include "GCore_printf.h"
#include "GDebugger_core.h"

using namespace god;

GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( ENTITY, Entity );
GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( ANIMATION, Animation );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(ENTITY),
	"Type.........................: 0x%X\n"
	//"Family.......................: 0x%X\n"
	"State........................: %i\n"
	"LifeTimeout..................: %i\n"
	"Visible......................: %s\n"
	"Solid........................: %s\n"
	"Static.......................: %s\n"
	"PhysicsMode..................: 0x%X\n"
	"AnimationMode................: 0x%X"
	, (int)in_CoreInstance->Type
	//, (int)in_CoreInstance->Family
	, (int)in_CoreInstance->State
	, (int)in_CoreInstance->LifeTimeout
	, G_CSTRING_FROM_BOOL( in_CoreInstance->Visible )
	, G_CSTRING_FROM_BOOL( in_CoreInstance->Solid   )
	, G_CSTRING_FROM_BOOL( in_CoreInstance->Static  )
	, (int)in_CoreInstance->PhysicsMode
	, (int)in_CoreInstance->AnimationMode
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(ANIMATION),
	"TimeStart................: %f\n"
	"TimeEnd..................: %f\n"
	"Speed....................: %f\n"
	"Loop.....................: 0x%X\n"
	"Reverse..................: 0x%X\n"
	, (float)in_CoreInstance->TimeStart
	, (float)in_CoreInstance->TimeEnd
	, (float)in_CoreInstance->Speed
	, (int)in_CoreInstance->Loop
	, (int)in_CoreInstance->Reverse
	);
