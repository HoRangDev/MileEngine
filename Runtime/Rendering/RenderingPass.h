#pragma once
#include "Rendering/RenderObject.h"

#define DEFINE_CONSTANT_BUFFER(BUFFER_NAME) struct alignas(16) BUFFER_NAME
namespace Mile
{
   class VertexShaderDX11;
   class PixelShaderDX11;
   class ConstantBufferDX11;
   class Texture2dDX11;
   class RenderTargetDX11;
   class DepthStencilBufferDX11;
   using CBufferPtr = ConstantBufferDX11 *;

   /**
    * @brief	���������� ���� ������� ����� ���� ���� ����� �� �ܰ���� ��Ÿ���� Ŭ���� �Դϴ�.
   * @todo		�ٸ� ���̴���(GS, Tessellation stage, CS.. )�� ������ �н����� ����Ҽ� �ֵ��� �ϱ�.
   * @todo		�� ���̴����� file path �и� (���̹� ���ϳ��ο��� include �ϴ� ����� ���)
    */
   class MEAPI RenderingPass : public RenderObject
   {
   public:
      RenderingPass(RendererDX11* renderer);
      virtual ~RenderingPass();

      virtual bool Init(const String& shaderPath);

      /**
       * @brief   �ε�� ���̴��鸸 ���ε� �մϴ�. 
       *          (��� ���� Ŭ�������� Bind �� RenderingPass::Bind �� ȣ���Ͽ��� �մϴ�.)
       */
      virtual bool Bind(ID3D11DeviceContext& deviceContext);

      /**
       * @brief   �ε�� ���̴����� ����ε� �մϴ�.
       *          (��� ���� Ŭ�������� Unbind �� RenderingPass::Unbind �� ȣ���Ͽ��� �մϴ�.)
       */
      virtual void Unbind(ID3D11DeviceContext& deviceContext);

      FORCEINLINE VertexShaderDX11* GetVertexShader() const { return m_vertexShader; }
      FORCEINLINE PixelShaderDX11* GetPixelShader() const { return m_pixelShader; }

      void SetClearStateWhenBind(bool bClearStateWhenBind) { m_bClearStateWhenBind = bClearStateWhenBind; }
      void SetPrimitiveTopology(EPrimitiveTopology primitiveTopology) { m_primitiveTopology = primitiveTopology; }

   private:
      bool InitVS(const String& filePath);
      bool InitPS(const String& filePath);

   private:
      VertexShaderDX11* m_vertexShader;
      PixelShaderDX11* m_pixelShader;

      bool m_bClearStateWhenBind;
      EPrimitiveTopology m_primitiveTopology;

   };
}