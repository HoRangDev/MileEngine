#pragma once
#include "Rendering/RenderingPass.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Mile
{
   /* @TODO:
   ����Ʈ�� �ѹ��� shader ���� ó��
   vs
   ���� �� ������ ����Ʈ ���� ���� light buffer pass �� ��ġ�� �Ͽ� ����Ʈ�� ���� ���������� ��Ʈ�� �����ϰ� �ϱ�
   @comment Unreal Engine �� ��� light type �� ���� �ִ� ����Ʈ ���� �ٸ���(Static ���� dynamic ����..) => ����Ʈ�� baking �ϰ� �Ǹ� ����غ���
   */
   class GBuffer;
   class MEAPI LightBufferPass : public RenderingPass
   {
      struct LightParamConstantBuffer
      {
         alignas(16) Vector3 LightPos;
         alignas(16) Vector3 LightColor;
         alignas(16) Vector3 LightDirection;
         alignas(16) Vector2 SpotlightAngles;
         alignas(16) Vector3 LightRange;
         unsigned int LightType;
      };

      struct alignas(16) CameraConstantBuffer
      {
         Vector3 CameraPos;
      };

   public:
      LightBufferPass(RendererDX11* renderer);
      ~LightBufferPass();

      virtual bool Init(const String& filePath);
      virtual bool Bind(ID3D11DeviceContext& deviceContext) override;
      virtual void Unbind(ID3D11DeviceContext& deviceContext) override;

      void SetGBuffer(GBuffer* gBuffer);
      // CheckerBoard Setting does not change after decided at GBuffer-Pass
      void SetCheckerBoardBuffer(CBufferPtr checkerBoardBuffer) { m_checkerBoardBuffer = checkerBoardBuffer; }
      void SetLightBuffer(RenderTargetDX11* lightBuffer);

      void UpdateLightParamBuffer(ID3D11DeviceContext& deviceContext, 
         const Vector3& lightPos,
         const Vector3& lightColor,
         const Vector3& lightDirection,
         const Vector2& spotlightAngles,
         const Vector3& lightRange,
         unsigned int lightType);

      void UpdateCameraBuffer(ID3D11DeviceContext& deviceContext, const Vector3& camPos);

   private:
      /* Light Buffer pass Constant buffers */
      ConstantBufferDX11* m_lightParamBuffer;
      ConstantBufferDX11* m_cameraBuffer;

      /* External Constant Buffer*/
      ConstantBufferDX11* m_checkerBoardBuffer;

      GBuffer* m_gBuffer;
      RenderTargetDX11* m_lightBuffer;

   };
}