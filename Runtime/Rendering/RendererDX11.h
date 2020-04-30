#pragma once
#include "Rendering/RenderingCore.h"
#include <array>
#include <vector>

namespace Mile
{
   enum class ERenderContextType : UINT32
   {
      GeometryPass,
      LightingPass,
      PostProcessPass,
      Immediate,
      EnumSize
   };

   enum class EBloomType : UINT32
   {
      Box,
      Gaussian,
      None
   };

   constexpr size_t REQUIRED_RENDERCONTEXT_NUM = (size_t)ERenderContextType::EnumSize - 1;
   constexpr unsigned int DYNAMIC_CUBEMAP_SIZE = 512;
   constexpr unsigned int IRRADIANCEMAP_SIZE = 32;
   constexpr unsigned int PREFILTERED_CUBEMAP_SIZE = 128;
   constexpr unsigned int PREFILTERED_CUBEMAP_MAX_MIPLEVELS = 4 + 1;
   constexpr float DEFAULT_EXPOSURE_FACTOR = 1.0f;
   constexpr float DISABLE_TONE_MAPPING = -1.0f;
   constexpr float DEFAULT_GAMMA_FACTOR = 2.2f;
   constexpr float DISABLE_GAMMA_CORRECTION = -1.0f;

   class DepthStencilBufferDX11;
   class RenderTargetDX11;
   class Mesh;
   class Quad;
   class Cube;
   class GBuffer;
   class Viewport;
   class Texture2D;
   class Equirect2CubemapPass;
   class IrradianceConvPass;
   class PrefilteringPass;
   class IntegrateBRDFPass;
   class GeometryPass;
   class LightingPass;
   class AmbientEmissivePass;
   class SkyboxPass;
   class BoxBloomPass;
   class BlendingPass;
   class ToneMappingPass;
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
      void SetComputeIBLAsRealtime(bool bComputeIBLAsRealtime = false);

      void SetAmbientOcclusionFactor(float factor) { m_aoFactor = factor; }
      float GetAmbientOcclusionFactor() const { return m_aoFactor; }

      void SetGammaFactor(float gammaFactor) { m_gammaFactor = gammaFactor; }
      float GetGammaFactor() const { return m_gammaFactor; }

      void SetExposure(float exposureFactor) { m_exposureFactor = exposureFactor; }
      float GetExposureFactor() const { return m_exposureFactor; }

      void SetBloomType(EBloomType type) { m_bloomType = type; }
      EBloomType GetBloomType() const { return m_bloomType; }

   private:
      /* Initialization methods **/
      bool CreateDeviceAndSwapChain();
      bool CreateDepthStencilBuffer();

      /* Rendering Workflow **/
      ID3D11CommandList* RunGeometryPass(ID3D11DeviceContext* deviceContextPtr);

      /* Pre Computation **/
      void CalculateDiffuseIrradiance(ID3D11DeviceContext& deviceContext);
      void ConvertEquirectToCubemap(ID3D11DeviceContext& deviceContext, const std::array<Matrix, CUBE_FACES>& captureMatrix);
      void SolveDiffuseIntegral(ID3D11DeviceContext& deviceContext, const std::array<Matrix, CUBE_FACES>& captureMatrix);
      void ComputePrefilteredEnvMap(ID3D11DeviceContext& deviceContext, const std::array<Matrix, CUBE_FACES>& captureMatrix);
      void IntegrateBRDF(ID3D11DeviceContext& deviceContext);

      /* Lighting **/
      void RenderLight(ID3D11DeviceContext& deviceContext);
      void RenderAmbientEmissive(ID3D11DeviceContext& deviceContext);
      void RenderSkybox(ID3D11DeviceContext& deviceContext);
      ID3D11CommandList* RunLightingPass(ID3D11DeviceContext* deviceContextPtr);

      /* Post-Process **/
      RenderTargetDX11* Bloom(ID3D11DeviceContext& deviceContext, RenderTargetDX11* renderBuffer);
      RenderTargetDX11* BoxBloom(ID3D11DeviceContext& deviceContext, RenderTargetDX11* renderBuffer);
      RenderTargetDX11* Blending(ID3D11DeviceContext& deviceContext, RenderTargetDX11* srcBuffer, RenderTargetDX11* destBuffer, float srcRatio = 1.0f, float destRatio = 1.0f);
      void ToneMappingWithGammaCorrection(ID3D11DeviceContext& deviceContext, RenderTargetDX11* renderBuffer);
      ID3D11CommandList* RunPostProcessPass(ID3D11DeviceContext* deviceContextPtr);
   
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

      Quad* m_screenQuad;
      Cube* m_cubeMesh;

      /* PBS Workflow **/
      GBuffer* m_gBuffer;
      GeometryPass* m_geometryPass;
      LightingPass* m_lightingPass;
      RenderTargetDX11* m_lightingPassRenderBuffer;

      /** Cubemap / Environment Map */
      bool  m_bCubemapDirtyFlag; // Diffuse Irradiance �� Specular IBL �� ����� ���� �Ŀ� false�� �����Ͽ��� �Ѵ�.
      Equirect2CubemapPass* m_equirectToCubemapPass;
      Texture2D* m_equirectangularMap;
      DynamicCubemap* m_envMap;

      /** Diffuse Irradiance  */
      bool  m_bAlwaysComputeIBL;
      IrradianceConvPass* m_irradianceConvPass;
      DynamicCubemap* m_irradianceMap;

      /** Specular IBL */
      PrefilteringPass* m_prefilteringPass;
      DynamicCubemap* m_prefilterdEnvMap;

      IntegrateBRDFPass* m_integrateBRDFPass;
      RenderTargetDX11* m_brdfLUT;

      /** Ambient Emissive Pass */
      float m_aoFactor;
      AmbientEmissivePass* m_ambientEmissivePass;
      RenderTargetDX11* m_ambientEmissivePassRenderBuffer;

      SkyboxPass* m_skyboxPass;

      /** Post-Process */
      RenderTargetDX11* m_hdrBuffer;

      /** Bloom */
      EBloomType    m_bloomType;
      BoxBloomPass* m_boxBloomPass;

      /** Blending */
      BlendingPass* m_blendingPass;

      /** Tone Mapping */
      ToneMappingPass* m_toneMappingPass;
      float m_exposureFactor;
      float m_gammaFactor;

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