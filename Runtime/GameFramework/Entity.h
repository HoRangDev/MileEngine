#pragma once
#include "Core/Helper.h"
#include "Component/Component.h"
#include "GameFramework/Transform.h"

#define DEFAULT_ENTITY_NAME TEXT("Entity")
#define DEFAULT_ENTITY_TAG TEXT("Default")

namespace Mile
{
   class Context;
   class World;
   class Component;
   class MEAPI Entity
   {
      friend World;
      friend class ModelLoader;

   private:
      Entity(World* world, const String& name = DEFAULT_ENTITY_NAME, const String& tag = DEFAULT_ENTITY_TAG);

   public:
      ~Entity();

      json Serialize() const;
      // Null world entitiy must not deserialized.
      void DeSerialize(const json& jsonData);

      /**
      * @brief    Entity�� �ʱ�ȭ �մϴ�. ( �� �������� Entity�� Transform �� �����˴ϴ�. )
      * @param    Entity�� ������ Transform ������Ʈ
      */
      bool Init();

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� �߰��մϴ�.
      * @return   �߰��� ������Ʈ
      */
      template <typename Ty> Ty* AddComponent();

      /**
      * @brief    �־��� ������Ʈ�� Entity�� ��ϵǾ��ִٸ� �����մϴ�.
      * @param    Entity�� ��ϵǾ��ִ� ������Ʈ
      */
      template <typename Ty> void RemoveComponent(Ty* target);

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ��� ������Ʈ���� �����մϴ�. ( �� �޸��� �Ҵ������� �Ͼ�� �ʽ��ϴ�. )
      */
      template <typename Ty> void RemoveComponents();

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� ���� ���� �߰��� ������Ʈ�� ��ȯ�մϴ�.
      * @return   ���� ���� �߰��� �ش� ������ ������Ʈ
      */
      template <typename Ty> Ty* GetComponent();

      /**
      * @brief    Entitiy�� �߰��Ǿ��ִ� ��� ������Ʈ���� ��ȯ�մϴ�.
      * @return   Entitiy�� �߰��Ǿ��ִ� ������Ʈ ���� �迭
      */
      template <typename Ty> std::vector<Ty*> GetComponents();

      /**
      * @brief    ���ø� �Ķ���ͷ� �־��� ������ ������Ʈ�� Entity�� �߰��Ǿ��ִ��� Ȯ���մϴ�.
      * @return   �ش� ������ ������Ʈ�� �߰� ����
      */
      template <typename Ty> bool HasComponent() const;

      /**
      * @brief    Entity�� Ȱ��ȭ �Ǿ��ִ��� Ȯ���մϴ�.
      * @return   Entity�� Ȱ��ȭ ����
      */
      bool IsActivated() const { return m_bIsActivated; }

      /**
      * @brief    Entity�� Ȱ��ȭ ��Ű�ų� �� Ȱ��ȭ ��ŵ�ϴ�.
      * @param    Entity�� Ȱ��ȭ ����
      */
      void SetActive(bool bIsActive);

      /**
      * @brief    Entity�� ��ϵǾ��ִ� Transform ������Ʈ�� ��ȯ�մϴ�.
      * @return   Entity�� Transform ������Ʈ
      */
      Transform* GetTransform() { return m_transform; }

      void SetTransform(const Transform& transform);

      /**
      * @brief    Entity�� �����Ǿ��ִ� �̸��� ��ȯ�մϴ�.
      * @return   Entity�� �̸�
      */
      String GetName() const { return m_name; }

      /**
      * @brief    Entity�� ���ο� �̸��� �������ݴϴ�.
      * @param    Entity�� �������� ���ο� �̸�
      */
      void SetName(const String& name) { m_name = name; }

      String GetTag() const { return m_tag; }

      void SetTag(const String& tag) { m_tag = tag; }

      bool AttachChild(Entity* child);
      bool DetachChild(Entity* child);

      Entity* GetParent() { return m_parent; }
      Entity* GetChildByName(const String& name);
      bool HasParent() { return m_parent != nullptr; }

      std::vector<Entity*> GetChildren() const { return m_children; }
      std::vector<Entity*> GetChildren() { return m_children; }

      void Start();
      void Update();
      void OnEnable();
      void OnDisable();

      World* GetWorld() const { return m_world; }
      Context* GetContext() const { return m_context; }

   private:
      bool     m_bIsActivated;
      Context* m_context;
      World*   m_world;
      Entity*  m_parent;
      Transform* m_transform;
      std::vector<Component*> m_components;
      std::vector<Entity*>    m_children;
      String   m_name;
      String   m_tag;

   };

   template <typename Ty>
   Ty* Entity::AddComponent()
   {
      Component* component = new Ty(this);
      m_components.push_back(component);

      component->Reset();
      component->SetActive(true);

      return static_cast<Ty*>(component);
   }

   template <typename Ty>
   void Entity::RemoveComponent(Ty* target)
   {
      if (target != nullptr)
      {
         for (auto itr = m_components.begin(); itr != m_components.end(); ++itr)
         {
            SafeDelete(target);
            m_components.erase(itr);
            break;
         }
      }
   }

   template <typename Ty>
   void Entity::RemoveComponents()
   {
      for (auto itr = m_components.begin(); itr != m_components.end(); ++itr)
      {
         Component* foundComponent = *itr;
         if (typeid(Ty) == typeid (*(foundComponent)))
         {
            SafeDelete(foundComponent);
            m_components.erase(itr);
         }
      }
   }

   template <typename Ty>
   Ty* Entity::GetComponent()
   {
      for (auto component : m_components)
      {
         if (typeid(Ty) == typeid(*component))
         {
            return reinterpret_cast<Ty*>(component);
         }
      }

      return nullptr;
   }

   template <typename Ty>
   std::vector<Ty*> Entity::GetComponents()
   {
      std::vector<Ty*> tempArr{ };

      for (auto component : m_components)
      {
         if (typeid(Ty) == typeid(*component))
         {
            tempArr.push_back(component);
         }
      }

      return std::move(tempArr);
   }

   template <typename Ty>
   bool Entity::HasComponent() const
   {
      return ((GetComponent<Ty>()) != nullptr);
   }
}
