/// File:GCore_macros.h
///
/// Contains macros for declaring the core functions and structures of the system.
///
/// This file is part of the Game Object Data System.
/// 2012-2013 (c) Pablo Ariel Zorrilla Cepeda
///

#ifndef _GCORE_MACROS_H
#define _GCORE_MACROS_H

// Core structures ----------------------------------------------------------------------------------------------------------------------------------------------
#define __GODS_CORE_STRUCT_PREFIX	__G
#define __GODS_CORE_STRUCT_POSTFIX	_DATA
#define GODS_CORE_STRUCT_PREFIX		G
#define GODS_CORE_STRUCT_POSTFIX	_DATA

#define __GODS(TOKEN)	__G##TOKEN##_DATA
#define GODS(TOKEN)		G##TOKEN##_DATA

// This macro is used to declare factory/refcount functions for the structures that implement them
#define __GDECLARE_CORE_ALLOC_FUNCTIONS(_structName, Token)													\
	void			gcreateData( _structName** inout_p##Token );											\
	void			gfreeData( _structName** inout_p##Token );												\
	void			gfreeData( _structName** inout_p##Token, uint32_t* n##Token##Count );					\
	_structName*	gacquireData( _structName* in_##Token );												\
	void			gcopyData( _structName* dst_##Token, const _structName* src_##Token );					\
	void			gcloneData( _structName** out_p##Token, const _structName* src_##Token );				\
	int32_t			gcompareData( const _structName* in_##Token##A, const _structName* in_##Token##B );		\
	bool			gwritableData( const _structName* in_##Token );											\

// This macro is used to declare read/write functions of the structure data (serialization)
#define __GDECLARE_CORE_SERIALIZATION_FUNCTIONS(_structName, Token)																	\
	uint32_t fileDeserializeData( _structName** out_lstDataInstances, uint32_t in_nInstanceCount, FILE* in_fp );					\
	uint32_t fileSerializeData( _structName* const* in_lstDataInstances, uint32_t in_nInstanceCount, FILE* out_fp );				\
	uint32_t memDeserializeData( _structName** out_lstDataInstances, uint32_t in_nInstanceCount, const void* in_pMemoryBuffer );	\
	uint32_t memSerializeData( _structName* const* in_lstDataInstances, uint32_t in_nInstanceCount, void* out_pMemoryBuffer );		\

#define __GDECLARE_CORE_STREAMING_FUNCTIONS( _structName, Token)																			\
	uint32_t fileReadData( _structName** out_lstDataInstances, uint32_t in_nInstanceCount, FILE* in_fp );									\
	uint32_t fileWriteData( _structName* const* in_lstDataInstances, uint32_t in_nInstanceCount, FILE* out_fp, _structName* DefaultData );	\
	uint32_t memReadData( _structName** out_lstDataInstances, uint32_t in_nInstanceCount, const void* in_pMemoryBuffer );					\
	uint32_t memWriteData( _structName*  const* in_lstDataInstances, uint32_t in_nInstanceCount, void* out_pMemoryBuffer, _structName* DefaultData );

// This macro declares functions to retrieve formatted informative text about the structures. These must be coded individually for each structure 
#define __GDECLARE_CORE_DEBUG_STRING_FUNCTIONS(_structName, Token)											\
	void printInfoString( const _structName* in_##Token );													\
	void getInfoString( wchar_t* dst_pOutputBuffer, uint32_t nBufferSize, const _structName* in_##Token );	\
	void getInfoString( char* dst_pOutputBuffer, uint32_t nBufferSize, const _structName* in_##Token );

// This macro is used to declare all core functions at once
#define __GDECLARE_CORE_STRUCT_FUNCTIONS( TOKEN, Token )			\
	__GDECLARE_CORE_ALLOC_FUNCTIONS( __GODS(TOKEN), Token );			\
	__GDECLARE_CORE_STREAMING_FUNCTIONS( __GODS(TOKEN), Token );		\
	__GDECLARE_CORE_SERIALIZATION_FUNCTIONS( __GODS(TOKEN), Token );	\
	__GDECLARE_CORE_DEBUG_STRING_FUNCTIONS( __GODS(TOKEN), Token );

#define __GDECLARE_CORE_STRUCT_TYPEDEFS( TOKEN, Token )	\
	typedef gbase_ptr_safe<GODS(TOKEN)>			gptr_##Token;	\
	typedef gcore_ptr<GODS(TOKEN)>				goptr_##Token;	\
	typedef genius_ptr<GODS(TOKEN)>				ggns_##Token;	\
	typedef gbase_handler_safe<GODS(TOKEN)>		ghdlr_##Token;

#define __GDECLARE_CORE_STRUCT_HANDLERS( Token )	//
	//void onCreate##Token##Default( ggns_##Token* stuff );

// This macro is used to mark the beginning of a core structure declaration. It must be called to open the declaration of the structure before defining the member list.
#define GBEGIN_CORE_STRUCT(TOKEN)	\
	typedef struct __GODS(TOKEN)	 *GODS(TOKEN);			\
	typedef struct __GODS(TOKEN)		\
	{								\
		refcount_t				__nRefCount;

// This macro is used to close the declaration of a core function. It must be called to close the member list.
// NOTE: This macro also declares all the core functions related to the structure by referencing the __GDECLARE_CORE_FUNCTIONS() macro.
#define GEND_CORE_STRUCT(TOKEN, Token)					\
	public:												\
		void	(*__pfreeData)( GODS(TOKEN)* );			\
		static const uint32_t	__kDataSize;			\
		static const uint8_t	__kDataOffset;			\
		static const cue_t		__kCue;					\
		static const uint8_t	__kCueLen;				\
		static const uint8_t	__kDataTurbo;			\
		static const uint8_t	__kDataReserved;		\
	} *GODS(TOKEN);										\
	__GDECLARE_CORE_STRUCT_FUNCTIONS( TOKEN, Token );	\
	__GDECLARE_CORE_STRUCT_TYPEDEFS( TOKEN, Token )		\
	__GDECLARE_CORE_STRUCT_HANDLERS( Token )

#define GDECLARE_CORE_STRUCT( TOKEN, Token, Members )	\
	struct S##Token										\
	{													\
		Members;										\
	};													\
	GBEGIN_CORE_STRUCT(TOKEN)							\
		Members;										\
	GEND_CORE_STRUCT(TOKEN, Token)						\
	bool			gpowerizeData( __GODS(TOKEN)** out_G##Token##Instance, const S##Token& in_S##Token );	\
	bool			gdepowerizeData( S##Token& out_S##Token, const __GODS(TOKEN)* in_G##Token##Instance );	\

// Core interfaces ----------------------------------------------------------------------------------------------------------------------------------------------
#define __GODS_CORE_INTERFACE_PREFIX		__G
#define __GODS_CORE_INTERFACE_POSTFIX	_HANDLER
#define GODS_CORE_INTERFACE_PREFIX		G
#define GODS_CORE_INTERFACE_POSTFIX		_HANDLER

#define __GOSH(TOKEN)	__G##TOKEN##_HANDLER
#define GOSH(TOKEN)		G##TOKEN##_HANDLER

// This macro is used to declare factory/refcount functions for the structures that implement them
#define __GDECLARE_CORE_INTERFACE_FUNCTIONS(_structName, Token)						\
	void gcreateInterface( _structName** inout_p##Token );							\
	void gfreeInterface( _structName** inout_p##Token );							\
	void gfreeInterface( _structName** inout_p##Token, uint32_t* n##Token##Count );	\
	_structName* gacquireInterface( _structName* in_##Token );						\
	void gfreeInterfaceArray( _structName** in_lst##Token, uint32_t* nElementCount );
//
#define __GDECLARE_ALL_INTERFACE_FUNCTIONS(TOKEN, Token)		\
	__GDECLARE_CORE_INTERFACE_FUNCTIONS(__GOSH(TOKEN), Token)

#define __GDECLARE_CORE_HANDLER_TYPEDEFS( TOKEN, Token )	\
	typedef void (*GGHC_##TOKEN)(GENIUSH(TOKEN)*);			\
	typedef void (*GHHC_##TOKEN)(GOSH(TOKEN));				\
	typedef ginterface_ptr<GOSH(TOKEN)> ghdlr_##Token;

// This macro is used to mark the beginning of a core structure declaration. It must be called to open the declaration of the structure before defining the member list.
#define GBEGIN_CORE_HANDLER(TOKEN)	\
	typedef struct __GOSH(TOKEN)	 *GOSH(TOKEN);			\
	typedef struct __GOSH(TOKEN)		\
	{								\
		refcount_t				__nRefCount;
#define GBEGIN_DERIVED_CORE_HANDLER(TOKEN, ...)	\
	typedef struct __GOSH(TOKEN)	 *GOSH(TOKEN);			\
	typedef struct __GOSH(TOKEN) : __VA_ARGS__	\
	{											\
		refcount_t				__nRefCount;

// This macro is used to close the declaration of a core function. It must be called to close the member list.
// NOTE: This macro also declares all the core functions related to the structure by referencing the __GDECLARE_CORE_FUNCTIONS() macro.
#define GEND_CORE_HANDLER(TOKEN, Token)														\
	public: 																				\
		virtual void*			GetInterfacePointer( GINTERFACEID_TYPE InterfaceID )=0;		\
		void					(*__pfreeInterface)( GOSH(TOKEN)* );						\
		static const cue_t		__kCue;														\
		static const size_t		__kCueLen;													\
		virtual ~__GOSH(TOKEN)(void){};														\
	} *GOSH(TOKEN);																			\
	__GDECLARE_ALL_INTERFACE_FUNCTIONS(TOKEN,Token);										\
	__GDECLARE_CORE_HANDLER_TYPEDEFS(TOKEN,Token);

#endif // _GCORE_MACROS_H