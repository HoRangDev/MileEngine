#include "Core/InputManager.h"
#include <utility>

namespace Mile
{
   DEFINE_LOG_CATEGORY(MileInputManager);

   InputManager::InputManager(Context* context) :
      SubSystem(context)
   {
   }

   InputManager::~InputManager()
   {
      DeInit();
   }

   bool InputManager::Init()
   {
      if (SubSystem::Init())
      {
         ME_LOG(MileInputManager, Log, TEXT("InputManager initialized."));
         SubSystem::InitSucceed();
         return true;
      }

      return false;
   }

   void InputManager::DeInit()
   {
      if (IsInitialized())
      {
         ME_LOG(MileInputManager, Log, TEXT("InputManager deinitialized."));
         SubSystem::DeInit();
      }
   }

   void InputManager::MapAction(EInputKey key, const String& actionName)
   {
      for (auto mappingInfo : m_actionMappings)
      {
         if (mappingInfo.second == actionName)
         {
            return;
         }
      }

      m_actionMappings.push_back(std::make_pair(key, actionName));
   }

   void InputManager::BindAction(const String& actionName, EInputEvent inputEvent, ActionCallback callback)
   {
      for (auto mappingInfo : m_actionMappings)
      {
         if (mappingInfo.second == actionName)
         {
            m_actionBindings.push_back(std::make_pair(actionName, std::make_pair(inputEvent, callback)));
         }
      }
   }

   void InputManager::HandleWin32(unsigned int msg, WPARAM wParam, LPARAM lParam)
   {
      //@TODO: Axis �Էµ� ó��
      BroadcastAction(ConvertWin32Key(wParam), ConvertWin32InputEvent(msg));
   }

   void InputManager::BroadcastAction(EInputKey targetKey, EInputEvent targetEvent)
   {
      for (auto mappingInfo : m_actionMappings)
      {
         if (targetKey == mappingInfo.first)
         {
            for (auto bindingInfo : m_actionBindings)
            {
               if (mappingInfo.second == bindingInfo.first)
               {
                  ActionInputEvent inputEvent = bindingInfo.second;
                  if (inputEvent.first == targetEvent)
                  {
                     inputEvent.second();
                  }
               }
            }
         }
      }
   }
}