
#include "GBaseBuffer.h"
#include <vector>
#include "GCore_printf.h"

#if defined( _WIN32 )
//#include <Windows.h>
#endif

//	std::vector
using namespace god;

_CGBaseBuffer::_CGBaseBuffer( void )
{
};

_CGBaseBuffer::_CGBaseBuffer( const _CGBaseBuffer& l )
{
	m_BufferData = l.m_BufferData;
};

_CGBaseBuffer::~_CGBaseBuffer( void )
{
};

_CGBaseBuffer&		 _CGBaseBuffer::operator =( const _CGBaseBuffer& l )
{
	m_BufferData = l.m_BufferData;
	return *this;
};

uint32_t			_CGBaseBuffer::GetSizeInBytes( void ) const
{
	if( m_BufferData )
		return m_BufferData->nSizeInBytes;
	return 0;
};

uint32_t			_CGBaseBuffer::GetElementCount( void ) const
{
	if( m_BufferData )
		return m_BufferData->nElementCount;
	return 0;
};

void*				_CGBaseBuffer::GetAddress( void )
{
	if( m_BufferData )
		return m_BufferData->pByteArray;
	return 0;
};

const void*			_CGBaseBuffer::GetAddress( void ) const
{
	if( m_BufferData )
		return m_BufferData->pByteArray;
	return 0;
};

GDATA_TYPE			_CGBaseBuffer::GetDataFormat( void ) const
{
	if( m_BufferData )
		return m_BufferData->DataFormat;
	return GDATATYPE_UNKNOWN;
};

void				_CGBaseBuffer::SetBufferData( GODS(BUFFER) ListBuffer )
{
	m_BufferData = gacquireData( ListBuffer );
};

GODS(BUFFER)		_CGBaseBuffer::GetBufferData( void ) const 
{ 
	return gacquireData( m_BufferData ); 
};