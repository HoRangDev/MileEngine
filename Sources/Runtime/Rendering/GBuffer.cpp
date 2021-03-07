#include "Rendering/GBuffer.h"
#include "Rendering/RenderTargetDX11.h"
#include "Rendering/DepthStencilBufferDX11.h"
#include "Rendering/Texture2dDX11.h"
#include "Rendering/BlendState.h"

constexpr unsigned int GBUFFER_RENDER_TARGET_NUM = 5;

namespace Mile
{
   DEFINE_LOG_CATEGORY(MileGBuffer);

   GBuffer::GBuffer(RendererDX11* renderer) :
      m_depthStencilBuffer(nullptr),
      m_positionBuffer(nullptr),
      m_albedoBuffer(nullptr),
      m_emissiveAOBuffer(nullptr),
      m_normalBuffer(nullptr),
      m_extraComponents(nullptr),
      m_blendState(nullptr),
      m_tempRenderTarget(nullptr),
      RenderObject(renderer)
   {
   }

   GBuffer::~GBuffer()
   {
      SafeDelete(m_positionBuffer);
      SafeDelete(m_albedoBuffer);
      SafeDelete(m_emissiveAOBuffer);
      SafeDelete(m_normalBuffer);
      SafeDelete(m_extraComponents);
      SafeDelete(m_depthStencilBuffer);
      SafeDelete(m_blendState);
   }

   bool GBuffer::Init(unsigned int width, unsigned int height)
   {
      if (RenderObject::IsInitializable())
      {
         RendererDX11* renderer = GetRenderer();
         m_positionBuffer = new RenderTargetDX11(renderer);
         m_albedoBuffer = new RenderTargetDX11(renderer);
         m_emissiveAOBuffer = new RenderTargetDX11(renderer);
         m_normalBuffer = new RenderTargetDX11(renderer);
         m_extraComponents = new RenderTargetDX11(renderer);
         m_blendState = new BlendState(renderer);
         m_depthStencilBuffer = new DepthStencilBufferDX11(renderer);

         bool bBuffersInitialized =
            m_positionBuffer->Init(width, height, EColorFormat::R32G32B32A32_FLOAT) &&
            m_albedoBuffer->Init(width, height, EColorFormat::R16G16B16A16_FLOAT) &&
            m_emissiveAOBuffer->Init(width, height, EColorFormat::R16G16B16A16_FLOAT) &&
            m_normalBuffer->Init(width, height, EColorFormat::R16G16B16A16_FLOAT) &&
            m_extraComponents->Init(width, height, EColorFormat::R16G16B16A16_FLOAT) &&
            m_depthStencilBuffer->Init(width, height, true);
         if (bBuffersInitialized && m_blendState->Init())
         {
            RenderObject::ConfirmInit();
            return true;
         }
      }

      return false;
   }

   bool GBuffer::BindRenderTargetView(ID3D11DeviceContext& deviceContext, bool clearRenderTargets, bool clearDepthStencil)
   {
      if (RenderObject::IsBindable())
      {
         ID3D11DepthStencilView* dsv = nullptr;
         if (m_depthStencilBuffer != nullptr)
         {
            dsv = m_depthStencilBuffer->GetDSV();
            if (clearDepthStencil)
            {
               deviceContext.ClearDepthStencilView(dsv,
                  D3D11_CLEAR_DEPTH,
                  1.0f,
                  0);
            }
         }

         std::array<ID3D11RenderTargetView*, GBUFFER_RENDER_TARGET_NUM> targets{
            m_positionBuffer->GetRTV(),
            m_albedoBuffer->GetRTV(),
            m_emissiveAOBuffer->GetRTV(),
            m_normalBuffer->GetRTV(),
            m_extraComponents->GetRTV()
         };

         if (clearRenderTargets)
         {
            const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            for (auto rtv : targets)
            {
               deviceContext.ClearRenderTargetView(rtv, clearColor);
            }
         }

         deviceContext.OMSetRenderTargets(GBUFFER_RENDER_TARGET_NUM, targets.data(), dsv);

         if (m_blendState != nullptr)
         {
            if (m_blendState->Bind(deviceContext))
            {
               return true;
            }
         }
      }
      
      return false;
   }

