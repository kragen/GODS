#include "gstd.h"

using namespace god;

uint32_t gstrlen( const char* p, size_t nMaxCount )
{
	uint32_t i=0;
	while( p[i] != 0 && i < nMaxCount )
		i++;

	return i;
};

#define __GDEFINE_SECURE_TPL_FUNCTION0(_ReturnType, _FunctionName, _SrcType)	\
	template <size_t _Size> _ReturnType _FunctionName(_SrcType (&src)[_Size])	\
	{																			\
		return _FunctionName(src, _Size);										\
	}

#define __GDEFINE_SECURE_TPL_FUNCTION1(_ReturnType, _FunctionName, _DstType, _SrcType)		\
	template <size_t _Size> _ReturnType _FunctionName(_DstType (&dst)[_Size], _SrcType src)	\
	{																						\
		return _FunctionName(dst, _Size, src);												\
	}

__GDEFINE_SECURE_TPL_FUNCTION0(uint32_t, god::gstrlen, const char *)
__GDEFINE_SECURE_TPL_FUNCTION1(error_t, god::gstrcpy, char, const char *)
__GDEFINE_SECURE_TPL_FUNCTION1(error_t, god::gstrcat, char, const char *)
