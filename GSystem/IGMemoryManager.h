#include "GCore_constants.h"
#include "GMemory_core.h"


namespace god
{
	class IGMemoryManager
	{
	public:
		IGMemoryManager( void ){};
		virtual ~IGMemoryManager( void ){};

		virtual error_t		CreateBuffer( GDATA_TYPE DataType, GRESOURCE_USAGE Usage, uint32_t Size, uint32_t RowWidth, GODS(BUFFER)* out_pCreatedBuffer )=0;
		virtual error_t		FreeBuffer( GODS(BUFFER) Buffer )=0;
	};
}