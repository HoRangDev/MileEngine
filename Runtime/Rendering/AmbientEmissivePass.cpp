#include "Rendering/AmbientEmissivePass.h"
#include "Rendering/GBuffer.h"
#include "Rendering/ConstantBufferDX11.h"
#include "Rendering/PixelShaderDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/DynamicCubemap.h"
#include "Rendering/BlendState.h"
#include "Rendering/RenderTargetDX11.h"

namespace Mile
{
   AmbientEmissivePass::AmbientEmissivePass(class RendererDX11* renderer) :
      m_gBuffer(nullptr),
      m_irradianceMap(nullptr),
      m_prefilteredMap(nullptr),
      m_brdfLUT(nullptr),
      m_ambientParamsBuffer(nullptr),
      RenderingPass(renderer)
   {
   }

   AmbientEmissivePass::~AmbientEmissivePass()
   {
      SafeDelete(m_ambientParamsBuffer);
   }

   bool AmbientEmissivePass::Init()
   {
      if (!RenderingPass::Init(TEXT("Contents/Shaders/AmbientEmissivePass.hlsl")))
      {
         return false;
      }

      RendererDX11* renderer = GetRenderer();
      m_ambientParamsBuffer = new ConstantBufferDX11(renderer);
      if (!m_ambientParamsBuffer->Init(sizeof(AmbientParamsConstantBuffer)))
      {
         return false;
      }

      PixelShaderDX11* pixelShader = GetPixelShader();
      pixelShader->AddSampler(
         D3D11_FILTER_ANISOTROPIC,
         D3D11_TEXTURE_ADDRESS_WRAP,
         D3D11_COMPARISON_ALWAYS);
      pixelShader->AddSampler(
         D3D11_FILTER_MIN_MAG_MIP_LINEAR,
         D3D11_TEXTURE_ADDRESS_WRAP,
         D3D11_COMPARISON_ALWAYS);

      RenderObject::ConfirmInit();
      return true;
   }

   bool AmbientEmissivePass::Bind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderingPass::Bind(deviceContext))
      {
         bool bSuccess =
            m_gBuffer->BindAsShaderResource(deviceContext, 0) &&
            m_irradianceMap->Bind(deviceContext, 5, EShaderType::PixelShader) &&
            m_prefilteredMap->Bind(deviceContext, 6, EShaderType::PixelShader) &&
            m_brdfLUT->BindAsShaderResource(deviceContext, 7, EShaderType::PixelShader) &&
            m_ambientParamsBuffer->Bind(deviceContext, 0, EShaderType::PixelShader);
         if (bSuccess)
         {
            return true;
         }
      }

      Unbind(deviceContext);
      return false;
   }

   void AmbientEmissivePass::Unbind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         if (m_gBuffer != nullptr)
         {
            m_gBuffer->UnbindShaderResource(deviceContext);
         }

         if (m_ambientParamsBuffer != nullptr)
         {
            m_ambientParamsBuffer->Unbind(deviceContext);
         }

         if (m_irradianceMap != nullptr)
         {
            m_irradianceMap->Unbind(deviceContext);
         }

         if (m_prefilteredMap != nullptr)
         {
            m_prefilteredMap->Unbind(deviceContext);
         }

         if (m_brdfLUT != nullptr)
         {
            m_brdfLUT->UnbindShaderResource(deviceContext);
         }

         RenderingPass::Unbind(deviceContext);
      }
   }

   void AmbientEmissivePass::SetGBuffer(GBuffer* gBuffer)
   {
      m_gBuffer = gBuffer;
   }

   void AmbientEmissivePass::SetIrradianceMap(DynamicCubemap* irradianceMap)
   {
      m_irradianceMap = irradianceMap;
   }

   void AmbientEmissivePass::SetPrefilteredMap(DynamicCubemap* prefilteredEnvMap)
   {
      m_prefilteredMap = prefilteredEnvMap;
   }

   void AmbientEmissivePass::SetBRDFLUT(RenderTargetDX11* brdfLUT)
   {
      m_brdfLUT = brdfLUT;
   }

   void AmbientEmissivePass::UpdateAmbientParamsBuffer(ID3D11DeviceContext& deviceContext, AmbientParamsConstantBuffer buffer)
   {
      if (m_ambientParamsBuffer != nullptr)
      {
         m_ambientParamsBuffer->Update(deviceContext, buffer);
      }
   }
}
