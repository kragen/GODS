#include "GMemory_core.h"
#include "GCore_printf.h"

#ifndef _GBASEBUFFER_H
#define _GBASEBUFFER_H

namespace god
{
	typedef class _CGBaseBuffer
	{
	public:
		_CGBaseBuffer( void );
		_CGBaseBuffer( const _CGBaseBuffer& l );
		virtual ~_CGBaseBuffer( void );
		virtual _CGBaseBuffer&			operator =( const _CGBaseBuffer& );
	protected:
		virtual uint32_t				GetSizeInBytes( void ) const;
		virtual uint32_t				GetElementCount( void ) const;
		virtual void					SetBufferData( GODS(BUFFER) newBufferData );
		virtual GODS(BUFFER)			GetBufferData( void ) const;
		virtual GDATA_TYPE				GetDataFormat( void ) const;
		virtual void*					GetAddress( void );
		virtual const void*				GetAddress( void ) const;
	protected:
		GOPTR(BUFFER)					m_BufferData;
	} CGBaseBuffer;
};

#endif // _GBASEBUFFER_H