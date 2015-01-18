#include "GBaseBuffer.h"
#include "ginterface_ptr.h"
#ifdef _WIN32
//#include <Windows.h>
#endif

#ifndef _GBASELIST_H
#define _GBASELIST_H

namespace god
{
	template <typename _T> class cbaselist_safe : public CGBaseBuffer
	{
	public:
		virtual cbaselist_safe&			operator =( const cbaselist_safe& l )
		{
			GOPTR(BUFFER) listBuffer = l.GetBufferData();
			SetBufferData( listBuffer );
			return *this;
		};
		virtual uint32_t				size( void ) const
		{
			return CGBaseBuffer::GetElementCount();
		};
		virtual _T*						get_pointer( void )
		{
			return (_T*)GetAddress();
		};
		virtual const _T*				get_pointer( void ) const
		{
			return (_T*)GetAddress();
		};
	};

	template<typename _T> class gbaselist_safe : public cbaselist_safe<_T>
	{
		static int _initOnce()
		{
			gbaselist_safe::_INVALID = INVALID;
			return 0;
		};
	protected:
		static	const _T	INVALID;
		static	_T			_INVALID;
	public:
		gbaselist_safe( void )
		{	//
			static int initOnce = _initOnce();
		};
		gbaselist_safe( uint32_t nInitialSize )
		{
			resize( nInitialSize );	
		}
		virtual ~gbaselist_safe( void )
		{	// for some reason, clear() override is not being called properly on template destruction.
			SetBufferData(0);
		};
		virtual error_t			create( _T* in_lstCoreInstances, uint32_t nInstanceCount )
		{
			if( nInstanceCount && 0 == in_lstCoreInstances )
			{
				error_printf("Cannot create list from a null address!");
				return -1;
			}
			GOPTR(BUFFER) newListBuffer;
			if( gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nInstanceCount, &newListBuffer ) )
			{
				error_printf("Cannot create buffer for list!");
				return -1;
			};
			for( uint32_t iCoreInstance=0; iCoreInstance < nInstanceCount; iCoreInstance++ )
				((_T*)newListBuffer->pByteArray)[iCoreInstance] = gacquireData( in_lstCoreInstances[iCoreInstance] );
			SetBufferData( newListBuffer );
			return 0;
		};
		virtual error_t			create( gbase_ptr<_T>* in_lstCoreInstances, uint32_t nInstanceCount )
		{
			if( nInstanceCount && 0 == in_lstCoreInstances )
			{
				error_printf("Cannot create list from a null address!");
				return -1;
			}
			GOPTR(BUFFER) newListBuffer;
			if( gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nInstanceCount, &newListBuffer ) )
			{
				error_printf("Cannot create buffer for list!");
				return -1;
			};
			for( uint32_t iCoreInstance=0; iCoreInstance < nInstanceCount; iCoreInstance++ )
				((_T*)newListBuffer->pByteArray)[iCoreInstance] = in_lstCoreInstances[iCoreInstance].acquire();
			SetBufferData( newListBuffer );
			return 0;
		};
		virtual error_t			resize( uint32_t nSize )
		{
			if( nSize == this->size() )
				return 0;	// size already set
			if( 0 == nSize )
			{
				SetBufferData(0);
				return 0;
			}
			goptr_Buffer newListBuffer;
			error_t errMy = gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nSize, &newListBuffer );
			if( 0 > errMy )
			{
				error_printf("Failed to create buffer for list! Out of memory?");
				return -1;
			}
			if( this->size() )
			{
				GOPTR(BUFFER) oldListBuffer = this->GetBufferData();
				uint32_t iCoreInstance=0, nMaxCount = oldListBuffer->nElementCount > nSize ? nSize : oldListBuffer->nElementCount; 
				for( ; iCoreInstance < nMaxCount; iCoreInstance++ )
					((_T*)newListBuffer->pByteArray)[iCoreInstance] = gacquireData( ((_T*)oldListBuffer->pByteArray)[iCoreInstance] );
				if( iCoreInstance < nSize )
					memset(	&((_T*)newListBuffer->pByteArray)[iCoreInstance], 0, sizeof( _T )*(nSize-nMaxCount) );
			}
			else
				memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES(newListBuffer->DataFormat)*nSize );
			SetBufferData( newListBuffer );
			return 0;
		};
		//-----------------------------------------------
		virtual error_t			serialize( FILE* fp ) const
		{
			if( 0 == fp )
			{
				error_printf("A null pointer is not a valid file handler!");
				return -1;
			}
			uint32_t nSize = this->size();
			if( 1 != fwrite( &nSize, sizeof(uint32_t), 1, fp ) )
			{
				error_printf("Failed to write to file!");
				return -1;
			}
			if( 0 == nSize )
				return 0;
			uint32_t nInstancesWritten = fileSerializeData( this->get_pointer(), nSize, fp );
			if( nSize != nInstancesWritten )
			{
				error_printf( "Failed to serialize the requested instances! %u requested to be written, %u actually written."
					, nSize
					, nInstancesWritten );
			};
			return 0;
		};
		virtual error_t			deserialize( FILE* fp )
		{
			if( 0 == fp )
			{
				error_printf("A null pointer is not a valid file handler!");
				return -1;
			}
			uint32_t nSize = 0;
			if( 1 != fread( &nSize, sizeof(uint32_t), 1, fp ) )
			{
				error_printf("Failed to read list from file!");
				return -1;
			}
			if( nSize == 0 )
			{
				SetBufferData(0);
				return 0;
			}
			GOPTR(BUFFER) newListBuffer=0;
			GDATA_TYPE DataFormat = GTYPEID_DATA_MAKE(_T);
			if( 0 > gcreateBuffer( DataFormat, GUSAGE_POINTER, nSize, &newListBuffer ) )
			{
				error_printf("Failed to create allocate list buffer! Out of memory?");
				return -1;
			};
			memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES(DataFormat)*newListBuffer->nElementCount );
			uint32_t nInstancesRead = fileDeserializeData( (_T*)newListBuffer->pByteArray, nSize, fp );
			if( nSize != nInstancesRead )
			{
				error_printf( "Failed to read the requested instances! %u requested to be read, %u actually read."
					, nSize
					, nInstancesRead );
			};

			SetBufferData( newListBuffer );
			return 0;
		};
		//-----------------------------------------------
		virtual size_t			serialize( void* out_pByteArray ) const
		{
			uint32_t nSize = this->size();
			uint32_t nBytesWritten = 0;
			if( out_pByteArray )
				memcpy( out_pByteArray, &nSize, sizeof(uint32_t) );
			nBytesWritten += sizeof(uint32_t);
			if( 0 == nSize )
				return nBytesWritten;
			nBytesWritten += memSerializeData( this->get_pointer(), nSize, 0 == out_pByteArray ? 0 : ((char*)out_pByteArray)+nBytesWritten );
			return nBytesWritten;
		};
		virtual size_t			deserialize( const void* in_pByteArray )
		{
			if( 0 == in_pByteArray )
			{
				error_printf("Cannot deserialize from a null address!");
				return 0;
			}
			uint32_t nReadBytes=0;
			uint32_t nSize = 0;
			memcpy( &nSize, in_pByteArray, sizeof(uint32_t) );
			nReadBytes += sizeof(uint32_t);
			if( nSize == 0 )
			{
				SetBufferData(0);
				return nReadBytes;
			}
			GOPTR(BUFFER) newListBuffer;
			GDATA_TYPE DataFormat = GTYPEID_DATA_MAKE(_T);
			if( 0 > gcreateBuffer( DataFormat, GUSAGE_POINTER, nSize, &newListBuffer ) )
			{
				error_printf("Cannot create buffer for list! Out of memory?");
				return -1;
			};
			memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES(DataFormat)*newListBuffer->nElementCount );
			nReadBytes += memDeserializeData( (_T*)newListBuffer->pByteArray, nSize, ((char*)in_pByteArray)+nReadBytes );
			SetBufferData( newListBuffer );
			return nReadBytes;
		};

		virtual error_t			write( FILE* fp, _T DefaultData=0 ) const
		{
			static const _T check=0;
			if( check->__kCue == __GODS(BUFFER)::__kCue )
			{
				error_printf("Cannot \"write\" a list of GBUFFER_DATA instances! Only \"serialize\".");
				return -1;
			}
			if( 0 == fp )
			{
				error_printf("A null pointer is not a valid file handler!");
				return -1;
			}
			uint32_t nSize = this->size();
			if( 1 != fwrite( &nSize, sizeof(uint32_t), 1, fp ) )
			{
				error_printf("Failed to write to file!");
				return -1;
			}
			uint32_t nInstancesWritten = fileWriteData( this->get_pointer(), nSize, fp, DefaultData );
			if( nSize != nInstancesWritten )
			{
				error_printf( "Failed to write the requested instances! %u requested to be written, %u actually written."
					, nSize
					, nInstancesWritten );
				return -1;
			};
			return 0;
		};
		virtual error_t			read( FILE* fp )
		{
			static const _T check=0;
			if( check->__kCue == __GODS(BUFFER)::__kCue )
			{
				error_printf("Cannot \"read\" a list of GBUFFER_DATA instances! Only \"deserialize\".");
				return -1;
			}
			if( 0 == fp )
			{
				error_printf("A null pointer is not a valid file handler!");
				return -1;
			}
			uint32_t nSize = 0;
			if( 1 != fread( &nSize, sizeof(uint32_t), 1, fp ) )
			{
				error_printf("Failed to read list from file!");
				return -1;
			}
			if( nSize == 0 )
			{
				SetBufferData(0);
				return 0;
			}
			GOPTR(BUFFER) newListBuffer=0;
			GDATA_TYPE DataFormat = GTYPEID_DATA_MAKE(_T);
			if( 0 > gcreateBuffer( DataFormat, GUSAGE_POINTER, nSize, &newListBuffer ) )
			{
				error_printf("Cannot create buffer! Out of memory?");
				return -1;
			};
			memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES(DataFormat)*newListBuffer->nElementCount );
			uint32_t nInstancesRead = fileReadData( (_T*)newListBuffer->pByteArray, nSize, fp );
			if( nSize != nInstancesRead )
			{
				error_printf( "Failed to read the requested instances! %u requested to be read, %u actually read."
					, nSize
					, nInstancesRead );
			};
			SetBufferData( newListBuffer );
			return 0;
		};
		virtual size_t			write( void* out_pByteArray, _T DefaultData=0 ) const
		{
			uint32_t nSize = this->size();
			uint32_t nBytesWritten = 0;
			if( out_pByteArray )
				memcpy( out_pByteArray, &nSize, sizeof(uint32_t) );
			nBytesWritten += sizeof(uint32_t);
			if( 0 == nSize )
				return nBytesWritten;
			nBytesWritten += memWriteData( this->get_pointer(), nSize, 0 == out_pByteArray ? 0 : ((char*)out_pByteArray)+nBytesWritten, DefaultData );
			return nBytesWritten;
		};
		virtual size_t			read( const void* in_pByteArray )
		{
			if( 0 == in_pByteArray )
			{
				error_printf("Cannot deserialize from a null address!");
				return 0;
			}
			uint32_t nReadBytes=0;
			uint32_t nSize = 0;
			memcpy( &nSize, in_pByteArray, sizeof(uint32_t) );
			nReadBytes += sizeof(uint32_t);
			if( nSize == 0 )
			{
				SetBufferData(0);
				return nReadBytes;
			}
			GOPTR(BUFFER) newListBuffer=0;
			GDATA_TYPE DataFormat = GTYPEID_DATA_MAKE(_T);
			if( 0 > gcreateBuffer( DataFormat, GUSAGE_POINTER, nSize, &newListBuffer ) )
			{
				error_printf("Cannot allocate buffer for list! Out of memory?");
				return -1;
			};
			memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES(DataFormat)*newListBuffer->nElementCount );
			nReadBytes += memReadData( (_T*)newListBuffer->pByteArray, nSize, ((char*)in_pByteArray)+nReadBytes );
			SetBufferData( newListBuffer );
			return nReadBytes;
		};

		virtual error_t			free_all( void )
		{
			GOPTR(BUFFER) newListBuffer;
			uint32_t nSize = this->size();
			if( nSize )
			{
				error_t errMy = gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nSize, &newListBuffer );
				if( 0 > errMy )
				{
					error_printf("Failed to allocate list buffer! Out of memory?");
					return -1;
				}
				memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES( newListBuffer->DataFormat )*nSize );
			}
			SetBufferData(newListBuffer);
			return 0;
		};
		//-----------------------------------------------------
		virtual error_t			set( _T CoreInstance, uint32_t nIndex )
		{
			if( nIndex >= this->size() )
			{
				error_printf("Invalid index! Index=%u.", nIndex);
				_CrtDbgBreak();
				return -1;
			}
			_T oldInstance = this->get_pointer()[nIndex];
			if( oldInstance == CoreInstance )
				return 0;
			this->get_pointer()[nIndex] = gacquireData( CoreInstance );
			gfreeData( &oldInstance );
			return 0;
		};
		virtual _T				acquire( uint32_t nIndex ) const
		{
			if( nIndex >= this->size() )
			{
				error_printf("Invalid index! \"%u\"", nIndex);
				_CrtDbgBreak();
				return 0;
			}
			return gacquireData( this->get_pointer()[nIndex] );
		};
		virtual error_t			free( uint32_t nIndex )
		{
			if( nIndex >= this->size() )
			{
				error_printf( "Invalid index! Index=%u.", nIndex );
				return -1;
			}
			set( 0, nIndex );
			return 0;
		};
		// Checks all elements for valid instances (non-null instance pointers)
		virtual uint32_t		get_valid_element_count( void ) const
		{
			if( !this->get_pointer() )
				return 0;
			uint32_t nElementCount = this->size();
			uint32_t nValidElementCount = 0;
			for( uint32_t iElement=0; iElement < nElementCount; iElement++ )
			{
				if( this->get_pointer()[iElement] )
					nValidElementCount++;
			}
			return nValidElementCount;
		};

	protected:
		virtual void		SetBufferData( GODS(BUFFER) ListBuffer )
		{
			GOPTR(BUFFER) oldBuffer = this->GetBufferData();	// Catch the old list so we can manage its elements.
			CGBaseBuffer::SetBufferData( ListBuffer );
			if( oldBuffer && oldBuffer.am_I_owning() )
			{
				while( oldBuffer->nElementCount )
				{
					gcore_ptr<_T> old = ((_T*)oldBuffer->pByteArray)[--oldBuffer->nElementCount];
					((_T*)oldBuffer->pByteArray)[oldBuffer->nElementCount] = 0;
				}
				//gfreeData( (_T*)oldBuffer->pByteArray, &oldBuffer->nElementCount ); this is harder to debug
			}
		};
	};

	template<typename _T> class ginterfacelist_safe : public cbaselist_safe<_T>
	{
		static int _initOnce()
		{
			ginterfacelist_safe::_INVALID = INVALID;
			return 0;
		};
	protected:
		static	const _T	INVALID;
		static	_T			_INVALID;
	public:
		ginterfacelist_safe( void )
		{	//
			static int initOnce = _initOnce();
		};
		virtual ~ginterfacelist_safe( void )
		{	// for some reason, clear() override is not being called properly on template destruction.
			SetBufferData(0);
		};
		virtual error_t			free_all( void )
		{
			GOPTR(BUFFER) newListBuffer;
			uint32_t nSize = this->size();
			if( nSize )
			{
				error_t errMy = gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nSize, &newListBuffer );
				if( 0 > errMy )
				{
					error_printf("Failed to allocate list buffer! Out of memory?");
					return -1;
				}
				memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES( newListBuffer->DataFormat )*nSize );
			}
			SetBufferData(newListBuffer);
			return 0;
		};
		//-----------------------------------------------------
		virtual error_t			set( _T CoreInstance, uint32_t nIndex )
		{
			if( nIndex >= this->size() )
			{
				error_printf("Invalid index! Index=%u.", nIndex);
				return -1;
			}
			_T oldInstance = this->get_pointer()[nIndex];
			if( oldInstance == CoreInstance )
				return 0;
			this->get_pointer()[nIndex] = gacquireInterface( CoreInstance );
			gfreeInterface( &oldInstance );
			return 0;
		};
		virtual _T				acquire( uint32_t nIndex ) const
		{
			if( nIndex >= this->size() )
			{
				error_printf("Invalid index! \"%u\"", nIndex);
				return 0;
			}
			return gacquireInterface( this->get_pointer()[nIndex] );
		};
		virtual error_t			free( uint32_t nIndex )
		{
			if( nIndex >= this->size() )
			{
				error_printf( "Invalid index! Index=%u.", nIndex );
				return -1;
			}
			this->set( 0, nIndex );
			return 0;
		};

	protected:
		virtual void		SetBufferData( GODS(BUFFER) ListBuffer )
		{
			GOPTR(BUFFER) oldBuffer = this->GetBufferData();	// Catch the old list so we can manage its elements.
			CGBaseBuffer::SetBufferData( ListBuffer );
			if( oldBuffer && oldBuffer.am_I_owning() )
			{
				while( oldBuffer->nElementCount )
				{
					ginterface_ptr<_T> old = ((_T*)oldBuffer->pByteArray)[--oldBuffer->nElementCount];
					((_T*)oldBuffer->pByteArray)[oldBuffer->nElementCount] = 0;
				}
				//gfreeData( (_T*)oldBuffer->pByteArray, &oldBuffer->nElementCount ); this is harder to debug
			}
		};
	};

}; // namespace

#endif // _GBASELIST_H