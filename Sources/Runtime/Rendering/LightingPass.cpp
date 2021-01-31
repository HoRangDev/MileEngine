#include "Rendering/LightingPass.h"
#include "Rendering/GBuffer.h"
#include "Rendering/ConstantBufferDX11.h"
#include "Rendering/PixelShaderDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/BlendState.h"
#include "Rendering/RenderTargetDX11.h"

namespace Mile
{
   LightingPass::LightingPass(class RendererDX11* renderer) :
      m_gBuffer(nullptr),
      m_cameraParamsBuffer(nullptr),
      m_lightParamsBuffer(nullptr),
      RenderingPass(renderer)
   {
   }

   LightingPass::~LightingPass()
   {
      SafeDelete(m_cameraParamsBuffer);
      SafeDelete(m_lightParamsBuffer);
   }

   bool LightingPass::Init()
   {
      if (!RenderingPass::Init(TEXT("Contents/Shaders/LightingPass.hlsl")))
      {
         return false;
      }

      RendererDX11* renderer = GetRenderer();
      m_cameraParamsBuffer = new ConstantBufferDX11(renderer);
      if (!m_cameraParamsBuffer->Init(sizeof(CameraParamsConstantBuffer)))
      {
         return false;
      }

      m_lightParamsBuffer = new ConstantBufferDX11(renderer);
      if (!m_lightParamsBuffer->Init(sizeof(LightParamsConstantBuffer)))
      {
         return false;
      }

      PixelShaderDX11* pixelShader = GetPixelShader();
      pixelShader->AddSampler(
         D3D11_FILTER_ANISOTROPIC,
         D3D11_TEXTURE_ADDRESS_BORDER,
         D3D11_COMPARISON_ALWAYS);

      RenderObject::ConfirmInit();
      return true;
   }

   bool LightingPass::Bind(ID3D11DeviceContext& deviceContext)
   {
      bool bValidParams = m_gBuffer != nullptr;
      if (bValidParams && RenderingPass::Bind(deviceContext))
      {
         bool bSuccess =
            m_gBuffer->BindAsShaderResource(deviceContext, 0) &&
            m_cameraParamsBuffer->Bind(deviceContext, 0, EShaderType::PixelShader) &&
            m_lightParamsBuffer->Bind(deviceContext, 1, EShaderType::PixelShader);
         if (bSuccess)
         {
            return true;
         }
      }

      Unbind(deviceContext);
      return false;
   }

   void LightingPass::Unbind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         if (m_gBuffer != nullptr)
         {
            m_gBuffer->UnbindShaderResource(deviceContext);
         }

         if (m_cameraParamsBuffer != nullptr)
         {
            m_cameraParamsBuffer->Unbind(deviceContext);
         }

         if (m_lightParamsBuffer != nullptr)
         {
            m_lightParamsBuffer->Unbind(deviceContext);
         }

         RenderingPass::Unbind(deviceContext);
      }
   }

   void LightingPass::SetGBuffer(GBuffer* gBuffer)
   {
      m_gBuffer = gBuffer;
   }

   void LightingPass::UpdateCameraParamsBuffer(ID3D11DeviceContext& deviceContext, CameraParamsConstantBuffer buffer)
   {
      if (m_cameraParamsBuffer != nullptr)
      {
         m_cameraParamsBuffer->Update(deviceContext, buffer);
      }
   }

   void LightingPass::UpdateLightParamsBuffer(ID3D11DeviceContext& deviceContext, LightParamsConstantBuffer buffer)
   {
      if (m_lightParamsBuffer != nullptr)
      {
         m_lightParamsBuffer->Update(deviceContext, buffer);
      }
   }
}
