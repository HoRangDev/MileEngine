#include "Component/LightComponent.h"
#include "GameFramework/Entity.h"
#include "GameFramework/Transform.h"

namespace Mile
{
   RegisterComponent(LightComponent);

   LightComponent::LightComponent(Entity* entity) :
      m_type(ELightType::Directional),
      m_radiance(Vector3(1.0f, 1.0f, 1.0f)),
      Component(entity)
   {
      m_bCanEverUpdate = false;
   }

   Vector3 LightComponent::GetLightDirection() const
   {
      auto transform = m_entity->GetTransform();
      return transform->GetForward();
   }

   Vector3 LightComponent::GetLightPosition() const
   {
      auto transform = m_entity->GetTransform();
      return transform->GetPosition(TransformSpace::World);
   }

   json LightComponent::Serialize() const
   {
      json serialized = Component::Serialize();
      serialized["LightType"] = LightTypeToString(m_type);
      serialized["Radiance"] = m_radiance.Serialize();
      return serialized;
   }

   void LightComponent::DeSerialize(const json& jsonData)
   {
      Component::DeSerialize(jsonData);
      m_type = StringToLightType(GetValueSafelyFromJson<std::string>(jsonData, "LightType", ""));
      m_radiance.DeSerialize(jsonData["Radiance"]);
   }
}