#include "Rendering/ToneMappingPass.h"
#include "Rendering/RenderTargetDX11.h"
#include "Rendering/PixelShaderDX11.h"
#include "Rendering/ConstantBufferDX11.h"

namespace Mile
{
   ToneMappingPass::ToneMappingPass(RendererDX11* renderer) :
      m_boundHdrBuffer(nullptr),
      m_params(nullptr),
      RenderingPass(renderer)
   {
   }

   ToneMappingPass::~ToneMappingPass()
   {
      SafeDelete(m_params);
   }

   bool ToneMappingPass::Init()
   {
      if (RenderingPass::Init(TEXT("Contents/Shaders/ToneMapping.hlsl")))
      {
         RendererDX11* renderer = GetRenderer();

         m_params = new ConstantBufferDX11(renderer);
         if (!m_params->Init<PackedToneMappingParams>())
         {
            return false;
         }

         PixelShaderDX11* pixelShader = GetPixelShader();
         pixelShader->AddSampler(
            D3D11_FILTER_ANISOTROPIC,
            D3D11_TEXTURE_ADDRESS_CLAMP,
            D3D11_COMPARISON_ALWAYS);

         RenderObject::ConfirmInit();
         return true;
      }

      return false;
   }

   bool ToneMappingPass::Bind(ID3D11DeviceContext& deviceContext, RenderTargetDX11* hdrBuffer)
   {
      bool bIsValidParams = hdrBuffer != nullptr;
      if (bIsValidParams && RenderingPass::Bind(deviceContext))
      {
         bool bSuccess =
            hdrBuffer->BindAsShaderResource(deviceContext, 0, EShaderType::PixelShader) &&
            m_params->Bind(deviceContext, 0, EShaderType::PixelShader);
         if (bSuccess)
         {
            m_boundHdrBuffer = hdrBuffer;
            return true;
         }
      }

      Unbind(deviceContext);
      return false;
   }

   void ToneMappingPass::Unbind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderingPass::IsBindable())
      {
         m_boundHdrBuffer->UnbindShaderResource(deviceContext);
         m_params->Unbind(deviceContext);
         RenderingPass::Unbind(deviceContext);
      }
   }

   void ToneMappingPass::UpdateParameters(ID3D11DeviceContext& deviceContext, ToneMappingParams buffer)
   {
      if (m_params != nullptr)
      {
         m_params->Update(deviceContext, PackedToneMappingParams{ Vector2(buffer.ExposureFactor, buffer.GammaFactor) });
      }
   }
}

