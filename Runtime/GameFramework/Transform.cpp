#include "GameFramework/Transform.h"
#include "GameFramework/Entity.h"

namespace Mile
{
   Transform* Transform::GetParent() const
   {
      Entity* parent = m_entity->GetParent();
      Transform* parentTrasnfrom = nullptr;
      if (parent != nullptr)
      {
         parentTrasnfrom = parent->GetTransform();
      }

      return parentTrasnfrom;
   }

   Vector3 Transform::GetForward(TransformSpace space) const
   {
      Vector3 res = GetRotation(space).Rotated(Vector3::Forward());
      res.Normalize();
      return res;
   }

   Vector3 Transform::GetUp(TransformSpace space) const
   {
      Vector3 res = GetRotation(space).Rotated(Vector3::Up());
      res.Normalize();
      return res;
   }
}