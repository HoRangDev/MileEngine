#pragma once
#include "Rendering/RenderingCore.h"
#include "Core/Logger.h"
#include "Core/Delegate.h"

namespace Mile
{
   DECLARE_LOG_CATEGORY_EXTERN(MileRenderer, Log);

   class Window;
   class World;
   class Quad;
   class Cube;
   class RenderTargetDX11;
   class DepthStencilBufferDX11;
   class OnWindowResizeDelegate;
   class GPUProfiler;

   /**
    * @brief	Mile ������ ������ ���� �ý����Դϴ�. �������� World �� �����Ǿ��ִ� Entity�� ���� Mesh Renderer ����, �� ����,
   *			�׸��� ī�޶� ������ ���� �������� �ʿ��� ������ �� �����Ӹ��� ����Ͽ� ȭ���� ������ �ϴ� ������ ������ �ֽ��ϴ�.
   *			���� �ʱ�ȭ�ÿ� Direct3D 11�� �ʱ�ȭ�� ���� �����ϰ� �˴ϴ�.
    */
   class MEAPI RendererDX11 : public SubSystem
   {
   public:
      RendererDX11(Context* context, size_t maximumThreads);
      virtual ~RendererDX11();

      virtual bool Init(Window& window);

      ID3D11Device& GetDevice() const 
      { 
         return (*m_device); 
      }

      ID3D11DeviceContext& GetImmediateContext() const 
      {
         return (*m_immediateContext);
      }

      ID3D11DeviceContext& GetDeferredContext(size_t idx) const
      { 
         return (*m_deferredContexts[idx]); 
      }

      RenderTargetDX11& GetBackBuffer() const 
      {
         return (*m_backBuffer); 
      }

      size_t GetMaximumThreads() const 
      { 
         return m_maximumThreads;
      }

      void SetVsync(bool enable)
      {
         m_bVsyncEnabled = enable;
      }
      bool IsVsyncEnabled() const
      {
         return m_bVsyncEnabled;
      }

      void SetRenderResolution(const Vector2& newResolution)
      {
         bool bIsValidResolution = newResolution.x > 0.0f && newResolution.y > 0.0f;
         if (bIsValidResolution)
         {
            m_renderResolution = newResolution;
            OnRenderResolutionChanged();
         }
         else
         {
            ME_LOG(MileRenderer, Warning, TEXT("Input resolution is not a valid resolution."));
         }
      }

      Vector2 GetRenderResolution() const
      {
         return m_renderResolution;
      }

      void Render(const World& world);
      void Present();

      void OnWindowReisze(unsigned int width, unsigned int height);

      void SetBackBufferAsRenderTarget(ID3D11DeviceContext& deviceContext, bool bClear = true, bool bClearDepthStencil = true);

      Quad* GetPrimitiveQuad() const { return m_quad; }
      Cube* GetPrimitiveCube() const { return m_cube; }

      const GPUProfiler& GetProfiler() const;
      GPUProfiler& GetProfiler();

   protected:
      virtual void RenderImpl(const World& world) { }
      virtual void OnRenderResolutionChanged() { };


   private:
      bool InitLowLevelAPI(Window& window);
      bool InitPrimitives();

   private:
      size_t m_maximumThreads;

      GPUProfiler* m_profiler;

      /** Low level APIs */
      ID3D11Device* m_device;
      ID3D11DeviceContext* m_immediateContext;
      std::vector<ID3D11DeviceContext*> m_deferredContexts;
      IDXGISwapChain* m_swapChain;
      RenderTargetDX11* m_backBuffer;
      DepthStencilBufferDX11* m_backBufferDepthStencil;

      /** Application level */
      Vector2 m_renderResolution;
      OnWindowResizeDelegate* m_onWindowResize;
      bool m_bVsyncEnabled;

      /** Primitive */
      Quad* m_quad;
      Cube* m_cube;

   };
}