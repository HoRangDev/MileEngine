#pragma once
#include "Rendering/ResourceDX11.h"

namespace Mile
{
   /**
    * @brief	Direct3D 11 �� ����-���ٽ� ���۸� �����ϴ� Ŭ���� �Դϴ�.
    * @TODO    DSV�� SRV�� Ȱ�� �� �� �ֵ��� �����
    */
   class MEAPI DepthStencilBufferDX11 : public ResourceDX11
   {
   public:
      DepthStencilBufferDX11(RendererDX11* renderer);
      virtual ~DepthStencilBufferDX11();

      /**
       * @brief	����-���ٽ� ���۸� �ʱ�ȭ �մϴ�.
       * @param width			����-���ٽ� ������ �ʺ�
       * @param height			����-���ٽ� ������ ����
       * @param stencilEnable	����-���ٽ� ���ۿ��� ���ٽ� ������ Ȱ��ȭ ����
       * @return	�ʱ�ȭ ���� ����
       */
      bool Init(unsigned int width, unsigned int height, bool stencilEnable);

      virtual ERenderResourceType GetResourceType() const override { return ERenderResourceType::DepthStencilBuffer; }
      /**
       * @brief	����-���ٽ� ������ ���ҽ� �並 ��ȯ �մϴ�.
       * @return Direct3D Depth-stencil buffer resource view
       */
      ID3D11DepthStencilView* GetDSV() const { return m_depthStencilView; }

      /**
       * @brief	�ش� ���۰� ���ٽ��� Ȱ��ȭ �� ���·� �ʱ�ȭ �Ǿ����� Ȯ���մϴ�.
       * @return	���ٽ� ���� Ȱ��ȭ ����
       */
      bool IsStencilEnabled() const { return m_bStencilEnabled; }

      void Clear(ID3D11DeviceContext& deviceContext, float depth, UINT8 stencil);

   private:
      ID3D11DepthStencilView* m_depthStencilView;
      bool m_bStencilEnabled;

   };
}
