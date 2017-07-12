#pragma once

#include "SubSystem.h"

namespace Mile
{
    class World;
    class MEAPI Engine : public SubSystem
    {
    public:
        Engine( Context* context );
        virtual ~Engine( ) { ShutDown( ); }

        /**
        * @brief Engine�� Subsystem ���� �ʱ�ȭ�մϴ�.
        * @return �ʱ�ȭ ���� ����
        */
        virtual bool Initialize( ) override;

        /**
        * @brief Engine�� Subsystem ���� ������Ʈ�մϴ�.
        */
        void Update( );

        /**
        * @brief Engine�� Subsystem ���� �����մϴ�.
        */
        void ShutDown( );

    private:
        World*  m_world;

    };
}