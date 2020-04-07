#pragma once
#include "Rendering/RenderObject.h"

namespace Mile
{
   enum class CullMode
   {
      NONE,
      FRONT,
      BACK
   };

   enum class WindingOrder
   {
      CCW,
      CW
   };

   class RendererDX11;
   /**
    * @brief	�������� Rasterizer �ܰ迡�� ��� ���������� ���� ����ϴ� Ŭ���� �Դϴ�.
    */
   class MEAPI RasterizerState : public RenderObject
   {
   public:
      RasterizerState(RendererDX11* renderer);
      ~RasterizerState();

      bool Init();
      bool Bind(ID3D11DeviceContext& deviceContext);

      /**
       * @brief	Mesh�� ���� ��ü���� ��� ���̾� ������ ���·� �׸����� �մϴ�.
       * @param	���̾� ���������� ������ ���� ����
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
       * @brief �������� ��ü�� ����� �������� �����մϴ�.
       * @param	cullMode	������ ��
       */
      void SetCullMode(CullMode cullMode)
      {
         if (m_cullMode != cullMode)
         {
            m_bIsDirty = true;
         }

         m_cullMode = cullMode;
      }

      CullMode GetCullMode() const { return m_cullMode; }

      /**
       * @brief �ð���� �Ǵ� �ݽð������ ��� �������� ���� �������� �ո����� ���� �����մϴ�.
       * @param windingOrder  �ո����� ������ �������� ���� ����
       */
      void SetWindingOrder(WindingOrder windingOrder)
      {
         if (m_windingOrder != windingOrder)
         {
            m_bIsDirty = true;
         }

         m_windingOrder = windingOrder;
      }

      WindingOrder GetWindingOrder() const { return m_windingOrder; }

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
      CullMode       m_cullMode;
      WindingOrder   m_windingOrder;
      int            m_depthBias;
      float          m_slopeScaledDepthBias;
      float          m_depthBiasClamp;
      bool           m_bIsDepthClipEnable;

      ID3D11RasterizerState* m_rasterizerState;

   };
}