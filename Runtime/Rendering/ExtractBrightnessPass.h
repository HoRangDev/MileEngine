#pragma once
#include "Rendering/RenderingPass.h"
#include "Math/Vector3.h"

namespace Mile
{
   class BlendState;
   class RenderTargetDX11;
   class DepthStencilBufferDX11;
   class MEAPI ExtractBrightnessPass : public RenderingPass
   {
      DEFINE_CONSTANT_BUFFER(ExtractParams)
      {
         Vector3 Threshold;
      };

   public:
      ExtractBrightnessPass(class RendererDX11* renderer);
      ~ExtractBrightnessPass();

      bool Init(unsigned int width, unsigned int height);
      bool Bind(ID3D11DeviceContext& deviceContext, RenderTargetDX11* hdrBuffer);
      virtual void Unbind(ID3D11DeviceContext& deviceContext) override;

      void UpdateParameters(ID3D11DeviceContext& deviceContext, ExtractParams buffer);

      RenderTargetDX11* GetOutputBuffer() const { return m_outputHDRBuffer; }

   private:
      DepthStencilBufferDX11* m_depthStencilBuffer;
      RenderTargetDX11* m_boundHdrBuffer;
      RenderTargetDX11* m_outputHDRBuffer;
      CBufferPtr m_params;

   };
}