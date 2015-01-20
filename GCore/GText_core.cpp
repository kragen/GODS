/// This file is part of the Game Object Data System
/// Copyright © 2011: Pablo Ariel Zorrilla Cepeda
// 4637-4251
#include "GText_core.h"
#include "GMemory_core.h"
#include "GCore_function_macros.h"
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <strsafe.h>
#endif


#include "GCore_printf.h"
#include "GDebugger_core.h"
#include "glist.h"

using namespace god;

//__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( FILE, File );
//
//__GDEFINE_PRINTINFOSTRING_FUNCTIONW( __GODS(FILE),
//	L"nNameStrSize..................: %i\n"
//	L"wszName (Address).............: 0x%p\n"
//	L"wszName (Data)................: %s"
//	, (int)in_CoreInstance->nNameStrSize
//	, in_CoreInstance->wszName
//	, in_CoreInstance->wszName
//	);

#if defined(ANDROID)
#define GET_TEXT_DATA_FROM_STRING( TOKEN )																					\
uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TOKEN)* out_pText )	\
{																															\
	GOPTR(TOKEN) newText;																									\
	if( 0 == out_pText )																									\
	{																														\
		error_printf("null pointer as parameter.");																			\
		return 0;																											\
	}																														\
	if( nCharCount != -1 )																									\
	{																														\
		memcpy( newText->Text, szText, nCharCount );																		\
		newText->Text[nCharCount] = 0;																						\
		memset(newText->WText, 0, sizeof(newText->WText));																	\
		mbstowcs( newText->WText, newText->Text, sizeof(newText->Text)-1 );													\
		newText->WText[nCharCount] = 0;																						\
																															\
	}																														\
	else																													\
	{																														\
		nCharCount = 0;																										\
		while( szText[nCharCount] )																							\
		{																													\
			newText->Text[nCharCount] = szText[nCharCount];																	\
			nCharCount++;																									\
		}																													\
		newText->Text[nCharCount] = 0;																						\
		memset(newText->WText, 0, sizeof(newText->WText));																	\
		mbstowcs( newText->WText, newText->Text, sizeof(newText->Text)-1 );													\
		newText->WText[nCharCount] = 0;																						\
	}																														\
	*out_pText = newText.acquire();																							\
	return nCharCount;																										\
}
#elif defined(WIN32) || defined(_WIN32)
#define GET_TEXT_DATA_FROM_STRING( TOKEN )																					\
uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TOKEN)* out_pText )	\
{																															\
	GOPTR(TOKEN) newText;																									\
	if( 0 == out_pText )																									\
	{																														\
		error_printf("null pointer as parameter.");																			\
		return 0;																											\
	}																														\
	uint32_t _Index = 0;																									\
	if( nCharCount != 0xFFFFFFFFUL )																						\
	{																														\
		while( (_Index < nCharCount)  && szText[_Index] )																	\
		{																													\
			newText->Text[_Index] = szText[_Index];																			\
			_Index++;																										\
		}																													\
		/*memcpy( newText->Text, szText, nCharCount );*/																	\
		newText->Text[_Index] = 0;																							\
		memset(newText->WText, 0, sizeof(newText->WText));																	\
		MultiByteToWideChar(nCodePage, MB_PRECOMPOSED, newText->Text, _Index, newText->WText, 80);						\
		newText->WText[_Index] = 0;																							\
																															\
	}																														\
	else																													\
	{																														\
		while( szText[_Index] )																								\
		{																													\
			newText->Text[_Index] = szText[_Index];																			\
			_Index++;																										\
		}																													\
		newText->Text[_Index] = 0;																							\
		memset(newText->WText, 0, sizeof(newText->WText));																	\
		MultiByteToWideChar(nCodePage, MB_PRECOMPOSED, newText->Text, _Index, newText->WText, 80);							\
		newText->WText[_Index] = 0;																							\
	}																														\
	newText->CharCount = _Index;																							\
	*out_pText = newText.acquire();																							\
	return nCharCount;																										\
}
#endif

GET_TEXT_DATA_FROM_STRING( TEXT_XXS )
GET_TEXT_DATA_FROM_STRING( TEXT_XS )
//GET_TEXT_DATA_FROM_STRING( TEXT_SMALL )
uint32_t god::getTextDataFromString(const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_SMALL)* out_pText)
{
	GOPTR(TEXT_SMALL) newText;
	if (0 == out_pText)
	{
		error_printf("null pointer as parameter.")
		return 0;
	}
	uint32_t _Index = 0;
	if (nCharCount != 0xFFFFFFFFUL)
	{
		while ((_Index < nCharCount) && szText[_Index])
		{
			newText->Text[_Index] = szText[_Index];
			_Index++;
		}
		/*memcpy( newText->Text, szText, nCharCount );*/
		newText->Text[_Index] = 0;
		memset(newText->WText, 0, sizeof(newText->WText));							
		MultiByteToWideChar(nCodePage, MB_PRECOMPOSED, newText->Text, _Index, newText->WText, 80);
		newText->WText[_Index] = 0;
	}
	else
	{
		while (szText[_Index])
		{
			newText->Text[_Index] = szText[_Index];
			_Index++;
		}
		newText->Text[_Index] = 0;
		memset(newText->WText, 0, sizeof(newText->WText));
		MultiByteToWideChar(nCodePage, MB_PRECOMPOSED, newText->Text, _Index, newText->WText, 80);
		newText->WText[_Index] = 0;
	}
	newText->CharCount = _Index;
	*out_pText = newText.acquire();
	return nCharCount;
}
GET_TEXT_DATA_FROM_STRING(TEXT_MEDIUM)
GET_TEXT_DATA_FROM_STRING(TEXT_LARGE)
GET_TEXT_DATA_FROM_STRING(TEXT_XL)
GET_TEXT_DATA_FROM_STRING(TEXT_XXL)

uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XXS)		* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XS)		* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_SMALL)	* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_MEDIUM)	* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_LARGE)	* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XL)		* out_pText);
uint32_t god::getTextDataFromString(const wchar_t* szText, uint32_t nCharCount, GODS(TEXT_XXL)		* out_pText);
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XXS)		* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XS)			* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_SMALL)		* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_MEDIUM)		* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_LARGE)		* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XL)			* out_pText );
//uint32_t god::getTextDataFromString( const char* szText, uint32_t nCharCount, uint32_t nCodePage, GODS(TEXT_XXL)		* out_pText );



__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_XXS,TextXXS);
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_XS,TextXS);
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_SMALL,TextSmall);
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_MEDIUM,TextMedium);
	//__GCORE_STRUCT_INIT_STATIC_MEMBERS( TEXT_LARGE )
	const god::cue_t	__GODS(TEXT_LARGE)::__kCue			= "TEXT_LARGE";
	const uint32_t		__GODS(TEXT_LARGE)::__kDataSize		= GCORE_STRUCT_DATA_SIZE(__GODS(TEXT_LARGE));
	const uint8_t		__GODS(TEXT_LARGE)::__kCueLen		= (uint8_t)strlen("TEXT_LARGE");
	const uint8_t		__GODS(TEXT_LARGE)::__kDataOffset	= GCORE_STRUCT_HEADER_SIZE;
	const uint8_t		__GODS(TEXT_LARGE)::__kDataTurbo		= (uint8_t)(
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 8) ) ? 8 :
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 4) ) ? 4 :
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 2) ) ? 2 :
																1 );
	const uint8_t		__GODS(TEXT_LARGE)::__kDataReserved	= (uint8_t) (
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 8) ) ? (sizeof(__GODS(TEXT_LARGE))-GCORE_STRUCT_HEADER_SIZE)/8 :
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 4) ) ? (sizeof(__GODS(TEXT_LARGE))-GCORE_STRUCT_HEADER_SIZE)/4 :
																( 0 == (sizeof(__GODS(TEXT_LARGE)) % 2) ) ? (sizeof(__GODS(TEXT_LARGE))-GCORE_STRUCT_HEADER_SIZE)/2 :
																sizeof(__GODS(TEXT_LARGE))-GCORE_STRUCT_HEADER_SIZE );
	template<> GODS(TEXT_LARGE)	const 	gbaselist_safe<GODS(TEXT_LARGE)>::INVALID	= 0;
	template<> GODS(TEXT_LARGE)			gbaselist_safe<GODS(TEXT_LARGE)>::_INVALID	= 0;
	template<> GODS(TEXT_LARGE)			gbase_ptr<GODS(TEXT_LARGE)>::INVALID		= 0;

	__GDEFINE_CREATEDATA_FUNCTION_NO_MEMBER_INIT( __GODS(TEXT_LARGE), TextLarge );
	__GDEFINE_FREEDATA_FUNCTION_NO_MEMBER_CLEANUP( __GODS(TEXT_LARGE), TextLarge );
	__GDEFINE_ACQUIREDATA_FUNCTION( __GODS(TEXT_LARGE), TextLarge );
	__GDEFINE_COPYDATA_FUNCTION_NO_MEMBER_POINTERS( __GODS(TEXT_LARGE) );
	__GDEFINE_CLONEDATA_FUNCTION_NO_MEMBER_POINTERS( __GODS(TEXT_LARGE) );
	__GDEFINE_COMPAREDATA_FUNCTION( __GODS(TEXT_LARGE) );
	__GDEFINE_FILEMEM_SERIALIZATION_FUNCTIONS_NO_MEMBER_POINTERS( __GODS(TEXT_LARGE) );
	__GDEFINE_FILEMEM_STREAMING_FUNCTIONS_NO_MEMBER_POINTERS( __GODS(TEXT_LARGE) );
	__GDEFINE_FREEARRAY_FUNCTION( __GODS(TEXT_LARGE) );
//__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD( TEXT_LARGE, TextLarge );
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_XL, TextXL);
__GDEFINE_CORE_FUNCTIONS_NO_MEMBER_INIT_CLEANUP_SAVELOAD(TEXT_XXL, TextXXL);

#define __GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TOKEN )	\
	__GDEFINE_PRINTINFOSTRING_FUNCTIONA( __GODS(TOKEN),		\
		"CharCount....................: %i\n"				\
		"Text.........................: 0x%p"				\
		"Text.........................: %s"					\
		, (int)in_CoreInstance->CharCount					\
		, in_CoreInstance->Text								\
		, in_CoreInstance->Text								\
	);

__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_XXS );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_XS );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_SMALL );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_MEDIUM );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_LARGE );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_XL );
__GDEFINE_TEXT_PRINTINFOSTRING_FUNCTIONA( TEXT_XXL );

