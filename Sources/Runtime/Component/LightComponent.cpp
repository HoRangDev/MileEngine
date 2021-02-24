#include "Component/LightComponent.h"
#include "GameFramework/Entity.h"
#include "GameFramework/Transform.h"
#include "Core/ImGuiHelper.h"

namespace Mile
{
   DefineComponent(LightComponent);

   LightComponent::LightComponent(Entity* entity) :
      m_type(ELightType::Directional),
      m_color(Vector3(1.0f, 1.0f, 1.0f)),
      m_intensity(1.0f),
      m_radius(1.0f),
      m_outerAngle(15.0f),
      Component(entity)
   {
      m_bCanEverUpdate = false;
   }

   void LightComponent::SetColor(const Vector3& newColor)
   {
      m_color.x = std::clamp(newColor.x, 0.0f, 1.0f);
      m_color.y = std::clamp(newColor.y, 0.0f, 1.0f);
      m_color.z = std::clamp(newColor.z, 0.0f, 1.0f);
   }

   Vector3 LightComponent::GetLightDirection() const
   {
      auto transform = m_entity->GetTransform();
      return transform->GetForward();
   }

   Vector3 LightComponent::GetLightPosition() const
   {
      auto transform = m_entity->GetTransform();
      return transform->GetPosition(ETransformSpace::World);
   }

   json LightComponent::Serialize() const
   {
      json serialized = Component::Serialize();
      serialized["LightType"] = static_cast<UINT32>(m_type);
      serialized["Color"] = m_color.Serialize();
      serialized["Intensity"] = m_intensity;
      serialized["Radius"] = m_radius;
      serialized["InnerAngle"] = m_innerAngle;
      serialized["OuterAngle"] = m_outerAngle;
      return serialized;
   }

   void LightComponent::DeSerialize(const json& jsonData)
   {
      Component::DeSerialize(jsonData);
      m_type = static_cast<ELightType>(GetValueSafelyFromJson<UINT32>(jsonData, "LightType", 0));

      Vector3 tempColor;
      tempColor.DeSerialize(jsonData["Color"]);
      SetColor(tempColor);

      m_intensity = GetValueSafelyFromJson<float>(jsonData, "Intensity", 1.0f);
      m_radius = GetValueSafelyFromJson<float>(jsonData, "Radius", 1.0f);

      SetOuterAngle(GetValueSafelyFromJson<float>(jsonData, "OuterAngle", 15.0f));
      SetInnerAngle(GetValueSafelyFromJson<float>(jsonData, "InnerAngle", 15.0f));
   }

   void LightComponent::OnGUI()
   {
      std::string intensityInputLabel = "Light Intensity (";
      intensityInputLabel.append(LightIntensityUnitToString(LightIntensityUnitOf(m_type)));
      intensityInputLabel.append(")");

      unsigned int selectedType = static_cast<unsigned int>(m_type);
      std::vector<std::string> typeComboboxItems{ "Directional", "Point", "Spot" };
      std::string typeComboboxItem = typeComboboxItems[selectedType];
      GUI::Combobox("Light Type", typeComboboxItems, typeComboboxItem, selectedType);
      m_type = static_cast<ELightType>(selectedType);

      GUI::Vector3Input("Light Color", m_color, 0.01f, 0.0f, 1.0f);
      GUI::FloatInput(intensityInputLabel, m_intensity, 10.0f, 0.0f, 100000.0f, true);

      switch (m_type)
      {
      case Mile::ELightType::Spot:
         GUI::FloatInput("Outer Angle", m_outerAngle, 10.0f, 0.0f, 180.0f);
         GUI::FloatInput("Inner Angle", m_innerAngle, 10.0f, 0.0f, 180.0f);
      case Mile::ELightType::Point:
         GUI::FloatInput("Radius", m_radius, 1.0f, 0.0f, FLT_MAX);
         break;
      }
   }
}