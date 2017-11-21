#pragma once

#include "Helper.h"
#include "Component\Component.h"
#include "Transform.h"

namespace Mile
{
   class Context;
   class Component;
   class Entity
   {
   public:
      Entity( Context* context, const String& name );
      ~Entity( );

      std::string Serialize( ) const;
      void DeSerialize( const json& jsonData );

      /**
      * @brief    Entity�� �ʱ�ȭ �մϴ�. ( �� �������� Entity�� Transform �� �����˴ϴ�. )
      * @param    Entity�� ������ Transform ������Ʈ
      */
      bool Init( );

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� �߰��մϴ�.
      * @return   �߰��� ������Ʈ
      */
      template <typename Ty> Ty* AddComponent( );

      /**
      * @brief    �־��� ������Ʈ�� Entity�� ��ϵǾ��ִٸ� �����մϴ�.
      * @param    Entity�� ��ϵǾ��ִ� ������Ʈ
      */
      template <typename Ty> void RemoveComponent( Ty* target );

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ��� ������Ʈ���� �����մϴ�. ( �� �޸��� �Ҵ������� �Ͼ�� �ʽ��ϴ�. )
      */
      template <typename Ty> void RemoveComponents( );

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� ���� ���� �߰��� ������Ʈ�� ��ȯ�մϴ�.
      * @return   ���� ���� �߰��� �ش� ������ ������Ʈ
      */
      template <typename Ty> Ty* GetComponent( );

      /**
      * @brief    Entitiy�� �߰��Ǿ��ִ� ��� ������Ʈ���� ��ȯ�մϴ�.
      * @return   Entitiy�� �߰��Ǿ��ִ� ������Ʈ ���� �迭
      */
      template <typename Ty> std::vector<Ty*> GetComponents( );

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� Entity�� �߰��Ǿ��ִ��� Ȯ���մϴ�.
      * @return   �ش� ������ ������Ʈ�� �߰� ����
      */
      template <typename Ty> bool HasComponent( ) const;

      /**
      * @brief    Entity�� Ȱ��ȭ �Ǿ��ִ��� Ȯ���մϴ�.
      * @return   Entity�� Ȱ��ȭ ����
      */
      bool IsActive( ) const { return m_bIsActive; }

      /**
      * @brief    Entity�� Ȱ��ȭ ��Ű�ų� �� Ȱ��ȭ ��ŵ�ϴ�.
      * @param    Entity�� Ȱ��ȭ ����
      */
      void SetActive( bool bIsActive );

      /**
      * @brief    Entity�� ��ϵǾ��ִ� Transform ������Ʈ�� ��ȯ�մϴ�.
      * @return   Entity�� Transform ������Ʈ
      */
      Transform* GetTransform( ) { return m_transform; }

      void SetTransform( const Transform& transform );

      /**
      * @brief    Entity�� �����Ǿ��ִ� �̸��� ��ȯ�մϴ�.
      * @return   Entity�� �̸�
      */
      String GetName( ) const { return m_name; }

      /**
      * @brief    Entity�� ���ο� �̸��� �������ݴϴ�.
      * @param    Entity�� �������� ���ο� �̸�
      */
      void SetName( const String& name ) { m_name = name; }

      bool AttachChild( Entity* child );
      bool DetachChild( Entity* child );

      Entity* GetParent( ) { return m_parent; }
      Entity* GetChildByName( const String& name );

      std::vector<Entity*> GetChildren( ) const { return m_children; }

      void Start( );
      void Update( );
      void OnEnable( );
      void OnDisable( );

   protected:
      Context*                  m_context;
      bool                      m_bIsActive;
      Transform*                m_transform;

   private:
      std::vector<Component*>   m_components;
      std::vector<Entity*>      m_children;
      Entity*                   m_parent;
      String                    m_name;

   };

   template <typename Ty>
   Ty* Entity::AddComponent( )
   {
      Component* component = new Ty( m_context );
      m_components.push_back( component );

      component->Reset( );
      component->SetActive( true );
      component->m_transform = m_transform;

      return static_cast< Ty* >( component );
   }

   template <typename Ty>
   void Entity::RemoveComponent( Ty* target )
   {
      if ( target != nullptr )
      {
         for ( auto itr = m_components.begin( ); itr != m_components.end( ); ++itr )
         {
            SafeDelete( target );
            m_components.erase( itr );
            break;
         }
      }
   }

   template <typename Ty>
   void Entity::RemoveComponents( )
   {
      for ( auto itr = m_components.begin( ); itr != m_components.end( ); ++itr )
      {
         Component* foundComponent = *itr;
         if ( typeid( Ty ) == typeid ( *( foundComponent ) ) )
         {
            SafeDelete( foundComponent );
            m_components.erase( itr );
         }
      }
   }

   template <typename Ty>
   Ty* Entity::GetComponent( )
   {
      auto typeID = typeid( Ty );

      if ( m_transform != nullptr )
      {
         if ( typeID == typeid( *m_transform ) )
         {
            return m_transform;
         }
      }

      for ( auto component : m_components )
      {
         if ( typeID == typeid( *component ) )
         {
            return component;
         }
      }

      return nullptr;
   }

   template <typename Ty>
   std::vector<Ty*> Entity::GetComponents( )
   {
      auto typeID = typeid( Ty );
      std::vector<Ty*> tempArr{ };

      if ( m_transform != nullptr )
      {
         if ( typeID == typeid( *m_transform ) )
         {
            tempArr.push_back( m_transform );
         }

         return std::move( tempArr );
      }

      for ( auto component : m_components )
      {
         if ( typeID == typeid( *component ) )
         {
            tempArr.push_back( component );
         }
      }

      return std::move( tempArr );
   }

   template <typename Ty>
   bool Entity::HasComponent( ) const
   {
      return ( ( GetComponent<Ty>( ) ) != nullptr );
   }
}
