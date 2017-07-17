#pragma once

#pragma warning( disable : 4251 )

#include "Rumia\Allocator.h"

#define MEAPI __declspec(dllexport)

namespace Mile
{
    using namespace Rumia;

    /**
    @brief  Release�� ���� �޸� �Ҵ� ������ �ϴ� �ν��Ͻ��� �����ϰ� ���������ݴϴ�.
    */
    template <typename T>
    void SafeRelease( T& target )
    {
        if ( target != nullptr )
        {
            target->Release( );
            target = nullptr;
        }
    }
}