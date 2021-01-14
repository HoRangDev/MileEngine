#pragma once
#include "Core/Layer.h"

namespace Mile
{
   class World;
   class Entity;
   namespace Editor
   {
      class SceneHierarchyLayer : public Layer
      {
         public:
            SceneHierarchyLayer(Context* context);
            virtual void OnIMGUIRender();

            void SetTargetWorld(World* world) { m_target = world; }

      private:
            void ConstructTree(Entity* targetRoot);

      private:
         World* m_target;

      };
   }
}