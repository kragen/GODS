#include "GCore_constants.h"
#include "GCore_macros.h"
#include "GCore_printf.h"
#include <stdio.h>
#if defined(__linux__) || defined(ANDROID)
#define _CrtDbgBreak() do {} while(0)
#elif defined(WIN32) || defined(_WIN32) 
#include <crtdbg.h>
#endif


#ifndef _GBASE_PTR_H
#define _GBASE_PTR_H

namespace god
{
	template <typename _T> class gbase_ptr
	{
	protected:
		_T	m_CoreInstance;
		static _T	INVALID;
	protected:
		virtual void				_set( const _T& CoreInstance )=0;
	public:
		gbase_ptr( void )
			:m_CoreInstance(0){};
		virtual ~gbase_ptr( void )
		{};	// Warning! This class doesn't release the pointer!

		// methods -------------------------------------------------------------------------------
		virtual _T					acquire( void ) const=0;
		virtual void				free( void )
		{
			_set(0);
		};
		virtual bool				valid( void ) const
		{
			return 0 != m_CoreInstance;
		};
		virtual const _T			get_pointer( void ) const
		{
			return m_CoreInstance;
		};
		virtual bool				am_I_owning( void ) const
		{
			return m_CoreInstance == 0 || m_CoreInstance->__nRefCount == 1;
		};
		// operators -------------------------------------------------------------------------------
		virtual gbase_ptr&			operator =( const gbase_ptr<_T>& other )
		{
			if( other.m_CoreInstance && (0 == other.m_CoreInstance->__nRefCount) )
				_CrtDbgBreak();
			if( other.m_CoreInstance == m_CoreInstance )
				return *this;
			_set(other.acquire());
			return *this;
		};
		virtual bool				operator==( const gbase_ptr<_T>& other ) const
		{
			return m_CoreInstance == other.m_CoreInstance;
		};
		virtual bool				operator==( const _T& other ) const
		{
			return m_CoreInstance == other;
		};
		virtual bool				operator!=( const gbase_ptr<_T>& other ) const
		{
			return m_CoreInstance != other.m_CoreInstance;
		};
		virtual bool				operator!=( const _T& other ) const
		{
			return m_CoreInstance != other;
		};
		virtual bool				operator!( void ) const
		{
			return 0 == m_CoreInstance;
		};
		virtual const _T&			operator->( void ) const
		{
			// let's fail on reading uninitialized values
			return m_CoreInstance;
		};
	};

	template <typename _T> class gbase_ptr_safe : public gbase_ptr<_T>
	{
	public:
		gbase_ptr_safe( void ){};
		gbase_ptr_safe( const gbase_ptr_safe<_T>& other )
		{
			this->m_CoreInstance = gacquireData( other.m_CoreInstance );
		};
		virtual ~gbase_ptr_safe( void )
		{
			gfreeData( &this->m_CoreInstance );
		};
		virtual gbase_ptr_safe&			operator =( _T CoreInstance )
		{
			if( CoreInstance == this->m_CoreInstance )
			{
				gfreeData( &CoreInstance );
				return *this;
			}
			if( CoreInstance && (0 == CoreInstance->__nRefCount) )
			{
				printInfoString( CoreInstance );
				_CrtDbgBreak();
			}
			_set(CoreInstance);
			return *this;
		};
		virtual gbase_ptr_safe&			operator =( const gbase_ptr_safe<_T>& other )
		{
			if( other.m_CoreInstance && (0 == other.m_CoreInstance->__nRefCount) )
			{
				printInfoString( other.m_CoreInstance );
				_CrtDbgBreak();
			}
			if( other == this->m_CoreInstance )
				return *this;

			_set(other.acquire());
			return *this;
		};
		virtual _T					acquire( void ) const
		{
			return gacquireData( this->m_CoreInstance );
		};
		virtual void				acquire( const _T& CoreInstance )
		{
			_set( gacquireData( CoreInstance ) );
		};
	protected:
		virtual void				_create( void )
		{
			gcreateData( &this->m_CoreInstance );
		};
		virtual void				_set( const _T& CoreInstance )
		{
			_T old = this->m_CoreInstance;
			this->m_CoreInstance = CoreInstance;
			gfreeData( &old );
		}
	}; // gbase_ptr_safe

	template <typename _T> class ginterface_ptr_safe : public gbase_ptr<_T>
	{
	public:
		ginterface_ptr_safe( void ){};
		ginterface_ptr_safe( const ginterface_ptr_safe<_T>& other )
		{
			this->m_CoreInstance = gacquireInterface( other.m_CoreInstance );
		};
		virtual ~ginterface_ptr_safe( void )
		{
			gfreeInterface( &this->m_CoreInstance );
		};
		virtual ginterface_ptr_safe&			operator =( _T CoreInstance )
		{
			if( CoreInstance == this->m_CoreInstance )
			{
				gfreeInterface( &CoreInstance );
				return *this;
			}
			if( CoreInstance  && (0 == CoreInstance->__nRefCount) )
			{
				error_printf( "%p", CoreInstance );
				_CrtDbgBreak();
			}
			_set(CoreInstance);
			return *this;
		};
		virtual ginterface_ptr_safe&			operator =( const ginterface_ptr_safe<_T>& other )
		{
			if( other.m_CoreInstance && (0 == other.m_CoreInstance->__nRefCount) )
			{
				error_printf( "%p", other.m_CoreInstance );
				_CrtDbgBreak();
			}
			if( other.m_CoreInstance == this->m_CoreInstance )
				return *this;

			_set(other.acquire());
			return *this;
		};
		virtual _T					acquire( void ) const
		{
			return gacquireInterface( this->m_CoreInstance );
		};
		virtual void				acquire( const _T& CoreInstance )
		{
			_set( gacquireInterface( CoreInstance ) );
		};
	protected:
		//virtual void				_create( void ) can't create pure virtual objects like this
		//{
		//	gcreateInterface( &m_CoreInstance );
		//};
		virtual void				_set( const _T& CoreInstance )
		{
			_T old = this->m_CoreInstance;
			this->m_CoreInstance = CoreInstance;
			gfreeInterface( &old );
		}
	}; // ginterface_ptr_safe
};

#endif // _GBASE_PTR_H
