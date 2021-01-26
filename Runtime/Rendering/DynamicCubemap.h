#pragma once
#include "Rendering/Texture2DBaseDX11.h"

namespace Mile
{
   /**
    * @brief   Dynamic Cubemap�� ����Ÿ��(GPU Readable/Writable)���� ���ϼ��ִ� ť����Դϴ�.
    *          Face Index: (0, +X), (1, -X), (2, +Y), (3, -Y), (4, +Z), (5, -Z)
    * @format  (R16, G16, B16, A16)_UNORM
    *          
    */
   class DepthStencilBufferDX11;
   class MEAPI DynamicCubemap : public Texture2DBaseDX11
   {
   public:
      DynamicCubemap(RendererDX11* renderer);
      virtual ~DynamicCubemap();

      bool Init(unsigned int size);
      virtual ERenderResourceType GetResourceType() const override { return ERenderResourceType::DynamicCubemap; }

      bool BindAsRenderTarget(ID3D11DeviceContext& deviceContext, unsigned int faceIdx, unsigned int mipLevel = 0, bool clearRenderTarget = true, bool clearDepth = true);
      /* 
      * @warn  ������ Cubemap�� ���� Ÿ������ ���ε� �Ǿ��ִ��� ���θ� ������ �ʰ� ������ unbind �մϴ�.
      **/
      void UnbindAsRenderTarget(ID3D11DeviceContext& deviceContext);

      unsigned int GetMaxMipLevels() const { return m_maxMipLevels; }

   private:
      std::array<std::vector<ID3D11RenderTargetView*>, CUBE_FACES> m_rtvs;
      std::vector<DepthStencilBufferDX11*> m_depthStencils;
      unsigned int m_maxMipLevels;

   };
}
