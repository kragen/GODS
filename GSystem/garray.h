#include "GMemory_core.h"

#ifndef _GARRAY_H
#define _GARRAY_H

namespace god
{
	template<typename _T> class gelementary
	{
	public:
		gelementary( void ){};
		gelementary( const gelementary& other )
		{
			m_ArrayBuffer = gacquireData( other.m_ArrayBuffer );
		};
		gelementary( uint32_t size )
		{
			if( 0 > resize( size ) )
			{
				error_printf("Out of memory!");
			};
		};
		gelementary( uint32_t size, const _T* valueList )
		{
			if( 0 > resize( size ) )
			{
				error_printf("Out of memory!");
			} 
			else if( valueList )
			{
				for( uint32_t i=0; i<size; i++ )
					set_value( i, valueList[i] );
			}
		};
		virtual ~gelementary( void ){};

		virtual const _T&			operator[]( size_t index ) const 
		{
			if( !m_ArrayBuffer )
			{
				error_printf("Cannot access a null array!");
#if defined( ANDROID )
#else
				throw(L"");
#endif
				//return 0;
			}
			if( index >= m_ArrayBuffer->nElementCount )
			{
				error_printf( "Invalid index #%u!", (uint32_t)index );
#if defined( ANDROID )
#else
				throw(L"");
#endif
				//return 0;
			}
			return ((_T*)m_ArrayBuffer->pByteArray)[index];
		}
 
		virtual _T					get_value( size_t index ) const
		{
			return (*this)[index];
		}

		virtual error_t		push( const _T& value )
		{
			uint32_t oldSize = size();
			if( 0 > resize( oldSize+1 ) )
			{
				error_printf("Cannot resize array! Out of memory?");
				return -1;
			}
			else if( m_ArrayBuffer.am_I_owning() )
			{
				((_T*)m_ArrayBuffer->pByteArray)[oldSize] = value;
			}
			else
			{
				GOPTR(BUFFER) newArray( m_ArrayBuffer );
				newArray.clone( newArray );
				((_T*)newArray->pByteArray)[oldSize] = value;
				m_ArrayBuffer = newArray;
			}
			return 0;
		};

		virtual error_t				pop( _T* out_value )
		{
			uint32_t oldSize = size();
			_T value = get_value(oldSize-1);
			if( 0 > resize( oldSize-1 ) )
			{
				error_printf("Cannot resize array! Out of memory?");
				return -1;
			}
			if( out_value )
				*out_value = value;
			return 0;
		};

		virtual error_t				set_value( size_t index, const _T& value )
		{
			if( !m_ArrayBuffer )
			{
				error_printf("Cannot set a value into a null array!");
				return -1;
			}
			else if( index >= size() )
			{
				error_printf( "Invalid index: #%u!", (uint32_t)index );
				return -1;
			}
			else if( m_ArrayBuffer.am_I_owning() )
			{
				((_T*)m_ArrayBuffer->pByteArray)[index] = value;
				return 0;
			}
			GOPTR(BUFFER) newArray, oldArray( m_ArrayBuffer );
			bool bIsText = oldArray->Usage == GUSAGE_TEXT;
			if( 0 > gcreateBuffer( oldArray->DataFormat, oldArray->Usage, oldArray->nElementCount, &newArray ) )
			{
				newArray->DataFormat		= oldArray->DataFormat;
				newArray->Usage				= oldArray->Usage;
				newArray->nElementCount		= oldArray->nElementCount;
				newArray->nRowElementCount	= oldArray->nRowElementCount;
				newArray->nSizeInBytes		= sizeof(_T)*(newArray->nElementCount+one_if(bIsText));
				newArray->pByteArray		= malloc(newArray->nSizeInBytes);
				if( 0 == newArray->pByteArray )
				{
					newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
					error_printf( "malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)(sizeof(_T)*(newArray->nElementCount+one_if(bIsText))) );
					return -1;
				}
			}
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
			for( uint32_t iElement=0; iElement<copyCount; iElement++ )
			{
				((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];
			}
			((_T*)newArray->pByteArray)[index] = value;
			if( bIsText )
				memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );
				//((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
			m_ArrayBuffer = newArray;
			return 0;
		}

		virtual int32_t				find( const _T& value ) const
		{
			if( 0 == m_ArrayBuffer )
				return -1;

			for( uint32_t iElement=0; iElement<m_ArrayBuffer->nElementCount; iElement++ )
			{
				//if( ((_T*)m_ArrayBuffer->pByteArray)[iElement] == value )
				if( 0 == memcmp(&((_T*)m_ArrayBuffer->pByteArray)[iElement], &value, sizeof(_T) ) )
					return iElement;
			}

			return -1;
		}
		
		virtual void				clear( void )
		{
			if(0 == m_ArrayBuffer)
				return;
			if(m_ArrayBuffer.am_I_owning())
				m_ArrayBuffer->nElementCount = 0;
			else
				m_ArrayBuffer.free();
		}
		
		virtual _T*					get_pointer( void )
		{
			return m_ArrayBuffer ? ((_T*)m_ArrayBuffer->pByteArray) : 0;
		};

		virtual _T const*			get_pointer( void ) const
		{
			return m_ArrayBuffer ? ((_T*)m_ArrayBuffer->pByteArray) : 0;
		};

		virtual size_t				size( void ) const
		{
			return m_ArrayBuffer ? m_ArrayBuffer->nElementCount : 0;
		};

		virtual error_t				join( const gelementary<_T>& _A, const gelementary<_T>& _B )
		{
			return gjoinBuffers( _A.m_ArrayBuffer, _B.m_ArrayBuffer, &m_ArrayBuffer );
		};

		virtual error_t				split( uint32_t nIndex, gelementary<_T>& _A, gelementary<_T>& _B )
		{
			return gsplitBuffer( m_ArrayBuffer, nIndex, &_A.m_ArrayBuffer, &_B.m_ArrayBuffer );
		};

		virtual error_t				resize( size_t newSize )
		{
			if( 0 == newSize )
			{
				m_ArrayBuffer.free();
				return 0;
			}
			if( m_ArrayBuffer )
			{
				if( m_ArrayBuffer->nElementCount == newSize )
					return 0;
				uint32_t typeSize = sizeof(_T);
				bool bIsText = m_ArrayBuffer->Usage == GUSAGE_TEXT;
				if( ((newSize+one_if(bIsText))*typeSize) <= m_ArrayBuffer->nSizeInBytes
					&& m_ArrayBuffer.am_I_owning() )
				{
					uint32_t oldSize = m_ArrayBuffer->nElementCount;
					m_ArrayBuffer->nElementCount = newSize;
					if( newSize > oldSize )
						memset(&((_T*)m_ArrayBuffer->pByteArray)[oldSize], 0, sizeof(_T)*(newSize-oldSize) );
					return 0;
				}
				GOPTR(BUFFER) newArray, oldArray( m_ArrayBuffer );
				if (0 > gcreateBuffer(oldArray->DataFormat, oldArray->Usage, newSize + one_if(bIsText), &newArray))
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= oldArray->DataFormat;
					newArray->Usage				= oldArray->Usage;
					newArray->nRowElementCount	= newArray->nElementCount	= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(bIsText));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if( 0 == newArray->pByteArray )
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf( "malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize );
						return -1;
					}
				};
				newArray->nElementCount = newSize;
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
				memcpy(newArray->pByteArray, oldArray->pByteArray, sizeof(_T)*copyCount );
				if( oldArray->nElementCount < newArray->nElementCount )
					memset(&((_T*)newArray->pByteArray)[copyCount], 0, sizeof(_T)*(newArray->nElementCount-copyCount));
				//for( uint32_t iElement=0; iElement<copyCount; iElement++ )
				//{
				//	((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];
				//}
				if (bIsText)
					memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );;
					//((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
				m_ArrayBuffer = newArray;
			}
			else
			{
				GOPTR(BUFFER) newArray;
				error_t errMy = gcreateBuffer(GTYPEID_DATA_MAKE(_T), GUSAGE_TEXT, newSize + one_if(1), &newArray);
				if( 0 > errMy )
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= GTYPEID_MAKE( false, false, false, false, 1, sizeof(_T)*8 );
					newArray->Usage				= GUSAGE_TEXT;
					newArray->nRowElementCount	= newArray->nElementCount		= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(1));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if( 0 == newArray->pByteArray )
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf( "malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize );
						return -1;
					}
					newArray->nElementCount = newSize;
					memset( newArray->pByteArray, 0, sizeof(_T)*newArray->nElementCount );
					memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );
					//((_T*)newArray->pByteArray)[newSize] = 0;
				}
				newArray->nElementCount = newSize;
				m_ArrayBuffer = newArray;
			}

			return 0;
		};
		virtual error_t				insert( uint32_t nIndex, const _T& value )
		{
			if( nIndex >= size() )
			{
				error_printf("Invalid index! Index=%u. Max index=%u", nIndex, size()-1);
				return -1;
			}
			uint32_t nOldSize = m_ArrayBuffer->nElementCount,
				nNewSize = m_ArrayBuffer->nElementCount+1;
			bool bIsText = m_ArrayBuffer->Usage == GUSAGE_TEXT;
			if( m_ArrayBuffer.am_I_owning() && 
				m_ArrayBuffer->nSizeInBytes >= (sizeof(_T)*(nNewSize+one_if(bIsText))) )
			{
				for( uint32_t nOldIndex = nOldSize; nOldIndex>nIndex; nOldIndex-- )
					get_pointer()[nOldIndex] = get_pointer()[nOldIndex-1];
				get_pointer()[nIndex] = value;
				m_ArrayBuffer->nElementCount++;
				return 0;
			}

			// WARNING! Unsafe pointer handling in course.
			GOPTR(BUFFER) newListBuffer, oldListBuffer( m_ArrayBuffer );
			if( 0 > gcreateBuffer( GTYPEID_DATA_MAKE(_T), oldListBuffer->Usage, nNewSize, &newListBuffer ) )
			{
				error_printf("gcreateBuffer() FAILED!");
				return -1;
			};
			newListBuffer->nRowElementCount = newListBuffer->nElementCount;
			uint32_t iElement;
			for( iElement=0; iElement < nIndex; iElement++ )
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement];
			((_T*)newListBuffer->pByteArray)[iElement] = value;
			iElement++;
			for( iElement; iElement < nNewSize; iElement++ )
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement-1];
			oldListBuffer.free();
			m_ArrayBuffer = newListBuffer;
			return 0;
		};

		virtual error_t				remove( uint32_t nIndex )
		{
			if( nIndex >= size() )
			{
				error_printf("Invalid index! Index=%u. Max index=%u", nIndex, size()-1);
				return -1;
			}
			uint32_t nOldSize = m_ArrayBuffer->nElementCount,
				nNewSize = m_ArrayBuffer->nElementCount-1;
			bool bIsText = m_ArrayBuffer->Usage == GUSAGE_TEXT;
			if( m_ArrayBuffer.am_I_owning() && 
				m_ArrayBuffer->nSizeInBytes >= (sizeof(_T)*(nNewSize+one_if(bIsText))) )
			{
				for( uint32_t nOldIndex = nIndex; nOldIndex<nNewSize; nOldIndex++ )
					get_pointer()[nOldIndex] = get_pointer()[nOldIndex+1];
				//get_pointer()[nIndex] = value;
				m_ArrayBuffer->nElementCount--;
				return 0;
			}

			// WARNING! Unsafe pointer handling in course.
			GOPTR(BUFFER) newListBuffer, oldListBuffer( m_ArrayBuffer );
			if( 0 > gcreateBuffer( GTYPEID_DATA_MAKE(_T), oldListBuffer->Usage, nNewSize, &newListBuffer ) )
			{
				error_printf("gcreateBuffer() FAILED!");
				return -1;
			};
			newListBuffer->nRowElementCount = newListBuffer->nElementCount;
			uint32_t iElement;
			for( iElement=0; iElement < nIndex; iElement++ )
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement];
			for( iElement; iElement < nNewSize; iElement++ )
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement+1];
			oldListBuffer.free();
			m_ArrayBuffer = newListBuffer;
			return 0;
		};
		virtual size_t				size_in_bytes( void ) const
		{
			return m_ArrayBuffer ? (m_ArrayBuffer->nElementCount*sizeof(_T)) : 0;
		};

		virtual size_t				element_size( void ) const
		{
			return m_ArrayBuffer ? GTYPEID_TOTALBYTES(m_ArrayBuffer->DataFormat) : sizeof(_T);
		};

		virtual error_t				serialize( void* out_pMemoryBuffer, uint32_t* nWrittenSize ) const
		{
			uint32_t writtenSize = memSerializeData( &m_ArrayBuffer, 1, out_pMemoryBuffer );
			if( *nWrittenSize )
				nWrittenSize += writtenSize;
			return 0;
		};
		virtual error_t				deserialize( const void* in_pMemoryBuffer, uint32_t* nReadSize )
		{
			GODS(BUFFER) b=0;
			uint32_t readSize = memDeserializeData( &b, 1, in_pMemoryBuffer );
			if( *nReadSize )
				nReadSize += readSize;
			m_ArrayBuffer = GOPTR(BUFFER)(b);
			return 0;
		};
		virtual error_t				serialize( FILE* out_pFile ) const
		{
			uint32_t writtenInstances = fileSerializeData( &m_ArrayBuffer, 1, out_pFile );
			return 0;
		};
		virtual error_t				deserialize( FILE* in_pFile )
		{
			uint32_t readSize = fileDeserializeData( &m_ArrayBuffer, 1, in_pFile );
			return 0;
		};

		virtual bool				am_I_owning( void ) const
		{
			return m_ArrayBuffer.am_I_owning();
		};
		virtual GODS(BUFFER)					GetBufferData(void) const
		{
			return gacquireData( m_ArrayBuffer );
		}

	protected:
		GOPTR(BUFFER)				m_ArrayBuffer;
	};


	template<typename _T, god::GDATA_TYPE _F> class gelementaryex : public gelementary<_T>
	{
	public:
		gelementaryex(void){};
		gelementaryex(const gelementary<_T>& other)
			:gelementary<_T>::gelementary(other)
		{};
		gelementaryex(uint32_t size)
		{
			if(0 > resize(size))
			{
				error_printf("Out of memory!");
			};
		};
		gelementaryex(uint32_t size, const _T* valueList)
		{
			if(0 > resize(size))
			{
				error_printf("Out of memory!");
			}
			else if(valueList)
			{
				for(uint32_t i=0; i<size; i++)
					set_value(i, valueList[i]);
			}
		};
		virtual ~gelementaryex(void){};

		virtual error_t							set_value(size_t index, const _T& value)
		{
			if(!this->m_ArrayBuffer)
			{
				error_printf("Cannot set a value into a null array!");
				return -1;
			}
			else if(index >= this->size())
			{
				error_printf("Invalid index: #%u!", (uint32_t)index);
				return -1;
			}
			else if(this->m_ArrayBuffer.am_I_owning())
			{
				((_T*)this->m_ArrayBuffer->pByteArray)[index] = value;
				return 0;
			}
			GOPTR(BUFFER) newArray, oldArray(this->m_ArrayBuffer);
			bool bIsText = oldArray->Usage == GUSAGE_TEXT;
			if(0 > gcreateBuffer(_F, oldArray->Usage, oldArray->nElementCount, &newArray))
			{
				newArray->DataFormat		= oldArray->DataFormat;
				newArray->Usage				= oldArray->Usage;
				newArray->nElementCount		= oldArray->nElementCount;
				newArray->nRowElementCount	= oldArray->nRowElementCount;
				newArray->nSizeInBytes		= sizeof(_T)*(newArray->nElementCount+one_if(bIsText));
				newArray->pByteArray		= malloc(newArray->nSizeInBytes);
				if(0 == newArray->pByteArray)
				{
					newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
					error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)(sizeof(_T)*(newArray->nElementCount+one_if(bIsText))));
					return -1;
				}
			}
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
			for(uint32_t iElement=0; iElement<copyCount; iElement++)
			{
				((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];
			}
			((_T*)newArray->pByteArray)[index] = value;
			if(bIsText)
				((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
			this->m_ArrayBuffer = newArray;
			return 0;
		}

		virtual void							clear(void)
		{
			if(0 == this->m_ArrayBuffer)
				return;
			if(this->m_ArrayBuffer.am_I_owning())
				this->m_ArrayBuffer->nElementCount = 0;
			else
				this->m_ArrayBuffer.free();
		}

		virtual error_t							resize(size_t newSize)
		{
			if(0 == newSize)
			{
				this->m_ArrayBuffer.free();
				return 0;
			}
			if(this->m_ArrayBuffer)
			{
				if(this->m_ArrayBuffer->nElementCount == newSize)
					return 0;
				uint32_t typeSize = sizeof(_T);
				bool bIsText = this->m_ArrayBuffer->Usage == GUSAGE_TEXT;
				if(((newSize+one_if(bIsText))*typeSize) <= this->m_ArrayBuffer->nSizeInBytes
					&& this->m_ArrayBuffer.am_I_owning())
				{
					this->m_ArrayBuffer->nElementCount = newSize;
					return 0;
				}
				GOPTR(BUFFER) newArray, oldArray(this->m_ArrayBuffer);
				if(0 > gcreateBuffer(_F, oldArray->Usage, newSize + one_if(bIsText), &newArray))
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= _F;
					newArray->Usage				= oldArray->Usage;
					newArray->nRowElementCount	= newArray->nElementCount	= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(bIsText));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if(0 == newArray->pByteArray)
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize);
						return -1;
					}
				};
				newArray->nElementCount = newSize;
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
				memcpy(newArray->pByteArray, oldArray->pByteArray, sizeof(_T)*copyCount);
				//for( uint32_t iElement=0; iElement<copyCount; iElement++ )
				//{
				//	((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];
				//}
				if(bIsText)
					((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
				this->m_ArrayBuffer = newArray;
			}
			else
			{
				GOPTR(BUFFER) newArray;
				int bIsText = 1;
				error_t errMy = gcreateBuffer(_F, GUSAGE_TEXT, newSize + one_if(bIsText), &newArray);
				if(0 > errMy)
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= _F;
					newArray->Usage				= GUSAGE_TEXT;
					newArray->nRowElementCount	= newArray->nElementCount		= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(bIsText));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if(0 == newArray->pByteArray)
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize);
						return -1;
					}
					newArray->nElementCount = newSize;
					((_T*)newArray->pByteArray)[newSize] = 0;
				}
				newArray->nElementCount = newSize;
				this->m_ArrayBuffer = newArray;
			}

			return 0;
		};
		virtual error_t							insert(uint32_t nIndex, const _T& value)
		{
			if(nIndex >= this->size())
			{
				error_printf("Invalid index! Index=%u. Max index=%u", nIndex, size()-1);
				return -1;
			}
			uint32_t nOldSize = this->m_ArrayBuffer->nElementCount,
				nNewSize = this->m_ArrayBuffer->nElementCount+1;
			bool bIsText = this->m_ArrayBuffer->Usage == GUSAGE_TEXT;
			if(this->m_ArrayBuffer.am_I_owning() &&
				this->m_ArrayBuffer->nSizeInBytes >= (sizeof(_T)*(nNewSize+one_if(bIsText))))
			{
				for(uint32_t nOldIndex = nOldSize; nOldIndex>nIndex; nOldIndex--)
					this->get_pointer()[nOldIndex] = this->get_pointer()[nOldIndex-1];
				this->get_pointer()[nIndex] = value;
				this->m_ArrayBuffer->nElementCount++;
				return 0;
			}

			// WARNING! Unsafe pointer handling in course.
			GOPTR(BUFFER) newListBuffer, oldListBuffer(this->m_ArrayBuffer);
			if(0 > gcreateBuffer(_F, oldListBuffer->Usage, nNewSize, &newListBuffer))
			{
				error_printf("gcreateBuffer() FAILED!");
				return -1;
			};
			newListBuffer->nRowElementCount = newListBuffer->nElementCount;
			uint32_t iElement;
			for(iElement=0; iElement < nIndex; iElement++)
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement];
			((_T*)newListBuffer->pByteArray)[iElement] = value;
			iElement++;
			for(iElement; iElement < nNewSize; iElement++)
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement-1];
			oldListBuffer.free();
			this->m_ArrayBuffer = newListBuffer;
			return 0;
		};
		virtual size_t							element_size(void) const
		{
			return (this->m_ArrayBuffer) ? GTYPEID_TOTALBYTES(_F) : sizeof(_T);
		};
		virtual GODS(BUFFER)					GetBufferData(void) const
		{
			return gacquireData( this->m_ArrayBuffer );
		}
	};

	template<typename _T, god::GDATA_TYPE _F, god::GRESOURCE_USAGE _U> class gelementaryex2 : public gelementary<_T>
	{
	public:
		gelementaryex2(void){};
		gelementaryex2(const gelementary<_T>& other)
		{
			GOPTR(BUFFER) fuck = other.GetBufferData();
			if( fuck )
			{
				if( _F != fuck->DataFormat )
				{
					error_printf("Invalid element format!");
					throw(L"");
					this->m_ArrayBuffer.free();
				}
				else if( _U != fuck->Usage )
				{
					error_printf("Invalid element usage!");
					throw(L"");
					this->m_ArrayBuffer.free();
				}
				else
				{
					this->m_ArrayBuffer = fuck;
				}
			}
			else
			{
				this->m_ArrayBuffer.free();					
			}
		};
		gelementaryex2( const gelementaryex2<_T, _F, _U>& other )
			:gelementary<_T>::gelementary(other)
		{
		};
		gelementaryex2(uint32_t size)
		{
			if(0 > resize(size))
			{
				error_printf("Out of memory!");
			};
		};
		gelementaryex2(uint32_t size, const _T* valueList)
		{
			if(0 > resize(size))
			{
				error_printf("Out of memory!");
			}
			else if(valueList)
			{
				for(uint32_t i=0; i<size; i++)
					set_value(i, valueList[i]);
			}
		};
		virtual ~gelementaryex2(void){};

		//virtual const gelementaryex2& operator =( GODS(BUFFER) InputBuffer )
		//{
		//	if( 0 == InputBuffer )
		//	{
		//		m_ArrayBuffer.free();
		//		return *this;
		//	}
		//	if( InputBuffer->DataFormat != _F || InputBuffer->Usage != _U )
		//	{
		//		error_printf("Invalid buffer format!");
		//		throw(L"");
		//		return *this;
		//	}
		//	m_ArrayBuffer = gacquireData( InputBuffer );
		//	return *this;
		//}

		virtual void							set( GODS(BUFFER) InputBuffer )
		{
			if( 0 == InputBuffer )
			{
				this->m_ArrayBuffer.free();
				//return *this;
			}
			else if( InputBuffer->DataFormat != _F || InputBuffer->Usage != _U )
			{
				error_printf("Invalid buffer format!");
#if defined(ANDROID)
#else
				throw(L"");
#endif

				//return *this;
			}
			else
				this->m_ArrayBuffer = gacquireData( InputBuffer );
			//return *this;
		}
		virtual error_t							set_value(size_t index, const _T& value)
		{
			if(!this->m_ArrayBuffer)
			{
				error_printf("Cannot set a value into a null array!");
				return -1;
			}
			else if(index >= this->size())
			{
				error_printf("Invalid index: #%u!", (uint32_t)index);
				return -1;
			}
			else if(this->m_ArrayBuffer.am_I_owning())
			{
				((_T*)this->m_ArrayBuffer->pByteArray)[index] = value;
				return 0;
			}
			GOPTR(BUFFER) newArray, oldArray(this->m_ArrayBuffer);
			bool bIsText = oldArray->Usage == GUSAGE_TEXT;
			if(0 > gcreateBuffer(_F, _U, oldArray->nElementCount, &newArray))
			{
				newArray->DataFormat		= _F;
				newArray->Usage				= _U;
				newArray->nElementCount		= oldArray->nElementCount;
				newArray->nRowElementCount	= oldArray->nRowElementCount;
				newArray->nSizeInBytes		= sizeof(_T)*(newArray->nElementCount+one_if(bIsText));
				newArray->pByteArray		= malloc(newArray->nSizeInBytes);
				if(0 == newArray->pByteArray)
				{
					newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
					error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)(sizeof(_T)*(newArray->nElementCount+one_if(bIsText))));
					return -1;
				}
			}
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
			uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
			for(uint32_t iElement=0; iElement<copyCount; iElement++)
				((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];

			((_T*)newArray->pByteArray)[index] = value;
			if(bIsText)
				//((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
				memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );;
			this->m_ArrayBuffer = newArray;
			return 0;
		}

		virtual void							clear(void)
		{
			if(0 == this->m_ArrayBuffer)
				return;
			if(this->m_ArrayBuffer.am_I_owning())
				this->m_ArrayBuffer->nElementCount = 0;
			else
				this->m_ArrayBuffer.free();
		}

		virtual error_t							resize(size_t newSize)
		{
			if(0 == newSize)
			{
				this->m_ArrayBuffer.free();
				return 0;
			}
			if(this->m_ArrayBuffer)
			{
				if(this->m_ArrayBuffer->nElementCount == newSize)
					return 0;
				uint32_t typeSize = sizeof(_T);
				bool bIsText = this->m_ArrayBuffer->Usage == GUSAGE_TEXT;
				if(((newSize+one_if(bIsText))*typeSize) <= this->m_ArrayBuffer->nSizeInBytes
					&& this->m_ArrayBuffer.am_I_owning())
				{
					this->m_ArrayBuffer->nElementCount = newSize;
					return 0;
				}
				GOPTR(BUFFER) newArray, oldArray(this->m_ArrayBuffer);
				if(0 > gcreateBuffer(_F, _U, newSize + one_if(bIsText), &newArray))
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= _F;
					newArray->Usage				= _U;
					newArray->nRowElementCount	= newArray->nElementCount	= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(bIsText));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if(0 == newArray->pByteArray)
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize);
						return -1;
					}
				};
				newArray->nElementCount = newSize;
