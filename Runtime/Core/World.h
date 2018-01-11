#pragma once

#include "SubSystem.h"

namespace Mile
{
    class Entity;
    template<typename Ty>
    class PlainText;

    class MEAPI World : public SubSystem
    {
    public:
        World( Context* context );
        virtual ~World( );

        std::string Serialize( ) const;
        void DeSerialize( const json& jsonData );

        /**
        * @brief    World�� �ʱ�ȭ�մϴ�.
        */
        virtual bool Init( ) override;
        virtual void DeInit( ) override;

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
        Entity* CreateEntity( const String& name );

        /**
        * @brief    World�� �����Ǿ��ִ� Entitiy�� �־��� �̸��� ���� Entity�� ��ȯ�մϴ�.
        *           ( �� ���� �̸��� ������ Entity�� ���� ���� �߰��� Entity�� ��ȯ��. )
        * @param    �ش��ϴ� �̸��� ������ Entity
        */
        Entity* GetEntityByName( const String& name );

        /**
        * @brief    World�� �����Ǿ��ִ� ��� Entity���� �迭�� ��ȯ�մϴ�.
        * @return    Entity �迭
        */
        std::vector<Entity*> GetEntities( );

        /**
        * @brief    World�� �����Ǿ��ִ� Entity �� Root Entitiy���� �迭�� ��ȯ�մϴ�.
        */
        std::vector<Entity*> GetRootEntities( );

        bool LoadFrom( const String& filePath );
        bool SaveTo( const String& filePath );
        bool Save( );

    private:
        std::vector<Entity*> m_entities;
        std::weak_ptr<PlainText<std::string>> m_loadedData;

    };
}