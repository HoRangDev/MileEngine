#pragma once

#include "SubSystem.h"
#include "Rumia/Array.h"

namespace Mile
{
    class Entity;
    using WeakEntity = std::weak_ptr<Entity>;
    using SharedEntity = std::shared_ptr<Entity>;

    class MEAPI World : public SubSystem
    {
    public:
        World( Context* context );
        virtual ~World( );

        /**
        * @brief    World�� �ʱ�ȭ�մϴ�.
        */
        virtual bool Initialize( ) override;

        /**
        * @brief    World�� �����Ǿ��ִ� Entity���� Start �Լ��� ȣ���մϴ�.
        */
        void Start( );

        /**
        * @brief    World�� �����Ǿ��ִ� Entity���� Update �մϴ�.
        */
        void Update( );

        /**
        * @brief    World�� ���ο� Entity�� �����մϴ�.
        * @return   ������ Entity
        */
        WeakEntity CreateEntity( );

        /**
        * @brief    World�� �����Ǿ��ִ� Entitiy�� �־��� �̸��� ���� Entity�� ��ȯ�մϴ�.
        *           ( �� ���� �̸��� ������ Entity�� ���� ���� �߰��� Entity�� ��ȯ��. )
        * @param    �ش��ϴ� �̸��� ������ Entity
        */
        WeakEntity GetEntityByName( const std::string& name );

        /**
        * @brief    World�� �������ִ� ��� Entity���� �迭�� ��ȯ�մϴ�.
        * @param    Entity �迭
        */
        Array<WeakEntity> GetEntities( );

    private:
        Array<SharedEntity> m_entities;

    };
}