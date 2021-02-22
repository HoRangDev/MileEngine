#pragma once
#include "Component/ComponentRegister.h"
#include "Math/MathMinimal.h"
#include "Rendering/Light.h"

namespace Mile
{
   /**
   * @brief ����Ʈ ������Ʈ�� ���Ǵ� Ÿ�Կ� ���� ���� ���⸦ ǥ���ϴµ� ������ ������ ������
   * @Directional Light Lux(lx, lm/m^2)
   * @Point Light/Spot Light  Lumen(lm)
   */
   class MEAPI LightComponent : public Component
   {
      DeclareComponent(LightComponent);

   public:
      LightComponent(Entity* entity);

      ELightType GetLightType() const { return m_type; }
      void SetLightType(ELightType type) { m_type = type; }

      Vector3 GetColor() const { return m_color; }
      void SetColor(const Vector3& newColor);

      float GetIntensity() const { return m_intensity; }
      float& Intensity() { return m_intensity; }

      float GetLuminousIntensity() const
      {
         switch (m_type)
         {
         case ELightType::Directional:
            return m_intensity;
         case ELightType::Point:
            return m_intensity / (4.0f * Math::Pi);
         case ELightType::Spot:
            return (m_intensity / Math::Pi);
         }

         return 0.0f;
      }

      Vector3 GetLightDirection() const;
      Vector3 GetLightPosition() const;

      float GetRadius() const { return m_radius; }
      float& Radius() { return m_radius; }

      float GetInnerAngle() const { return m_innerAngle; }
      float GetInnerAngleAsRadians() const { return Math::DegreeToRadian(m_innerAngle); }
      void SetInnerAngle(float degree)
      {
         m_innerAngle = std::clamp(degree, 0.0f, m_outerAngle);
      }

      float GetOuterAngle() const { return m_outerAngle; }
      float GetOuterAngleAsRadians() const { return Math::DegreeToRadian(m_outerAngle); }
      void SetOuterAngle(float degree)
      {
         m_outerAngle = std::clamp(degree, 0.0f, 180.0f);
      }

      virtual json Serialize() const override;
      virtual void DeSerialize(const json& jsonData) override;

      void OnGUI() override;

   private:
      ELightType	m_type;
      Vector3	   m_color;
      float       m_intensity;
      float       m_radius;
      float       m_innerAngle;
      float       m_outerAngle;

   };
}
