#pragma once

#include "Core/Helper.h"

namespace Mile
{
   class Context;
   class MEAPI Layer
   {
   public:
      Layer(Context* context, const String& name = TEXT("Layer"));
      virtual ~Layer() { }

      virtual void OnAttach() { }
      virtual void OnDetach() { }
      virtual void OnUpdate() { }
      virtual void OnIMGUIRender() { }

      String GetName() const { return m_name; }

      Context* GetContext() const { return m_context; }

   protected:
      Context* m_context;
      String m_name;

   };
}