#pragma once
#include "Rendering/RenderingCore.h"

namespace Mile
{
   /**
    * @brief   �������� �������ִ� ������Ʈ���� ���� �������̽�.
    *          �� Ŭ������ ����ϴ� �ڽ� Ŭ�������� Init �Լ��� �����Ҷ� IsInitializable�� �̸�
    *          �ʱ�ȭ �������� Ȯ���Ͽ��� �ϸ�, �ʱ�ȭ�� �Ϸ�Ǿ����� RenderObject::ConfirmInit() �޼��带 ȣ���Ͽ��� �մϴ�.
    *          Bind �Լ��� �����ҽÿ��� IsBindable �Լ���, Bind ���� ���θ� üũ�Ͽ��� �մϴ�.
    */
   class RendererDX11;
   class MEAPI RenderObject
   {
   public:
      RenderObject(RendererDX11* renderer);
      virtual ~RenderObject();

      FORCEINLINE bool IsInitialized() const { return m_bIsInitialized; }
      /**
       * @brief   ���� ������Ʈ�� ��������(�Ǵ� ������Ʈ�� �����ϴ�) �������� �������� ����ϰ� �ִ� �� ���������� Ȯ�� �մϴ�.
       */
      bool HasAvailableRenderer() const;

      /**
       * @brief   ������Ʈ�� �ʱ��� �� �ִ��� üũ�մϴ�. �̹� �ʱ�ȭ �Ǿ����� �ʾƾ��ϸ�, 
       *          ������ �� �������� ������Ʈ�� �������� �����Ͽ��� �մϴ�.
       */
      FORCEINLINE bool IsInitializable() const { return (!IsInitialized() && HasAvailableRenderer()); }

      /**
       * @brief   ������Ʈ�� ���ε��� �� �ִ��� üũ�մϴ�. �ʱ�ȭ�� �Ϸ�Ǿ��־�� �ϸ�,
       *          ������ �� �������� ������Ʈ�� �������� �����Ͽ��� �մϴ�.
       *
       * @warn    ������Ʈ�� ������ ���������ο� ���ε�Ǿ� �ִ��� ���οʹ� ������ �����ϴ�.   
       */
      FORCEINLINE bool IsBindable() const { return (IsInitialized() && HasAvailableRenderer()); }
      FORCEINLINE RendererDX11* GetRenderer() const { return m_renderer; }

   protected:
      FORCEINLINE void ConfirmInit() { m_bIsInitialized = true; }
      FORCEINLINE void ConfirmDeInit() { m_bIsInitialized = false; }

   private:
      RendererDX11* m_renderer;
      bool m_bIsInitialized;

   };
}