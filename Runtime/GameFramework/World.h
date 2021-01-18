#pragma once
#include "Core/SubSystem.h"
#include "Component/Component.h"
#include "GameFramework/Entity.h"

#define MILE_WORLD_LOG_CATEGORY TEXT("World")

namespace Mile
{
   template<typename Ty>
   class PlainText;
   class Entity;
   class MEAPI World : public SubSystem
   {
   public:
      World(Context* context);
      virtual ~World();

      //std::string Serialize( ) const;
      json Serialize() const;
      void DeSerialize(const json& jsonData);

      /**
      * @brief    World�� �ʱ�ȭ�մϴ�.
      */
      virtual bool Init() override;
      virtual void DeInit() override;

      /**
      * @brief    World�� �����Ǿ��ִ� Entity���� Start �Լ��� ȣ���մϴ�.
      */
      void Start();

      /**
      * @brief    World�� �����Ǿ��ִ� Entity���� Update �մϴ�.
      */
      void Update();

      /**
      * @brief    World�� ���ο� Entity�� �����մϴ�.
      * @return   ������ Entity
      */
      Entity* CreateEntity(const String& name);

      /**
       * @brief   World���� ������ Entity�� ã�� �����մϴ�
       * @return  ������ �����ߴ���
       */
      bool DestroyEntity(Entity* target);

      /**
       * @breif   �� World�� �����ִ� Entity���� Ȯ���մϴ�.
       */
      bool CheckEntityValidation(Entity* target) const;

      /**
      * @brief    World�� �����Ǿ��ִ� Entitiy�� �־��� �̸��� ���� Entity�� ��ȯ�մϴ�.
      *           ( �� ���� �̸��� ������ Entity�� ���� ���� �߰��� Entity�� ��ȯ��. )
      * @param    �ش��ϴ� �̸��� ������ Entity
      */
      Entity* GetEntityByName(const String& name) const;

      /**
      * @brief    ���忡 �����Ǿ��ִ� ��� Entity���� �迭�� ��ȯ�մϴ�.
      * @return   Entity �迭
      */
      std::vector<Entity*> GetEntities() const;

      /**
      * @brief    ���忡 �����Ǿ��ִ� Entity �� Root Entitiy���� �迭�� ��ȯ�մϴ�.
      */
      std::vector<Entity*> GetRootEntities() const;

      /**
       * @brief   ���忡 �����Ǿ��ִ� ��� Entity�� ���� �־��� ������Ʈ Ÿ���� ������Ʈ���� ��� ��ȯ�մϴ�.
       */
      template <typename ComponentType, 
         std::enable_if_t<std::is_base_of_v<Component, ComponentType>, bool> = true>
      std::vector<ComponentType*> GetComponentsFromEntities(bool onlyActivated = true) const
      {
         std::vector<ComponentType*> foundComponents;
         for (Entity* entity : m_entities)
         {
            if (entity != nullptr)
            {
               ComponentType* foundComponent = nullptr;
               foundComponent = entity->GetComponent<ComponentType>();

               if (foundComponent != nullptr)
               {
                  if ((onlyActivated && foundComponent->IsActivated()) || (!onlyActivated))
                  {
                     foundComponents.push_back(foundComponent);
                  }
               }
            }
         }

         return foundComponents;
      }

      bool LoadFrom(const String& filePath, bool bClearWorld = true);
      bool SaveTo(const String& filePath);
      bool Save();

      bool IsSaved() const { return (m_loadedData != nullptr); }

      /**
       * @breif   ���忡 �����Ǿ� �ִ� ��� Entity�� �����մϴ�.
       */
      void Clear();

      String GetWorldName() const { return m_name; }

   private:
      String m_name;
      std::vector<Entity*> m_entities;
      PlainText<std::string>* m_loadedData;

   };
}
