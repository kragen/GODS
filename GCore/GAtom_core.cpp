#include "GAtom_core.h"

#include "GCore_function_macros.h"

#include "glist.h"
//#include "GCore_printf.h"
#include "GDebugger_core.h"
#include <stdlib.h>

using namespace god;

GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( ATOM, Atom );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( AMINOACID, Aminoacid );
GDEFINE_CORE_FUNCTIONS_MEMBER_INIT_CLEANUP_SAVELOAD( BOND, Bond );

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(ATOM),
	"AtomicNumber..................: %u\n"
	"Symbol[4].....................: %s\n"
	"Name[64]......................: %s\n"
	"NameOrigin[256]...............: %s\n"
	"Group.........................: %u\n"
	"Period........................: %u\n"
	"AtomicWeight..................: %LG\n"
	"Density.......................: %LG\n"
	"MeltPoint.....................: %LG\n"
	"BoilPoint.....................: %LG\n"
	"Heat..........................: %LG\n"
	"Electronegativity.............: %LG\n"
	"Abundance.....................: %LG\n"
	, (uint32_t)in_CoreInstance->AtomicNumber			
	, in_CoreInstance->Symbol[4]				
	, in_CoreInstance->Name[64]			
	, in_CoreInstance->NameOrigin[256]		
	, (uint32_t)in_CoreInstance->Group					
	, (uint32_t)in_CoreInstance->Period					
	, in_CoreInstance->AtomicWeight			
	, in_CoreInstance->Density				
	, in_CoreInstance->MeltPoint
	, in_CoreInstance->BoilPoint
	, in_CoreInstance->Heat
	, in_CoreInstance->Electronegativity		
	, in_CoreInstance->Abundance
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(AMINOACID),
	"Type..........................: %u\n"
	"Letter........................: %c\n"
	"AtomCount.....................: %u\n"
	"AtomsAtomicNumber[32].........: %s\n"
	"AtomsSymbol[32]...............: %s\n"
	"AtomsPacked[30]...............: %s\n"
	"InternalName[64]..............: %s\n"
	"CommonName[64]................: %s\n"
	, (uint32_t)in_CoreInstance->Type
	, in_CoreInstance->Letter
	, (uint32_t)in_CoreInstance->AtomCount
	, in_CoreInstance->AtomsPacked
	, in_CoreInstance->AtomsSymbol
	, in_CoreInstance->AtomsAtomicNumber
	, in_CoreInstance->InternalName
	, in_CoreInstance->CommonName
	);

GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(BOND),
	"Type..........................: %u\n"
	"SectionA......................: %u\n"
	"IndexA........................: %u\n"
	"SectionB......................: %u\n"
	"IndexB........................: %u\n"
	, (uint32_t)in_CoreInstance->Type
	, (uint32_t)in_CoreInstance->SectionA
	, (uint32_t)in_CoreInstance->IndexA
	, (uint32_t)in_CoreInstance->SectionB
	, (uint32_t)in_CoreInstance->IndexB
	);

