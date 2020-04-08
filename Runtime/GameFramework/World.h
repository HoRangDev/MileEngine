#pragma once
#include "Core/SubSystem.h"
#include "Component/Component.h"

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
      std::vector<ComponentType*> GetComponentsFromEntities(bool onlyActivatedOwner = false, bool onlyActivated = false) const
      {
         std::vector<ComponentType*> foundComponents;
         for (Entity* entity : m_entities)
         {
            if (entity != nullptr)
            {
               ComponentType* foundComponent = nullptr;
               if ((onlyActivatedOwner && entity->IsActivated()) || (!onlyActivatedOwner))
               {
                  foundComponent = entity->GetComponent<ComponentType>();
                  if (onlyActivated && !foundComponent->IsActivated())
                  {
                     foundComponent = nullptr;
                  }
               }
               /**
               *  ���忡���� �ڽ� entity/�θ� entity ������� ����� ������ �� �ִ� ��� entity���� ������ �ֱ⶧����.
               *  ������ entity�� ���ؼ� ���������� GetComponent�� �̿��ؼ� ������Ʈ�� �����´�.
               */
               if (foundComponent != nullptr)
               {
                  foundComponents.push_back(foundComponent);
               }
            }
         }

         return foundComponents;
      }

      bool LoadFrom(const String& filePath);
      bool SaveTo(const String& filePath);
      bool Save();

   private:
      std::vector<Entity*> m_entities;
      PlainText<std::string>* m_loadedData;

   };
}
