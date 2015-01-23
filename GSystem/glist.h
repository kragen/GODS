#include "gbaselist.h"
#ifdef _WIN32
//#include <Windows.h>
#endif

#ifndef _GLIST_H
#define _GLIST_H

namespace god
{
	//-------------------------------------------------------------- glist -----------------------------------------------------------------------
#define GOLIST( TOKEN ) glist<GODS(TOKEN)>
	template<typename _T> class glist : public gbaselist_safe<_T>
	{
	public:
		glist( void )
		{}		
		glist( uint32_t nInitialSize )
			:gbaselist_safe<_T>::gbaselist_safe( nInitialSize )
		{
		}		
		glist( const gcore_ptr<_T>* SourceList, uint32_t nElementCount )
			:gbaselist_safe<_T>::gbaselist_safe( nElementCount )
		{
			if( SourceList )
			{
				for( uint32_t i=0; i<nElementCount; i++ )
					this->get_pointer()[i] = gacquireData( SourceList[i] );
			}
			else
			{
				error_printf("Source list is NULL!");
			}
		}		
		virtual operator			_T*( void )
		{
			return this->get_pointer();
		}		
		virtual operator			const _T*( void ) const
		{
			return this->get_pointer();
		}		
		virtual error_t				push( _T CoreInstance )
		{
			uint32_t nNewSize = this->size()+1;
			error_t errMy = this->resize(nNewSize);
			if( errMy )
			{
				error_printf( "Failed to resize list! Out of memory?" );
				return -1;
			}
			this->set( CoreInstance, nNewSize-1 );
			return 0;
		};
		virtual error_t				pop( _T* out_pElement=0 )
		{
			// WARNING! Unsafe pointer handling in course!
			GOPTR(BUFFER) ListBuffer;
			ListBuffer = this->GetBufferData();
			if( !ListBuffer )
			{
				error_printf("Cannot pop elements from an empty list!");
				return -1;
			}
			gcore_ptr<_T> newElement, oldElement;
			if( 2 == ListBuffer->__nRefCount )
			{
				if( out_pElement )
				{
					oldElement = *out_pElement;
					*out_pElement = ((_T*)ListBuffer->pByteArray)[--ListBuffer->nElementCount];
				}
				else
				{
					gfreeData( &((_T*)ListBuffer->pByteArray)[--ListBuffer->nElementCount] );

				}
				return 0;
			}
			if( out_pElement )
				newElement = gacquireData( ((_T*)ListBuffer->pByteArray)[ListBuffer->nElementCount-1] );
			ListBuffer.free();
			error_t errMy = this->resize( this->size()-1 );
			if( errMy )
			{
				error_printf("Failed to allocate buffer for list!");
				return -1;
			}
			if( out_pElement )
			{
				oldElement = *out_pElement;
				*out_pElement = newElement.acquire();
			}
			return 0;
		};

		virtual error_t				insert( _T CoreInstance, uint32_t nIndex )
		{
			if( nIndex >= this->size() )
			{
				error_printf("Invalid index! Index=%u. Max index=%u", nIndex, this->size()-1);
				return -1;
			}
			// WARNING! Unsafe pointer handling in course.
			GOPTR(BUFFER) oldListBuffer;
			oldListBuffer = this->GetBufferData();
			uint32_t nOldSize = oldListBuffer->nElementCount,
				nNewSize = this->size()+1;
			if( 2 != oldListBuffer->__nRefCount 
				|| ( oldListBuffer->nSizeInBytes < (GTYPEID_TOTALBYTES(oldListBuffer->DataFormat)*nNewSize) ) )
			{
				GOPTR(BUFFER) newListBuffer;
				if( 0 > gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nNewSize, &newListBuffer ) )
				{
					error_printf("gcreateBuffer() FAILED!");
					return -1;
				};
				uint32_t iElement;
				for( iElement=0; iElement < nIndex; iElement++ )
					((_T*)newListBuffer->pByteArray)[iElement] = gacquireData( ((_T*)oldListBuffer->pByteArray)[iElement] );
				((_T*)newListBuffer->pByteArray)[iElement] = gacquireData( CoreInstance );
				iElement++;
				for(; iElement < nNewSize; iElement++ )
					((_T*)newListBuffer->pByteArray)[iElement] = gacquireData( ((_T*)oldListBuffer->pByteArray)[iElement-1] );
				oldListBuffer.free();
				SetBufferData( newListBuffer );
			}
			else
			{
				for( uint32_t nOldIndex = nOldSize; nOldIndex>nIndex; nOldIndex-- )
					this->get_pointer()[nOldIndex] = this->get_pointer()[nOldIndex-1];
				this->get_pointer()[nIndex] = gacquireData( CoreInstance );
				GOPTR(BUFFER) tmpBufferToIncrease = this->GetBufferData();
				tmpBufferToIncrease->nElementCount++;
			}
			return 0;
		};
		virtual error_t				pack( void )
		{
			glist<_T> lstPacked(this->get_valid_element_count());
			uint32_t iPacked = 0;
			for( uint32_t iLocal=0; iLocal<this->size(); iLocal++ )
			{
				if( (*this)[iLocal] )
				{
					lstPacked[iPacked] = gacquireData( (*this)[iLocal] );
					iPacked++;
				}
			}
			(*this) = lstPacked;
			return 0;
		};
	private:
		void						SetBufferData( GODS(BUFFER) newListBuffer )
		{
			gbaselist_safe<_T>::SetBufferData( newListBuffer );
		}

	};

	template<typename _T> class glist_handled : public gbaselist_safe<_T>
	{
		void			(*onCreate)( _T instance ); 
		void			(*onUpdate)( _T instance ); 
		void			(*onFree)( _T instance );
	public:
		glist_handled( void )
			:onCreate(0), 
			onUpdate (0), 
			onFree(0)
		{}
		~glist_handled( void )
		{
			SetBufferData(0);
		}
		glist_handled(	void (*onCreate)( _T ),
						void (*onUpdate)( _T ),
						void (*onFree)( _T ) )
			:onCreate(0), 
			onUpdate (0), 
			onFree(onFree)
		{

		}
		glist_handled( const glist_handled<_T>& other )
			:onCreate(other.onCreate), 
			onUpdate (other.onUpdate), 
			onFree(other.onFree),
			gbaselist_safe<_T>::gbaselist_safe(other)
		{
		}
		virtual const _T&		operator[](uint32_t nIndex) const
		{
			if( nIndex >= this->size() )
			{
				if( nIndex != -1 )
				{
					error_printf("Index out of boundaries!");
				}
				return this->INVALID;
			}
			return this->get_pointer()[nIndex];
		}
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
			this->get_pointer()[nIndex] = gacquireData( CoreInstance );
			if( CoreInstance
				&& CoreInstance != oldInstance 
				&& onUpdate )
				(*onUpdate)(CoreInstance);
			if( oldInstance && onFree )
				(*onFree)(oldInstance);
			gfreeData( &oldInstance );
			return 0;
		};
		virtual error_t			push( _T CoreInstance )
		{
			uint32_t nNewSize = this->size()+1;
			error_t errMy = this->resize(nNewSize);
			if( errMy )
			{
				error_printf( "Failed to resize list! Out of memory?" );
				return -1;
			}
			set( CoreInstance, nNewSize-1 );
			if( CoreInstance && onCreate )
				onCreate( CoreInstance );
			return 0;
		};
		virtual void			onFreeHandler( void (*Handler)( _T ) )
		{
			onFree		= Handler;
		};
	protected:
		virtual void		SetBufferData( GODS(BUFFER) ListBuffer )
		{
			GOPTR(BUFFER) oldBuffer = this->GetBufferData();	// Catch the old list so we can manage its elements.
			CGBaseBuffer::SetBufferData( ListBuffer );
			if( oldBuffer && oldBuffer.am_I_owning() )
			{
				if( oldBuffer.am_I_owning() )
				{
					gcore_ptr<_T> old;
					uint32_t iEl=0;
					while( oldBuffer->nElementCount )
					{
						old = ((_T*)oldBuffer->pByteArray)[iEl = --oldBuffer->nElementCount];
						((_T*)oldBuffer->pByteArray)[iEl] = 0;
						if( old && onFree )
							(*onFree)(old);
					}
				}
			}
		};// SetBufferData
	}; // glist_handled

