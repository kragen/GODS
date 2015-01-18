#include "gbase_ptr.h"
#include <string.h>

#ifndef _GCORE_PTR_H
#define _GCORE_PTR_H

namespace god
{
	// ------------------------------------------------------------ gcore_ptr ------------------------------------------------------------  
#define GOPTR(TOKEN)	gcore_ptr<GODS(TOKEN)>
	template <typename _T> class gcore_ptr : public gbase_ptr_safe<_T>
	{
	public:
		gcore_ptr( void )
		{};
		gcore_ptr( const gbase_ptr_safe<_T>& other )
			:gbase_ptr_safe<_T>::gbase_ptr_safe(other){};
		gcore_ptr( _T CoreInstance )
		{
			this->m_CoreInstance = CoreInstance;
		};
		virtual ~gcore_ptr( void )
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
		virtual gcore_ptr&				operator =( const gbase_ptr_safe<_T>& other )
		{
			gbase_ptr_safe<_T>::operator=(other);
			return *this;
		};

		virtual gcore_ptr&				operator =( const gcore_ptr<_T>& other )
		{
			if( other.m_CoreInstance == this->m_CoreInstance )
				return *this;
			this->_set( other.acquire() );
			return *this;
		};
		virtual gcore_ptr&				operator =( _T CoreInstance )
		{
			gbase_ptr_safe<_T>::operator=(CoreInstance);
			return *this;
		};

		virtual _T&						operator->( void )
		{
			if( 0 == this->m_CoreInstance )
				this->_create();
			return this->m_CoreInstance;
		};
		virtual const _T&				operator->( void ) const
		{
			// let's fail on reading uninitialized values
			return this->m_CoreInstance;
		};
		virtual void					create( void )
		{
			this->_create();
		};
		virtual void					clone( gcore_ptr<_T>& target ) const
		{
			_T newInstance = 0;
			gcloneData( &newInstance, this->get_pointer() );
			
			target = newInstance;

		};
		virtual int32_t					compare_data( _T other ) const
		{
			return gcompareData( other, this->m_CoreInstance );
		};
		virtual error_t					serialize( FILE* out_fp ) const
		{
			return fileSerializeData( &this->m_CoreInstance, 1, out_fp ) != 1;
		};
		virtual error_t					deserialize( FILE* in_fp )
		{
			return fileDeserializeData( &this->m_CoreInstance, 1, in_fp ) != 1;
		};
		virtual error_t					serialize( void* out_fp, uint32_t* out_nReadBytes ) const
		{
			if( 0 == out_fp )
				return -1;
			uint32_t nReadBytes = memSerializeData( &this->m_CoreInstance, 1, out_fp );
			if( out_nReadBytes )
				*out_nReadBytes += nReadBytes;
			return 0;
		};
		virtual error_t					deserialize( const void* in_fp, uint32_t* out_nReadBytes )
		{
			if( 0 == in_fp )
				return -1;
			uint32_t nReadBytes = memDeserializeData( &this->m_CoreInstance, 1, in_fp );
			if( out_nReadBytes )
				*out_nReadBytes += nReadBytes;
			return 0;
		};
		virtual error_t					write( FILE* out_fp, _T DefaultData=0 ) const
		{
			return fileWriteData( &this->m_CoreInstance, 1, out_fp, DefaultData ) != 1;
		};
		virtual error_t					read( FILE* in_fp )
		{
			return fileReadData( &this->m_CoreInstance, 1, in_fp ) != 1;
		};
		virtual error_t					write( void* out_fp, uint32_t* out_nReadBytes, _T DefaultData=0 ) const
		{
			if( 0 == out_fp )
				return -1;
			uint32_t nReadBytes = memWriteData( &this->m_CoreInstance, 1, out_fp, DefaultData );
			if( out_nReadBytes )
				*out_nReadBytes += nReadBytes;
			return 0;
		};
		virtual error_t					read( const void* in_fp, uint32_t* out_nReadBytes )
		{
			if( 0 == in_fp )
				return -1;
			uint32_t nReadBytes = memReadData( &this->m_CoreInstance, 1, in_fp );
			if( out_nReadBytes )
				*out_nReadBytes += nReadBytes;
			return 0;
		};
		virtual char*					get_data_address( void )
		{
			return this->m_CoreInstance ? ((char*)this->m_CoreInstance) + (this->m_CoreInstance->__kDataOffset) : 0;
		};
		virtual const char*				get_data_address( void ) const
		{
			return this->m_CoreInstance ? ((char*)this->m_CoreInstance) + (this->m_CoreInstance->__kDataOffset) : 0;
		};
		virtual uint32_t				get_data_size( void ) const
		{
			return this->m_CoreInstance ? this->m_CoreInstance->__kDataSize : 0;
		};
	}; // gcore_ptr

