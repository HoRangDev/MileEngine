#pragma once
#include "Core/Logger.h"

namespace Mile
{
   DECLARE_LOG_CATEGORY_EXTERN(MileDelegate, Log);

   /**
   * @todo const function �� �����ϴ� delegate �߰�
   */
   template<typename RetType, typename... Params>
   class MEAPI Delegate
   {
   public:
      using FuncType = std::function<RetType(Params...)>;

   public:
      Delegate() :
         m_bIsBound(false)
      {
      }

      ~Delegate() = default;

      void BindLambda(const FuncType& lambda)
      {
         m_callback = lambda;
         m_bIsBound = true;
      }

      template <typename T>
      void Bind(RetType (T::*func)(Params...), T* ptr)
      {
         BindLambda([ptr, func](Params... args) {return (ptr->*func)(args...); });
      }

      void Unbind()
      {
         if (m_bIsBound)
         {
            m_callback = FuncType();
            m_bIsBound = false;
         }
      }

      RetType Execute(Params... args)
      {
         return m_callback(args...);
      }

      RetType ExecuteIfBound(Params... args)
      {
         if (IsBound())
         {
            return Execute(args...);
         }

         return RetType();
      }

      bool IsBound() const { return m_bIsBound; }

   private:
      FuncType m_callback;
      bool m_bIsBound;

   };

   /**
    * ��� Delegate ���� �ڽ��� ������ Ŭ����/������Ʈ���� �޸� ���� å���� �ְ�,
    * �޸� ���� ���� MulticastDelegate �� ���(Add)�� Delegate �ڵ��� Remove��
    * �������־�� �մϴ�.
    */
   template<typename... Params>
   class MEAPI MulticastDelegate
   {
   public:
      using FuncType = std::function<void(Params...)>;
      using DelegateHandle = Delegate<void, Params...>;

   public:
      void Add(DelegateHandle* target)
      {
         if (!HasDelegate(target))
         {
            m_delegates.push_back(target);
         }
         else
         {
            ME_LOG(MileDelegate, Warning, TEXT("Trying duplicated addition to MulticastDelegate."));
         }
      }

      void Remove(DelegateHandle* target)
      {
         for (auto itr = m_delegates.begin(); itr != m_delegates.end(); ++itr)
         {
            if ((*itr) == target)
            {
               m_delegates.erase(itr);
               return;
            }
         }
      }

      void Broadcast(Params... args)
      {
         for (auto delegateHandle : m_delegates)
         {
            delegateHandle->ExecuteIfBound(args...);
         }
      }

      bool HasDelegate(DelegateHandle* target) const
      {
         for (auto delegateHandle : m_delegates)
         {
            if (target == delegateHandle)
            {
               return true;
            }
         }

         return false;
      }

   private:
      std::vector<DelegateHandle*> m_delegates;

   };
}

#define DECLARE_DELEGATE_RetVal_Params(DelegateName, RetVal, ...) \
class MEAPI DelegateName##Delegate : public Mile::Delegate<RetVal, __VA_ARGS__> \
{ \
}
#define DECLARE_DELEGATE_Params(DelegateName, ...) DECLARE_DELEGATE_RetVal_Params(DelegateName, void, __VA_ARGS__)
#define DECLARE_DELEGATE(DelegateName) DECLARE_DELEGATE_RetVal_Params(DelegateName, void)

#define DECLARE_MULTICAST_DELEGATE(DelegateName) DECLARE_MULTICAST_DELEGATE_Params(DelegateName)
#define DECLARE_MULTICAST_DELEGATE_Params(DelegateName, ...) \
DECLARE_DELEGATE_Params(DelegateName, __VA_ARGS__); \
class MEAPI DelegateName##MulticastDelegate : public Mile::MulticastDelegate<__VA_ARGS__> \
{ \
}