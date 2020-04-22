#pragma once
#include "Rendering/RenderingCore.h"
#include <array>
#include <vector>

namespace Mile
{
   enum class ERenderContextType : UINT32
   {
      PreProcess,
      GeometryPass,
      LightingPass,
      PostProcessPass,
      Immediate,
      EnumSize
   };

   constexpr size_t REQUIRED_RENDERCONTEXT_NUM = (size_t)ERenderContextType::EnumSize - 1;
   constexpr unsigned int DYNAMIC_CUBEMAP_SIZE = 512;

   class DepthStencilBufferDX11;
   class RenderTargetDX11;
   class Mesh;
   class Quad;
   class Cube;
   class GBuffer;
   class Viewport;
   class Texture2D;
   class Equirect2CubemapPass;
   class GeometryPass;
   class LightingPass;
   class SkyboxPass;
   class Window;
   class World;
   class Entity;
   class MeshRenderComponent;
   class LightComponent;
   class CameraComponent;
   class Material;
   class RasterizerState;
   class BlendState;
   class DepthStencilState;
   class CommandListDX11;
   class DynamicCubemap;
   /**
    * @brief	Mile ������ ������ ���� �ý����Դϴ�. �������� World �� �����Ǿ��ִ� Entity�� ���� Mesh Renderer ����, �� ����,
   *			�׸��� ī�޶� ������ ���� �������� �ʿ��� ������ �� �����Ӹ��� ����Ͽ� ȭ���� ������ �ϴ� ������ ������ �ֽ��ϴ�.
   *			���� �ʱ�ȭ�ÿ� Direct3D 11�� �ʱ�ȭ�� ���� �����ϰ� �˴ϴ�.
    */
   class MEAPI RendererDX11 : public SubSystem
   {
      using MaterialMap = std::map<Material*, std::vector<MeshRenderComponent*>>;
   public:
      RendererDX11(Context* context);
      virtual ~RendererDX11();

      virtual bool Init() override;
      virtual void DeInit() override;

      /* Rendering Methods **/
      void Render();

      ID3D11Device* GetDevice() { return m_device; }

      void SetBackbufferAsRenderTarget(ID3D11DeviceContext& deviceContext);
      void SetClearColor(Vector4 clearColor);
      Vector4 GetClearColor() const { return m_clearColor; }

      void SetEquirectangularMap(Texture2D* texture);
      void SetAlwaysCalculateDiffuseIrradiacne(bool bAlwaysCalculateDiffuseIrraidiance = false);

   private:
      /* Initialization methods **/
      bool CreateDeviceAndSwapChain();
      bool CreateDepthStencilBuffer();

      /* Rendering Workflow **/
      /* Pre compute **/
      ID3D11CommandList* CalculateDiffuseIrradiance(ID3D11DeviceContext* deviceContextPtr);
      void ConvertEquirectToCubemap(ID3D11DeviceContext& deviceContext);
      void SolveDiffuseIntegral(ID3D11DeviceContext& deviceContext);

      /* Physically Based Shading Workflow **/
      ID3D11CommandList* RunGeometryPass(ID3D11DeviceContext* deviceContextPtr);
      ID3D11CommandList* RunLightingPass(ID3D11DeviceContext* deviceContextPtr);
      //ID3D11CommandList* RunPostProcessPass(ID3D11DeviceContext* deviceContext);
   
      /* Helper Methods **/
      /* @TODO Entity �迭�� �޴°� �ƴ� World Subsystem�� �����ͼ� Ư�� ������Ʈ�� ã�� �� �ֵ��� �ϱ�. **/
      /*
       * @brief   �־��� Entity�� �迭���� Mesh Renderer ������Ʈ�� Material���� ����Ͽ� ���������� �����մϴ�.
       * @warn    �̹� �����ӿ����� ���Ǿ���ϸ� ���� �����ӿ����� ���� ����ؾ��Ҽ��� �ֽ��ϴ�.
       **/
      void AcquireMeshRenderersAndMaterial(World* world);

      /*
       * @brief   �־��� Entity�� �迭���� Light ������Ʈ�� ����Ͽ� ���������� �����մϴ�.
       * @warn    �̹� �����ӿ����� ���Ǿ���ϸ� ���� �����ӿ����� ���� ����ؾ��Ҽ��� �ֽ��ϴ�.
       **/
      void AcquireLights(World* world);

      /*
      * @brief    �־��� Entity�� �迭���� Camera ������Ʈ�� ����Ͽ� ���������� �����մϴ�.
      * @warn    �̹� �����ӿ����� ���Ǿ���ϸ� ���� �����ӿ����� ���� ����ؾ��Ҽ��� �ֽ��ϴ�.
      **/
      void AcquireCameras(World* world);

      ID3D11DeviceContext* GetImmediateContext() { return m_immediateContext; }
      ID3D11DeviceContext* GetRenderContextByType(ERenderContextType type);

      void SetDepthStencilEnable(ID3D11DeviceContext& deviceContext, bool bDepthEnabled);
      bool IsDepthStencilEnabled() const { return m_bDepthStencilEnabled; }

      void Clear(ID3D11DeviceContext& deviceContext);
      void ClearDepthStencil(ID3D11DeviceContext& deviceContext);
      void Present();

   private:
      Window* m_window;
      ID3D11Device* m_device;
      ID3D11DeviceContext* m_immediateContext;

      /* Deferred Contexts **/
      std::array<ID3D11DeviceContext*, REQUIRED_RENDERCONTEXT_NUM> m_deferredContexts;

      /* Back Buffer Variables **/
      IDXGISwapChain* m_swapChain;
      ID3D11RenderTargetView* m_renderTargetView;
      DepthStencilBufferDX11* m_depthStencilBuffer;
      RenderTargetDX11* m_backBuffer;

      /* PBS Workflow **/
      GBuffer* m_gBuffer;
      GeometryPass* m_geometryPass;
      LightingPass* m_lightingPass;
      Quad* m_screenQuad;

      /** Diffuse Irradiance  */
      Equirect2CubemapPass* m_equirectToCubemapPass;
      Texture2D* m_equirectangularMap;
      Cube* m_cubeMesh;
      bool  m_bCubemapDirtyFlag;
      bool  m_bAlwaysCalculateDiffuseIrradiance;
      DynamicCubemap* m_cubemap;

      SkyboxPass* m_skyboxPass;

      /* Renderable objects **/
      std::vector<MeshRenderComponent*> m_meshRenderComponents;
      std::vector<LightComponent*>      m_lightComponents;
      std::vector<CameraComponent*>     m_cameras;
      MaterialMap                       m_materialMap;
      CameraComponent* m_mainCamera;

      /* Render State **/
      bool      m_bDepthStencilEnabled;
      Vector4   m_clearColor;
      BlendState* m_additiveBlendState;
      BlendState* m_defaultBlendState;
      DepthStencilState* m_depthLessEqual;
      DepthStencilState* m_depthDisable;

      // @TODO: Multiple Viewports
      Viewport* m_viewport;

      /* Rasterizer State **/
      RasterizerState* m_defaultRasterizerState;
      RasterizerState* m_noCulling;

   };
}