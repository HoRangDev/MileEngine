#include "Rendering/DynamicCubemap.h"
#include "Rendering/DepthStencilBufferDX11.h"
#include "Rendering/RendererDX11.h"
#include "Core/Helper.h"

namespace Mile
{
   DynamicCubemap::DynamicCubemap(RendererDX11* renderer) :
      m_rtvs({ nullptr, }),
      m_depthStencil(nullptr),
      Texture2DBaseDX11(renderer)
   {
   }

   DynamicCubemap::~DynamicCubemap()
   {
      for (unsigned int idx = 0; idx < 6; ++idx)
      {
         SafeRelease(m_rtvs[idx]);
      }
      SafeDelete(m_depthStencil);
   }
   
   bool DynamicCubemap::Init(unsigned int size)
   {
      if (RenderObject::IsInitializable())
      {
         RendererDX11* renderer = GetRenderer();
         auto device = renderer->GetDevice();
         D3D11_TEXTURE2D_DESC desc;
         ZeroMemory(&desc, sizeof(desc));
         desc.Width = size;
         desc.Height = size;
         desc.MipLevels = 0;
         desc.ArraySize = 6;
         desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
         desc.Usage = D3D11_USAGE_DEFAULT;
         desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
         desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
         desc.CPUAccessFlags = 0;
         desc.SampleDesc.Count = 1;
         desc.SampleDesc.Quality = 0;

         auto result = device->CreateTexture2D(&desc, nullptr, &m_texture);
         if (!FAILED(result))
         {
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
            ZeroMemory(&rtvDesc, sizeof(rtvDesc));
            rtvDesc.Format = desc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            rtvDesc.Texture2DArray.ArraySize = 1;
            rtvDesc.Texture2DArray.MipSlice = 0;

            for (unsigned int idx = 0; idx < 6; ++idx)
            {
               rtvDesc.Texture2DArray.FirstArraySlice = idx;
               result = device->CreateRenderTargetView(m_texture, &rtvDesc, &m_rtvs[idx]);
               if (FAILED(result))
               {
                  return false;
               }
            }

            if (!FAILED(result))
            {
               if (InitSRV(desc))
               {
                  m_depthStencil = new DepthStencilBufferDX11(renderer);
                  if (m_depthStencil->Init(size, size, false))
                  {
                     RenderObject::ConfirmInit();
                     return true;
                  }
               }
            }
         }
      }

      return false;
   }

   bool DynamicCubemap::BindAsRenderTarget(ID3D11DeviceContext& deviceContext, unsigned int faceIdx, bool clearRenderTarget, bool clearDepth)
   {
      /*
      * @todo  ���� Ÿ�� Ŭ���� �̿�/���� Ÿ�� Ŭ���� ��� Ȯ��(�������̽� ����) : �Ʒ� ���� ������ �ߺ���
      **/
      if (RenderObject::IsBindable() && !IsBoundAsShaderResource())
      {
         if (faceIdx < 6)
         {
            ID3D11DepthStencilView* dsv = m_depthStencil->GetDSV();
            if (clearRenderTarget)
            {
               float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
               deviceContext.ClearRenderTargetView(m_rtvs[faceIdx], clearColor);
            }
            if (clearDepth)
            {
               if (dsv != nullptr)
               {
                  deviceContext.ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
               }
            }

            deviceContext.OMSetRenderTargets(1, &m_rtvs[faceIdx], dsv);
            return true;
         }
      }

      return false;
   }

   void DynamicCubemap::UnbindAsRenderTarget(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         deviceContext.OMSetRenderTargets(0, nullptr, nullptr);
      }
   }
}