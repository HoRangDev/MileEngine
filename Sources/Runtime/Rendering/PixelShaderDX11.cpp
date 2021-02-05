#include "Rendering/PixelShaderDX11.h"
#include "Rendering/SamplerDX11.h"

namespace Mile
{
   PixelShaderDX11::PixelShaderDX11(RendererDX11* renderer) :
      m_shader(nullptr),
      ShaderDX11(renderer)
   {
   }

   PixelShaderDX11::~PixelShaderDX11()
   {
      for (auto sampler : m_samplers)
      {
         SafeDelete(sampler);
      }

      SafeRelease(m_shader);
   }

   bool PixelShaderDX11::Init(const String& shaderPath)
   {
      if (RenderObject::IsInitializable())
      {
         if (Compile(shaderPath, EShaderType::PixelShader))
         {
            RendererDX11* renderer = GetRenderer();
            auto& device = renderer->GetDevice();
            auto result = device.CreatePixelShader(
               m_blob->GetBufferPointer(),
               m_blob->GetBufferSize(),
               nullptr,
               &m_shader);

            if (!FAILED(result))
            {
               RenderObject::ConfirmInit();
               return true;
            }
         }
      }

      return false;
   }

   bool PixelShaderDX11::Bind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         deviceContext.PSSetShader(m_shader,
            nullptr,
            0);

         for (int idx = 0; idx < m_samplers.size(); ++idx)
         {
            if (!m_samplers[idx]->Bind(deviceContext, idx))
            {
               return false;
            }
         }

         return true;
      }

      return false;
   }

   void PixelShaderDX11::Unbind(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         for (auto sampler : m_samplers)
         {
            sampler->Unbind(deviceContext);
         }

         deviceContext.PSSetShader(nullptr, nullptr, 0);
      }
   }

   bool PixelShaderDX11::AddSampler(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE AddressModeU, D3D11_TEXTURE_ADDRESS_MODE AddressModeV, D3D11_TEXTURE_ADDRESS_MODE AddressModeW, D3D11_COMPARISON_FUNC compFunc)
   {
      RendererDX11* renderer = GetRenderer();
      SamplerDX11* sampler = new SamplerDX11(renderer);
      if (!sampler->Init(filter, AddressModeU, AddressModeV, AddressModeW, compFunc))
      {
         SafeDelete(sampler);
         return false;
      }

      this->m_samplers.push_back(sampler);
      return true;
   }

   bool PixelShaderDX11::AddSampler(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE AddressMode, D3D11_COMPARISON_FUNC compFunc)
   {
      return this->AddSampler(filter, AddressMode, AddressMode, AddressMode, compFunc);
   }
}