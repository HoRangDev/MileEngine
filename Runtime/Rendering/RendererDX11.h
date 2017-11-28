#pragma once

#include "Rendering.h"

namespace Mile
{
   enum class ShaderType
   {
      VertexShader,
      HullShader,
      DomainShader,
      GeometryShader,
      PixelShader
   };

   class Window;
   class DepthStencilBufferDX11;
   class RenderTargetDX11;
   class GBuffer;
   class GBufferPass;
   class LightBufferPass;
   class ShadingPass;
   class MEAPI RendererDX11 : public SubSystem
   {
   public:
      RendererDX11( Context* context );
      virtual ~RendererDX11( );

      virtual bool Init( ) override;

      void Render( );
      void RenderGBuffer( );
      void RenderLightBuffer( );
      void RenderShading( );

      void Clear( );
      void Present( );

      ID3D11Device* GetDevice( ) { return m_device; }
      ID3D11DeviceContext* GetDeviceContext( ) { return m_deviceContext; }

      void SetClearColor( Vector4 clearColor );
      Vector4 GetClearColor( ) const { return m_clearColor; }

      void SetDepthStencilEnable( bool bDepthEnabled );
      bool IsDepthStencilEnabled( ) const { return m_bDepthStencilEnabled; }

      void SetBackbufferAsRenderTarget( );

   private:
      bool CreateDeviceAndSwapChain( );
      bool CreateDepthStencilBuffer( );

   private:
      Window*                 m_window;
      ID3D11Device*           m_device;
      ID3D11DeviceContext*    m_deviceContext;

      // Back Buffer Variables
      IDXGISwapChain*         m_swapChain;
      ID3D11RenderTargetView* m_renderTargetView;
      DepthStencilBufferDX11*   m_depthStencilBuffer;
      // \Back Buffer Variables

      // Pre light pass Rendering
      GBuffer*          m_gBuffer;
      GBufferPass*      m_gBufferPass;
      RenderTargetDX11* m_lightBuffer;
      LightBufferPass*  m_lightBufferPass;
      ShadingPass*      m_shadingPass;
      // \Pre light pass Rendering

      bool                    m_bDepthStencilEnabled;
      Vector4                 m_clearColor;

   };
}