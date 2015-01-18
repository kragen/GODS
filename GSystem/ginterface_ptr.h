#include "gbase_ptr.h"

#ifndef _GINTERFACE_PTR_H
#define _GINTERFACE_PTR_H

namespace god
{
	// ------------------------------------------------------------ gcore_ptr ------------------------------------------------------------  
#define GHPTR(TOKEN)	ginterface_ptr<GOSH(TOKEN)>
	template <typename _T> class ginterface_ptr : public ginterface_ptr_safe<_T>
	{
	public:
		ginterface_ptr( void )
		{};
		ginterface_ptr( const ginterface_ptr<_T>& other )
			:ginterface_ptr_safe<_T>::ginterface_ptr_safe(other){};
		ginterface_ptr( _T CoreInstance )
		{
			this->m_CoreInstance = CoreInstance;
		};
		virtual ~ginterface_ptr( void )
		{};
		virtual _T*						operator &( void )
		{
			return &this->m_CoreInstance;
		};
		virtual const _T*				operator &( void ) const
		{
			return &this->m_CoreInstance;
		};
		virtual operator				_T&( void )
		{
			return this->m_CoreInstance;
		};
		virtual operator				const _T&( void ) const
		{
			return this->m_CoreInstance;
		};
		virtual ginterface_ptr&			operator =( const ginterface_ptr<_T>& other )
		{
			if( other.m_CoreInstance == this->m_CoreInstance )
				return *this;
			this->_set( other.acquire() );
			return *this;
		};
		virtual ginterface_ptr&			operator =( _T CoreInstance )
		{
			ginterface_ptr_safe<_T>::operator=(CoreInstance);
			return *this;
		};

		//virtual _T&						operator->( void )
		//{
		//	if( 0 == m_CoreInstance )
		//		_create();
		//	return m_CoreInstance;
		//};
		virtual const _T&				operator->( void ) const
		{
			// let's fail on reading uninitialized values
			return this->m_CoreInstance;
		};
		//virtual void					create( void )
		//{
		//	_create();
		//};
		//virtual void					clone( ginterface_ptr<_T>& target ) const
		//{
		//	_T newInstance = 0;
		//	gcloneInterface( &newInstance, get_pointer() );
		//	if( target )
		//		target = newInstance;
		//};
		//virtual int32_t					compare_data( _T other ) const
		//{
		//	return gcompareInterface( other, m_CoreInstance );
		//};
		//virtual error_t					serialize( FILE* out_fp ) const
		//{
		//	return fileSerializeInterface( &m_CoreInstance, 1, out_fp ) != 1;
		//};
		//virtual error_t					deserialize( FILE* in_fp )
		//{
		//	return fileDeserializeInterface( &m_CoreInstance, 1, in_fp ) != 1;
		//};
		//virtual error_t					serialize( void* out_fp, uint32_t* out_nReadBytes ) const
		//{
		//	if( 0 == out_fp )
		//		return -1;
		//	uint32_t nReadBytes = memSerializeInterface( &m_CoreInstance, 1, out_fp );
		//	if( out_nReadBytes )
		//		*out_nReadBytes += nReadBytes;
		//	return 0;
		//};
		//virtual error_t					deserialize( const void* in_fp, uint32_t* out_nReadBytes )
		//{
		//	if( 0 == in_fp )
		//		return -1;
		//	uint32_t nReadBytes = memDeserializeInterface( &m_CoreInstance, 1, in_fp );
		//	if( out_nReadBytes )
		//		*out_nReadBytes += nReadBytes;
		//	return 0;
		//};
		//virtual error_t					write( FILE* out_fp, _T DefaultInterface=0 ) const
		//{
		//	return fileWriteInterface( &m_CoreInstance, 1, out_fp, DefaultInterface ) != 1;
		//};
		//virtual error_t					read( FILE* in_fp )
		//{
		//	return fileReadInterface( &m_CoreInstance, 1, in_fp ) != 1;
		//};
		//virtual error_t					write( void* out_fp, uint32_t* out_nReadBytes, _T DefaultInterface=0 ) const
		//{
		//	if( 0 == out_fp )
		//		return -1;
		//	uint32_t nReadBytes = memWriteInterface( &m_CoreInstance, 1, out_fp, DefaultInterface );
		//	if( out_nReadBytes )
		//		*out_nReadBytes += nReadBytes;
		//	return 0;
		//};
		//virtual error_t					read( const void* in_fp, uint32_t* out_nReadBytes )
		//{
		//	if( 0 == in_fp )
		//		return -1;
		//	uint32_t nReadBytes = memReadInterface( &m_CoreInstance, 1, in_fp );
		//	if( out_nReadBytes )
		//		*out_nReadBytes += nReadBytes;
		//	return 0;
		//};
		//virtual char*					get_data_address( void )
		//{
		//	return m_CoreInstance ? ((char*)m_CoreInstance) + (m_CoreInstance->__kInterfaceOffset) : 0;
		//};
		//virtual const char*				get_data_address( void ) const
		//{
		//	return m_CoreInstance ? ((char*)m_CoreInstance) + (m_CoreInstance->__kInterfaceOffset) : 0;
		//};
		//virtual uint32_t				get_data_size( void ) const
		//{
		//	return m_CoreInstance ? m_CoreInstance->__kInterfaceSize : 0;
		//};
	}; // ginterface_ptr
};

#endif // _GINTERFACE_PTR_H