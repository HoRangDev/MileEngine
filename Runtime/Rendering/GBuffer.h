#pragma once
#include "Rendering/RenderObject.h"

namespace Mile
{
   class RenderTargetDX11;
   class DepthStencilBufferDX11;
   class BlendState;
   class MEAPI GBuffer : public RenderObject
   {
   public:
      GBuffer(RendererDX11* renderer);
      ~GBuffer();

      bool Init(unsigned int width, unsigned int height);

      void SetDepthStencilBuffer(DepthStencilBufferDX11* buffer) { this->m_depthStencilBuffer = buffer; }

      bool BindAsRenderTarget(ID3D11DeviceContext& deviceContext);
      bool BindAsShaderResource(ID3D11DeviceContext& deviceContext, unsigned int startSlot);
      void UnbindRenderTarget(ID3D11DeviceContext& deviceContext);
      void UnbindShaderResource(ID3D11DeviceContext& deviceContext);

   private:
      DepthStencilBufferDX11* m_depthStencilBuffer;
      RenderTargetDX11* m_positionBuffer;
      RenderTargetDX11* m_albedoBuffer;
      RenderTargetDX11* m_emissiveAOBuffer;
      RenderTargetDX11* m_normalBuffer;
      RenderTargetDX11* m_metallicRoughnessBuffer;
      BlendState*       m_blendState;

   };
}