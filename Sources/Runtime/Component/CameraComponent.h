#pragma once
#include "Component/ComponentRegister.h"
#include "Math/Vector4.h"

namespace Mile
{
   class RenderTexture;
   /**
    * @brief	ī�޶��� �Ӽ����� ������ �ִ� ������Ʈ �Դϴ�.
    *			�⺻������ FOV ��, ������� �Ÿ�, ������ ���۸� �ʱ�ȭ ��ų ������ �Ӽ����� ������ �ֽ��ϴ�.
    */
   class MEAPI CameraComponent : public Component
   {
      DeclareComponent(CameraComponent);

   public:
      CameraComponent(Entity* entity);

      /**
       * @brief ī�޶��� field of view(FOV)���� �����ɴϴ�.
       */
      float GetFov() const { return m_fov; }
      /**
       * @brief ī�޶��� field of view(FOV)���� �����մϴ�.
       * @param FOV �� (Degrees)
       */
      void SetFov(float fov) { m_fov = fov; }

      /**
      * @brief	ī�޶󿡼� ����� ������ �Ÿ��� �����ɴϴ�.
      */
      float GetNearPlane() const { return m_nearPlane; }
      /**
       * @brief	ī�޶󿡼� ���� ����� ������ �Ÿ��� �����մϴ�.
       * @param	�Ÿ� ��
       */
      void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }

      /**
       * @brief ī�޶󿡼� ���� �� ������ �Ÿ��� �����ɴϴ�.
       */
      float GetFarPlane() const { return m_farPlane; }
      /**
       * @brief ī�޶󿡼� ���� �� ������ �Ÿ��� �����մϴ�.
       * @param �Ÿ� ��
       */
      void SetFarPlane(float farPlane) { m_farPlane = farPlane; }

      /**
       * @brief �������� ������ ������ ���۸� ���� ���� ���� �����ɴϴ�.
       */
      Vector4 GetClearColor() const { return m_clearColor; }
      /**
      * @brief �������� ������ ������ ���۸� ���� ���� ���� �����ɴϴ�.
      * @param ���� ���� ����
      */
      void SetClearColor(const Vector4& clearColor) { m_clearColor = clearColor; }

      void SetRenderTexture(RenderTexture* renderTexture) { m_renderTexture = renderTexture; }
      RenderTexture* GetRenderTexture() const { return m_renderTexture; }

      virtual json Serialize() const override;
      virtual void DeSerialize(const json& jsonData) override;

      float Aperture() const { return m_aperture; }
      float& Aperture() { return m_aperture; }

      float ShutterSpeed() const { return m_shutterSpeed; }
      float& ShutterSpeed() { return m_shutterSpeed; }

      float Sensitivity() const { return m_sensitivity; }
      float& Sensitivity() { return m_sensitivity; }

      void OnGUI() override;

   private:
      float m_fov;
      float m_nearPlane;
      float m_farPlane;

      float m_aperture;
      float m_shutterSpeed;
      float m_sensitivity;

      Vector4 m_clearColor;

      RenderTexture* m_renderTexture;

   };
}