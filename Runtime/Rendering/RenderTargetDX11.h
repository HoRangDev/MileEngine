#pragma once
#include "Rendering/RenderObject.h"

namespace Mile
{
   class Texture2dDX11;
   class DepthStencilBufferDX11;
   class MEAPI RenderTargetDX11 : public RenderObject
   {
   public:
      RenderTargetDX11(RendererDX11* renderer);
      virtual ~RenderTargetDX11();

      bool Init(unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DepthStencilBufferDX11* depthStencilBuffer = nullptr);

      /**
       * @remark  �� �Լ��� ���� ��ü�� �ʱ�ȭ �� ���, ���̴� ���ҽ��δ� �ٿ�� ��ų �� �����ϴ�. (�׻� ���� Ÿ�����θ� Output Merge Stage �� ���ε� �Ǿ�� �մϴ�.)
       */
      bool Init(ID3D11RenderTargetView* rtv, DepthStencilBufferDX11* depthStencilBuffer = nullptr);

      unsigned int GetWidth() const { return m_width; }
      unsigned int GetHeight() const { return m_height; }
      Texture2dDX11* GetTexture() const { return m_texture; }
      ID3D11RenderTargetView* GetRTV() const { return m_rtv; }

      bool BindAsRenderTarget(ID3D11DeviceContext& deviceContext, bool clearTarget = true);
      bool BindAsShaderResource(ID3D11DeviceContext& deviceContext, unsigned int startSlot, EShaderType shader);
      void UnbindRenderTarget(ID3D11DeviceContext& deviceContext);
      void UnbindShaderResource(ID3D11DeviceContext& deviceContext);

      void SetDepthStencilBuffer(DepthStencilBufferDX11* buffer) { this->m_depthStencilBuffer = buffer; }
      void SetClearColor(const Vector4& color);
      void ClearDepthStencil(ID3D11DeviceContext& deviceContext);

   private:
      ID3D11RenderTargetView* m_rtv;
      Texture2dDX11* m_texture;
      DepthStencilBufferDX11* m_depthStencilBuffer;

      unsigned int               m_width;
      unsigned int               m_height;
      Vector4                    m_clearColor;

   };
}
