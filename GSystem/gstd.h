#include "GCore_constants.h"

#define __GDECLARE_SECURE_TPL_FUNCTION0(_ReturnType, _FunctionName, _SrcType)	\
	template <size_t _Size> _ReturnType _FunctionName(_SrcType (&src)[_Size]);

#define __GDECLARE_SECURE_TPL_FUNCTION1(_ReturnType, _FunctionName, _DstType, _SrcType)	\
	template <size_t _Size> _ReturnType _FunctionName(_DstType (&dst)[_Size], _SrcType src);


namespace god
{
    extern "C++"
    {
		uint32_t gstrlen( const char*, size_t nMaxCount );
		error_t gwcslen( const wchar_t*, size_t nMaxCount );

		error_t gstrcmp( const char*, size_t nMaxCount );
		error_t gwcscmp( const wchar_t*, size_t nMaxCount );

		error_t gstrcmp( const char*, size_t nMaxCount );
		error_t gwcscmp( const wchar_t*, size_t nMaxCount );

		__GDECLARE_SECURE_TPL_FUNCTION0(uint32_t, gstrlen, const char *)
		__GDECLARE_SECURE_TPL_FUNCTION1(error_t, gstrcpy, char, const char *)
		__GDECLARE_SECURE_TPL_FUNCTION1(error_t, gstrcat, char, const char *)
	}

}