   bool GBuffer::BindShaderResourceView(ID3D11DeviceContext& deviceContext, unsigned int bindSlot, EShaderType bindShader, bool bBindDepthStencil)
   {
      if (RenderObject::IsBindable())
      {
         std::array<RenderTargetDX11*, GBUFFER_RENDER_TARGET_NUM> targets{
         m_positionBuffer,
         m_albedoBuffer,
         m_emissiveAOBuffer,
         m_normalBuffer,
         m_extraComponents };

         for (unsigned int idx = 0; idx < GBUFFER_RENDER_TARGET_NUM; ++idx)
         {
            if (!targets[idx]->BindShaderResourceView(deviceContext, bindSlot + idx, bindShader))
            {
               return false;
            }
         }

         if (bBindDepthStencil)
         {
            m_depthStencilBuffer->BindShaderResourceView(deviceContext, bindSlot + GBUFFER_RENDER_TARGET_NUM, bindShader);
         }

         return true;
      }

      return false;
   }

   void GBuffer::UnbindShaderResourceView(ID3D11DeviceContext& deviceContext, unsigned int boundSlot, EShaderType boundShader, bool bBoundDepthStencil)
   {
      if (RenderObject::IsBindable())
      {
         m_positionBuffer->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
         m_albedoBuffer->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
         m_emissiveAOBuffer->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
         m_normalBuffer->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
         m_extraComponents->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);

         if (bBoundDepthStencil)
         {
            m_depthStencilBuffer->UnbindShaderResourceView(deviceContext, boundSlot, boundShader);
         }
      }
   }

   /*bool GBuffer::BindDepthBufferWithExternalTarget(ID3D11DeviceContext& deviceContext, RenderTargetDX11* renderTarget, bool clearTarget, bool clearDepthStencil)
   {
      bool bValidRenderTarget = renderTarget != nullptr && renderTarget->IsBindable();
      if (RenderObject::IsBindable() && bValidRenderTarget && m_tempRenderTarget == nullptr)
      {
         if (!m_bBoundDepthAsShaderResource)
         {
            m_tempRenderTarget = renderTarget;
            renderTarget->BindAsRenderTarget(deviceContext, clearTarget, false);

            ID3D11RenderTargetView* renderTargetView = renderTarget->GetRTV();
            ID3D11DepthStencilView* dsv = nullptr;
            if (m_depthStencilBuffer != nullptr)
            {
               dsv = m_depthStencilBuffer->GetDSV();
               if (clearDepthStencil)
               {
                  deviceContext.ClearDepthStencilView(dsv,
                     D3D11_CLEAR_DEPTH,
                     1.0f,
                     0);
               }
            }

            deviceContext.OMSetRenderTargets(1, &renderTargetView, dsv);
            return true;
         }
         else
         {
            ME_LOG(MileGBuffer, Warning, TEXT("GBuffer's depth buffer was already bounded as shader resource."));
         }
      }

      return false;
   }*/

   void GBuffer::UnbindDepthBuffer(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         if (m_tempRenderTarget != nullptr)
         {
            m_tempRenderTarget->UnbindRenderTargetView(deviceContext);
            m_tempRenderTarget = nullptr;
         }
      }
   }

   void GBuffer::UnbindRenderTargetView(ID3D11DeviceContext& deviceContext)
   {
      if (RenderObject::IsBindable())
      {
         std::array<ID3D11RenderTargetView*, GBUFFER_RENDER_TARGET_NUM> targets{
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
         };

         deviceContext.OMSetRenderTargets(GBUFFER_RENDER_TARGET_NUM, targets.data(), nullptr);
      }
   }
}