#ifndef min
#define min(a,b) ( (a)<(b) ? a : b )
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#undef min
#else
				uint32_t copyCount = min(oldArray->nElementCount, newArray->nElementCount);
#endif
				memcpy(newArray->pByteArray, oldArray->pByteArray, sizeof(_T)*copyCount);
				//for( uint32_t iElement=0; iElement<copyCount; iElement++ )
				//{
				//	((_T*)newArray->pByteArray)[iElement] = ((_T*)oldArray->pByteArray)[iElement];
				//}
				if(bIsText)
					memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );
					//((_T*)newArray->pByteArray)[newArray->nElementCount] = 0;
				this->m_ArrayBuffer = newArray;
			}
			else
			{
				GOPTR(BUFFER) newArray;
				bool bIsText = _U == GUSAGE_TEXT;
				error_t errMy = gcreateBuffer(_F, _U, newSize + one_if(bIsText), &newArray);
				if(0 > errMy)
				{
					warning_printf("Failed to create buffer! Trying to allocate manually.");
					newArray->DataFormat		= _F;
					newArray->Usage				= _U;
					newArray->nRowElementCount	= newArray->nElementCount		= newSize;
					newArray->nSizeInBytes		= sizeof(_T)*(newSize+one_if(bIsText));
					newArray->pByteArray		= malloc(newArray->nSizeInBytes);
					if(0 == newArray->pByteArray)
					{
						newArray->nRowElementCount = newArray->nElementCount = newArray->nSizeInBytes = 0;
						error_printf("malloc() FAILED! Out of memory? Size requested: %u", (uint32_t)newSize);
						return -1;
					}
					newArray->nElementCount = newSize;
					bool bIsText = _U == GUSAGE_TEXT;
					if( bIsText )
						memset( &((_T*)newArray->pByteArray)[newArray->nElementCount], 0, sizeof(_T) );
						//((_T*)newArray->pByteArray)[newSize] = 0;
				}
				newArray->nElementCount = newSize;
				this->m_ArrayBuffer = newArray;
			}

			return 0;
		};
		virtual error_t							insert(uint32_t nIndex, const _T& value)
		{
			if(nIndex >= this->size())
			{
				error_printf("Invalid index! Index=%u. Max index=%u", nIndex, size()-1);
				return -1;
			}
			uint32_t nOldSize = this->m_ArrayBuffer->nElementCount,
				nNewSize = this->m_ArrayBuffer->nElementCount+1;
			bool bIsText = this->m_ArrayBuffer->Usage == GUSAGE_TEXT;
			if(this->m_ArrayBuffer.am_I_owning() &&
				this->m_ArrayBuffer->nSizeInBytes >= (sizeof(_T)*(nNewSize+one_if(bIsText))))
			{
				for(uint32_t nOldIndex = nOldSize; nOldIndex>nIndex; nOldIndex--)
					this->get_pointer()[nOldIndex] = this->get_pointer()[nOldIndex-1];
				this->get_pointer()[nIndex] = value;
				this->m_ArrayBuffer->nElementCount++;
				return 0;
			}

			// WARNING! Unsafe pointer handling in course.
			GOPTR(BUFFER) newListBuffer, oldListBuffer(this->m_ArrayBuffer);
			if(0 > gcreateBuffer(_F, _U, nNewSize, &newListBuffer))
			{
				error_printf("gcreateBuffer() FAILED!");
				return -1;
			};
			newListBuffer->nRowElementCount = newListBuffer->nElementCount;
			uint32_t iElement;
			for(iElement=0; iElement < nIndex; iElement++)
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement];
			((_T*)newListBuffer->pByteArray)[iElement] = value;
			iElement++;
			for(iElement; iElement < nNewSize; iElement++)
				((_T*)newListBuffer->pByteArray)[iElement] = ((_T*)oldListBuffer->pByteArray)[iElement-1];
			oldListBuffer.free();
			this->m_ArrayBuffer = newListBuffer;
			return 0;
		};
		virtual size_t							element_size(void) const
		{
			return this->m_ArrayBuffer ? GTYPEID_TOTALBYTES(_F) : sizeof(_T);
		};
		virtual GODS(BUFFER)					GetBufferData(void) const
		{
			return gacquireData( this->m_ArrayBuffer );
		}
	};

};

#endif // _GARRAY_H