#pragma once
#include "Core/CoreMinimal.h"

namespace Mile
{
   class Context;
   /**
    * @brief	������ �����ϴ� �⺻���� �ý���, ��ɵ��� ǥ���ϴ� Ŭ�����Դϴ�.
    */
   class MEAPI SubSystem
   {
   public:
      SubSystem(Context* context);
      virtual ~SubSystem();

      virtual bool Init() { return (m_context != nullptr) && !m_bIsInitialized; }
      virtual void Update() { }
      virtual void DeInit() { m_bIsInitialized = false; }

      virtual void SaveConfig() { }
      virtual void LoadConfig() { }

      Context* GetContext() const { return m_context; }
      bool IsInitialized() const { return m_bIsInitialized; }

   protected:
      void InitSucceed() { m_bIsInitialized = true; }

   private:
      Context* m_context;
      bool     m_bIsInitialized;

   };
}