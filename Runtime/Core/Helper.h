#pragma once

#pragma warning( disable : 4251 )

#include "Rumia\Allocator.h"
#include "Rumia\Array.h"

#define MEAPI __declspec(dllexport)

namespace Mile
{
    using namespace Rumia;

    /**
    * @brief    �־��� ���ڿ��� ��ū�� �������� �����ϴ�.
    * @param    Array�� ����µ� ���� �Ҵ���
    * @param    ���� ���ڿ�
    * @param    ��ū
    * @return   �������� ���ڿ����� �迭
    */
    Array<std::string> Split( Allocator& allocator, const std::string& str, char token )
    {
        Array<std::string> tempArr{ allocator };
        size_t before = 0;
        for ( size_t idx = 0; idx < str.length( ); ++idx )
        {
            if ( str[ idx ] == token )
            {
                std::string tempStr = str.substr( before, idx - before );
                if ( !tempStr.empty( ) )
                {
                    tempArr.PushBack( std::move( tempStr ) );
                }
                before = idx;
            }
        }

        return tempArr;
    }

    /**
    * @brief    �־��� ���ڿ����� �迭�� ��Ĩ�ϴ�.
    * @param    ��ĥ ���ڿ����� �迭
    * @param    ���ڿ����� ��ġ�鼭 �߰��� �� ��ū
    * @return   ������ ���ڿ�
    */
    std::string Combine( Array<std::string> strings, const std::string& token = "" )
    {
        std::string temp{ };

        for ( auto str : strings )
        {
            temp += str;
            temp += token;
        }

        return temp;
    }

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