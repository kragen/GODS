/// This file is part of the Game Object Data System
/// 2011 (c) Pablo Ariel Zorrilla Cepeda
#include "gcore_ptr.h"
//#ifdef __cplusplus
//#include <cstdio>
//#else
#include <stdio.h>
//#endif

#ifndef _GFILE_CORE_H
#define _GFILE_CORE_H

namespace god
{
	
#define GTEXT_MAX_XXS_CHARCOUNT		7
#define GTEXT_MAX_XS_CHARCOUNT		31
#define GTEXT_MAX_SMALL_CHARCOUNT	127
#define GTEXT_MAX_MEDIUM_CHARCOUNT	383
#define GTEXT_MAX_LARGE_CHARCOUNT	1535
#define GTEXT_MAX_XL_CHARCOUNT		8191
#define GTEXT_MAX_XXL_CHARCOUNT		24575

#pragma pack( push, 1 )
	GDECLARE_CORE_STRUCT( TEXT_XXS, TextXXS,
		uint8_t		CharCount:4;
		uint8_t		WCharCount:4;
		char		Text[GTEXT_MAX_XXS_CHARCOUNT+1];
		wchar_t		WText[GTEXT_MAX_XXS_CHARCOUNT+1];
	);	// 
	static void onCreateTextXXSDefault( ggns_TextXXS* stuff )
	{
		(*stuff)->CharCount		= 0;
		(*stuff)->WCharCount	= 0;
		(*stuff)->Text[0]		= 0;
		(*stuff)->WText[0]		= 0;
	}

	GDECLARE_CORE_STRUCT( TEXT_XS, TextXS,
		uint8_t		CharCount;
		uint8_t		WCharCount;
		char		Text [GTEXT_MAX_XS_CHARCOUNT+1];
		wchar_t		WText[GTEXT_MAX_XS_CHARCOUNT+1];
	);	// 
	static void onCreateTextXSDefault( ggns_TextXS* stuff )
	{
		(*stuff)->CharCount		= 0;
		(*stuff)->WCharCount	= 0;
		(*stuff)->Text[0]		= 0;
		(*stuff)->WText[0]		= 0;
	}

	GDECLARE_CORE_STRUCT( TEXT_SMALL, TextSmall,
		uint8_t		CharCount;		
		uint8_t		WCharCount;
		char		Text [GTEXT_MAX_SMALL_CHARCOUNT+1];		
		wchar_t		WText[GTEXT_MAX_SMALL_CHARCOUNT+1];		
	);	// 
	static void onCreateTextSmallDefault( ggns_TextSmall* stuff )
	{
		(*stuff)->CharCount		= 0;
		(*stuff)->WCharCount	= 0;
		(*stuff)->Text[0]		= 0;
		(*stuff)->WText[0]		= 0;
	}

	GDECLARE_CORE_STRUCT( TEXT_MEDIUM, TextMedium,
		uint16_t	CharCount;		
		uint16_t	WCharCount;
		char		Text[GTEXT_MAX_MEDIUM_CHARCOUNT+1];		
		wchar_t		WText[GTEXT_MAX_MEDIUM_CHARCOUNT+1];		
	);	// 
	static void onCreateTextMediumDefault( ggns_TextMedium* stuff )
	{
		(*stuff)->CharCount		= 0;
		(*stuff)->WCharCount	= 0;
		(*stuff)->Text[0]		= 0;
		(*stuff)->WText[0]		= 0;
	}


	GDECLARE_CORE_STRUCT( TEXT_LARGE, TextLarge,
		uint16_t	CharCount;		
		uint16_t	WCharCount;
		char		Text [GTEXT_MAX_LARGE_CHARCOUNT+1];		
		wchar_t		WText[GTEXT_MAX_LARGE_CHARCOUNT+1];	
	);	// 
	static void onCreateTextMediumDefault( ggns_TextLarge* stuff )
	{
		(*stuff)->CharCount		= 0;
		(*stuff)->WCharCount	= 0;
		(*stuff)->Text[0]		= 0;
		(*stuff)->WText[0]		= 0;
	}


	GDECLARE_CORE_STRUCT( TEXT_XL, TextXL,
		uint16_t	CharCount;				
		uint16_t	WCharCount;
		char		Text [GTEXT_MAX_XL_CHARCOUNT+1];
		wchar_t		WText[GTEXT_MAX_XL_CHARCOUNT+1];
	);	// 

	GDECLARE_CORE_STRUCT( TEXT_XXL, TextXXL,
		uint16_t	CharCount;			
		uint16_t	WCharCount;
		char		Text [GTEXT_MAX_XXL_CHARCOUNT+1];		
		wchar_t		WText[GTEXT_MAX_XXL_CHARCOUNT+1];	
	);	// 

#pragma pack( pop )

	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XXS)			* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XS)			* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_SMALL)		* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_MEDIUM)		* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_LARGE)		* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XL)			* out_pText );
	uint32_t getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XXL)			* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XXS)		* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XS)		* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_SMALL)	* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_MEDIUM)	* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_LARGE)	* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XL)		* out_pText );
	uint32_t getTextDataFromString( const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XXL)		* out_pText );
};

#endif //_GFILE_CORE_H