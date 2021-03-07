#include "Rendering/RenderTargetDX11.h"
#include "Rendering/DepthStencilBufferDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/RendererDX11.h"

namespace Mile
{
   RenderTargetDX11::RenderTargetDX11(RendererDX11* renderer) :
      m_depthStencilBuffer(nullptr),
      m_texture(nullptr),
      m_rtv(nullptr),
      m_width(0),
      m_height(0),
      m_format(EColorFormat::R8G8B8A8_UNORM),
      RenderObject(renderer)
   {
   }

   RenderTargetDX11::~RenderTargetDX11()
   {
      DeInit();
   }

   bool RenderTargetDX11::Init(unsigned int width, unsigned int height, EColorFormat format, DepthStencilBufferDX11* depthStencilBuffer)
   {
      bool bIsValidParams = (width > 0 && height > 0);
      if (RenderObject::IsInitializable() && bIsValidParams)
      {
         m_width = width;
         m_height = height;
         m_format = format;

         D3D11_TEXTURE2D_DESC texDesc;
         ZeroMemory(&texDesc, sizeof(texDesc));
         texDesc.Width = width;
         texDesc.Height = height;
         texDesc.MipLevels = 1;
         texDesc.ArraySize = 1;
         texDesc.Format = static_cast<DXGI_FORMAT>(format);
         texDesc.SampleDesc.Count = 1;
         texDesc.SampleDesc.Quality = 0;
         texDesc.Usage = D3D11_USAGE_DEFAULT;
         texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
         texDesc.CPUAccessFlags = 0;
         texDesc.MiscFlags = 0;

         RendererDX11* renderer = GetRenderer();
         ID3D11Texture2D* texture = nullptr;
         auto& device = renderer->GetDevice();
         auto result = device.CreateTexture2D(
            &texDesc,
            nullptr,
            &texture);
         if (!FAILED(result))
         {
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
            ZeroMemory(&rtvDesc, sizeof(rtvDesc));
            rtvDesc.Format = static_cast<DXGI_FORMAT>(format);
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;

            result = device.CreateRenderTargetView(
               texture,
               &rtvDesc,
               &m_rtv);
            if (!FAILED(result))
            {
               m_texture = new Texture2dDX11(renderer);
               if (m_texture->Init(texture))
               {
                  m_depthStencilBuffer = depthStencilBuffer;
                  RenderObject::ConfirmInit();
                  return true;
               }
               else
               {
                  SafeDelete(m_texture);
               }
            }
            else
            {
               /* Faeild to Create RenderTargetView from D3D device. **/
            }
         }
         else
         {
            /* Failed to Create Texture2D from D3D Device. **/
         }
      }

      return false;
   }

   bool RenderTargetDX11::Init(ID3D11RenderTargetView* rtv, DepthStencilBufferDX11* depthStencilBuffer)
   {
      if (RenderObject::IsInitializable())
      {
         m_rtv = rtv;
         ID3D11Resource* rtvResource = nullptr;
         m_rtv->GetResource(&rtvResource);
         if (rtvResource != nullptr)
         {
            ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(rtvResource);
            D3D11_TEXTURE2D_DESC desc{ };
            texture->GetDesc(&desc);
            m_width = desc.Width;
            m_height = desc.Height;
            m_format = static_cast<EColorFormat>(desc.Format);

            m_depthStencilBuffer = depthStencilBuffer;

            SafeRelease(texture);
            RenderObject::ConfirmInit();
            return true;
         }
         else
         {
            m_rtv = nullptr;
         }
      }

      return false;
   }

   void RenderTargetDX11::DeInit()
   {
      if (IsInitialized())
      {
         SafeRelease(m_rtv);
         SafeDelete(m_texture);
         m_depthStencilBuffer = nullptr;
         RenderObject::ConfirmDeInit();
      }
   }

   bool RenderTargetDX11::BindRenderTargetView(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         ID3D11DepthStencilView* dsv = nullptr;
         if (m_depthStencilBuffer != nullptr)
         {
            dsv = m_depthStencilBuffer->GetDSV();
         }

         deviceContext.OMSetRenderTargets(1, &m_rtv, dsv);
         return true;
      }

      return false;
   }

   bool RenderTargetDX11::BindShaderResourceView(ID3D11DeviceContext& deviceContext, unsigned int bindSlot, EShaderType bindShader)
   {
      if (RenderObject::IsBindable())
      {
         return m_texture->BindShaderResourceView(deviceContext, bindSlot, bindShader);
      }

      return false;
   }

   void RenderTargetDX11::UnbindRenderTargetView(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         ID3D11RenderTargetView* nullRTV = nullptr;
         deviceContext.OMSetRenderTargets(1, &nullRTV, nullptr);
      }
   }

   void RenderTargetDX11::UnbindShaderResourceView(ID3D11DeviceContext& deviceContext, unsigned int boundSlot, EShaderType boundShader)
   {
      if (RenderObject::IsBindable())
      {
         m_texture->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
      }
   }

   void RenderTargetDX11::Clear(ID3D11DeviceContext& deviceContext, const Vector4& clearColor)
   {
      deviceContext.ClearRenderTargetView(m_rtv, clearColor.elements);
   }
}
