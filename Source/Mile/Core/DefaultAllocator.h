#pragma once
#include "Allocator.h"
#include "MileChar.h"

namespace Mile
{
	/**
	* �޸� ���� �Ҵ����ִ� �⺻ �Ҵ����Դϴ�.
	* @brief ���������� �۵��ϴ� �Ҵ����Դϴ�.
	*/
	class MILE_API DefaultAllocator : public Allocator
	{
	public:
		virtual void* Allocate( uint64 Size ) override
		{
            TEXT( )
			return malloc( Size );
		}

		virtual void Deallocate( void* Target ) override
		{
			free( Target );
		}

		virtual void* Reallocate( void* Target, uint64 Size ) override
		{
			return realloc( Target, Size );
		}

		virtual void* AllocateAligned( uint64 SizeBytes, uint64 Alignment )
		{
			UNUSED_PARAM( SizeBytes );
			UNUSED_PARAM( Alignment );
			ASSERT_MSG( false, "Default Allocator is not support Aligned Allocate." );
		}

		virtual void DeallocateAligned( void* Target ) override
		{
			UNUSED_PARAM( Target );
			ASSERT_MSG( false, "Default Allocator is not support Aligned Deallocate." );
		}
	};
}