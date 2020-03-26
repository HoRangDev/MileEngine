#include "Rendering/LightingPass.h"
#include "Rendering/GBuffer.h"
#include "Rendering/ConstantBufferDX11.h"
#include "Rendering/PixelShaderDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/BlendState.h"

namespace Mile
{
   LightingPass::LightingPass(class RendererDX11* renderer) :
      m_gBuffer(nullptr),
      m_cameraParamsBuffer(nullptr),
      m_lightParamsBuffer(nullptr),
      m_additiveBlendState(nullptr),
      RenderingPass(renderer)
   {
   }

   LightingPass::~LightingPass()
   {
      SafeDelete(m_cameraParamsBuffer);
      SafeDelete(m_lightParamsBuffer);
   }

   bool LightingPass::Init(const String& shaderPath)
   {
      if (!RenderingPass::Init(shaderPath))
      {
         return false;
      }

      m_cameraParamsBuffer = new ConstantBufferDX11(m_renderer);
      if (!m_cameraParamsBuffer->Init(sizeof(CameraParamsConstantBuffer)))
      {
         return false;
      }

      m_lightParamsBuffer = new ConstantBufferDX11(m_renderer);
      if (!m_lightParamsBuffer->Init(sizeof(LightParamsConstantBuffer)))
      {
         return false;
      }

      m_additiveBlendState = new BlendState(m_renderer);
      m_additiveBlendState->SetRenderTargetBlendState(
         {
            true,
            EBlend::ONE, EBlend::ONE, EBlendOP::ADD,
            EBlend::ONE, EBlend::ZERO, EBlendOP::ADD,
            (UINT8)EColorWriteEnable::ColorWriteEnalbeAll});

      if (!m_additiveBlendState->Init())
      {
         return false;
      }

      m_pixelShader->AddSampler(
         D3D11_FILTER_ANISOTROPIC,
         D3D11_TEXTURE_ADDRESS_BORDER,
         D3D11_COMPARISON_ALWAYS);

      return true;
   }

   bool LightingPass::Bind(ID3D11DeviceContext& deviceContext)
   {
      bool bIsNotReadyToBind = !RenderingPass::Bind(deviceContext) ||
         m_cameraParamsBuffer == nullptr ||
         m_lightParamsBuffer == nullptr ||
         m_gBuffer == nullptr ||
         m_additiveBlendState == nullptr;
      if (bIsNotReadyToBind)
      {
         return false;
      }

      if (!m_gBuffer->BindAsShaderResource(deviceContext, 0))
      {
         return false;
      }

      if (!m_cameraParamsBuffer->Bind(deviceContext, 0, EShaderType::PixelShader))
      {
         return false;
      }

      if (!m_cameraParamsBuffer->Bind(deviceContext, 1, EShaderType::PixelShader))
      {
         return false;
      }

      if (!m_additiveBlendState->Bind(deviceContext))
      {
         return false;
      }

      return true;
   }

   bool LightingPass::Unbind(ID3D11DeviceContext& deviceContext)
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

      return true;
   }

   void LightingPass::SetGBuffer(GBuffer* gBuffer)
   {
      m_gBuffer = gBuffer;
   }

   void LightingPass::UpdateCameraParamsBuffer(ID3D11DeviceContext& deviceContext, CameraParamsConstantBuffer buffer)
   {
      if (m_cameraParamsBuffer != nullptr)
      {
         auto mappedBuffer = reinterpret_cast<CameraParamsConstantBuffer*>(m_cameraParamsBuffer->Map(deviceContext));
         (*mappedBuffer) = buffer;
         m_cameraParamsBuffer->UnMap(deviceContext);
      }
   }

   void LightingPass::UpdateLightParamsBuffer(ID3D11DeviceContext& deviceContext, LightParamsConstantBuffer buffer)
   {
      if (m_lightParamsBuffer != nullptr)
      {
         auto mappedBuffer = reinterpret_cast<LightParamsConstantBuffer*>(m_lightParamsBuffer->Map(deviceContext));
         (*mappedBuffer) = buffer;
         m_lightParamsBuffer->UnMap(deviceContext);
      }
   }
}
