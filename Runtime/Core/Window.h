#pragma once
#include "Core/SubSystem.h"
#include "Math/Vector2.h"
#include <Windows.h>

namespace Mile
{
   enum class EWindowStyle
   {
      FullScreen,
      Borderless,
      Windowed
   };

   static EWindowStyle IndexToWindowStyle(unsigned int idx)
   {
      switch (idx)
      {
      case 0:
         return EWindowStyle::FullScreen;
      case 1:
         return EWindowStyle::Borderless;
      case 2:
      default:
         return EWindowStyle::Windowed;
      }
   }

   /**
    * @brief	ȭ���� ������ �����츦 ��Ÿ���� ���� �ý��� Ŭ�����Դϴ�.
    */
   class MEAPI Window : public SubSystem
   {
   public:
      Window(Context* context);
      virtual ~Window();

      virtual bool Init() override;
      virtual void DeInit() override;

      /**
       * @brief	Win32�� raw handle �����͸� ��ȯ�մϴ�.
       * @return	�ڵ� ������
       */
      void* GetHandle() { return m_handle; }

      /**
       * @brief	������ ������ â �ػ��� �ʺ� ��ȯ�մϴ�.
       * @return	 ������ â �ػ��� �ʺ�
       */
      unsigned int GetResWidth() const { return m_resWidth; }

      /**
       * @brief	������ ������ â �ػ��� ���̸� ��ȯ�մϴ�.
       * @return	 ������ â �ػ��� ����
       */
      unsigned int GetResHeight() const { return m_resHeight; }

      /**
       * @brief ������ ������ â�� �ػ󵵸� ��ȯ�մϴ�.
       * @return ������ â�� �ػ�
       */
      Vector2 GetResolution() const { return Vector2(static_cast<float>(m_resWidth), static_cast<float>(m_resHeight)); }
      /**
       * @brief ������ ������ â �ػ��� ��Ⱦ�� ��ȯ�մϴ�.
       * @return ������ â �ػ��� ��Ⱦ��
       */
      float GetAspectRatio() const { return (m_resWidth / static_cast<float>(m_resHeight)); }

      /**
       * @brief	������ ������ â�� ����󵵼��� ��ȯ�մϴ�.
       * @return ������ â�� ����󵵼�
       */
      Float GetRefreshRate() const { return m_refreshRate; }

      /**
       * @brief ������ â�� Ǯ ��ũ�� ������� Ȯ���մϴ�.
       * @return	Ǯ ��ũ�� ��� ����
       */
      bool IsFullScreen() const { return m_bIsFullScreen; }

      virtual void Update() override;

      void SetTitle(const String& title);

   private:
      HWND m_handle;
      unsigned int m_resWidth;
      unsigned int m_resHeight;
      Float        m_refreshRate;

      bool         m_bIsFullScreen;
   };

   LRESULT CALLBACK WinProc(HWND Handle, unsigned int Msg, WPARAM wParam, LPARAM lParam);
}