	// ------------------------------------------------------------ genius_ptr ------------------------------------------------------------  
#define GENIUSS(TOKEN) genius_ptr<GODS(TOKEN)>
	template<typename _T> class genius_ptr : public gcore_ptr<_T>
	{
	public:
		genius_ptr( void )
			:onCreate(0), onUpdate(0), onFree(0)
		{};
		genius_ptr( const gcore_ptr<_T>& other )
			:onCreate(0), onUpdate(0), onFree(0)
		{
			this->m_CoreInstance	= gacquireData( other.m_CoreInstance );
		};
		genius_ptr( const genius_ptr<_T>& other )
			:onCreate(other.onCreate), onUpdate(other.onUpdate), onFree(other.onFree)
		{
			this->m_CoreInstance	= gacquireData( other.m_CoreInstance );
		};
		genius_ptr( _T CoreInstance )
			:onCreate(0), onUpdate(0), onFree(0)
		{
			this->m_CoreInstance	= CoreInstance;
		};
		genius_ptr( void (*_onCreate)(god::genius_ptr<_T> *), void (*_onUpdate)(god::genius_ptr<_T> *), void (*_onFree)(god::genius_ptr<_T> *), _T other=0 )
			:onCreate(_onCreate), onUpdate(_onUpdate), onFree(_onFree)
		{
			this->m_CoreInstance = other;
		};
		genius_ptr( void (*_onCreate)(god::genius_ptr<_T> *), void (*_onUpdate)(god::genius_ptr<_T> *), void (*_onFree)(god::genius_ptr<_T> *), god::gcore_ptr<_T> other )
			:onCreate(_onCreate), onUpdate(_onUpdate), onFree(_onFree)
		{
			this->m_CoreInstance = gacquireData( other );
		};
		virtual ~genius_ptr( void )
		{
			if( this->valid() && onFree )
				(*onFree)( this );
			if( m_History )
				free_history();
		};
		// operators ------------------------------------------------------------------------------------
		virtual genius_ptr&				operator =( const gbase_ptr<_T>& other )
		{
			if( other == this->m_CoreInstance 
				&& other == m_History )
				return *this;
			_set( other.acquire() );
			if( this->valid() && onUpdate )
				onUpdate( this );
			return *this;
		};
		virtual genius_ptr&				operator =( const genius_ptr<_T>& other )
		{ // we don't copy the handlers!
			return operator=( (const gcore_ptr<_T>&) other );
		};
		virtual genius_ptr&				operator =( _T CoreInstance )
		{
			if( CoreInstance == this->m_CoreInstance 
				&& CoreInstance == m_History )
			{
				genius_ptr<_T>( 0, 0, onFree, CoreInstance );	// Catch the reference.
				return *this;
			}
			_set( CoreInstance );
			if( this->valid() && onUpdate )
				(*onUpdate)( this );
			return *this;
		};
		// methods ---------------------------------------------------------------------------------------
		virtual void					onCreateHandler( void (*Handler)( genius_ptr<_T>* ) )
		{
			onCreate	= Handler;
		};
		virtual void					onUpdateHandler( void (*Handler)( genius_ptr<_T>* ) )
		{
			onUpdate	= Handler;
		};
		virtual void					onFreeHandler( void (*Handler)( genius_ptr<_T>* ) )
		{
			onFree		= Handler;
		};
		virtual void					free( void )
		{
			if( this->valid() && onFree )
				(*onFree)( this );
			_set( 0 );
		};
		virtual bool					am_I_owning( void ) const
		{
			return gcore_ptr<_T>::am_I_owning()
				|| (this->m_CoreInstance->__nRefCount == 2 && m_History == this->m_CoreInstance);
		};
		virtual const gcore_ptr<_T>&	get_history( void ) const
		{
			return m_History;
		};
		virtual void					roll_back( void )
		{	// Swap the pointers.
			_T oldInstance	= this->m_CoreInstance;
			this->m_CoreInstance	= m_History.acquire();
			m_History		= oldInstance; // catch the reference
			if( this->valid() && onUpdate )
				(*onUpdate)( this );
		};
		virtual void					free_history( void )
		{
			genius_ptr<_T> old(0, 0, onFree, m_History);	// call destroy handlers for history contents and release the instance.
			m_History.free();
		};
	protected:
		virtual void					_create( void )
		{
			_T newInstance=0;
			gcreateData( &newInstance );
			_set( newInstance );
			if( this->valid() && onCreate )
				(*onCreate)( this );
		};
		virtual	void					_set( const _T& CoreInstance )
		{
			genius_ptr<_T> old(0, 0, onFree, m_History);	// call destroy handlers for history contents and release the instance.
			m_History = this->m_CoreInstance;
			this->m_CoreInstance = CoreInstance;
		}
		genius_ptr( void (*_onCreate)(god::genius_ptr<_T> *), void (*_onUpdate)(god::genius_ptr<_T> *), void (*_onFree)(god::genius_ptr<_T> *), god::gbase_ptr_safe<_T> other )
			:onCreate(_onCreate), onUpdate(_onUpdate), onFree(_onFree)
		{
			this->m_CoreInstance = gacquireData( other );
		};

	private:
		gcore_ptr<_T>		m_History;	// history doesn't call update handlers.
		void				(*onUpdate)( genius_ptr<_T>* instance );
		void				(*onCreate)( genius_ptr<_T>* instance );
		void				(*onFree)( genius_ptr<_T>* instance );
	};

