#pragma once

#include "SubSystem.h"
#include "Rumia/Array.h"
#include <vector>

namespace Mile
{
    class MEAPI Context
    {
    public:
        Context( Allocator& allocator );
        ~Context( );

    public:
        /**
        * @brief Context�� �����ϰ��ִ� �Ҵ��ڸ� �����ɴϴ�.
        * @return Context�� �Ҵ���
        */
        Allocator& GetAllocator( ) { return m_allocator; }

        /**
        * @brief ���ο� Subsystem�� Context�� ����մϴ�.
        * @param newSubSystem ���� ����� Subsystem�� �޸� �ּ�
        */
        void RegisterSubSystem( SubSystem* newSubSystem );
        template <typename T> T* GetSubSystem( );

        /**
        * @brief �־��� Ÿ���� Subsystem �� Context�� ��ϵǾ��ִ��� �� �� �ֽ��ϴ�.
        * @return Subsystem ���� ����
        */
        template <typename T> bool HasSubSystem( ) const;

    private:
        Allocator& m_allocator;
        Array<SubSystem*> m_subSystems;

    };

    template <typename T>
    T* Context::GetSubSystem( )
    {
        for ( const auto& subSys : m_subSystems )
        {
            if ( typeid( T ) == typeid( *subSys ) )
            {
                return static_cast<T*>( subSys );
            }
        }

        return nullptr;
    }

    template <typename T>
    bool Context::HasSubSystem( ) const
    {
        return ( GetSubSystem<T>( ) != nullptr );
    }
}