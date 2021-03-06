#pragma once
#include "Rendering/RenderObject.h"

namespace Mile
{
   class RendererDX11;
   /**
    * @brief	렌더러가 Rasterizer 단계에서 어떻게 동작할지에 대해 기술하는 클래스 입니다.
    */
   class MEAPI RasterizerState : public RenderObject
   {
   public:
      RasterizerState(RendererDX11* renderer);
      virtual ~RasterizerState();

      bool Init();
      bool Bind(ID3D11DeviceContext& deviceContext);

      /**
       * @brief	Mesh와 같은 물체들을 모두 와이어 프레임 형태로 그리도록 합니다.
       * @param	와이어 프레임으로 렌더링 할지 여부
       */
      void SetWireframeRender(bool bIsWireframe)
      {
         if (m_bIsWireframe != bIsWireframe)
         {
            m_bIsDirty = true;
         }

         m_bIsWireframe = bIsWireframe;
      }

      bool IsWireframeRender() const { return m_bIsWireframe; }

      /**
       * @brief 렌더링시 물체의 어떤면을 제외할지 설정합니다.
       * @param	cullMode	제외할 면
       */
      void SetCullMode(ECullMode cullMode)
      {
         if (m_cullMode != cullMode)
         {
            m_bIsDirty = true;
         }

         m_cullMode = cullMode;
      }

      ECullMode GetCullMode() const { return m_cullMode; }

      /**
       * @brief 시계방향 또는 반시계방향중 어느 방향으로 감긴 정점들을 앞면으로 볼지 설정합니다.
       * @param windingOrder  앞면으로 설정할 정점들의 감김 방향
       */
      void SetWindingOrder(EWindingOrder windingOrder)
      {
         if (m_windingOrder != windingOrder)
         {
            m_bIsDirty = true;
         }

         m_windingOrder = windingOrder;
      }

      EWindingOrder GetWindingOrder() const { return m_windingOrder; }

      void SetDepthBias(int depthBias)
      {
         if (depthBias != m_depthBias)
         {
            m_bIsDirty = true;
         }

         m_depthBias = depthBias;
      }

      int GetDepthBias() const { return m_depthBias; }

      void SetSlopeScaledDepthBias(float slopeScaledDepthBias)
      {
         if (m_slopeScaledDepthBias != slopeScaledDepthBias)
         {
            m_bIsDirty = true;
         }

         m_slopeScaledDepthBias = slopeScaledDepthBias;
      }

      float GetSlopeScaledDepthBias() const { return m_slopeScaledDepthBias; }

      void SetDepthBiasClamp(float depthBiasClamp)
      {
         if (m_depthBiasClamp != depthBiasClamp)
         {
            m_bIsDirty = true;
         }

         m_depthBiasClamp = depthBiasClamp;
      }

      float GetDepthBiasClamp() const { return m_depthBiasClamp; }

      void SetDepthClipEnable(bool bIsDepthClipEnable)
      {
         if (m_bIsDepthClipEnable = bIsDepthClipEnable)
         {
            m_bIsDirty = true;
         }

         m_bIsDepthClipEnable = bIsDepthClipEnable;
      }

      bool IsDepthClipEnabled() const { return m_bIsDepthClipEnable; }

   private:
      bool           m_bIsDirty;
      bool           m_bIsWireframe;
      ECullMode      m_cullMode;
      EWindingOrder  m_windingOrder;
      int            m_depthBias;
      float          m_slopeScaledDepthBias;
      float          m_depthBiasClamp;
      bool           m_bIsDepthClipEnable;

      ID3D11RasterizerState* m_rasterizerState;

   };
}