#define ggrid2d ggrid2d_safe
#define GOGRID2D( TOKEN ) ggrid2d<GODS(TOKEN)>
	template<typename _T> class ggrid2d_safe : public gbaselist_safe<_T>
	{
	public:
		ggrid2d_safe( void )
		{}		
		ggrid2d_safe( uint32_t nWidth, uint32_t nDepth )
		{
			resize( nWidth, nDepth );	
		}	
		ggrid2d_safe( const _T* in_elementList, uint32_t nWidth, uint32_t nDepth )
		{
			resize( nWidth, nDepth );	
			if( in_elementList )
			{
				uint32_t size = nWidth*nDepth;
				for( uint32_t i=0; i<size; i++ )
					this->get_pointer()[i] = gacquireData( in_elementList[i] );
				//for( uint32_t iElement=0, iDepth=0; iDepth<nDepth; iDepth++ )
				//	for( uint32_t iWidth=0; iWidth<nWidth; iWidth++ )
				//  {
				//		(*this)[iDepth][iWidth] = gacquireData( in_elementList[iElement] );
				//		iElement++;
				//	}
			}
		}	
		virtual _T*				operator[]( uint32_t z )
		{
			uint32_t nLinearIndex	= z*row_size();
			if( nLinearIndex >= this->size() )
			{
				error_printf("Index out of boundaries! Index requested=%u, list size=%u", nLinearIndex, size() );
				return &this->_INVALID;
			}
			return &this->get_pointer()[nLinearIndex];
		}
		virtual const _T*		operator[]( uint32_t z ) const
		{
			uint32_t nLinearIndex	= z*row_size();
			if( nLinearIndex >= this->size() )
			{
				error_printf("Index out of boundaries! Index requested=%u, list size=%u", nLinearIndex, size() );
				return &this->_INVALID;
			}
			return &this->get_pointer()[nLinearIndex];
		}
		virtual uint32_t		row_size() const
		{
			GOPTR(BUFFER) ListBuffer;
			ListBuffer = this->GetBufferData();
			if( !ListBuffer )
				return 0;
			uint32_t nRowSize = ListBuffer->nRowElementCount;
			return nRowSize;
		};		
		virtual uint32_t		column_size() const
		{
			int32_t nRowSize = row_size();
			if( nRowSize )
				return this->size()/nRowSize;
			return 0;
		};
		virtual uint32_t		create( _T* in_lstCoreInstances, uint32_t nWidth, uint32_t nDepth, uint32_t nMaxInstanceCount=-1 )
		{
			if( 0 == nWidth || 0 == nDepth ) 
			{
				this->free_all();
				return 0;
			}
			if( 0 == in_lstCoreInstances )
			{
				error_printf("Cannot create list from a null address!");
				return -1;
			};
			GOPTR(BUFFER) newListBuffer;
			uint32_t nInstanceCount = nWidth*nDepth;
			if( gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nInstanceCount, &newListBuffer ) )
			{
				error_printf("Cannot create buffer for list!");
				return -1;
			};
			newListBuffer->nRowElementCount = nWidth;
#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif // min
			nInstanceCount = min( nInstanceCount, nMaxInstanceCount );
			for( uint32_t iCoreInstance=0; iCoreInstance < nInstanceCount; iCoreInstance++ )
				((_T*)newListBuffer->pByteArray)[iCoreInstance] = gacquireData( in_lstCoreInstances[iCoreInstance] );
			gbaselist_safe<_T>::SetBufferData( newListBuffer );
			return 0;
		};
		//-----------------------------------------------------
		virtual error_t			set( _T CoreInstance, uint32_t _x, uint32_t _z )
		{
			if( !valid_cell(_x, _z) )
			{
				error_printf("Invalid index! x=%u, y=%u.", _x, _z);
				return -1;
			}
			uint32_t nLinearIndex = _z*row_size()+_x;
			return gbaselist_safe<_T>::set( CoreInstance, nLinearIndex );
		};
		virtual _T				acquire( uint32_t _x, uint32_t _z )
		{
			if( !valid_cell(_x, _z) )
			{
				warning_printf("Invalid index! x=%u, y=%u.", _x, _z);
				return 0;
			}
			uint32_t nLinearIndex = _z*row_size()+_x;
			return gbaselist_safe<_T>::acquire( nLinearIndex );
		};
		virtual error_t			free( uint32_t _x, uint32_t _z )
		{
			if( !valid_cell(_x, _z) )
			{
				error_printf("Invalid index! x=%u, y=%u.", _x, _z);
				return 0;
			}
			uint32_t nLinearIndex = _z*row_size()+_x;
			return gbaselist_safe<_T>::free( nLinearIndex );
		};
		virtual error_t			set( _T CoreInstance, uint32_t nLinearIndex )
		{
			return gbaselist_safe<_T>::set( CoreInstance, nLinearIndex );
		}
		virtual _T				acquire( uint32_t nLinearIndex ) const
		{
			return gbaselist_safe<_T>::acquire( nLinearIndex ); 
		}
		virtual error_t			free( uint32_t nLinearIndex )
		{
			return gbaselist_safe<_T>::free( nLinearIndex );
		}
		virtual bool			valid_cell( uint32_t _x, uint32_t _z ) const
		{
			uint32_t nRowSize		= row_size();
			uint32_t nColumnSize	= nRowSize ? this->size()/nRowSize : 0;
			if( _x >= nRowSize 
				|| _z >= nColumnSize )
				return false;
			return true;
		};
		//------------------------------------------
		virtual gcore_ptr<_T>				get_front( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x+1, _z ); 
		}
		virtual gcore_ptr<_T>				get_right( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x, _z+1 ); 
		}
		virtual gcore_ptr<_T>				get_back( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x-1, _z ); 
		}
		virtual gcore_ptr<_T>				get_left( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x, _z-1 ); 
		}
		virtual gcore_ptr<_T>				get_front_right( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x+1, _z+1 ); 
		}
		virtual gcore_ptr<_T>				get_front_left( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x+1, _z-1 ); 
		}
		virtual gcore_ptr<_T>				get_back_right( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x-1, _z+1 );
		}
		virtual gcore_ptr<_T>				get_back_left( uint32_t _x, uint32_t _z ) const
		{
			return get_at( _x-1, _z-1); 
		}
		virtual gcore_ptr<_T>				get_at( uint32_t _x, uint32_t _z ) const
		{
			if( !valid_cell( _x, _z ) )
				return 0;
			gcore_ptr<_T> result;
			const _T* temp = operator[](_z);
			result = gacquireData( temp[_x] );
			return result;
		}
		virtual error_t						resize( uint32_t nWidth, uint32_t nDepth )
		{
			if( nWidth == row_size() && nDepth == column_size() )
				return 0;	// size already set
			if( 0 == nWidth || 0 == nDepth )
			{
				SetBufferData(0);
				return 0;
			}
			GODS(BUFFER) newListBuffer=0;
			uint32_t nCellCount = nWidth*nDepth;
			error_t errMy = gcreateBuffer( GTYPEID_DATA_MAKE(_T), GUSAGE_POINTER, nCellCount, &newListBuffer );
			if( 0 > errMy )
			{
				error_printf("Failed to create buffer for list! Out of memory?");
				return -1;
			}
			memset( newListBuffer->pByteArray, 0, GTYPEID_TOTALBYTES( newListBuffer->DataFormat )*nCellCount );
			if( this->size() )
			{
				GODS(BUFFER) oldListBuffer = this->GetBufferData();
				uint32_t nMaxZ = min( oldListBuffer->nElementCount/oldListBuffer->nRowElementCount, nDepth );
				uint32_t nMaxX = min( oldListBuffer->nRowElementCount, nWidth );
				uint32_t nLinearIndex;
				for( uint32_t iZ=0; iZ<nMaxZ; iZ++ )
				{
					for( uint32_t iX=0; iX<nMaxX; iX++ )
					{
						nLinearIndex = iZ*nWidth+iX;
						((_T*)newListBuffer->pByteArray)[nLinearIndex] = gacquireData( ((_T*)oldListBuffer->pByteArray)[nLinearIndex] );
					}
				}
				gfreeData( &oldListBuffer );
			}
			newListBuffer->nRowElementCount = nWidth;
			SetBufferData( newListBuffer );
			gfreeData( &newListBuffer );
			return 0;
		};
	protected:
		virtual void		SetBufferData( GODS(BUFFER) ListBuffer )
		{
			gbaselist_safe<_T>::SetBufferData( ListBuffer );
		};
	};


	template<typename _T> class ginterfacelist_handled : public ginterfacelist_safe<_T>
	{
		void			(*onCreate)( _T instance ); 
		void			(*onUpdate)( _T instance ); 
		void			(*onFree)( _T instance );
	public:
		ginterfacelist_handled( void )
			:onCreate(0), 
			onUpdate (0), 
			onFree(0)
		{}
		~ginterfacelist_handled( void )
		{
			SetBufferData(0);
		}
		ginterfacelist_handled(	void (*onCreate)( _T ),
						void (*onUpdate)( _T ),
						void (*onFree)( _T ) )
			:onCreate(0), 
			onUpdate (0), 
			onFree(onFree)
		{

		}
		ginterfacelist_handled( const ginterfacelist_handled<_T>& other )
			:onCreate(other.onCreate), 
			onUpdate (other.onUpdate), 
			onFree(other.onFree),
			ginterfacelist_safe<_T>::ginterfacelist_safe(other)
		{
		}
		virtual const _T&		operator[](uint32_t nIndex) const
		{
			if( nIndex >= this->size() )
			{
				if( nIndex != -1 )
				{
					error_printf("Index out of boundaries!");
				}
				return this->INVALID;
			}
			return this->get_pointer()[nIndex];
		}
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
			this->get_pointer()[nIndex] = gacquireData( CoreInstance );
			if( CoreInstance
				&& CoreInstance != oldInstance 
				&& onUpdate )
				(*onUpdate)(oldInstance);
			if( oldInstance && onFree )
				(*onFree)(oldInstance);
			gfreeData( &oldInstance );
			return 0;
		};
		virtual error_t			push( _T CoreInstance )
		{
			uint32_t nNewSize = this->size()+1;
			error_t errMy = this->resize(nNewSize);
			if( errMy )
			{
				error_printf( "Failed to resize list! Out of memory?" );
				return -1;
			}
			this->set( CoreInstance, nNewSize-1 );
			if( CoreInstance && onCreate )
				onCreate( CoreInstance );
			return 0;
		};
		virtual void			onFreeHandler( void (*Handler)( _T ) )
		{
			onFree		= Handler;
		};
	protected:
		virtual void		SetBufferData( GODS(BUFFER) ListBuffer )
		{
			GOPTR(BUFFER) oldBuffer = this->GetBufferData();	// Catch the old list so we can manage its elements.
			CGBaseBuffer::SetBufferData( ListBuffer );
			if( oldBuffer && oldBuffer.am_I_owning() )
			{
				if( oldBuffer.am_I_owning() )
				{
					gcore_ptr<_T> old;
					uint32_t iEl=0;
					while( oldBuffer->nElementCount )
					{
						old = ((_T*)oldBuffer->pByteArray)[iEl = --oldBuffer->nElementCount];
						((_T*)oldBuffer->pByteArray)[iEl] = 0;
						if( old && onFree )
							(*onFree)(old);
					}
				}
			}
		};// SetBufferData
	}; // ginterfacelist_handled

};

#endif // _GLIST_H