	// ------------------------------------------------------------ gbase_handler_safe ------------------------------------------------------------  
#define GENIUSH(TOKEN) gbase_handler_safe<GOSH(TOKEN)>
	template<typename _T> class gbase_handler_safe : public gbase_ptr_safe<_T>
	{
		void			(*onCreate)( gbase_ptr_safe<_T>* instance );
		void			(*onUpdate)( gbase_ptr_safe<_T>* instance );
		void			(*onFree)( gbase_ptr_safe<_T>* instance );
	public:
		gbase_handler_safe( void )
			:onCreate(0), onUpdate(0), onFree(0)
		{};
		~gbase_handler_safe( void )
		{
			if( this->valid() && onFree )
				(*onFree)( this );
		}
		gbase_handler_safe( const gbase_ptr<_T>& other )
			:onCreate(0), onUpdate(0), onFree(0)
		{
			this->m_CoreInstance	= other.acquire();
		};
		gbase_handler_safe( const gbase_handler_safe<_T>& other )
			:onCreate(other.onUpdate), onUpdate(other.onCreate), onFree(other.onFree)
		{
			this->m_CoreInstance	= other.acquire();
		};
		gbase_handler_safe( void (*_onCreate)(god::gbase_ptr_safe<_T> *), void (*_onUpdate)(god::gbase_ptr_safe<_T> *), void (*_onFree)(god::gbase_ptr_safe<_T> *), const _T& other=0 )
			:onCreate(_onCreate), onUpdate(_onUpdate), onFree(_onFree)
		{
			this->m_CoreInstance = other;
		};
		gbase_handler_safe( void (*_onCreate)(god::gbase_ptr_safe<_T> *), void (*_onUpdate)(god::gbase_ptr_safe<_T> *), void (*_onFree)(god::gbase_ptr_safe<_T> *), const god::gbase_ptr_safe<_T>& other )
			:onCreate(_onCreate), onUpdate(_onUpdate), onFree(_onFree)
		{
			this->m_CoreInstance = other.acquire();
		};
		// methods ---------------------------------------------------------------------------------------
		virtual gbase_handler_safe&		operator =( _T CoreInstance )
		{
			if( CoreInstance == this->m_CoreInstance )
			{	// just free because there are no handlers to be called for this situation
				gfreeData( &CoreInstance );
				return *this;
			}
			_set( CoreInstance );
			if( this->valid() && onUpdate )
				(*onUpdate)( this );
			return *this;
		};
		virtual gbase_handler_safe&		operator =( const gbase_ptr<_T>& other )
		{
			if( other.get_pointer() == this->m_CoreInstance )
				return *this;

			_set( other.acquire() );
			if( this->valid() && onUpdate )
				(*onUpdate)( this );
			return *this;
		};
		virtual gbase_handler_safe&		operator =( const gbase_handler_safe<_T>& other )
		{
			if( other.m_CoreInstance == this->m_CoreInstance )
				return *this;
			_set( other.acquire() );
			if( this->valid() && onUpdate )
				(*onUpdate)( this );
			return *this;
		};
		virtual void					create( void )
		{
			if( this->m_CoreInstance && onFree )
				(*onFree)( this );
			this->_create();
			if( this->valid() && onCreate )
				(*onCreate)( this );
		};
		virtual void					free( void )
		{
			if( this->valid() && onFree )
				(*onFree)( this );
			gbase_ptr_safe<_T>::free();
		};
		virtual void					onCreateHandler( void (*Handler)( gbase_ptr_safe<_T>* ) )
		{
			onCreate	= Handler;
		};
		virtual void					onUpdateHandler( void (*Handler)( gbase_ptr_safe<_T>* ) )
		{
			onUpdate	= Handler;
		};
		virtual void					onFreeHandler( void (*Handler)( gbase_ptr_safe<_T>* ) )
		{
			onFree		= Handler;
		};
	protected:
		virtual void					_set( const _T& CoreInstance )
		{
			// this will call the onFree handler and release the reference on destruction
			gbase_handler_safe<_T> old(0, 0, onFree, this->m_CoreInstance);
			this->m_CoreInstance = CoreInstance;
		}
		virtual gbase_handler_safe&		operator =( const gcore_ptr<_T>& other )
		{
			operator=( (const gbase_ptr_safe<_T>&)other );
			return *this;
		};
	}; // gbase_handler_safe


} // namespace

#endif // _GCORE_PTR_H