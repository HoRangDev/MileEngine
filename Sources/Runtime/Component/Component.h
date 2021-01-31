#pragma once
#include "Core/CoreMinimal.h"

namespace Mile
{
   class Entity;
   class Context;
   class Transform;
   class MEAPI Component
   {
   public:
      Component(Entity* entity);
      virtual ~Component() { }

      template <typename Ty,
         std::enable_if_t<std::is_base_of_v<Component, Ty>, bool> = true>
      static Ty* Create(Entity* entity)
      {
         return new Ty(entity);
      }

      //virtual std::string Serialize( ) const { return ( "\"IsActivated\": " + Mile::BoolSerialize( m_bIsActive )); }
      virtual json Serialize() const { json obj; obj["IsActivated"] = m_bIsActivated; obj["Type"] = WString2String(GetType()); return obj; }
      virtual void DeSerialize(const json& jsonData)
      {
         m_bIsActivated = jsonData["IsActivated"];
      }

      /**
       * @brief   ������Ʈ�� Ȱ��ȭ �Ǿ��ִ��� üũ�մϴ�. 
       */
      bool IsActivated() const;

      /**
       * @brief   ������Ʈ�� Ȱ��ȭ ���θ� �����մϴ�. 
       */
      void SetActive(bool bIsActive);

      virtual void Reset() { }
      virtual void Start() { }
      virtual void Update() { OPTICK_EVENT(); }
      virtual void OnEnable() { }
      virtual void OnDisable() { }

      void OnGUIBegin();
      virtual void OnGUI() { }
      void OnGUIEnd();

      virtual String GetType() const { return TEXT("Component"); }

      Entity* GetEntity() const { return m_entity; }
      Context* GetContext() const;
      Transform* GetTransform() const;

      bool IsUpdateEnabled() const { return m_bCanEverUpdate; }

   protected:
      Component() : Component(nullptr)
      {
      }

   protected:
      Entity* m_entity;
      bool    m_bIsActivated;
      bool    m_bCanEverUpdate;

   };
}